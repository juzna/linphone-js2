
// Common macros for all
#define mmethod(name) make_method(this, &linphoneAPI::name)
#define rmethod(name) registerMethod(#name, make_method(this, &linphoneAPI::call_##name))
#define rmethod2(name, func) registerMethod(#name, make_method(this, &linphoneAPI::call_##func))
#define rproperty(name) registerProperty(#name, make_property(this, &linphoneAPI::get_##name, &linphoneAPI::set_##name))
#define rpropertyg(name) registerProperty(#name, make_property(this, &linphoneAPI::get_##name))
