#include <string>
#include <sstream>
#include "JSAPIAuto.h"
#include "BrowserHostWrapper.h"

#ifndef H_CallAPI
#define H_CallAPI

#include "linphoneAPI.h"


class CallAPI : public FB::JSAPIAuto
{
public:
    CallAPI(const FB::BrowserHostPtr&, pthread_mutex_t*, LinphoneCore **, LinphoneCall*);
    virtual ~CallAPI();

    // Exported methods
    bool call_accept(void);
    bool call_terminate(void);
    bool call_pause(void);
    bool call_resume(void);

    // Property manipulation methods
    int get_state(void);
    int get_reason(void);
    int get_duration(void);
    bool get_paused(void);
    FB::JSAPIPtr get_remote(void);
    std::string get_remote_str(void);
    
private:
//    linphoneWeakPtr m_plugin;
    FB::BrowserHostPtr m_host;
    pthread_mutex_t *_mutex;
    
    LinphoneCore **_lin;
    LinphoneCall *_call;
    FB::JSAPIPtr _remote;
};

#endif // H_CallAPI
