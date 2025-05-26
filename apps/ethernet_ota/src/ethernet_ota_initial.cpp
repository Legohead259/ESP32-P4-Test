#include <ETH.h>
#include <ESPmDNS.h>
#include <NetworkUdp.h>
#include <ArduinoOTA.h>

#ifndef ETH_PHY_MDC
#define ETH_PHY_TYPE ETH_PHY_TLK110
#if CONFIG_IDF_TARGET_ESP32
#define ETH_PHY_ADDR  31
#define ETH_PHY_MDC   23
#define ETH_PHY_MDIO  18
#define ETH_PHY_POWER 17
#define ETH_CLK_MODE  ETH_CLOCK_GPIO0_IN
#elif CONFIG_IDF_TARGET_ESP32P4
#define ETH_PHY_ADDR  1
#define ETH_PHY_MDC   31
#define ETH_PHY_MDIO  52
#define ETH_PHY_POWER 51
#define ETH_CLK_MODE  EMAC_CLK_EXT_IN
#endif
#endif

static bool eth_connected = false;

// Configure IP options based on build flags
#ifdef CONTROLLER_ID
#ifdef IP_DOMAIN_OCTET_0 // If octet 0 set, assume octet 1 and 2 are also set
IPAddress local_ip(IP_DOMAIN_OCTET_0, IP_DOMAIN_OCTET_1, IP_DOMAIN_OCTET_2, CONTROLLER_ID);
IPAddress gateway(IP_DOMAIN_OCTET_0, IP_DOMAIN_OCTET_1, IP_DOMAIN_OCTET_2, 1);
#else // IP Domain not set. Default to 192.168.2.0/24
IPAddress local_ip(192, 168, 2, CONTROLLER_ID);
IPAddress gateway(192, 168, 2, 1);
#endif // IP_DOMAIN_OCTET_0
#else // If CONTROLLER_ID not set, go to default IP address
IPAddress local_ip(192, 168, 2, 20);
IPAddress gateway(192, 168, 2, 1);
#endif // CONTROLLER_ID

IPAddress subnet(255, 255, 255, 0); // Always assume 24-bit subnet mask

// WARNING: onEvent is called from a separate FreeRTOS task (thread)!
void onEvent(arduino_event_id_t event) {
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
    case ARDUINO_EVENT_ETH_CONNECTED: Serial.println("ETH Connected"); break;
    case ARDUINO_EVENT_ETH_GOT_IP:
      Serial.println("ETH Got IP");
      Serial.println(ETH);
      eth_connected = true;
      break;
    case ARDUINO_EVENT_ETH_LOST_IP:
      Serial.println("ETH Lost IP");
      eth_connected = false;
      break;
    case ARDUINO_EVENT_ETH_DISCONNECTED:
      Serial.println("ETH Disconnected");
      eth_connected = false;
      break;
    case ARDUINO_EVENT_ETH_STOP:
      Serial.println("ETH Stopped");
      eth_connected = false;
      break;
    default: break;
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");
  Network.onEvent(onEvent);  // Will call onEvent() from another thread.
  ETH.begin(ETH_PHY_TYPE, ETH_PHY_ADDR, ETH_PHY_MDC, ETH_PHY_MDIO, ETH_PHY_POWER, ETH_CLK_MODE);
  ETH.config(local_ip, gateway, subnet);
  while (!eth_connected) {
    Serial.print(".");
  }

  // Port defaults to 3232
  ArduinoOTA.setPort(3232);

  // Hostname defaults to esp3232-[MAC]
  // ArduinoOTA.setHostname("myesp32");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(local_ip.toString());
}

void loop() {
  ArduinoOTA.handle();
}