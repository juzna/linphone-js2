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
  rmethod(start);

  // Initialize mutex
  pthread_mutex_init(&mutex, NULL);  
  
  // Initialize as null pointer
  lin = NULL;
  iterate_thread = NULL;

  
  
  /*
	registerMethod("echo",      make_method(this, &linphoneAPI::echo));
    registerMethod("testEvent", make_method(this, &linphoneAPI::testEvent));

    // Read-write property
    registerProperty("testString",
                     make_property(this,
                        &linphoneAPI::get_testString,
                        &linphoneAPI::set_testString));

    // Read-only property
    registerProperty("version",
                     make_property(this,
                        &linphoneAPI::get_version)); */
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
//   call_quit();
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
* Initialie linphone core and start iterate thread
*/
bool linphoneAPI::call_start(void) {
    if(lin) return false; // Already initialized
      
    Lock lck(&mutex, NULL);
    
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
    
    // Disable/enable logs
    //linphone_core_disable_logs();
    linphone_core_enable_logs(stdout);
    
    //linphone_core_set_firewall_policy(lin, LINPHONE_POLICY_USE_STUN);
    //linphone_core_set_stun_server(lin, "stun.helemik.cz");
    
    linphone_core_enable_video(lin, false, false);
    
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
      //t->iterateWithMutex();
      usleep(20000);
    }
    printf("iterate thread stopped\n");
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

