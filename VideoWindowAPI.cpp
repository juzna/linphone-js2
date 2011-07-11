#include "JSObject.h"
#include "variant_list.h"
#include "DOM/Document.h"

#include "VideoWindowAPI.h"
// #include "common.h"
#include "lock.h"



#define mmethod(name) make_method(this, &VideoWindowAPI::name)
#define rmethod(name) registerMethod(#name, make_method(this, &VideoWindowAPI::call_##name))
#define rmethod2(name, func) registerMethod(#name, make_method(this, &VideoWindowAPI::call_##func))
#define rproperty(name) registerProperty(#name, make_property(this, &VideoWindowAPI::get_##name, &VideoWindowAPI::set_##name))
#define rpropertyg(name) registerProperty(#name, make_property(this, &VideoWindowAPI::get_##name))



VideoWindowAPI::VideoWindowAPI(const linphonePtr& plugin, const FB::BrowserHostPtr& host) :
    m_plugin(plugin), m_host(host)
{
  printf("VideoWindowAPI instance\n");

  // Register exported methods
  rmethod(test);

  // Register properties
  rproperty(x);
  rpropertyg(pluginWindowId);
}

VideoWindowAPI::~VideoWindowAPI()
{
  printf("deallocating VideoWindowAPI instance\n");
}

linphonePtr VideoWindowAPI::getPlugin()
{
    linphonePtr plugin(m_plugin.lock());
    if (!plugin) {
        throw FB::script_error("The plugin is invalid");
    }
    return plugin;
}



bool VideoWindowAPI::call_test(std::string uri) {
  return true;
}

std::string VideoWindowAPI::get_x(void) {
 return _x;
}

void VideoWindowAPI::set_x(std::string x) {
  _x = x;
}

unsigned long VideoWindowAPI::get_pluginWindowId(void) {
	return getPlugin()->getNativeWindowId();
}
