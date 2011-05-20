#include <string>
#include <sstream>
#include "JSAPIAuto.h"
#include "BrowserHostWrapper.h"

#ifndef H_sampleAPI
#define H_sampleAPI

#include "linphoneAPI.h"


class sampleAPI : public FB::JSAPIAuto
{
public:
    sampleAPI(const FB::BrowserHostPtr& host);
    virtual ~sampleAPI();

    // Exported methods
    bool call_test(std::string uri);

    // Property manipulation methods
    std::string get_x(void);
    void set_x(std::string);
    
private:
    linphoneWeakPtr m_plugin;
    FB::BrowserHostPtr m_host;
    std::string _x;
    
};

#endif // H_sampleAPI

