#include <app.h>

static int APP_exec (App *this);
static bool APP_open (App *this, Factory *factory);
static void APP_close (App *this);
static void APP_accept (App *this);
static void APP_handleData   (App* this, int fd);
static void APP_handleAccept (App* this, int fd);
static void APP_handleError  (App* this, int fd);
static void APP_handleEnd    (App* this, int fd);

App* get_app(void);

static AppFuncs App_vft = (AppFuncs) {
    .exec	        = &APP_exec,
    .open           = &APP_open,
    .close 	        = &APP_close,
    .handleData     = &APP_handleData,
    .handleAccept   = &APP_handleAccept,
    .handleError    = &APP_handleError,
    .handleEnd      = &APP_handleEnd
};

static AppMetaFuncs AppMeta_vft = {
    .open           = { ._name  = "open"   },
    .close          = { ._name  = "close"  },
    .exec           = { ._name  = "exec"   }
};

OBJECT_REGISTER(App,65000,App_vft,AppMeta_vft,Object)


static App* _app = NULL;

App* get_app(void){
    return (!_app) ? (_app = static_cast(App*,AppLD.new(&AppLD))) : _app;
}

static bool APP_open (App *this, Factory *factory){
    if(!this || !factory) return false;
    char on = 1;
    const struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(6565),
        .sin_addr   = {
            .s_addr = htonl(INADDR_ANY)
        }
    };
    
    //
    //
    
    this->_data._lqueue = 5;
    memset(this->_data._fds,0,sizeof(struct pollfd)*MAX_CLIENTS);
    
    if(!(this->_data._fd = socket(AF_INET,SOCK_STREAM,0))) return false;
    if(setsockopt(this->_data._fd, SOL_SOCKET,  SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0) { perror(0); return false; }
    if(bind(this->_data._fd, static_cast(struct sockaddr*,&server_addr), sizeof(struct sockaddr_in)) != 0) return false;
    if(listen(this->_data._fd,this->_data._lqueue) != 0) return false;

    int flags = fcntl(this->_data._fd, F_GETFL, 0);
    if (flags == -1) return false;
    flags |= O_NONBLOCK;
    if(fcntl(this->_data._fd, F_SETFL, flags) != 0) return false;
    
    this->_data._factory = factory;
    this->_data._fds[0].fd = this->_data._fd;
    this->_data._fds[0].events = READ_EVENT;    
}

static void APP_close (App *this){
}

static int APP_exec(App *this){
    if(!this || !this->_data._factory) return 1;
    
    for(;;)
    {
        /* -- handle shells without terminal - process & isr routines -- */
        call(Factory,this->_data._factory,foreach,LAMBDA(void _(void* plug,void *data){
            if(!(call(Object,plug,type) & (OBJECT_SHELL | OBJECT_HARDWARE)))  return;
            
            if(call(Object,plug,type) & OBJECT_SHELL && !call(Shell,plug,terminal)){
                call(Shell,plug,exec,0);
                return;
            }
            
            if(call(Object,plug,type) & OBJECT_HARDWARE){
                call(Hardware,plug,isr);
                return;
            }
        
        } ),0,FMODE_PLUGIN);
        
        /* -- handle sockets -- */
        if(poll(this->_data._fds, MAX_CLIENTS, EVENT_TIMEOUT) > 0){
            if(this->_data._fds[0].revents & READ_EVENT)    { call(App,this,handleAccept,0); }
            if(this->_data._fds[0].revents & POLLERR)       { call(App,this,handleError,0); }
            
            for(int i = 1; i < MAX_CLIENTS; ++i){
                if(this->_data._fds[i].revents & READ_EVENT)    { call(App,this,handleData,i); }
                if(this->_data._fds[i].revents & POLLERR)       { call(App,this,handleError,i); }
                if(this->_data._fds[i].revents & POLLHUP)       { call(App,this,handleEnd,i); }
            }
        }
        
        usleep(250);
    }

    return 0;
};

static void APP_handleData(App* this, int fd){
    if(!this) return;
    
    /* find shell->terminal() checkfd call shell->exec() */
    call(Factory,this->_data._factory,foreach,LAMBDA(void _(void* plug,void *data){
        Terminal *term = 0;
        FILE *in = 0;
        
        if(call(Object,plug,type) & OBJECT_SHELL && (term = call(Shell,plug,terminal)) 
            && (in = call(Terminal,term,in)) && fileno(in) == *static_cast(int*,data)){
            call(Shell,plug,exec,0);
        }
    } ),&this->_data._fds[fd].fd,FMODE_PLUGIN);
}

static void APP_handleAccept(App* this, int fd){
    Terminal *term = 0;
    Shell* shell = 0;
    int sock = 0;
    
    if(!this) return;
    
    if((sock = accept(this->_data._fds[fd].fd,NULL,0)) <= 0)
        return;
        
    for(int i = 1; i < MAX_CLIENTS; ++i){
        if(this->_data._fds[i].fd <= 0){
            if(!(term = static_cast(Terminal*,call(Factory,this->_data._factory,newInstance,"FileTerminal")))){ 
                /*ERROR_JUMP(3,"Failed to obtain new Terminal instance");*/ 
            }    
            
            if(!(shell = static_cast(Shell*,call(Factory,this->_data._factory,newInstance,"CommandShell")))){ 
                /*ERROR_JUMP(4,"Failed to obtain new Shell instance");*/ 
            } 

            if(!call(Terminal,term,open,fdopen(sock,"r"),fdopen(sock,"w"),STREAM_NON_BLOCK)){ 
                /*ERROR_JUMP(5,"Failed to open Terminal");*/ 
            }    

            if(!call(Shell,shell,open,term)){ 
                /*ERROR_JUMP(6,"Failed to open Shell");*/ 
            }  
            
            this->_data._fds[i].fd = sock;
            this->_data._fds[i].events = READ_EVENT;
            
            break;
        }
    }
}

static void APP_handleError(App* this, int fd){
    if(!this) return;
    printf("error\r\n");
}

static void APP_handleEnd(App* this, int fd){
    if(!this) return;

    this->_data._fds[fd].fd = 0;
    this->_data._fds[fd].revents = 0;
    
    call(Factory,this->_data._factory,foreach,LAMBDA(void _(void* plug,void *data){
        Terminal *term = 0;
        FILE *in = 0;
        
        if(call(Object,plug,type) & OBJECT_SHELL && (term = call(Shell,plug,terminal)) 
            && (in = call(Terminal,term,in)) && fileno(in) == *static_cast(int*,data)){
            call(Shell,plug,close);
        }
    } ),&this->_data._fds[fd].fd,FMODE_PLUGIN);
}
