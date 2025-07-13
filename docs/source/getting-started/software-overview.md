# Software Overview

This project utilizes existing libraries, open source software, and development frameworks where possible to simplify development.
This section will go over all of the software frameworks used.

---

## ROS2

The core software is ROS2 which provides DDS capabilities and the underlying framework for all of the software nodes.
The autonomy computer runs Ubuntu 22.04.5 and has [ROS2 Humble Hawksbill](https://docs.ros.org/en/humble/index.html) installed on it according to [the documentation](https://docs.ros.org/en/humble/Installation/Ubuntu-Install-Debs.html) provided by the Open Source Robotics Foundation (OSRF).

![ROS2 Humble Hawksbill Logo](../{{ images_dir }}/humble_hawksbill_logo.jpg)

## Micro-ROS

All of the hardware nodes (autonomy computer and four microcontrollers) are connected via LAN, so we could use a traditional REST API or TCP/UDP API to transfer data between the different nodes to accomplish tasks.
However, [micro-ROS](https://micro.ros.org/) provides a unique capability that allows microcontrollers to interact with a ROS2 stack via a transport layer. Micro-ROS runs on a variety of microcontrollers - including the ESP32 family, and allows seamless integration with a ROS2 stack through an agent node.
Setting up the micro-ROS workspace on the autonomy computer will be covered in a later section.

![Micro-ROS Logo](../{{ images_dir }}/micro_ros_logo.png)

## IDE

In a future iteration of the project, the microcontrollers will be running different applications, so we need a way to manage multiple libraries, environmental variables, and source applications - ideally within the same programming workspace to minimize clutter and the need to have multiple windows open simultaneously.
[Visual Studio Code](code.visualstudio.com) is a powerful IDE that has a wide-array of extensions and workspace management tools to enable our requirements.
VS Code supports [PlatformIO](platformio.org) through an extension which allows us to manage different platforms, board specifications, frameworks, libraries, etc. within a single workspace.

Using the [platformio.ini](https://docs.platformio.org/en/latest/projectconf/) file, we can create different programming environments that have a variety of variables we can change; this allows us to load different applications to different upload ports using different compilation variables - perfect for our use-case.
Because of the project's complexity, we will not just be using the single platformio.ini file.
A detailed breakdown of the environmental organizational structure will be provided in a later section.

| VS Code                                  | PlatformIO                                           |
| ---------------------------------------- | ---------------------------------------------------- |
| ![VSCode Logo](../{{ images_dir }}/vs_code.png){: style="width:350px"} | ![PlatformIO Logo](../{{ images_dir }}/platformio-logo.png){: style="width:350px"} |

## Docker

For simplicity, we will containerize the final ROS2 workspace and launch file so that it can be quickly spun up and deployed like an application without the need for multiple different processes running on the host machine. Therefore, we will use [Docker engine](https://docs.docker.com/engine/install/ubuntu/) to create and manage the container for us.

![Docker Logo](../{{ images_dir }}/docker_logo.png)
