#include <stdio.h>
#include "JSObject.h"
#include "variant_list.h"
#include "DOM/Document.h"

#include "AddressAPI.h"



#define mmethod(name) make_method(this, &AddressAPI::name)
#define rmethod(name) registerMethod(#name, make_method(this, &AddressAPI::call_##name))
#define rmethod2(name, func) registerMethod(#name, make_method(this, &AddressAPI::call_##func))
#define rproperty(name) registerProperty(#name, make_property(this, &AddressAPI::get_##name, &AddressAPI::set_##name))
#define rpropertyg(name) registerProperty(#name, make_property(this, &AddressAPI::get_##name))
#define Lo Lock lck(_mutex, NULL);


AddressAPI::AddressAPI(const FB::BrowserHostPtr& host, pthread_mutex_t *mutex, LinphoneAddress *address, bool isOwner)
 : m_host(host), _mutex(mutex), _address(address), _isOwner(isOwner)
{
  printf("AddressAPI instance: %u\n", (void *) address);
  
  // Register exported methods
  rmethod(toString);
  
  // Register properties
  rproperty(scheme);
  rproperty(displayName);
  rproperty(username);
  rproperty(domain);
  rproperty(port);
}

AddressAPI::~AddressAPI()
{
  printf("deallocating AddressAPI instance\n");
  if(_isOwner) linphone_address_destroy(_address);
}

// Scheme
std::string AddressAPI::get_scheme(void) {
	return linphone_address_get_scheme(_address);
}

void AddressAPI::set_scheme(std::string scheme) {
	// linphone_address_set_scheme(_address, scheme.c_str());
}

// Display mane
std::string AddressAPI::get_displayName(void) {
	return linphone_address_get_display_name(_address);
}

void AddressAPI::set_displayName(std::string name) {
	linphone_address_set_display_name(_address, name.c_str());
}

// Username
std::string AddressAPI::get_username(void) {
	return linphone_address_get_username(_address);
}

void AddressAPI::set_username(std::string username) {
	linphone_address_set_username(_address, username.c_str());
}

// domain
std::string AddressAPI::get_domain(void) {
	return linphone_address_get_domain(_address);
}

void AddressAPI::set_domain(std::string domain) {
	linphone_address_set_domain(_address, domain.c_str());
}

// Port
int AddressAPI::get_port(void) {
	return linphone_address_get_port_int(_address);
}

void AddressAPI::set_port(int port) {
	linphone_address_set_port_int(_address, port);
}

// toString
std::string AddressAPI::call_toString(void) {
	return linphone_address_as_string(_address);
}
