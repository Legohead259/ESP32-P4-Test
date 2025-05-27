#include <ETH.h>
#include <ArduinoOTA.h>

class Controller {
public:
    Controller();
    ~Controller() {}

    void begin();

private:
    NetworkManager _network;
    IPAddress localIP;
    IPAddress gateway;
    IPAddress subnet;

    TaskHandle_t arduinoOTATask;

    bool isEthConnected;

    void _onNetworkEvent(arduino_event_id_t event);

    void _onOTAStart();
    void _onOTAEnd();
    void _onOTAProgress(unsigned int progress, unsigned int total);
    void _onOTAError(ota_error_t error);

    static void otaTaskCallback(void* pvParameters) {
        for (;;) {
            ArduinoOTA.handle();
        }
    }
};

extern Controller controller;