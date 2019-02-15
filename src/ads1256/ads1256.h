#ifndef _ADS1256_H_
#define _ADS1256_H_

#include <bcm2835.h>  
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <bool.h>
#include <utils.h>
#include <adc.h>

#ifndef DRDY
#define DRDY    17
#endif

#ifndef RST
#define RST     18
#endif

#ifndef SPICS
#define SPICS   22
#endif

#define CS_1() 	bcm2835_gpio_write(SPICS,HIGH)
#define CS_0()  bcm2835_gpio_write(SPICS,LOW)

#define DRDY_IS_LOW()	((bcm2835_gpio_lev(DRDY)==0))

#define RST_1() 	bcm2835_gpio_write(RST,HIGH)
#define RST_0() 	bcm2835_gpio_write(RST,LOW)

/* -- DECLARATIONS -- */

typedef enum _ADS1256Gain       ADS1256Gain;
typedef enum _ADS1256DataRate   ADS1256DataRate;
typedef enum _ADS1256Command    ADS1256Command;
typedef enum _ADS1256Register   ADS1256Register;
typedef enum _ADS1256Mode       ADS1256Mode;
typedef enum _ADS1256Channels   ADS1256Channels;
typedef enum _ADS1256Timings    ADS1256Timings;

/*
typedef struct _ADS1256Data         ADS1256Data; 
typedef struct _ADS1256Funcs        ADS1256Funcs;
typedef struct _ADS1256MetaFuncs    ADS1256MetaFuncs;
typedef struct _ADS1256             ADS1256;
*/

typedef int32_t ADS1256Value;
typedef uint8_t ADS1256Channel;
typedef uint8_t ADS1256RegValue;

/* -- DEFINITIONS -- */

//definition of gain channel values
//enum _ADS1256Gain {
//	ADS1256_GAIN_1	= 0,	/* GAIN 1 	*/
//	ADS1256_GAIN_2	= 1,	/* GAIN 2 	*/
//	ADS1256_GAIN_4	= 2,	/* GAIN 4 	*/
//	ADS1256_GAIN_8	= 3,	/* GAIN 8 	*/
//	ADS1256_GAIN_16	= 4,	/* GAIN	16 	*/
//	ADS1256_GAIN_32	= 5,	/* GAIN 32 	*/
//	ADS1256_GAIN_64	= 6	    /* GAIN 64 	*/
//};

SMART_ENUM(ADS1256Gain,
	NUM(GAIN_1,		0),
    NUM(GAIN_2,		1),
    NUM(GAIN_4,		2),
    NUM(GAIN_8,		3),
    NUM(GAIN_16,	4),
    NUM(GAIN_32,	5),
    NUM(GAIN_64,	6),
)
    
//definition of sampling speed values
/*
enum _ADS1256DataRate {
	ADS1256_30000SPS   = 0xF0, //reset the default values 
	ADS1256_15000SPS   = 0xE0,
	ADS1256_7500SPS    = 0xD0,
	ADS1256_3750SPS    = 0xC0,
	ADS1256_2000SPS    = 0xB0,
	ADS1256_1000SPS    = 0xA1,
	ADS1256_500SPS     = 0x92,
	ADS1256_100SPS     = 0x82,
	ADS1256_60SPS      = 0x72,
	ADS1256_50SPS      = 0x63,
	ADS1256_30SPS      = 0x53,
	ADS1256_25SPS      = 0x43,
	ADS1256_15SPS      = 0x33,
	ADS1256_10SPS      = 0x20,
	ADS1256_5SPS       = 0x13,
	ADS1256_2d5SPS     = 0x03
};
*/
SMART_ENUM(ADS1256DataRate,
	NUM(30000SPS,		0xF0),
    NUM(15000SPS,		0xE0),
    NUM(7500SPS,		0xD0),
    NUM(3750SPS,		0xC0),
    NUM(2000SPS,        0xB0),
	NUM(1000SPS,        0xA1),
	NUM(500SPS,         0x92),
	NUM(100SPS,         0x82),
	NUM(60SPS,          0x72),
	NUM(50SPS,          0x63),
	NUM(30SPS,          0x53),
	NUM(25SPS,          0x43),
	NUM(15SPS,          0x33),
	NUM(10SPS,          0x20),
	NUM(5SPS,           0x13),
	NUM(2d5SPS,         0x03)
)
//

//enum _ADS1256Register {
	/*Register address, followed by reset the default values */
//	REG_STATUS = 0,    // x1H
//	REG_MUX    = 1,    // 01H
//	REG_ADCON  = 2,    // 20H
//	REG_DRATE  = 3,    // F0H
//	REG_IO     = 4,    // E0H
//	REG_OFC0   = 5,    // xxH
//	REG_OFC1   = 6,    // xxH
//	REG_OFC2   = 7,    // xxH
//	REG_FSC0   = 8,    // xxH
//	REG_FSC1   = 9,    // xxH
//	REG_FSC2   = 10,   // xxH
//};

SMART_ENUM(ADS1256Register,
    NUM(REG_STATUS,     0),
    NUM(REG_MUX         1),
	NUM(REG_ADCON       2),
	NUM(REG_DRATE       3),
	NUM(REG_IO          4),
	NUM(REG_OFC0        5),
	NUM(REG_OFC1        6),
	NUM(REG_OFC2        7),
	NUM(REG_FSC0        8),
	NUM(REG_FSC1        9),
	NUM(REG_FSC2        10)
)

/* Command definition TTable 24. Command Definitions --- ADS1256 datasheet Page 34 */
/*
enum _ADS1256Command {
    CMD_WAKEUP  = 0x00, // Completes SYNC and Exits Standby Mode 0000  0000 (00h)
    CMD_RDATA   = 0x01, // Read Data 0000  0001 (01h)
    CMD_RDATAC  = 0x03, // Read Data Continuously 0000   0011 (03h)
    CMD_SDATAC  = 0x0F, // Stop Read Data Continuously 0000   1111 (0Fh)
    CMD_RREG    = 0x10, // Read from REG rrr 0001 rrrr (1xh)
    CMD_WREG    = 0x50, // Write to REG rrr 0101 rrrr (5xh)
    CMD_SELFCAL = 0xF0, // Offset and Gain Self-Calibration 1111    0000 (F0h)
    CMD_SELFOCAL= 0xF1, // Offset Self-Calibration 1111    0001 (F1h)
    CMD_SELFGCAL= 0xF2, // Gain Self-Calibration 1111    0010 (F2h)
    CMD_SYSOCAL = 0xF3, // System Offset Calibration 1111   0011 (F3h)
    CMD_SYSGCAL = 0xF4, // System Gain Calibration 1111    0100 (F4h)
    CMD_SYNC    = 0xFC, // Synchronize the A/D Conversion 1111   1100 (FCh)
    CMD_STANDBY = 0xFD, // Begin Standby Mode 1111   1101 (FDh)
    CMD_RESET   = 0xFE, // Reset to Power-Up Values 1111   1110 (FEh)
};
*/
SMART_ENUM(ADS1256Command,
    NUM(CMD_WAKEUP,     0x00),
    NUM(CMD_RDATA,      0x01),
    NUM(CMD_RDATAC,     0x03),
    NUM(CMD_SDATAC,     0x0F),
    NUM(CMD_RREG,       0x10),
    NUM(CMD_WREG,       0x50),
    NUM(CMD_SELFCAL,    0xF0),
    NUM(CMD_SELFOCAL,   0xF1),
    NUM(CMD_SELFGCAL,   0xF2),
    NUM(CMD_SYSOCAL,    0xF3),
    NUM(CMD_SYSGCAL,    0xF4),
    NUM(CMD_SYNC,       0xFC),
    NUM(CMD_STANDBY,    0xFD),
    NUM(CMD_RESET,      0xFE)
)
/*
enum _ADS1256Mode {
    SINGLE_ENDED = 0,
    DIFFERENTIAL = 1
};
*/
SMART_ENUM(ADS1256Mode,
    NUM(SINGLE_ENDED,   0),
    NUM(DIFFERENTIAL,   1)
)
/*
enum _ADS1256Channels {
	EIGHT_CHANNELS 	= 8,
	FOUR_CHANNELS 	= 4
};
*/
SMART_ENUM(ADS1256Channels,
    NUM(EIGHT_CHANNELS, 8),
    NUM(FOUR_CHANNELS,  4)
)
/*
enum _ADS1256Timings {
    DELAY_SEND      = 2,
    DELAY_CONFIGURE = 50,
    DELAY_DATA      = 10,
    DELAY_WRITE     = 5,
    DELAY_SYNC      = 5,
    DELAY_WAKEUP    = 25
};
*/
SMART_ENUM(ADS1256Timings,
    NUM(DELAY_SEND,           2),
    NUM(DELAY_CONFIGURE,      50),
    NUM(DELAY_DATA,           10),
    NUM(DELAY_WRITE,          5),
    NUM(DELAY_SYNC,           5),
    NUM(DELAY_WAKEUP,         25)
)
/* -- structs -- */


/*struct _ADS1256Data {*/
/*	ADS1256Gain        gain;           /* GAIN  */
/*	ADS1256DataRate    dataRate;       /* DATA output  speed*/
/*	ADS1256Value       adc[8];         /* ADC  Conversion value */
/*	ADS1256Channel     channel;        /* The current channel in the ISR (interrupt service routine) */
/*	ADS1256Mode        mode;           /* Scanning mode, 0 Single-ended input  8 channel
/*                                                         1 Differential input  4 channel
/*                                        */
/*};

/*
struct _ADS1256Funcs {
    ADCFuncs        _base;
    //protected
    ADS1256Value    (*value)            (ADS1256 *this, ADS1256Channel chan);
    void            (*delayUS)          (ADS1256 *this, time_t micros);
    void            (*send8Bit)         (ADS1256 *this, ADS1256RegValue data);
    void            (*cfgADC)           (ADS1256 *this, ADS1256Gain gain, ADS1256DataRate drate);
    void            (*delayData)        (ADS1256 *this);
    ADS1256RegValue (*recive8Bit)       (ADS1256 *this);
    void            (*writeReg)         (ADS1256 *this, ADS1256Register reg, ADS1256RegValue data);
    ADS1256RegValue (*readReg)          (ADS1256 *this, ADS1256Register reg);
    void            (*writeCmd)         (ADS1256 *this, ADS1256Command cmd);
    void            (*setChannel)       (ADS1256 *this, ADS1256Channel chan);
    void            (*setDiffChannel)   (ADS1256 *this, ADS1256Channel chan);
    void            (*waitDrdy)         (ADS1256 *this);
    ADS1256Value    (*readData)         (ADS1256 *this);
};

struct _ADS1256MetaFuncs {
    ADCMetaFuncs _base;
};

struct _ADS1256 {
    ADC                 _base;
    ADS1256Data         _data;
};
*/

OBJECT(ADS1256,Object
        METHODS
        (
            METHOD(ADS1256Value,    value,          ADS1256Channel),
            METHOD(void,            delayUS,        time_t),
            METHOD(void,            send8Bit,       ADS1256RegValue),
            METHOD(void,            cfgADC,         ADS1256Gain,        ADS1256DataRate),
            METHOD(void,            delayData),
            METHOD(ADS1256RegValue, recive8Bit),
            METHOD(void,            writeReg,       ADS1256Register,    ADS1256RegValue),
            METHOD(ADS1256RegValue, readReg,        ADS1256Register),
            METHOD(void,            writeCmd,       ADS1256Command),
            METHOD(void,            setChannel,     ADS1256Channel),
            METHOD(void,            setDiffChannel, ADS1256Channel),
            METHOD(void,            waitDrdy),
            METHOD(ADS1256Value,    readData)
        )
)
                
OBJECT_EXPORT(ADS1256)

#endif
