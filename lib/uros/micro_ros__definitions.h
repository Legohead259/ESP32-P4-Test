#ifndef MICRO_ROS__DEFINITIONS_H
#define MICRO_ROS__DEFINITIONS_H

#include <Arduino.h>

// The micro_ros_platformio library provides the functions to communicate with ROS2
#include <micro_ros_platformio.h>

// These are core ROS2 libraries for creating nodes, publishers, and executors
#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <rclc_parameter/rclc_parameter.h>
#include <micro_ros_utilities/type_utilities.h>
#include <micro_ros_utilities/string_utilities.h>

// Define macros for checking function success
#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){ while(1); }} // Blocking
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){ }} // Non-blocking
#define RCRETCHECK(fn) { rcl_ret_t temp_rc = fn; return temp_rc != RCL_RET_OK} // Returns RCL_RET_OK

// Define macros for executing a task every interval
#define EXECUTE_EVERY_N_MS(MS, X)  do { \
    static volatile int64_t init = -1; \
    if (init == -1) { init = uxr_millis();} \
    if (uxr_millis() - init > MS) { X; init = uxr_millis();} \
} while (0)\

#endif // MICRO_ROS__DEFINITIONS_H