#include <ETH.h>
#include <ArduinoOTA.h>
#include <esp_log.h>

typedef enum {
    LOG_SILENT,
    LOG_ERROR,
    LOG_WARN,
    LOG_INFO,
    LOG_DEBUG,
    LOG_VERBOSE
} controller_log_level_t;

class Controller {
public:
    Controller();
    ~Controller() {}

    bool begin();
    
    void setLogLevel(controller_log_level_t level) { currentLogLevel = level; }

    controller_log_level_t getLogLevel() { return currentLogLevel; }

protected:
    // Network configuration
    IPAddress localIP;
    IPAddress gateway;
    IPAddress subnet;
    bool isEthConnected;

    static constexpr const char* TAG = "Controller";

    void log(controller_log_level_t level, const char* message) {
        if (level <= currentLogLevel) {
            esp_log_level_t espLevel = convertToEspLogLevel(level);
            switch (level) {
                case ESP_LOG_ERROR:     ESP_LOGE(TAG, "%s", message); break;
                case ESP_LOG_WARN:      ESP_LOGW(TAG, "%s", message); break;
                case ESP_LOG_INFO:      ESP_LOGI(TAG, "%s", message); break;
                case ESP_LOG_DEBUG:     ESP_LOGD(TAG, "%s", message); break;
                case ESP_LOG_VERBOSE:   ESP_LOGV(TAG, "%s", message); break;
                default:                break;
            }
        }
    }

    void logf(controller_log_level_t level, const char* format, ...) {
        if (level <= currentLogLevel) {
            char buffer[1024];
            va_list args;
            va_start(args, format);
            vsnprintf(buffer, sizeof(buffer), format, args);
            log(level, buffer);
            va_end(args);
        }
    }

private:
    controller_log_level_t currentLogLevel = LOG_INFO;

    esp_log_level_t convertToEspLogLevel(controller_log_level_t level) {
        switch(level) {
            case LOG_ERROR:     return ESP_LOG_ERROR;
            case LOG_WARN:      return ESP_LOG_WARN;
            case LOG_INFO:      return ESP_LOG_INFO;
            case LOG_DEBUG:     return ESP_LOG_DEBUG;
            case LOG_VERBOSE:   return ESP_LOG_VERBOSE;
            default:            return ESP_LOG_NONE;
        }
    }

    TaskHandle_t arduinoOTATask;

    bool _beginNetwork(long timeout=10000);
    bool _beginOTA();

    void _onNetworkEvent(arduino_event_id_t event);

    void _onOTAStart();
    void _onOTAEnd();
    void _onOTAProgress(unsigned int progress, unsigned int total);
    void _onOTAError(ota_error_t error);

    static void otaTaskCallback(void* pvParameters) {
        for (;;) {
            ArduinoOTA.handle();
            vTaskDelay(pdMS_TO_TICKS(10));  // Yield to scheduler every 10 ms
        }
    }
};

extern Controller controller;