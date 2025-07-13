# ROSphoros

*Bringing illumination - and ROS topics - to the world of embedded electronics*

---

This is the documentation hub for ROSphoros - a proving playground for PlatformIO environment experimentation, Over the Air (OTA) updates, and micro-ROS on edge devices.
ROSphoros investigates connecting multiple microcontrollers and a main ROS2 autonomy stack over a Local Area Network (LAN).
The project taps directly into the strengths of ROS2's Data Distribution Service (DDS) architecture by using the micro-ROS library to deploy those capabilities natively to the microcontrollers.
An agent running on the main ROS2 stack manages the connections to the microcontrollers and shares data between the different hardware nodes.
This documentation records the "Hello World" initialization of these features as well as the initial Research and Development, Testing and Evaluation (RDTE) system architecture and methodology.

![The RDTE testbed for the system]({{ images_dir }}/Server_rack_iso.jpg "The 10-inch rack serving as the RDTE testbed for the system.")

These docs will start from the beginning with setting up the programming and development environments and configuring the ROS2 installation and workspace.
We will then walk through the process to configure Over the Air (OTA) programming, detail the underlying libraries in the codebase, and how to program and test a micro-ROS installation using a ROS2 agent.
Building the hardware will be out of scope of this write-up, but the drawings, components, and 3D printed files will be provided.

## Project Overview

For this project, we need to identify the scope of effort required and determine measurable metrics from which we can guide the project's development and determine its efficacy.
To accomplish this, we will establish the Need, Goals, and Objectives (NGO).
The Need is the driving reason why this project was undertaken.
Goals define measures that are needed to accomplish the Need.
Objectives define requirements that fulfill each Goal.
The completion of all Goals and Objectives should therefore fulfill the Need.
The NGO chart is shown below.

![NGO chart for the project]({{ images_dir }}/needs_goals_objectives.png "The Needs, Goals, and Objectives chart for this project defining the scope of effort.")

## Project layout

{{ build_tree('../', graylist=["test", "include"]) }}

## Resources

