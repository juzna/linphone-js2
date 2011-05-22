
#include <stdio.h>
#include "lock.h"

// Initialize a lock
Lock::Lock(pthread_mutex_t *plock, const char *from) {
	if(from) {
	  printf("locking mutex: ");
	}
	
	pthread_mutex_lock(_lock = plock);
	
	if(_from = from) {
	  printf("locked from %s\n", from); 
	}
}
 
  // Destroy lock (unlock it)
Lock::~Lock() {
	if(_from) {
	  printf("unlocking mutex: %s\n", _from);
	}

	pthread_mutex_unlock(_lock); 
}
