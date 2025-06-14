/*
    This sketch shows the Ethernet event usage

*/

#include <ETH.h>
#include <ESP32Ping.h>

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

IPAddress local_ip(192, 168, 34, 21);
IPAddress gateway(192, 168, 34, 1);
IPAddress subnet(255, 255, 255, 0);

// WARNING: onEvent is called from a separate FreeRTOS task (thread)!
void onEvent(arduino_event_id_t event) {
  switch (event) {
    case ARDUINO_EVENT_ETH_START:
      Serial.println("ETH Started");
      // The hostname must be set after the interface is started, but needs
      // to be set before DHCP, so set it from the event handler thread.
      ETH.setHostname("esp32-ethernet");
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

void testClient(const char *host, uint16_t port) {
  IPAddress ip (192, 168, 34, 5); // The remote ip to ping
  bool ret = Ping.ping(ip);
  if (ret) {
    Serial.print("Pinged host with average time: "); Serial.println(Ping.averageTime());
  }
  else {
    Serial.println("Failed to ping host");
  }
}

void setup() {
    Serial.begin(115200);
    Network.onEvent(onEvent);  // Will call onEvent() from another thread.
    ETH.begin(ETH_PHY_TYPE, ETH_PHY_ADDR, ETH_PHY_MDC, ETH_PHY_MDIO, ETH_PHY_POWER, ETH_CLK_MODE);
    ETH.config(local_ip, gateway, subnet);
}

void loop() {
  if (eth_connected) {
    testClient("google.com", 80);
  }
  delay(10000);
}