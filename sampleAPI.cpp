#include "JSObject.h"
#include "variant_list.h"
#include "DOM/Document.h"

#include "sampleAPI.h"
// #include "common.h"
#include "lock.h"



#define mmethod(name) make_method(this, &sampleAPI::name)
#define rmethod(name) registerMethod(#name, make_method(this, &sampleAPI::call_##name))
#define rmethod2(name, func) registerMethod(#name, make_method(this, &sampleAPI::call_##func))
#define rproperty(name) registerProperty(#name, make_property(this, &sampleAPI::get_##name, &sampleAPI::set_##name))
#define rpropertyg(name) registerProperty(#name, make_property(this, &sampleAPI::get_##name))



sampleAPI::sampleAPI(const FB::BrowserHostPtr& host) : m_host(host)
{
  printf("sampleAPI instance\n");
  
  // Register exported methods
  rmethod(test);
  
  // Register properties
  rproperty(x);
}

sampleAPI::~sampleAPI()
{
  printf("deallocating sampleAPI instance\n");
}

bool sampleAPI::call_test(std::string uri) {
  return true;
}

std::string sampleAPI::get_x(void) {
 return _x;
}

void sampleAPI::set_x(std::string x) {
  _x = x;
}
