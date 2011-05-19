

#ifndef H_lock
#define H_lock


#include <pthread.h>


/* Lock class */
class Lock {
public:
 Lock(pthread_mutex_t* plock, char* from) { if(from) printf("locking mutex: "); pthread_mutex_lock(_lock = plock); if(_from = from) printf("locked from %s\n", from); }
 ~Lock() { if(_from) printf("unlocking mutex: %s\n", _from); pthread_mutex_unlock(_lock); }

private:
 pthread_mutex_t* _lock;
 char *_from;
};


#endif