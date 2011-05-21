/**********************************************************\

  Auto-generated linphoneAPI.cpp

\**********************************************************/

#include "JSObject.h"
#include "variant_list.h"
#include "DOM/Document.h"

#include "linphoneAPI.h"
#include "sampleAPI.h"
#include "CallAPI.h"
#include "common.h"
#include "lock.h"
#include <stdio.h>

// Original callbacks from linphone core
#define GLC if(!linphone_core_get_user_data(lc)) printf("not found linphone api\n"); else ((linphoneAPI*) linphone_core_get_user_data(lc))
static void cb_global_state_changed(LinphoneCore *lc, LinphoneGlobalState gstate, const char *msg) {
	GLC->lcb_global_state_changed(gstate, msg);
}
static void cb_call_state_changed(LinphoneCore *lc, LinphoneCall *call, LinphoneCallState cstate, const char *message) {
	GLC->lcb_call_state_changed(call, cstate, message);
}
static void cb_registration_state_changed(LinphoneCore *lc, LinphoneProxyConfig *cfg, LinphoneRegistrationState cstate, const char *message) {
	GLC->lcb_registration_state_changed(cfg, cstate, message);
}
static void cb_auth_info_requested(LinphoneCore *lc, const char *realm, const char *username) {
	GLC->lcb_auth_info_requested(realm, username);
}


///////////////////////////////////////////////////////////////////////////////
/// @fn linphoneAPI::linphoneAPI(const linphonePtr& plugin, const FB::BrowserHostPtr host)
///
/// @brief  Constructor for your JSAPI object.  You should register your methods, properties, and events
///         that should be accessible to Javascript from here.
///
/// @see FB::JSAPIAuto::registerMethod
/// @see FB::JSAPIAuto::registerProperty
/// @see FB::JSAPIAuto::registerEvent
///////////////////////////////////////////////////////////////////////////////
linphoneAPI::linphoneAPI(const linphonePtr& plugin, const FB::BrowserHostPtr& host)
  : m_plugin(plugin), m_host(host), _sample(),
    _call_list(), _call_list_counter(0)
{
  printf("creating new plugin instance\n");
  
  // Register exported methods
  rmethod(init);
  rmethod(start);
  rmethod(quit);
  rmethod(addAuthInfo);
  rmethod(addProxy);
  rmethod(accept);
  rmethod(terminate);
  rmethod(call);
  rmethod(disableLogs);
  rmethod(enableLogs);
  rmethod(enableStun);
  
  // Register exported properties
  rpropertyg(running);
  rpropertyg(registered);
  rpropertyg(sample);
  rpropertyg(inCall);
  rpropertyg(logging);
  rproperty(videoEnabled);
  rproperty(videoPreviewEnabled);
  rproperty(videoNativeId);
  rpropertyg(videoFilterName);
  
  
  // Initialize mutex
  pthread_mutex_init(&mutex, NULL);  
  
  // Initialize as null pointer
  lin = NULL;
  iterate_thread = NULL;
  iterate_thread_running = false;
}

///////////////////////////////////////////////////////////////////////////////
/// @fn linphoneAPI::~linphoneAPI()
///
/// @brief  Destructor.  Remember that this object will not be released until
///         the browser is done with it; this will almost definitely be after
///         the plugin is released.
///////////////////////////////////////////////////////////////////////////////
linphoneAPI::~linphoneAPI()
{
  printf("deallocating plugin instance\n");
  
  // Quit first
   call_quit();
}

///////////////////////////////////////////////////////////////////////////////
/// @fn linphonePtr linphoneAPI::getPlugin()
///
/// @brief  Gets a reference to the plugin that was passed in when the object
///         was created.  If the plugin has already been released then this
///         will throw a FB::script_error that will be translated into a
///         javascript exception in the page.
///////////////////////////////////////////////////////////////////////////////
linphonePtr linphoneAPI::getPlugin()
{
    linphonePtr plugin(m_plugin.lock());
    if (!plugin) {
        throw FB::script_error("The plugin is invalid");
    }
    return plugin;
}

/**
 * Initialize structures
 */
bool linphoneAPI::call_init(void) {    
    Lock lck(&mutex, NULL);
    if(lin) return false; // Already initialized	
    
	  
    // Initialize callback table
    memset(&lin_vtable, 0, sizeof(LinphoneCoreVTable));
    lin_vtable.global_state_changed = cb_global_state_changed;
    lin_vtable.call_state_changed = cb_call_state_changed;
    lin_vtable.registration_state_changed = cb_registration_state_changed;
    lin_vtable.auth_info_requested = cb_auth_info_requested;
    
    char configfile_name[PATH_MAX];
    snprintf(configfile_name, PATH_MAX, "%s/.linphonerc", getenv("HOME"));
    
    // Create linphone core
    lin = linphone_core_new(&lin_vtable, NULL, NULL, (void *) this);
    if(linphone_core_get_user_data(lin) != this) {
      printf("you have old version of linphone core\n");
      return false;
    }
    
    // Disable logs by default, can be enabled by enableLogs() method
    linphone_core_disable_logs();
    
    // TODO: remove later
    linphone_core_set_sip_port(lin, 6060);
	
    return true;
}



/**
 * Start main thread
 */
bool linphoneAPI::call_start(void) {
	if(!lin) return false;
        
    // Initialize iterating thread
    iterate_thread_running = true;
    ortp_thread_create(&iterate_thread,NULL, iterate_thread_main, this);
    
    return true;
}

/**
 * Thread, which iterates in linphone core each 20ms
 */
static void *iterate_thread_main(void*p){
    linphoneAPI *t = (linphoneAPI*) p; // Get main object
    printf("iterate thread started\n");
    
    while(t->iterate_thread_running) {
      t->iterateWithMutex();
      usleep(20000);
    }
    printf("iterate thread stopped\n");
}

/**
 * Quit linphone core: stop iterate thread
 */
bool linphoneAPI::call_quit(void) {
  if(!lin) return false;
  
  {
    Lock lck(&mutex, "terminate call");
    
    // Terminate call
    linphone_core_terminate_call(lin, NULL);
  }
  
  // Stop iterating
  iterate_thread_running = false;
  ortp_thread_join(iterate_thread,NULL);
  printf("iterate thread joined\n");
  
  // Close log file
  if(_logging_fp) fclose(_logging_fp);
  
  // Destroy linphone core
  linphone_core_destroy(lin);
  lin = NULL;
  
  return true;
}

/**
 * Check whether main thread is running
 */
bool linphoneAPI::get_running(void) {
  return lin && iterate_thread_running;
}

/**
 * Disable logging
 */
void linphoneAPI::call_disableLogs(void) {
	linphone_core_disable_logs();
	_logging = "";
}

/**
 * Enable logging to file (or to stdout when dash is passed)
 */
void linphoneAPI::call_enableLogs(std::string file) {
	if(file == "-") {
		linphone_core_enable_logs(stdout);
		_logging = file;
	}
	else {
		if(_logging_fp) fclose(_logging_fp); // close of file
		FILE *fp = fopen(file.c_str(), "w");
		if(fp) {
			_logging = file;
			_logging_fp = fp;
			linphone_core_enable_logs(fp);
		}
		else throw FB::script_error("Unable to open file");
	}
}

std::string linphoneAPI::get_logging(void) {
	return _logging;
}

void linphoneAPI::call_enableStun(std::string server) {
	linphone_core_set_firewall_policy(lin, LinphonePolicyUseStun);
    linphone_core_set_stun_server(lin, server.c_str());
}




/**
 * Add authentication info
 */
void linphoneAPI::call_addAuthInfo(std::string username, std::string realm, std::string passwd) {
  Lock lck(&mutex, "add auth info");
  LinphoneAuthInfo *info;
  
  info = linphone_auth_info_new(username.c_str(), NULL, passwd.c_str(), NULL, realm.c_str());
  linphone_core_add_auth_info(lin, info);
  linphone_auth_info_destroy(info);
}

/**
 * Add proxy server
 */
void linphoneAPI::call_addProxy(std::string proxy, std::string identity) {
    Lock lck(&mutex, "add proxy");
    
    LinphoneProxyConfig *cfg;
    cfg = linphone_proxy_config_new();
    
    linphone_proxy_config_set_identity(cfg, identity.c_str());
    linphone_proxy_config_set_server_addr(cfg, proxy.c_str());
    linphone_proxy_config_enable_register(cfg, TRUE);

    // finish the config
    linphone_core_add_proxy_config(lin, cfg);

    // set config as default proxy
    linphone_core_set_default_proxy(lin, cfg);
}

/**
 * Accept incoming call
 */
bool linphoneAPI::call_accept(void) {
  Lock lck(&mutex, "accept");
  return linphone_core_accept_call(lin, NULL) != -1;
}

/**
 * Terminate actual call
 */
bool linphoneAPI::call_terminate(void) {
  Lock lck(&mutex, "terminate");
  return linphone_core_terminate_call(lin, NULL) != -1;
}

/**
 * Check whether we're registered to proxy
 */
bool linphoneAPI::get_registered(void) {
  Lock lck(&mutex, "get-registered");
  if(!lin) return false;
  
  LinphoneProxyConfig *cfg;
  int ret;
  
  linphone_core_get_default_proxy(lin, &cfg); // Get default proxy
  if(!cfg) {
    printf("get registered: no proxy present\n");
    return false;
  }
  ret = linphone_proxy_config_is_registered(cfg);
  
  printf("get registered: got cfg %u; %d\n", (void *) cfg, ret);

  if(cfg) return ret > 0;
  else return false;
}

FB::JSAPIPtr linphoneAPI::_add_call(LinphoneCall *call) {
	unsigned long index = ++_call_list_counter;
	printf("Creating new call under index %u\n", index);

	CallAPIPtr cptr = boost::make_shared<CallAPI>(m_host, &mutex, &lin, call);  
	
	// Store in map
	_call_list.insert(std::pair<int, CallAPIPtr>(index, cptr));
	
	// Set linphone pointer
	linphone_call_set_user_pointer(call, (void*) index);
	
	return cptr;
}


	

/**
 * Initialize new call
 */
FB::JSAPIPtr linphoneAPI::call_call(std::string uri) {
  Lock lck(&mutex, "call");
  LinphoneCall *call = linphone_core_invite(lin, uri.c_str());
    
  if(!call) {
	  printf("Unable to place call\n");
	//  throw FB::script_error("Unable to place call");
    //return (FB::JSAPIPtr) NULL; //boost::make_shared<CallAPI>(m_host, &_lin, call); // NULL;
  }
  else {
	printf("Call initialized, storing to map\n");
	return _add_call(call);//boost::make_shared<CallAPI>(m_host, &mutex, &lin, call);  
  }
}

FB::JSAPIPtr linphoneAPI::get_sample(void) {
  // Create instance if not yet exists
  if(!_sample.use_count()) {
	_sample = boost::make_shared<sampleAPI>(m_host); //reset(new sampleAPI(m_host));
  }
  
  return _sample;
}



// Events
void linphoneAPI::lcb_global_state_changed(LinphoneGlobalState gstate, const char *msg) {
	fire_globalStateChanged(gstate, msg);
}

void linphoneAPI::lcb_call_state_changed(LinphoneCall *call, LinphoneCallState cstate, const char *message) {
	unsigned long index = (unsigned long) linphone_call_get_user_pointer(call);
	if(!index) {
		printf("Call %u has no stored index to CallAPI (%d: %s)\n", call, cstate, message);
		return;
	}
	if(!_call_list.count(index)) {
		printf("CallAPI %d not found in registry\n", index);
		return;
	}
	
	printf("Call %d state changed to %d - %s\n", index, cstate, message);
	fire_callStateChanged(_call_list[index], cstate, message);
	
	// Call can be released
	if(cstate == LinphoneCallReleased) {
		// Remove from call list
		linphone_call_set_user_pointer(call, (void*) NULL);
		_call_list.erase(index);
	}
}

void linphoneAPI::lcb_registration_state_changed(LinphoneProxyConfig *cfg, LinphoneRegistrationState cstate, const char *message) {
	// TODO: find proxy object and fire event with it
	fire_registrationStateChanged(cstate, message);
}

void linphoneAPI::lcb_auth_info_requested(const char *realm, const char *username) {
	fire_authInfoRequested(realm, username);
}

/*

// Method echo
FB::variant linphoneAPI::echo(const FB::variant& msg)
{
    static int n(0);
    fire_echo(msg, n++);
    return msg;
}

*/

