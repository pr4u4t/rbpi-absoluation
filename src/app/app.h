#ifndef _APP_H_
#define _APP_H_

#include <factory.h>
#include <shell.h>
#include <terminal.h>
#include <hardware.h>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <poll.h>

#define MAX_CLIENTS 25
#define MAX_QUEUE   5
#define EVENT_TIMEOUT  100
#define READ_EVENT  POLLIN 
//| POLLRDNORM | POLLRDBAND | POLLPRI
#define WRITE_EVENT POLLOUT 
//| POLLWRNORM | POLLWRBAND

typedef struct _App App;
typedef struct _AppData AppData;
typedef struct _AppFuncs AppFuncs;
typedef struct _AppMetaFuncs AppMetaFuncs;

typedef struct pollfd PollFD;

struct _AppData {
    int         _fd;
    int         _lqueue;
    Factory*    _factory;
    PollFD      _fds[MAX_CLIENTS];
};

struct _AppFuncs {
    ObjectFuncs _base;
    bool   (*open)         (App *this, Factory *factory);
	void   (*close)        (App *this); 
	int    (*exec)         (App *this);
    void   (*accept)       (App* this);
    void   (*handleData)   (App* this, int fd);
    void   (*handleAccept) (App* this, int fd);
    void   (*handleError)  (App* this, int fd);
    void   (*handleEnd)    (App* this, int fd);
};

struct _AppMetaFuncs {
    ObjectMetaFuncs _base;
    ObjectMetaFunc  open;
    ObjectMetaFunc  close;
    ObjectMetaFunc  exec;
};

struct _App {
    Object  _base;
	AppData _data;
};

App* get_app(void);

#endif
