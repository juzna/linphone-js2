#include "JSObject.h"
#include "variant_list.h"
#include "DOM/Document.h"

#include "CallAPI.h"
// #include "common.h"
#include "lock.h"



#define mmethod(name) make_method(this, &CallAPI::name)
#define rmethod(name) registerMethod(#name, make_method(this, &CallAPI::call_##name))
#define rmethod2(name, func) registerMethod(#name, make_method(this, &CallAPI::call_##func))
#define rproperty(name) registerProperty(#name, make_property(this, &CallAPI::get_##name, &CallAPI::set_##name))
#define rpropertyg(name) registerProperty(#name, make_property(this, &CallAPI::get_##name))



CallAPI::CallAPI(const FB::BrowserHostPtr& host, LinphoneCore **lin, LinphoneCall *call)
 : m_host(host), _lin(lin), _call(call)
{
  printf("CallAPI instance: %u\n", (void *) call);
  
  // Register exported methods
  rmethod(accept);
  rmethod(terminate);
  rmethod(resume);
  rmethod(pause);
  
  // Register properties
  rpropertyg(state);
  rpropertyg(reason);
  rpropertyg(duration);
  rpropertyg(paused);
}

CallAPI::~CallAPI()
{
  printf("deallocating CallAPI instance\n");
}

bool CallAPI::call_accept(void) {
	return linphone_core_accept_call(*_lin, _call);
}

bool CallAPI::call_terminate(void) {
	return linphone_core_terminate_call(*_lin, _call);
}

bool CallAPI::call_pause(void) {
	return linphone_core_pause_call(*_lin, _call);
}

bool CallAPI::call_resume(void) {
	return linphone_core_resume_call(*_lin, _call);
}

// Property manipulation methods
int CallAPI::get_state(void) {
	return linphone_call_get_state(_call);
}

int CallAPI::get_reason(void) {
	return linphone_call_get_reason(_call);
}

int CallAPI::get_duration(void) {
	return linphone_call_get_duration(_call);
}

bool CallAPI::get_paused(void) {
	return linphone_call_get_state(_call) == LinphoneCallPaused;
}

