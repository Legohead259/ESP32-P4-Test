#include "Controller.h"

Controller::Controller()
    :   // Configure IP options based on build flags
        #ifdef CONTROLLER_ID
        #ifdef IP_DOMAIN_OCTET_0 // If octet 0 set, assume octet 1 and 2 are also set
        localIP(IP_DOMAIN_OCTET_0, IP_DOMAIN_OCTET_1, IP_DOMAIN_OCTET_2, CONTROLLER_ID),
        gateway(IP_DOMAIN_OCTET_0, IP_DOMAIN_OCTET_1, IP_DOMAIN_OCTET_2, 1),
        #else // IP Domain not set. Default to 192.168.2.0/24
        localIP(192, 168, 2, CONTROLLER_ID),
        gateway(192, 168, 2, 1),
        #endif // IP_DOMAIN_OCTET_0
        #else // If CONTROLLER_ID not set, go to default IP address
        localIP(192, 168, 2, 20),
        gateway(192, 168, 2, 1),
        #endif // CONTROLLER_ID

        subnet(255, 255, 255, 0) // Always assume 24-bit subnet mask
    {
}

bool Controller::begin() {
    bool _isNetworkStarted = _beginNetwork();
    bool _isOTAStarted = _beginOTA();

    xTaskCreate(
        otaTaskCallback,    // Function to implement the task
        "ArduinoOTA::main", // Name of the task
        4096,               // Stack size in words
        nullptr,            // Task input parameter
        1,                  // Priority of the task
        &arduinoOTATask     // Task handle
    );

    return (_isNetworkStarted && _isOTAStarted);
}

bool Controller::_beginNetwork(long timeout) {
    // Must use global Network object for some reason. Does not work if we define our own instance *shrug*
    if (!Network.begin()) {
        log(LOG_ERROR, "Network failed to start!");
        return false;
    }
    log(LOG_INFO, "Network started");
    Network.onEvent(std::bind(&Controller::_onNetworkEvent, this, std::placeholders::_1));  
    
    // Must use global ETH object for some reason. Does not work if we define our own instance *shrug*
    if (!ETH.begin(ETH_PHY_TYPE, ETH_PHY_ADDR, ETH_PHY_MDC, ETH_PHY_MDIO, ETH_PHY_POWER, ETH_CLK_MODE)) {
        log(LOG_ERROR, "Ethernet controller failed to start!");
        return false;
    }
    log(LOG_INFO, "Ethernet controller started");
    ETH.config(localIP, gateway, subnet);

    // Check connection timeout
    long _timeoutStart = millis();
    long _timeoutEnd = _timeoutStart + timeout;
    while (!isEthConnected) {
        log(LOG_DEBUG, ".");
        if (millis() >= _timeoutEnd) {
            log(LOG_ERROR, "Ethernet connection timeout! Check connection!");
            return false;
        }
    }

    return true;
}

bool Controller::_beginOTA() {
    // TODO: Enable setting custom upload port
    // Port defaults to 3232
    // ArduinoOTA.setPort(3232);

    #ifdef CONTROLLER_HOSTNAME
    ArduinoOTA.setHostname(CONTROLLER_HOSTNAME);
    #else
    ArduinoOTA.setHostname("esp-hostname");
    #endif // DEF(CONTROLLER_HOSTNAME)

    // TODO: Enable setting authentication
    // No authentication by default
    // ArduinoOTA.setPassword("admin");

    // Password can be set with it's md5 value as well
    // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
    // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

    // Must use global ArduinoOTA object. Static callback for otaTaskCallback does not work with instance-specific ArduinoOTA object
    ArduinoOTA
        .onStart(std::bind(&Controller::_onOTAStart, this))
        .onEnd(std::bind(&Controller::_onOTAEnd, this))
        .onProgress(std::bind(&Controller::_onOTAProgress, this, std::placeholders::_1, std::placeholders::_2))
        .onError(std::bind(&Controller::_onOTAError, this, std::placeholders::_1));

    ArduinoOTA.begin();
    return true; // Currently no logic to check if OTA is active or not.
}

void Controller::_onNetworkEvent(arduino_event_id_t event) {
    switch (event) {
        case ARDUINO_EVENT_ETH_START:
            // The hostname must be set after the interface is started, but needs
            // to be set before DHCP, so set it from the event handler thread.
            #ifdef CONTROLLER_HOSTNAME
            ETH.setHostname(CONTROLLER_HOSTNAME);
            #else
            ETH.setHostname("esp-hostname");
            #endif // DEF(CONTROLLER_HOSTNAME)
            logf(LOG_INFO, "ETH Started\n\r\tHostname: %s", ETH.getHostname());
            break;
        case ARDUINO_EVENT_ETH_CONNECTED: 
            log(LOG_INFO, "ETH Connected");
            break;
        case ARDUINO_EVENT_ETH_GOT_IP:
            logf(LOG_INFO, "ETH Got IP\n\r\tIP Address: %s \r\n\tSubnet: %s\r\n\tGateway: %s", 
                ETH.localIP().toString().c_str(),
                ETH.subnetMask().toString().c_str(),
                ETH.gatewayIP().toString().c_str());
            isEthConnected = true;
            break;
        case ARDUINO_EVENT_ETH_LOST_IP:
            log(LOG_WARN, "ETH Lost IP");
            isEthConnected = false;
            break;
        case ARDUINO_EVENT_ETH_DISCONNECTED:
            log(LOG_WARN, "ETH Disconnected");
            isEthConnected = false;
            break;
        case ARDUINO_EVENT_ETH_STOP:
            log(LOG_ERROR, "ETH Stopped");
            isEthConnected = false;
            break;
        default: break;
    }
}

void Controller::_onOTAStart() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
    else // U_SPIFFS
        type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    logf(LOG_INFO, "Start OTA updating - %s", type);
}

void Controller::_onOTAEnd() {
    log(LOG_INFO, "OTA End");
}

void Controller::_onOTAProgress(unsigned int progress, unsigned int total) {
    logf(LOG_INFO, "OTA Progress: %u%%\r", (progress / (total / 100)));
}

void Controller::_onOTAError(ota_error_t error) {
    char error_str[16];
    switch(error) {
        case OTA_AUTH_ERROR: strcpy(error_str, "Auth Failed"); break;
        case OTA_BEGIN_ERROR: strcpy(error_str, "Begin Failed"); break;
        case OTA_CONNECT_ERROR: strcpy(error_str, "Connect Failed"); break;
        case OTA_RECEIVE_ERROR: strcpy(error_str, "Receive Failed"); break;
        case OTA_END_ERROR: strcpy(error_str, "End Failed"); break;
        default: strcpy(error_str, "Check OTA docs"); break;
    }
    logf(LOG_ERROR, "OTA Error[%u]: %s", error, error_str);
}

Controller controller;