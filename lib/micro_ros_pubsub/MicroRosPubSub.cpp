#include "MicroRosPubSub.h"

MicroRosPubSub::MicroRosPubSub() {}

bool MicroRosPubSub::begin() {
    instance_ = this;

    // Initialize message buffers
    received_msg.data.data = received_buffer;
    received_msg.data.capacity = sizeof(received_buffer);
    response_msg.data.data = response_buffer;
    response_msg.data.capacity = sizeof(response_buffer);
    
    xTaskCreate(
        microRosTaskCallbackStatic,     // Function to implement the task
        "MicroROS::main",               // Name of the task
        4096,                           // Stack size in words
        nullptr,                        // Task input parameter
        1,                              // Priority of the task
        &microRosTask                   // Task handle
    );

    return MicroRosController::begin();
}

bool MicroRosPubSub::createEntities() {
    allocator = rcl_get_default_allocator();

    // Initialize options and set domain ID
    rcl_init_options_t init_options = rcl_get_zero_initialized_init_options();
    if (rcl_init_options_init(&init_options, allocator) != RCL_RET_OK) {
        return false;
    }
    if (rcl_init_options_set_domain_id(&init_options, domainID) != RCL_RET_OK) {
        return false;
    }

    // Initialize support with domain ID options
    if (rclc_support_init_with_options(&support, 0, NULL, &init_options, &allocator) != RCL_RET_OK) {
        return false;
    }

    // Clean up initialization options
    if (rcl_init_options_fini(&init_options) != RCL_RET_OK) {
        return false;
    }

    // Initialize node and rest of entities
    if (rclc_node_init_default(&node, nodeName, "", &support) != RCL_RET_OK) {
        return false;
    }

    // Create publisher
    if (rclc_publisher_init_default(&publisher, &node, 
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String),
        publisherTopic) != RCL_RET_OK) {
        return false;
    }

    // Create subscriber
    if (rclc_subscription_init_default(
        &subscriber, 
        &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String),
        subscriberTopic) != RCL_RET_OK) {
        return false;
    }

    // Initialize executor
    if (rclc_executor_init(&executor, &support.context, 2, &allocator) != RCL_RET_OK) {
        return false;
    }

    // Add subscriber to executor
    if (rclc_executor_add_subscription(
        &executor, 
        &subscriber, 
        &received_msg, 
        &_subscriptionCallbackStatic, 
        ON_NEW_DATA) != RCL_RET_OK) {
        return false;
    }

    return true;
}

void MicroRosPubSub::_subscriptionCallback(const void* msgin) {
    const std_msgs__msg__String* msg = (const std_msgs__msg__String*)msgin;

    logf(controller_log_level_t::LOG_INFO, "Received: %s", msg->data.data);

    // Create response message: "Hello <received_name>!"
    snprintf(response_buffer, sizeof(response_buffer), "Hello %s!", msg->data.data);
    response_msg.data.size = strlen(response_buffer);
    
    // Publish response
    _publishResponse();
}

void MicroRosPubSub::_publishResponse() {
    rcl_ret_t ret = rcl_publish(&publisher, &response_msg, NULL);
    if (ret == RCL_RET_OK) {
        logf(controller_log_level_t::LOG_INFO, "Published: %s", response_msg.data.data);
    } 
    else {
        log(controller_log_level_t::LOG_ERROR, "Error publishing message!");
    }
}

void MicroRosPubSub::destroyEntities() {
    rmw_context_t* rmw_context = rcl_context_get_rmw_context(&support.context);
    (void)rmw_uros_set_context_entity_destroy_session_timeout(rmw_context, 0);

    rcl_ret_t rc = RCL_RET_OK;
    rc = rcl_subscription_fini(&subscriber, &node);
    rc = rcl_publisher_fini(&publisher, &node);
    rclc_executor_fini(&executor);
    rc = rcl_node_fini(&node);
    rclc_support_fini(&support);
}

MicroRosPubSub* MicroRosPubSub::instance_ = nullptr;
MicroRosPubSub controllerPubSub;