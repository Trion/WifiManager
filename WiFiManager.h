/**************************************************************
   WiFiManager is a library for the ESP8266/Arduino platform
   (https://github.com/esp8266/Arduino) to enable easy
   configuration and reconfiguration of WiFi credentials using a Captive Portal
   inspired by:
   http://www.esp8266.com/viewtopic.php?f=29&t=2520
   https://github.com/chriscook8/esp-arduino-apboot
   https://github.com/esp8266/Arduino/tree/master/libraries/DNSServer/examples/CaptivePortalAdvanced
   Built by AlexT https://github.com/tzapu
   Licensed under MIT license
   inspired by:
   https;//github.com/tzapy
   modified : tmm 2020
   <br><br><strong>USER MAC: {u}</p><p style=\"font-size:10px;\">DEVICE MAC: {D}</p></strong>
 **************************************************************/

#ifndef WiFiManager_h
#define WiFiManager_h

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <memory>

extern "C" { 
    
  #include "user_interface.h"
}

const char HTTP_HEADER[] PROGMEM          = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/><title>{v}</title>";
const char HTTP_STYLE[] PROGMEM           = "<style>.msg { background:#0ae; color:#fff; text-align:center; border-radius:4px;font-size:20px ; font-family:zawgyi-one } h2 {text-align:center; } svg {float:left;margin-top: 9px}label>*{display: inline}form>*{display: block;margin-bottom: 10px}.container{margin: auto;width: 90%} @media(min-width:1200px){.container{margin: auto;width: 30%}}.c{text-align: center;} div{padding:5px;font-size:1em;} input,textarea{outline: 0;font-size: 14px;border: 1px solid #ccc;padding: 8px;width: 90%} body,textarea,input{background: 0;border-radius: 0;font: 16px sans-serif;margin: 0}textarea:focus,input:focus,select:focus{border-color: #5ab}.btn a{text-decoration: none} .q{float: right;width: 64px;text-align: right;} .btn,h2{font-size: 2em}h1{font-size: 3em; text-align:center}.btn{background: #0ae;border-radius: 4px;border: 0;color: #fff;cursor: pointer;display: inline-block;margin: 2px 0;padding: 10px 14px 11px;width: 100%}.btn:hover{background: #0ae}.btn:active,.btn:focus{background: #0ae} .l{background: url(\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAAALVBMVEX///8EBwfBwsLw8PAzNjaCg4NTVVUjJiZDRUUUFxdiZGSho6OSk5Pg4eFydHTCjaf3AAAAZElEQVQ4je2NSw7AIAhEBamKn97/uMXEGBvozkWb9C2Zx4xzWykBhFAeYp9gkLyZE0zIMno9n4g19hmdY39scwqVkOXaxph0ZCXQcqxSpgQpONa59wkRDOL93eAXvimwlbPbwwVAegLS1HGfZAAAAABJRU5ErkJggg==\") no-repeat left center;background-size: 1em;}table, td, th { border: 1px solid #ddd;text-align: left;}table {border-collapse: collapse;width: 100%;}tr:nth-child(even) {background-color: #f2f2f2;} th, td {padding: 15px;}</style>";
const char HTTP_SCRIPT[] PROGMEM          = "<script>function c(l){document.getElementById('s').value=l.innerText||l.textContent;document.getElementById('p').focus();}</script>";
const char HTTP_HEADER_END[] PROGMEM      = "</head><body><div class=\"container\">";
const char HTTP_PORTAL_OPTIONS[] PROGMEM  = "<form action=\"/wifi\" method=\"get\"><button class=\"btn\"><svg height=\"1em\" viewBox=\"0 0 192 192\" width=\"1em\" xmlns=\"http://www.w3.org/2000/svg\"><circle stroke =\"#fff\" fill=\"#ffffff\" cx=\"96\" cy=\"112\" r=\"16\"/><path stroke=\"#fff\" fill=\"#ffffff\" d=\"m96 80a39.737 39.737 0 0 1 28.284 11.716l11.316-11.316a56 56 0 0 0 -79.2 0l11.316 11.316a39.737 39.737 0 0 1 28.284-11.716z\"/><path stroke =\"#fff\" fill=\"#ffffff\" d=\"m33.775 57.775 11.313 11.313a72 72 0 0 1 101.823 0l11.315-11.313a88 88 0 0 0 -124.451 0z\"/><path stroke =\"#fff\" fill=\"#ffffff\" d=\"m96 16a103.32 103.32 0 0 1 73.539 30.461l11.315-11.314a120 120 0 0 0 -169.707 0l11.314 11.314a103.32 103.32 0 0 1 73.539-30.461z\"/></svg>Wifi Setting</button></form><br/><form action=\"/i\" method=\"get\"><button class=\"btn\"><svg height=\"1em\"  width=\"1em\" viewBox=\"0 0 330 330\" style=\"enable-background:new 0 0 330 330;\"><path stroke =\"#fff\" fill=\"#ffffff\"d=\"M165,0C74.019,0,0,74.018,0,164.999C0,255.98,74.019,330,165,330s165-74.02,165-165.001C330,74.018,255.981,0,165,0zM165,300c-74.439,0-135-60.561-135-135.001C30,90.56,90.561,30,165,30s135,60.56,135,134.999C300,239.439,239.439,300,165,300z\"/><path stroke =\"#fff\" fill=\"#ffffff\" d=\"M165.002,230c-11.026,0-19.996,8.968-19.996,19.991c0,11.033,8.97,20.009,19.996,20.009c11.026,0,19.996-8.976,19.996-20.009C184.998,238.968,176.028,230,165.002,230z\"/><path stroke =\"#fff\" fill=\"#ffffff\" d=\"M165,60c-30.342,0-55.026,24.684-55.026,55.024c0,8.284,6.716,15,15,15c8.284,0,15-6.716,15-15C139.974,101.226,151.2,90,165,90s25.027,11.226,25.027,25.024c0,13.8-11.227,25.026-25.027,25.026c-8.284,0-15,6.716-15,15V185c0,8.284,6.716,15,15,15s15-6.716,15-15v-17.044c23.072-6.548,40.027-27.79,40.027-52.931C220.027,84.684,195.342,60,165,60z\"/></svg>Device Info</button></form><br/><form action=\"/r\" method=\"get\"><button class=\"btn\"><svg height=\"1em\" width=\"1em\" viewBox=\"0 0 512 512\"><path stroke=\"#fff\" fill=\"#ffffff\" d=\"M478.213,163.264l-46.805-80.939c-5.099-9.344-16.107-14.229-27.648-9.963l-52.885,21.248c-10.048-7.232-20.608-13.376-31.531-18.304L311.301,19.2C309.936,8.256,300.443,0,289.221,0h-93.867c-11.221,0-20.715,8.256-22.059,19.008l-8.064,56.277c-10.581,4.8-20.971,10.859-31.467,18.325L80.731,72.299c-10.197-3.925-22.357,0.491-27.435,9.771L6.405,163.157c-5.632,9.557-3.307,21.952,5.44,28.864l44.821,35.029c-0.704,6.763-1.045,12.672-1.045,18.283s0.341,11.52,1.024,18.283l-44.843,35.051c-8.555,6.763-10.901,18.752-5.44,28.736l46.805,80.96c5.099,9.323,16.128,14.208,27.669,9.941l52.885-21.248c10.048,7.232,20.608,13.376,31.531,18.304l8.043,56.085c1.344,10.965,10.837,19.221,22.059,19.221h73.515c3.989,0,7.616-2.219,9.451-5.739c1.835-3.52,1.557-7.765-0.704-11.029c-16.32-23.381-24.96-50.795-24.96-79.232c0-13.269,1.941-26.56,5.781-39.509c0.981-3.328,0.299-6.912-1.856-9.643c-2.155-2.709-5.461-4.16-8.939-4.053c-1.152,0.064-2.283,0.213-5.184,0.576c-53.056,0-96.213-43.157-96.213-96.213c0-53.056,43.157-96.235,96.213-96.235c53.056,0,96.213,43.157,96.021,97.984c-0.149,1.131-0.299,2.261-0.363,3.413c-0.171,3.477,1.344,6.805,4.053,8.939c2.731,2.155,6.336,2.795,9.643,1.856c20.757-6.187,41.216-7.659,64.384-3.456c2.837,0.512,5.952-0.768,8.341-2.539c2.368-1.771,3.947-5.056,4.267-8c0.043-0.384,0.149-2.112,0.149-2.496c0-6.635-0.469-12.885-1.024-18.283L472.752,192C481.307,185.259,483.653,173.269,478.213,163.264z\"/><path stroke=\"#fff\" fill=\"#ffffff\" d=\"M497.989,298.667c-5.888,0-10.667,4.779-10.667,10.667v26.027c-16.085-28.309-48.213-58.027-96-58.027c-64.704,0-117.333,52.629-117.333,117.333S326.619,512,391.323,512c5.888,0,10.667-4.779,10.667-10.667c0-5.888-4.779-10.667-10.667-10.667c-52.928,0-96-43.072-96-96s43.072-96,96-96c50.304,0,77.973,41.557,84.779,64h-42.112c-5.888,0-10.667,4.779-10.667,10.667c0,5.888,4.779,10.667,10.667,10.667h64c5.888,0,10.667-4.779,10.667-10.667v-64C508.656,303.445,503.899,298.667,497.989,298.667z\"/></svg>Reset</button></form>";
const char HTTP_ITEM[] PROGMEM            = "<div><a href='#p' onclick='c(this)'>{v}</a>&nbsp;<span class='q {i}'>{r}%</span></div>";
const char HTTP_FORM_START[] PROGMEM      = "<form method='get' action='wifisave'><label>SSID</label><input  id='s' name='s' length=32 placeholder='SSID'><label>Password</label><input id='p' name='p' length=64 type='password' placeholder='password'><br/>";
const char HTTP_FORM_PARAM[] PROGMEM      = "<br/><input id='{i}' name='{n}' maxlength={l} placeholder='{p}' value='{v}' {c}>";
const char HTTP_FORM_END[] PROGMEM        = "<br/><button class=\"btn\" type='submit'>save</button></form>";
const char HTTP_SCAN_LINK[] PROGMEM       = "<br/><div class=\"c\"><a href=\"/wifi\">Scan</a></div>";
const char HTTP_SAVED[] PROGMEM           = "<div class=\"msg\">{s}<br/>If it fails reconnect to <br/> <strong> {v} </strong> <br/>, try again</div>";
const char HTTP_END[] PROGMEM             = "</div></body></html>";

#ifndef WIFI_MANAGER_MAX_PARAMS
#define WIFI_MANAGER_MAX_PARAMS 10
#endif

class WiFiManagerParameter {
  public:
    /** 
        Create custom parameters that can be added to the WiFiManager setup web page
        @id is used for HTTP queries and must not contain spaces nor other special characters
    */
    WiFiManagerParameter(const char *custom);
    WiFiManagerParameter(const char *id, const char *placeholder, const char *defaultValue, int length);
    WiFiManagerParameter(const char *id, const char *placeholder, const char *defaultValue, int length, const char *custom);
    ~WiFiManagerParameter();

    const char *getID();
    const char *getValue();
    const char *getPlaceholder();
    int         getValueLength();
    const char *getCustomHTML();
  private:
    const char *_id;
    const char *_placeholder;
    char       *_value;
    int         _length;
    const char *_customHTML;

    void init(const char *id, const char *placeholder, const char *defaultValue, int length, const char *custom);

    friend class WiFiManager;
};


class WiFiManager
{
  public:
    WiFiManager();
    ~WiFiManager();

    boolean       autoConnect();
    boolean       autoConnect(char const *apName, char const *apPassword = NULL);

    //if you want to always start the config portal, without trying to connect first
    boolean       startConfigPortal();
    boolean       startConfigPortal(char const *apName, char const *apPassword = NULL);

    // Enter a user id and device id
    void          deviceinfo(char const *_uid, char const *_did);

    // get the AP name of the config portal, so it can be used in the callback
    String        getConfigPortalSSID();

    void          resetSettings();

    //sets timeout before webserver loop ends and exits even if there has been no setup.
    //useful for devices that failed to connect at some point and got stuck in a webserver loop
    //in seconds setConfigPortalTimeout is a new name for setTimeout
    void          setConfigPortalTimeout(unsigned long seconds);
    void          setTimeout(unsigned long seconds);

    //sets timeout for which to attempt connecting, useful if you get a lot of failed connects
    void          setConnectTimeout(unsigned long seconds);


    void          setDebugOutput(boolean debug);
    //defaults to not showing anything under 8% signal quality if called
    void          setMinimumSignalQuality(int quality = 8);
    //sets a custom ip /gateway /subnet configuration
    void          setAPStaticIPConfig(IPAddress ip, IPAddress gw, IPAddress sn);
    //sets config for a static IP
    void          setSTAStaticIPConfig(IPAddress ip, IPAddress gw, IPAddress sn);
    //called when AP mode and config portal is started
    void          setAPCallback( void (*func)(WiFiManager*) );
    //called when settings have been changed and connection was successful
    void          setSaveConfigCallback( void (*func)(void) );
    //adds a custom parameter, returns false on failure
    bool          addParameter(WiFiManagerParameter *p);
    //if this is set, it will exit after config, even if connection is unsuccessful.
    void          setBreakAfterConfig(boolean shouldBreak);
    //if this is set, try WPS setup when starting (this will delay config portal for up to 2 mins)
    //TODO
    //if this is set, customise style
    void          setCustomHeadElement(const char* element);
    //if this is true, remove duplicated Access Points - defaut true
    void          setRemoveDuplicateAPs(boolean removeDuplicates);

  private:
    std::unique_ptr<DNSServer>        dnsServer;
    std::unique_ptr<ESP8266WebServer> server;

    //const int     WM_DONE                 = 0;
    //const int     WM_WAIT                 = 10;

    //const String  HTTP_HEADER = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"/><title>{v}</title>";

    void          setupConfigPortal();
    void          startWPS();

    const char*   _apName                 = "no-net";
    const char*   _apPassword             = NULL;
    String        _ssid                   = "";
    String        _pass                   = "";
    const char*         uid_              = "";
    const char*         did_              = "";
    unsigned long _configPortalTimeout    = 0;
    unsigned long _connectTimeout         = 0;
    unsigned long _configPortalStart      = 0;

    IPAddress     _ap_static_ip;
    IPAddress     _ap_static_gw;
    IPAddress     _ap_static_sn;
    IPAddress     _sta_static_ip;
    IPAddress     _sta_static_gw;
    IPAddress     _sta_static_sn;

    int           _paramsCount            = 0;
    int           _minimumQuality         = -1;
    boolean       _removeDuplicateAPs     = true;
    boolean       _shouldBreakAfterConfig = false;
    boolean       _tryWPS                 = false;

    const char*   _customHeadElement      = "";

    //String        getEEPROMString(int start, int len);
    //void          setEEPROMString(int start, int len, String string);

    int           status = WL_IDLE_STATUS;
    int           connectWifi(String ssid, String pass);
    uint8_t       waitForConnectResult();

    void          handleRoot();
    void          handleWifi(boolean scan);
    void          handleWifiSave();
    void          handleInfo();
    void          handleReset();
    void          handleNotFound();
    void          handle204();
    boolean       captivePortal();
    boolean       configPortalHasTimeout();

    // DNS server
    const byte    DNS_PORT = 53;

    //helpers
    int           getRSSIasQuality(int RSSI);
    boolean       isIp(String str);
    String        toStringIp(IPAddress ip);

    boolean       connect;
    boolean       _debug = true;

    void (*_apcallback)(WiFiManager*) = NULL;
    void (*_savecallback)(void) = NULL;

    int                    _max_params;
    WiFiManagerParameter** _params;

    template <typename Generic>
    void          DEBUG_WM(Generic text);

    template <class T>
    auto optionalIPFromString(T *obj, const char *s) -> decltype(  obj->fromString(s)  ) {
      return  obj->fromString(s);
    }
    auto optionalIPFromString(...) -> bool {
      DEBUG_WM("NO fromString METHOD ON IPAddress, you need ESP8266 core 2.1.0 or newer for Custom IP configuration to work.");
      return false;
    }
};

#endif
