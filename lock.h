
#include <pthread.h>

#ifndef H_lock
#define H_lock


/* Lock class */
class Lock {
public:
  // Initialize a lock
  Lock(pthread_mutex_t *plock, char *from) {
	if(from) {
	  printf("locking mutex: ");
	}
	
	pthread_mutex_lock(_lock = plock);
	
	if(_from = from) {
	  printf("locked from %s\n", from); 
	}
  }
 
  // Destroy lock (unlock it)
  ~Lock() {
	if(_from) {
	  printf("unlocking mutex: %s\n", _from);
	}
	
	pthread_mutex_unlock(_lock); 
  }

private:
  pthread_mutex_t* _lock;
  char *_from;
};


#endif
