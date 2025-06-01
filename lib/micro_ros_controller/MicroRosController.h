#include "Controller.h"
#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rcutils/logging_macros.h>

class MicroROSController : public Controller {
public:
    MicroROSController();
    ~MicroROSController() override {}

protected:
    void log(LogLevel level, const String& message) override {
        switch (level) {
            case LOG_ERROR: RCLCPP_ERROR(nullptr, "%s", message.c_str()); break;
            case LOG_WARN:  RCLCPP_WARN(nullptr, "%s", message.c_str()); break;
            case LOG_INFO:  RCLCPP_INFO(nullptr, "%s", message.c_str()); break;
            case LOG_DEBUG: RCLCPP_DEBUG(nullptr, "%s", message.c_str()); break;
            default: break;
        }
    }

    void logf(LogLevel level, const char* format, ...) override {
        char buffer[256];
        va_list args;
        va_start(args, format);
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);

        switch (level) {
            case LOG_ERROR: RCLCPP_ERROR(nullptr, "%s", buffer); break;
            case LOG_WARN:  RCLCPP_WARN(nullptr, "%s", buffer); break;
            case LOG_INFO:  RCLCPP_INFO(nullptr, "%s", buffer); break;
            case LOG_DEBUG: RCLCPP_DEBUG(nullptr, "%s", buffer); break;
            default: break;
        }
    }
};
