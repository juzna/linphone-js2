/**********************************************************\

  Auto-generated linphoneAPI.h

\**********************************************************/

#include <string>
#include <sstream>
#include <boost/weak_ptr.hpp>
#include "JSAPIAuto.h"
#include "BrowserHost.h"
#include "linphone.h"

#include <linphonecore.h>
#include "private.h" /*coreapi/private.h, needed for LINPHONE_VERSION */


#ifndef H_linphoneAPI
#define H_linphoneAPI

class linphoneAPI : public FB::JSAPIAuto
{
public:
    linphoneAPI(const linphonePtr& plugin, const FB::BrowserHostPtr& host);
    virtual ~linphoneAPI();

    linphonePtr getPlugin();

	// Exported methods
	bool call_init(void);
    bool call_start(void);
	bool call_quit(void);
    void call_addAuthInfo(std::string username, std::string realm, std::string password);
    void call_addProxy(std::string proxy, std::string identity);
    bool call_accept(void);
    bool call_terminate(void);
	
	// Properties methods
	bool get_running(void);
	bool get_registered(void);

	
	
    void lock() { pthread_mutex_lock(&mutex); }
    void unlock() { pthread_mutex_unlock(&mutex); }
    void iterate() { if(lin) linphone_core_iterate(lin); }
    void iterateWithMutex() { lock(); iterate(); unlock(); }
	
	
	/*
	// Read/Write property ${PROPERTY.ident}
    std::string get_testString();
    void set_testString(const std::string& val);

    // Read-only property ${PROPERTY.ident}
    std::string get_version();

    // Method echo
    FB::variant echo(const FB::variant& msg);
    
    // Event helpers
    FB_JSAPI_EVENT(fired, 3, (const FB::variant&, bool, int));
    FB_JSAPI_EVENT(echo, 2, (const FB::variant&, const int));
    FB_JSAPI_EVENT(notify, 0, ());

    // Method test-event
    void testEvent(const FB::variant& s); */

private:
    linphoneWeakPtr m_plugin;
    FB::BrowserHostPtr m_host;

    LinphoneCore *lin;	// Linphone core object
    LinphoneCoreVTable lin_vtable; // Linphone callback methods table
    pthread_mutex_t mutex; // Mutex for serializing core calls
    ortp_thread_t iterate_thread; // Iterate thread
    
public:
	bool iterate_thread_running; // Indicate, when should iterate thread stop
};

static void stub () {}
static void* iterate_thread_main(void *p); // Main function for iterate thread


#endif // H_linphoneAPI

