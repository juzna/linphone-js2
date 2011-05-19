/**********************************************************\

  Auto-generated linphoneAPI.cpp

\**********************************************************/

#include "JSObject.h"
#include "variant_list.h"
#include "DOM/Document.h"

#include "linphoneAPI.h"
#include "common.h"
#include "lock.h"

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
linphoneAPI::linphoneAPI(const linphonePtr& plugin, const FB::BrowserHostPtr& host) : m_plugin(plugin), m_host(host)
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
  
  // Register exported properties
  rpropertyg(running);
  rpropertyg(registered);
  
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
/*    lin_vtable.show 			= (ShowInterfaceCb) stub;
    lin_vtable.inv_recv 		= mcb(lcb_call_received);
    lin_vtable.bye_recv 		= mcb(lcb_bye_received);
    lin_vtable.notify_recv 		= (NotifyReceivedCb) stub;
    lin_vtable.new_unknown_subscriber 	= (NewUnknownSubscriberCb) stub;
    lin_vtable.auth_info_requested 	= mcb(lcb_auth_info_requested);
    lin_vtable.display_status		= mcb(lcb_display_status);
    lin_vtable.display_message		= mcb(lcb_display_something);
    lin_vtable.display_warning		= mcb(lcb_display_warning);
    lin_vtable.display_url		= mcb(lcb_display_url);
    lin_vtable.display_question		= (DisplayQuestionCb) stub;
    lin_vtable.call_log_updated		= (CallLogUpdated) stub;
    lin_vtable.text_received		= mcb(lcb_text_received);
    lin_vtable.general_state		= mcb(lcb_general_state);
    lin_vtable.dtmf_received		= mcb(lcb_dtmf_received);
    lin_vtable.refer_received		= (ReferReceived) stub;
    lin_vtable.buddy_info_updated	= (BuddyInfoUpdated) stub;
*/    
    char configfile_name[PATH_MAX];
    snprintf(configfile_name, PATH_MAX, "%s/.linphonerc", getenv("HOME"));
    
    // Create linphone core
    lin = linphone_core_new(&lin_vtable, NULL, NULL, (void *) this);
    if(linphone_core_get_user_data(lin) != this) {
      printf("you have old version of linphone core\n");
      return false;
    }
    
    // TODO: move this to separate methods
    // Disable/enable logs
    //linphone_core_disable_logs();
    linphone_core_enable_logs(stdout);
    
    linphone_core_set_firewall_policy(lin, LinphonePolicyUseStun);
    linphone_core_set_stun_server(lin, "stun.helemik.cz");
    
    linphone_core_enable_video(lin, false, false);
    
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
  {
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
    
  //FireEvent("onProxyAdded", FB::variant_list_of(proxy)(identity));
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




/*

// Read/Write property testString
std::string linphoneAPI::get_testString()
{
    return m_testString;
}
void linphoneAPI::set_testString(const std::string& val)
{
    m_testString = val;
}

// Read-only property version
std::string linphoneAPI::get_version()
{
    return "CURRENT_VERSION";
}

// Method echo
FB::variant linphoneAPI::echo(const FB::variant& msg)
{
    static int n(0);
    fire_echo(msg, n++);
    return msg;
}

void linphoneAPI::testEvent(const FB::variant& var)
{
    fire_fired(var, true, 1);
}

*/

