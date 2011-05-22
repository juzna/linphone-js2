#include "JSObject.h"
#include "variant_list.h"
#include "DOM/Document.h"

#include "CallAPI.h"
#include "AddressAPI.h"


#define mmethod(name) make_method(this, &CallAPI::name)
#define rmethod(name) registerMethod(#name, make_method(this, &CallAPI::call_##name))
#define rmethod2(name, func) registerMethod(#name, make_method(this, &CallAPI::call_##func))
#define rproperty(name) registerProperty(#name, make_property(this, &CallAPI::get_##name, &CallAPI::set_##name))
#define rpropertyg(name) registerProperty(#name, make_property(this, &CallAPI::get_##name))
#define Lo Lock lck(_mutex, NULL);


CallAPI::CallAPI(const FB::BrowserHostPtr& host, pthread_mutex_t *mutex, LinphoneCore **lin, LinphoneCall *call)
 : m_host(host), _mutex(mutex), _lin(lin), _call(call), _remote()
{
  printf("CallAPI instance: %p\n", call);
  linphone_call_ref(call);
  
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
  rpropertyg(remote);
  rpropertyg(remote_str);
}

CallAPI::~CallAPI()
{
  printf("deallocating CallAPI instance\n");
  linphone_call_set_user_pointer(_call, (void*) NULL);
  linphone_call_unref(_call);
}

bool CallAPI::call_accept(void) {
	Lo; return linphone_core_accept_call(*_lin, _call);
}

bool CallAPI::call_terminate(void) {
	Lo; return linphone_core_terminate_call(*_lin, _call);
}

bool CallAPI::call_pause(void) {
	Lo; return linphone_core_pause_call(*_lin, _call);
}

bool CallAPI::call_resume(void) {
	Lo; return linphone_core_resume_call(*_lin, _call);
}

// Property manipulation methods
int CallAPI::get_state(void) {
	Lo; return linphone_call_get_state(_call);
}

int CallAPI::get_reason(void) {
	Lo; return linphone_call_get_reason(_call);
}

int CallAPI::get_duration(void) {
	Lo; return linphone_call_get_duration(_call);
}

bool CallAPI::get_paused(void) {
	Lo; return linphone_call_get_state(_call) == LinphoneCallPaused;
}

// Remote
FB::JSAPIPtr CallAPI::get_remote(void) {
	Lo;
	if(!_remote.use_count()) {
		const LinphoneAddress* addr = linphone_call_get_remote_address(_call);
		addr = linphone_address_clone(addr);
		printf("Got original addr pointer: %p\n", addr);
		_remote = boost::make_shared<AddressAPI>(m_host, _mutex, (LinphoneAddress*) addr, true);
		printf("Got object addr pointer\n");
	}
	
	return _remote;
}

std::string CallAPI::get_remote_str(void) {
	Lo; return linphone_call_get_remote_address_as_string(_call);
}
