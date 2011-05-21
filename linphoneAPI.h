/**********************************************************\

  Auto-generated linphoneAPI.h

\**********************************************************/

#include <string>
#include <sstream>
#include <boost/weak_ptr.hpp>
#include "JSAPIAuto.h"
#include "BrowserHost.h"
#include "linphone.h"
#include "types.h"

#include <linphonecore.h>
#include "private.h" /*coreapi/private.h, needed for LINPHONE_VERSION */


#ifndef H_linphoneAPI
#define H_linphoneAPI

#include "lock.h"
#define Lo Lock lck(&mutex, NULL);


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
	FB::JSAPIPtr call_call(std::string uri);
	void call_disableLogs(void);
	void call_enableLogs(std::string);
	void call_enableStun(std::string);
	void call_embedVideo(void);
	
	// Properties methods
	bool get_running(void);
	bool get_registered(void);
	FB::JSAPIPtr get_sample(void);
	std::string get_logging(void);
	bool get_inCall(void) { Lo; return linphone_core_in_call(lin); }
	bool get_videoEnabled(void) { Lo; return linphone_core_video_enabled(lin); }
	void set_videoEnabled(bool x) { Lo; linphone_core_enable_video(lin, x, x); }
	bool get_videoPreviewEnabled(void) { Lo; return linphone_core_video_preview_enabled(lin); }
	void set_videoPreviewEnabled(bool x) { Lo; linphone_core_enable_video_preview(lin, x); }
	unsigned long get_videoNativeId(void) { Lo; return linphone_core_get_native_video_window_id(lin); }
	void set_videoNativeId(unsigned long x) { Lo; linphone_core_set_native_video_window_id(lin, x); }
	unsigned long get_videoPreviewNativeId(void) { Lo; return linphone_core_get_native_preview_window_id(lin); }
	void set_videoPreviewNativeId(unsigned long x) { Lo; linphone_core_set_native_preview_window_id(lin, x); }	
	
	std::string get_videoFilterName(void) {
		Lo;
		if (lin->previewstream) {
			return lin->previewstream->display_name;
		}
	}

	unsigned long get_pluginWindowId(void);

	
	
    void lock() { pthread_mutex_lock(&mutex); }
    void unlock() { pthread_mutex_unlock(&mutex); }
    void iterate() { if(lin) linphone_core_iterate(lin); }
    void iterateWithMutex() { lock(); iterate(); unlock(); }
	
	
	// Event helpers
	FB_JSAPI_EVENT(globalStateChanged, 2, (const int, const std::string)) // int state, string msg
	FB_JSAPI_EVENT(callStateChanged, 3, (FB::JSAPIPtr, const int, const std::string)) // CallAPI call, int state, string msg
	FB_JSAPI_EVENT(registrationStateChanged, 2, (const int, const std::string)) // (ProxyAPI call,) int state, string msg
	FB_JSAPI_EVENT(authInfoRequested, 2, (std::string, std::string)); // string realm, string username
	
	
	/*

    // Method echo
    FB::variant echo(const FB::variant& msg);
    
    // Event helpers
    FB_JSAPI_EVENT(fired, 3, (const FB::variant&, bool, int));
    FB_JSAPI_EVENT(echo, 2, (const FB::variant&, const int));
    FB_JSAPI_EVENT(notify, 0, ());
*/


	// Callbacks from linphone core
	void lcb_global_state_changed(LinphoneGlobalState gstate, const char *msg);
	void lcb_call_state_changed(LinphoneCall *call, LinphoneCallState cstate, const char *message);
	void lcb_registration_state_changed(LinphoneProxyConfig *cfg, LinphoneRegistrationState cstate, const char *message);
	void lcb_auth_info_requested(const char *realm, const char *username);
	

	// Internal
	FB::JSAPIPtr _add_call(LinphoneCall *call);

private:
    linphoneWeakPtr m_plugin;
    FB::BrowserHostPtr m_host;

    LinphoneCore *lin;	// Linphone core object
    LinphoneCoreVTable lin_vtable; // Linphone callback methods table
    pthread_mutex_t mutex; // Mutex for serializing core calls
    ortp_thread_t iterate_thread; // Iterate thread
    
    std::string _logging;
    FILE *_logging_fp;
    FB::JSAPIPtr _sample;
    std::map<unsigned long, CallAPIPtr > _call_list; // We handle referenced calls here
    unsigned long _call_list_counter;
    
public:
	bool iterate_thread_running; // Indicate, when should iterate thread stop
};

static void stub () {}
static void* iterate_thread_main(void *p); // Main function for iterate thread


#endif // H_linphoneAPI

