#include <main.h>

typedef struct _AppArguments AppArguments;
struct _AppArguments {
	const char *plugins;
};

static AppArguments appArgs;
static const char shortOpts[] = "hvp:";
static struct option longOpts[] = 
{
    {"help", 	no_argument, 		0, 'h'},
    {"version", no_argument, 		0, 'v'},
    {"plugins", required_argument,  0, 'p'},
    {0,         0,                  0,  0 }
};

static AppArguments getArguments(int argc,char** argv);
#ifdef _GNU_SOURCE
static void sig_sigsegv(int nSignum, siginfo_t* si, void* vcontext);
#endif

//#include <collections.h>

int main(int argc, char **argv)
{
    MLoader* factory = 0; 
    App* app = 0;
    int ret = 0;
    char* msg = 0;
   /* 
    //ITERATORS
    printf("Array testing<<\r\n");
    printf("\tcreate:\r\n");
    Array* vec = Array_new(sizeof(int),10);
    printf("\tadd:\r\n");
    call(Array,vec,set,0,static_cast(void*,0));
    call(Array,vec,set,1,static_cast(void*,1));
    call(Array,vec,set,2,static_cast(void*,2));
    printf("\tget:\r\n");
    printf("\t\t%d\r\n",call(Array,vec,at,0));
    printf("\t\t%d\r\n",call(Array,vec,at,1));
    printf("\t\t%d\r\n",call(Array,vec,at,2));
    printf("\titerator:\r\n");
    Iterator begin = call(Iterable,vec,begin);
    printf("\t\t%d %d\r\n",begin._base,begin._state);
    Iterator end = call(Iterable,vec,end);
    printf("\t\t%d %d\r\n",end._base,end._state);
    printf("chuj\r\n",call(Iterator,begin,next));

    //    printf("iter\r\n");
    printf(">>\r\n");
    
    return 0;
  */  
    appArgs = getArguments(argc,argv);

#ifdef _GNU_SOURCE
    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_flags = SA_SIGINFO;
    action.sa_sigaction = sig_sigsegv;
    sigaction(SIGSEGV, &action, NULL);
#endif
    
    if(!(factory = get_factory()))
        ERROR_JUMP(1,"Failed to get module factory");
    
    if(!call(Factory,factory,open,appArgs.plugins)) 
    	ERROR_JUMP(2,"Failed to open module loader"); 
  
    if(!(app = get_app()))
        ERROR_JUMP(7,"Failed to get app instance");
    
    if(!call(App,app,open,static_cast(Factory*,factory))) 
        ERROR_JUMP(7,"Failed to open App");
    
    if(!call(App,app,exec))
        ERROR_JUMP(8,"Application exec failed");
    
EXIT:
    if(factory) call(Factory,factory,close);

    if(app)     call(App,app,close);
    if(msg) printf("Error occured: %s\r\n",msg);
    return ret;
}

static AppArguments getArguments(int argc,char** argv){
    AppArguments ret;    
    int optIndex = 0;
    int opt = 0;
    
    for(;;){
    	if((opt = getopt_long (argc, argv, shortOpts,longOpts, &optIndex)) == -1)
    		break;

    switch(opt) {
            case 'h':
                printf("Usage: %s -p plugins_directory\r\n",argv[0]);
                exit(0);
            case 'v':
                printf("%s %d\r\n",_NAME_,_VERSION_);
                exit(0);
            case 'p':
                ret.plugins = optarg;
                break;
        }
    }
    
    return ret;
}

#ifdef _GNU_SOURCE
static void sig_sigsegv(int nSignum, siginfo_t* si, void* vcontext){
    ucontext_t* context = (ucontext_t*)vcontext;
    
    #ifdef __x86_64__
        context->uc_mcontext.gregs[REG_RIP] += sizeof(int); 
    #elif __i386__
        context->uc_mcontext.gregs[REG_EIP] += sizeof(int);
    #elif __arm__
        context->uc_mcontext.arm_pc += sizeof(int);
    #else
        #error "Unknown architecture"
    #endif
        
    write(2,"sigsegv\r\n",strlen("sigsegv\r\n"));
}
#endif
