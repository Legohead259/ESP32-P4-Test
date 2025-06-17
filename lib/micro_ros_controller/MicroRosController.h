#include "Controller.h"
#include <micro_ros__definitions.h>

typedef enum {
    INITIALIZING,
    WAITING_FOR_AGENT,
    AGENT_AVAILABLE,
    AGENT_CONNECTED,
    AGENT_DISCONNECTED
} agent_state_t;

class MicroRosController : public Controller {
public:
    MicroRosController();
    ~MicroRosController() { destroyEntities(); }

    bool begin();

protected:
    // ROS node configuration
    const char* nodeName="micro_ros_controller_node";
    const int executorTimeout = 100;  // ms

    // ROS entities
    rclc_executor_t executor;
    rclc_support_t support;
    rcl_allocator_t allocator;
    rcl_node_t node; 
    
    // Micro-ROS configuration
    IPAddress agentIP;
    uint16_t agentPort;
    agent_state_t agentState;
    size_t domainID=8;

    virtual void handleConnectionState();
    virtual bool createEntities() { return true; }
    virtual void destroyEntities() {}

private:
};

extern MicroRosController controllerUros;