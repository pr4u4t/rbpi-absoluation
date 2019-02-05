#include <FileTerminal.h>

static bool FileTerminal_open (FileTerminal* this, void *in, void *out, TerminalStreamMode mode);
static int FileTerminal_read (FileTerminal* this, uint8_t *buff, size_t size);
static int FileTerminal_write (FileTerminal* this, uint8_t *buff, size_t size);
static void FileTerminal_close (FileTerminal* this);
static int FileTerminal_readf (FileTerminal* this, const uint8_t* format, uint8_t* buff);
static int FileTerminal_writef (FileTerminal* this, const uint8_t* format, ... );
static void FileTerminal_setStreamMode(FileTerminal* this, TerminalStreamMode mode);
static TerminalStreamMode FileTerminal_streamMode(FileTerminal* this);
static void FileTerminal_flush (FileTerminal* this);
static void* FileTerminal_in (FileTerminal* this);
static void* FileTerminal_out (FileTerminal* this);

static FileTerminalFuncs FileTerminal_vft = {
    ._base = {
        .open           = (bool (*) (Terminal*, void*, void*, TerminalStreamMode)) &FileTerminal_open,
        .close          = (void (*) (Terminal*)) &FileTerminal_close,
        .read          	= (int  (*) (Terminal*, uint8_t*, size_t)) &FileTerminal_read,
        .write          = (int  (*) (Terminal*, const uint8_t* , size_t)) &FileTerminal_write,
        .writef         = (int  (*) (Terminal*, const uint8_t*, ... )) &FileTerminal_writef,
        .readf          = (int  (*) (Terminal*, const uint8_t*, uint8_t*)) &FileTerminal_readf,
        .setStreamMode  = (void (*) (Terminal*, TerminalStreamMode)) &FileTerminal_setStreamMode,
        .streamMode     = (TerminalStreamMode (*) (Terminal*)) &FileTerminal_streamMode,
        .flush          = (void (*) (Terminal*)) &FileTerminal_flush,
        .in             = (void* (*) (Terminal*)) &FileTerminal_in,
        .out            = (void* (*) (Terminal*)) &FileTerminal_out
    }
};

static FileTerminalMetaFuncs FileTerminalMeta_vft = {};

OBJECT_REGISTER(FileTerminal,OBJECT_TERMINAL,FileTerminal_vft,FileTerminalMeta_vft,Terminal)

static bool FileTerminal_open (FileTerminal* this,void* in, void* out, TerminalStreamMode mode){
	if(!this || !in || !out || !call(Object,this,open)) return false;
    this->_data._in = in;
    this->_data._out = out;
    call(Terminal,this,setStreamMode,mode);
	return true;
}

static int FileTerminal_read (FileTerminal* this, uint8_t* buff,size_t size){
    return (this && size && buff) ? fread(buff, sizeof(uint8_t), size, this->_data._in) : 0;
}

static int FileTerminal_write (FileTerminal* this, uint8_t* buff, size_t size){
    return (this && size && buff) ? fwrite(buff, sizeof(uint8_t), size, this->_data._out) : 0;
}

static int FileTerminal_readf (FileTerminal* this, const uint8_t* format, uint8_t* buff){
    return (this && format && buff) ? fscanf(this->_data._in,format,buff) : 0;
}

static int FileTerminal_writef (FileTerminal* this, const uint8_t* format, ... ){
    int ret;
    if (!this || !format) return 0;
    
    va_list argp;
    va_start(argp, format);
    ret = vfprintf(this->_data._out,format,argp);
    va_end(argp);
    
    return ret;
}

void FileTerminal_close (FileTerminal* this){
    if(!this || !this->_data._in) return;
    if(this->_data._in) fclose(this->_data._in);
    if(this->_data._in) fclose(this->_data._out);
    call(Object,this,close);
}

static void FileTerminal_setStreamMode(FileTerminal* this, TerminalStreamMode mode){
    int flags;
    
    if(!this || !this->_data._in) return;
    
    flags = fcntl(static_cast(FILE*,this->_data._in)->_fileno, F_GETFL, 0);
    fcntl(static_cast(FILE*,this->_data._in)->_fileno, F_SETFL, (mode == STREAM_BLOCK) ? flags & ~O_NONBLOCK : flags | O_NONBLOCK);

    flags = fcntl(static_cast(FILE*,this->_data._out)->_fileno, F_GETFL, 0);
    fcntl(static_cast(FILE*,this->_data._in)->_fileno, F_SETFL, (mode == STREAM_BLOCK) ? flags & ~O_NONBLOCK : flags | O_NONBLOCK);
}

static TerminalStreamMode FileTerminal_streamMode(FileTerminal* this){
    return (this && this->_data._in) ? ((fcntl(static_cast(FILE*,this->_data._in)->_fileno, F_GETFL, 0) | O_NONBLOCK) ? STREAM_NON_BLOCK : STREAM_BLOCK ) : 0; 
}

static void FileTerminal_flush(FileTerminal* this){
    if(this) fflush(static_cast(FILE*,this->_data._out));
}    

static void* FileTerminal_in (FileTerminal* this){
    return (this) ? this->_data._in : NULL;
}

static void* FileTerminal_out (FileTerminal* this){
    return (this) ? this->_data._out : NULL;    
}
