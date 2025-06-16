#include "MicroRosController.h"

MicroROSController::MicroROSController() 
    :
    agentIP(192, 168, 34, 7)
{
    agentPort = 8888;
}

bool MicroROSController::begin() {
    if (!Controller::begin()) return false;

    set_microros_ethernet_transports(localIP, gateway, subnet, agentIP, agentPort);
    agentState = agent_state_t::WAITING_FOR_AGENT;

    return true;
}

void MicroROSController::handleConnectionState() {
    // Handle Micro-ROS tasking
    switch (agentState) {
        case agent_state_t::WAITING_FOR_AGENT:
            EXECUTE_EVERY_N_MS(500, agentState = (RMW_RET_OK == rmw_uros_ping_agent(100, 1)) ? AGENT_AVAILABLE : WAITING_FOR_AGENT;);
            break;

        case AGENT_AVAILABLE:
            agentState = createEntities() ? AGENT_CONNECTED : WAITING_FOR_AGENT; // Check if entities are properly created
            if (agentState == AGENT_CONNECTED) { // Update system state
            }
            if (agentState == WAITING_FOR_AGENT) { // If entities are not properly created, destroy them
                destroyEntities();
            };
            break;
        
        case AGENT_CONNECTED:
            EXECUTE_EVERY_N_MS(200, agentState = (RMW_RET_OK == rmw_uros_ping_agent(100, 1)) ? AGENT_CONNECTED : AGENT_DISCONNECTED;);
            if (agentState == AGENT_CONNECTED) {
                // Execute pending tasks in the executor. This will handle all ROS communications.
                RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
            }
            break;

        case AGENT_DISCONNECTED:
            destroyEntities();
            agentState = WAITING_FOR_AGENT;
            break;
            
        default:
            break;
    }
}

MicroROSController controllerUros;