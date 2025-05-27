#include "Controller.h"

Controller::Controller()
    :   _network(),
        // Configure IP options based on build flags
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

void Controller::begin() {
    // TODO: Replace with class network instance
    Network.onEvent(std::bind(&Controller::_onNetworkEvent, this, std::placeholders::_1));
    // TODO: Replace with class ETH instance
    ETH.begin(ETH_PHY_TYPE, ETH_PHY_ADDR, ETH_PHY_MDC, ETH_PHY_MDIO, ETH_PHY_POWER, ETH_CLK_MODE);
    ETH.config(localIP, gateway, subnet);

    while (!isEthConnected) {
        Serial.print(".");
        delay(100);
    }


    // TODO: Enable setting custom upload port
    // Port defaults to 3232
    // ArduinoOTA.setPort(3232);

    // TODO: Enable setting custom upload hostname
    // Hostname defaults to esp3232-[MAC]
    // ArduinoOTA.setHostname("myesp32");

    // TODO: Enable setting authentication
    // No authentication by default
    // ArduinoOTA.setPassword("admin");

    // Password can be set with it's md5 value as well
    // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
    // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

    // TODO: Replace with class instance of ArduinoOTA
    ArduinoOTA
        .onStart(std::bind(&Controller::_onOTAStart, this))
        .onEnd(std::bind(&Controller::_onOTAEnd, this))
        .onProgress(std::bind(&Controller::_onOTAProgress, this, std::placeholders::_1, std::placeholders::_2))
        .onError(std::bind(&Controller::_onOTAError, this, std::placeholders::_1));

    ArduinoOTA.begin();

    Serial.println("Ready");
    Serial.print("IP address: ");
    Serial.println(localIP.toString());

    xTaskCreate(
        otaTaskCallback,    // Function to implement the task
        "ArduinoOTA::main", // Name of the task
        4096,               // Stack size in words
        nullptr,            // Task input parameter
        1,                  // Priority of the task
        &arduinoOTATask     // Task handle
    );
}

void Controller::_onNetworkEvent(arduino_event_id_t event) {
    switch (event) {
        case ARDUINO_EVENT_ETH_START:
            Serial.println("ETH Started");
            // The hostname must be set after the interface is started, but needs
            // to be set before DHCP, so set it from the event handler thread.
            #ifdef CONTROLLER_HOSTNAME
            ETH.setHostname(CONTROLLER_HOSTNAME);
            #else
            ETH.setHostname("esp-hostname");
            #endif // DEF(CONTROLLER_HOSTNAME)
            break;
        case ARDUINO_EVENT_ETH_CONNECTED: 
            Serial.println("ETH Connected"); 
            break;
        case ARDUINO_EVENT_ETH_GOT_IP:
            Serial.println("ETH Got IP");
            Serial.println(ETH);
            isEthConnected = true;
            break;
        case ARDUINO_EVENT_ETH_LOST_IP:
            Serial.println("ETH Lost IP");
            isEthConnected = false;
            break;
        case ARDUINO_EVENT_ETH_DISCONNECTED:
            Serial.println("ETH Disconnected");
            isEthConnected = false;
            break;
        case ARDUINO_EVENT_ETH_STOP:
            Serial.println("ETH Stopped");
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
    Serial.println("Start updating " + type);
}

void Controller::_onOTAEnd() {
    Serial.println("\nEnd");
}

void Controller::_onOTAProgress(unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
}

void Controller::_onOTAError(ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
}

Controller controller;