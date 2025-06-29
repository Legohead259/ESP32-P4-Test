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
    #ifdef CONTROLLER_NAME
    const char* nodeName = CONTROLLER_NAME;
    #else
    const char* nodeName = "micro_ros_controller_node";
    #endif
    #ifdef CONTROLLER_NAMESPACE
    const char* nodeNamespace = CONTROLLER_NAMESPACE;
    #else
    const char* nodeNamespace = "";
    #endif
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
    #ifdef ROS_DOMAIN_ID
    size_t domainID = ROS_DOMAIN_ID;
    #else
    size_t domainID=0;
    #endif

    virtual void handleConnectionState();
    virtual bool createEntities() { return true; }
    virtual void destroyEntities() {}

private:
};

extern MicroRosController controllerUros;