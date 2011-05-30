#include <string>
#include <sstream>
#include "JSAPIAuto.h"
#include "BrowserHostWrapper.h"
#include "types.h"
#include <stdio.h>

#ifndef H_VideoWindowAPI
#define H_VideoWindowAPI


class VideoWindowAPI : public FB::JSAPIAuto
{
public:
    VideoWindowAPI(const linphonePtr& plugin, const FB::BrowserHostPtr& host);
    virtual ~VideoWindowAPI();

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

#endif // H_VideoWindowAPI

