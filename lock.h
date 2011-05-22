
#include <pthread.h>

#ifndef H_lock
#define H_lock


/* Lock class */
class Lock {
public:
  Lock(pthread_mutex_t *plock, const char *from);
  virtual ~Lock();

private:
  pthread_mutex_t* _lock;
  const char *_from;
};


#endif
