#include <MicroRosController.h>
#include <std_msgs/msg/string.h>

class MicroROSPubSub : MicroROSController {
public:
    MicroROSPubSub();
    ~MicroROSPubSub() { destroyEntities(); }

    bool begin();
    
protected:
    void handleConnectionState() override;
    bool createEntities() override;
    void destroyEntities() override;

private:
    static MicroROSPubSub* instance_;

    // ROS node configuration
    const char* publisherTopic="micro_ros_response";
    const char* subscriberTopic="micro_ros_name";

    // ROS entities
    rcl_publisher_t publisher;
    rcl_subscription_t subscriber;
    rcl_timer_t timer;

    // Message buffers
    std_msgs__msg__String received_msg;
    std_msgs__msg__String response_msg;
    char received_buffer[50];
    char response_buffer[100];

    void _subscriptionCallback(const void* msgin);
    void _publishResponse();

    static void _subscriptionCallbackStatic(const void* msgin) { instance_->_subscriptionCallback(msgin); }
};

MicroROSPubSub* MicroROSPubSub::instance_ = nullptr;
extern MicroROSPubSub controllerPubSub;