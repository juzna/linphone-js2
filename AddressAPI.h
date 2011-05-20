#include <string>
#include <sstream>
#include "JSAPIAuto.h"
#include "BrowserHostWrapper.h"

#ifndef H_AddressAPI
#define H_AddressAPI

#include "linphoneAPI.h"


class AddressAPI : public FB::JSAPIAuto
{
public:
    AddressAPI(const FB::BrowserHostPtr& host, pthread_mutex_t*, LinphoneAddress*, bool);
    virtual ~AddressAPI();

    // Exported methods
	std::string call_toString(void);

    // Property manipulation methods
    std::string get_scheme(void);
    void set_scheme(std::string);
    std::string get_displayName(void);
    void set_displayName(std::string);
    std::string get_username(void);
    void set_username(std::string);
    std::string get_domain(void);
    void set_domain(std::string);
    int get_port(void);
    void set_port(int);
    
private:
    linphoneWeakPtr m_plugin;
    FB::BrowserHostPtr m_host;
    pthread_mutex_t *_mutex;
    bool _isOwner; // Whether we're owner of LinphoneAddress memory
    LinphoneAddress *_address;

};

#endif // H_AddressAPI

