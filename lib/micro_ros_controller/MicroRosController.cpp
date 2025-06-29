#include "MicroRosController.h"

MicroRosController::MicroRosController() 
    :
    agentIP(192, 168, 34, 7)
{
    agentPort = 8888;
}

bool MicroRosController::begin() {
    if (!Controller::begin()) return false;

    set_microros_ethernet_transports(localIP, gateway, subnet, agentIP, agentPort);
    agentState = agent_state_t::WAITING_FOR_AGENT;

    return true;
}

void MicroRosController::handleConnectionState() {
    // Handle Micro-ROS tasking
    switch (agentState) {
        case WAITING_FOR_AGENT:
            if (RMW_RET_OK == rmw_uros_ping_agent(100, 3)) {
                log(LOG_INFO, "Agent found, establishing connection...");
                agentState = AGENT_AVAILABLE;
            }
            else {
                log(LOG_WARN, "Failed to find agent. Retrying...");
            }
            break;

        case AGENT_AVAILABLE:
            if (createEntities()) { // Update system state
                log(LOG_INFO, "Connected and ready!");
                agentState = AGENT_CONNECTED;
            }
            else { // If entities are not properly created, destroy them
                log(LOG_WARN, "Connection failed. Retrying...");
                agentState = WAITING_FOR_AGENT;
                destroyEntities();
            };
            break;
        
        case AGENT_CONNECTED:
            if (RMW_RET_OK == rmw_uros_ping_agent(100, 3)) {
                // Execute pending tasks in the executor. This will handle all ROS communications.
                RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
            }
            else {
                log(LOG_WARN, "Agent disconnected!");
                agentState = AGENT_DISCONNECTED;
            }
            break;

        case AGENT_DISCONNECTED:
            log(LOG_INFO, "Waiting for agent...");
            agentState = WAITING_FOR_AGENT;
            destroyEntities();
            break;
            
        default:
            break;
    }
}

MicroRosController controllerUros;