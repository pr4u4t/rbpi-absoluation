#include <ads1256.h>

/* -- proto -- */
static bool ADS1256_open(ADS1256 *this, ADS1256Gain gain, ADS1256DataRate drate, ADS1256Mode mode);
static void ADS1256_close(ADS1256 *this);
static void ADS1256_setMode(ADS1256 *this, ADS1256Mode mode);
static ADS1256Mode ADS1256_mode(ADS1256 *this);
static uint8_t ADS1256_channels(ADS1256 *this);
static void ADS1256_delayUS(ADS1256 *this, time_t micros);
static void ADS1256_send8Bit(ADS1256 *this, ADS1256RegValue value);
static void ADS1256_cfgADC(ADS1256 *this, ADS1256Gain gain, ADS1256DataRate drate);
static void ADS1256_delayData(ADS1256 *this);
static ADS1256RegValue ADS1256_recive8Bit(ADS1256 *this);
static void ADS1256_writeRegister(ADS1256 *this, ADS1256Register reg, ADS1256RegValue);
static ADS1256RegValue ADS1256_readRegister(ADS1256 *this, ADS1256Register reg);
static void ADS1256_writeCommand(ADS1256 *this, ADS1256Command cmd);
static ADS1256RegValue ADS1256_readChipID(ADS1256 *this);
static void ADS1256_setChannel(ADS1256 *this, ADS1256Channel chan);
static void ADS1256_setDiffChannel(ADS1256 *this, ADS1256Channel chan);
static void ADS1256_waitDRDY(ADS1256 *this);
static ADS1256Value ADS1256_readData(ADS1256 *this);
static ADS1256Value ADS1256_value(ADS1256 *this, ADS1256Channel chan);
static void ADS1256_ISR(ADS1256 *this);
static bool ADS1256_scan(ADS1256 *this);
static float ADS1256_voltage(ADS1256 *this, ADS1256Channel chan);

/* -- driver function table  -- */

static ADS1256Funcs  ADS1256_vft = {
    /* -- ADC -- */
    ._base  = {
        ._base  = {
            .isr   = (void (*)(Hardware*)) &ADS1256_ISR,
            .chipID = (int (*)(Hardware*))  &ADS1256_readChipID
        },
        .open           = (bool     (*)(ADC*,int,int,int))  &ADS1256_open,
        .close          = (void     (*)(ADC*))              &ADS1256_close,
        .mode           = (int      (*)(ADC*))              &ADS1256_mode, 
        .setMode        = (void     (*)(ADC*,int))          &ADS1256_setMode,
        .channels       = (uint8_t  (*)(ADC*))              &ADS1256_channels,
        .voltage	    = (float    (*)(ADC*,int))          &ADS1256_voltage
    },
    .value          = &ADS1256_value,
    .delayUS        = &ADS1256_delayUS,
    .send8Bit       = &ADS1256_send8Bit,      
    .cfgADC         = &ADS1256_cfgADC,
    .delayData      = &ADS1256_delayData,
    .recive8Bit     = &ADS1256_recive8Bit,
    .writeReg       = &ADS1256_writeRegister,
    .readReg        = &ADS1256_readRegister,
    .writeCmd       = &ADS1256_writeCommand,
    .setChannel     = &ADS1256_setChannel,
    .setDiffChannel = &ADS1256_setDiffChannel,
    .waitDrdy       = &ADS1256_waitDRDY,
    .readData       = &ADS1256_readData
};

static ADS1256MetaFuncs ADS1256Meta_vft;

/* -- public driver part -- */

OBJECT_REGISTER(ADS1256,OBJECT_ADC | OBJECT_HARDWARE | OBJECT,ADS1256_vft,ADS1256Meta_vft,ADC)

/* -- private driver part -- */

static bool ADS1256_open(ADS1256 *this, ADS1256Gain gain, ADS1256DataRate drate, ADS1256Mode mode){
    if(!this) return false;
    if(!call(Object,this,open) || !bcm2835_init()) return false;
    
    bcm2835_spi_begin();
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);   	//default
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE1);                 //default
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_256);	//default

    bcm2835_gpio_fsel(SPICS, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_write(SPICS, HIGH);
    bcm2835_gpio_fsel(DRDY, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(DRDY, BCM2835_GPIO_PUD_UP);
    
    call(ADC,this,setMode,mode);        //this->setMode(this,mode); //tutej
    call(ADS1256,this,cfgADC,gain,drate);   //this->cfgADC(this,gain,drate);
  
    return true;
}

static void ADS1256_close(ADS1256 *this){
    if(!this) return;
    bcm2835_spi_end();
    bcm2835_close();
}

static void ADS1256_delayUS(ADS1256 *this, time_t micros){ 
    bcm2835_delayMicroseconds (micros); 
}

static void ADS1256_setMode(ADS1256 *this, ADS1256Mode mode){
    if(!this) return;
    this->_data.mode = mode;
    this->_data.channel = 0;
    memset(this->_data.adc,0,FIELD_SIZEOF(ADS1256Data,adc));
}

static ADS1256Mode ADS1256_mode(ADS1256 *this){
    if(!this) return 0;
    return this->_data.mode;
}

static uint8_t ADS1256_channels(ADS1256 *this){
    if(!this) return 0;
    return (call(ADC,this,mode) == SINGLE_ENDED) ? EIGHT_CHANNELS : FOUR_CHANNELS;
}

static void ADS1256_send8Bit(ADS1256 *this,ADS1256RegValue data){
    if(!this) return;
    
	call(ADS1256,this,delayUS,DELAY_SEND); //this->delayUS(this,2);
    bcm2835_spi_transfer(data);
}

static void ADS1256_cfgADC(ADS1256 *this,ADS1256Gain gain, ADS1256DataRate drate){
    uint8_t buf[4];
    
    if(!this) return;
    
    this->_data.gain = gain;
    this->_data.dataRate = drate;
    call(ADS1256,this,waitDrdy);

    /* Storage ads1256 register configuration parameters */
    buf[0] = (0 << 3) | (1 << 2) | (0 << 1);  // The internal buffer is prohibited
    buf[1] = 0x08;	
    buf[2] = (0 << 5) | (0 << 3) | ( gain << 0 );
    buf[3] = drate;	// DRATE_10SPS;	

    CS_0();
    
    call(ADS1256,this,send8Bit,CMD_WREG | 0); //this->send8Bit(this,CMD_WREG | 0);	/* Write command register, send the register address */
    call(ADS1256,this,send8Bit,0x03);         //this->send8Bit(this,0x03);			/* Register number 4,Initialize the number  -1*/
    call(ADS1256,this,send8Bit,buf[0]);       //this->send8Bit(this,buf[0]);	/* Set the status register */
    call(ADS1256,this,send8Bit,buf[1]);       //this->send8Bit(this,buf[1]);	/* Set the input channel parameters */
    call(ADS1256,this,send8Bit,buf[2]);       //this->send8Bit(this,buf[2]);	/* Set the ADCON control register,gain */
    call(ADS1256,this,send8Bit,buf[3]);       //this->send8Bit(this,buf[3]);	/* Set the output rate */
    
    CS_1();

    call(ADS1256,this,delayUS,DELAY_CONFIGURE); //this->delayUS(this,50);
}

static void ADS1256_delayData(ADS1256 *this){
    if(!this) return;
    call(ADS1256,this,delayUS,DELAY_DATA); //this->delayUS(this,10);	/* The minimum time delay 6.5us */ 
}

static ADS1256RegValue ADS1256_recive8Bit(ADS1256 *this){
	uint8_t read = 0;
    if(!this) return 0;
	read = bcm2835_spi_transfer(0xff);
	return read;
}

static void ADS1256_writeRegister(ADS1256 *this, ADS1256Register id, ADS1256RegValue data){
    if(!this) return;
	CS_0();	/* SPI  cs  = 0 */
	call(ADS1256,this,send8Bit,CMD_WREG | id); //this->send8Bit(this,CMD_WREG | id);	/*Write command register */
	call(ADS1256,this,send8Bit,0x00);          //this->send8Bit(this,0x00);		/*Write the register number */
	call(ADS1256,this,send8Bit,data);          //this->send8Bit(this,data);	/*send register value */
	CS_1();	/* SPI   cs = 1 */
}

static ADS1256RegValue ADS1256_readRegister(ADS1256 *this, ADS1256Register id){
	uint8_t read;
    if(!this) return 0;
    
	CS_0();	/* SPI  cs  = 0 */
    call(ADS1256,this,send8Bit,CMD_RREG | id); //this->send8Bit(this,CMD_RREG | id);	/* Write command register */
	call(ADS1256,this,send8Bit,0x00);          //this->send8Bit(this,0x00);	/* Write the register number */
	call(ADS1256,this,delayData);              //this->delayData(this);	/*delay time */
	read = call(ADS1256,this,recive8Bit);      //this->recive8Bit(this);	   /* Read the register values */
	CS_1();	/* SPI   cs  = 1 */

	return read;
}

static void ADS1256_writeCommand(ADS1256 *this, ADS1256Command cmd){
    if(!this) return;
	CS_0();	/* SPI   cs = 0 */
	call(ADS1256,this,send8Bit,cmd); //this->send8Bit(this,cmd);
	CS_1();	/* SPI  cs  = 1 */
}

static ADS1256RegValue ADS1256_readChipID(ADS1256 *this){
	uint8_t id;

	call(ADS1256,this,waitDrdy);                   //this->waitDrdy(this);
	id = call(ADS1256,this,readReg,REG_STATUS);    //this->readReg(this,REG_STATUS);
	return (id >> 4);
}

static void ADS1256_setChannel(ADS1256 *this, ADS1256Channel chan){
	if (!this || chan > 7) return;
	//this->writeReg(this,REG_MUX, (chan << 4) | (1 << 3));	/* Bit3 = 1, AINN connection AINCOM */
    call(ADS1256,this,writeReg, REG_MUX, (chan << 4) | (1 << 3));
}

static void ADS1256_setDiffChannel(ADS1256 *this, ADS1256Channel chan){
    if(!this) return;
    ADS1256RegValue data;
    
    switch(chan)
    {
        case 0:
            data = (0 << 4) | 1; break;
            //this->writeReg(this,REG_MUX, (0 << 4) | 1); /* DiffChannal  AIN0 AIN1 */
            //return;
        case 1:
            data = (2 << 4) | 3; break;
            //this->writeReg(this,REG_MUX, (2 << 4) | 3); /* DiffChannal   AIN2 AIN3 */
            //return;
        case 2:
            data = (4 << 4) | 5; break;
            //this->writeReg(this,REG_MUX, (4 << 4) | 5); /* DiffChannal   AIN4 AIN5 */
            //return;
        case 3:
            data = (6 << 4) | 7; break;
            //this->writeReg(this,REG_MUX, (6 << 4) | 7); /* DiffChannal   AIN6 AIN7 */
            //return;
    }
    
    call(ADS1256,this,writeReg,REG_MUX,data);
}

static void ADS1256_waitDRDY(ADS1256 *this){
	uint32_t i;
    if(!this) return;
	for (i = 0; i < 400000; i++) 
        if (DRDY_IS_LOW()) break;
}

static ADS1256Value ADS1256_readData(ADS1256 *this){
	uint32_t read = 0;
    static uint8_t buf[3];
    if(!this) return 0;
	CS_0();	/* SPI   cs = 0 */

	call(ADS1256,this,send8Bit,CMD_RDATA); //this->send8Bit(this,CMD_RDATA);	/* read ADC command  */
	call(ADS1256,this,delayData);          //this->delayData(this);	/*delay time  */

	/*Read the sample results 24bit*/
    buf[0] = call(ADS1256,this,recive8Bit); //this->recive8Bit(this);
    buf[1] = call(ADS1256,this,recive8Bit); //this->recive8Bit(this);
    buf[2] = call(ADS1256,this,recive8Bit); //this->recive8Bit(this);

    read = ((uint32_t)  buf[0] << 16) & 0x00FF0000;
    read |= ((uint32_t) buf[1] << 8);  /* Pay attention to It is wrong   read |= (buf[1] << 8) */
    read |= buf[2];

	CS_1();	/* SPIT = 1 */

	/* Extend a signed number*/
    if (read & 0x800000) 
        read |= 0xFF000000;
    
	return (int32_t)read;
}

static ADS1256Value ADS1256_value(ADS1256 *this, ADS1256Channel chan){
	return (!this || chan > 7) ? 0 : this->_data.adc[chan];
}

static void ADS1256_ISR(ADS1256 *this){
    if(!this || !call(Object,this,isOpen) || !DRDY_IS_LOW()) return;
    
    uint8_t moduler = (!call(ADC,this,mode)) ? EIGHT_CHANNELS : FOUR_CHANNELS; 
    call(ADS1256,this,setChannel,this->_data.channel);                    //this->setChannel(this,this->_data.channel);	/*Switch channel mode */
    call(ADS1256,this,delayUS,DELAY_WRITE);           //this->delayUS(this,5);

    call(ADS1256,this,writeCmd,CMD_SYNC);   //this->writeCmd(this,CMD_SYNC);
    call(ADS1256,this,delayUS,DELAY_SYNC);           //this->delayUS(this,5);

    call(ADS1256,this,writeCmd,CMD_WAKEUP); //this->writeCmd(this,CMD_WAKEUP);
    call(ADS1256,this,delayUS,DELAY_WAKEUP);          //this->delayUS(this,25);
     
    this->_data.adc[(!this->_data.channel) ? (moduler-1) : (this->_data.channel-1) ] = call(ADS1256,this,readData); //ADS1256_readData(this);	
    this->_data.channel = (++this->_data.channel) % moduler;
}

static float ADS1256_voltage(ADS1256 *this, ADS1256Channel chan){
	int32_t tmp;
	float ret;

	if(!this) return 0;
	
	tmp = call(ADS1256,this,value,chan); //this->value(this,chan);
	tmp = (tmp*100)/167;
	ret = tmp; 
	ret /= 1000000;
    
	return (tmp < 0) ? -ret : ret;
}
