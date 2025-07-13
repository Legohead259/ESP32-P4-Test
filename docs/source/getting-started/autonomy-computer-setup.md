# Autonomoy Computer Software Setup

Let's get started making some software!

---

## Installation

The software setup for the autonomy computer will assume that host operating system is Ubuntu 22.04.5 LTS and the required software has been installed according to their respective documents.
The full list of software applications required and their installation procedures are listed below:

* [ROS2 Humble](https://docs.ros.org/en/humble/Installation/Ubuntu-Install-Debs.html)
* [Visual Studio Code](https://code.visualstudio.com/docs/setup/linux)
* [PlatformIO IDE for VSCode](https://docs.platformio.org/en/latest/integration/ide/vscode.html)
* [Docker Engine](https://code.visualstudio.com/docs/setup/linux)
* [micro-ROS](https://micro.ros.org/docs/tutorials/core/first_application_linux/) (only do the "Installing ROS2 and the micro-ROS build system" and "Creating the micro-ROS agent" sections)

With everything installed, we need to make a slight configuration change to Docker to make it easier to use.
With a fresh installation, Docker requires a root user (sudo) to function.
If your device policies allow it, follow the [Docker documentation](https://docs.docker.com/engine/install/linux-postinstall/) to manage Docker as a non-root user.

## Cloning the Repository

With that completed, we can clone the latest project workspace to a project directory on the machine.
`git clone {{ repository_url }}`
The project ({{ version_tag }}) has the following directory structure:

{{ build_tree('../', graylist=["test", "include"]) }}

## PlatformIO Environment File Interactions

Breaking down this structure, we can start with the platformio.ini file with is the main configuration file for the PlatformIO project workspace.
It imports the files located in the `env/` directory which in turn cross-reference each other and build the configurations for each controller programming environment.
The interactions between the environmental files and the rest of the application code is documented in the chart below.
Note that for brevity, only "controller_1" is shown in the chart.
The interactions are the same for the other controllers.
The blue boxes, "OTA Initial", "OTA Followup", and "MicroROS Test" denote the actual PlatformIO environments that are uploaded the microcontrollers.
These will be explore more in-depth shortly.

![PIO Environment File Interactions]({{ images_dir }}/pio_environment_diagram.png "The interactions between the PlatformIO environmental variables and the relevant sections of the workspace.")

The green "ESP32-P4 Base" component leverages the Arduino framework's capability to define custom board variants for a project.
This will allow us to define custom pin variables in the future depending on what is connected to the microcontroller.
For the moment, we have to source the ESP32-P4 build framework from the PIOArduino fork of PlatformIO since official support has not been rolled out at time of writing.
Within the "Common" configuration, we can also define environmental variables that will be shared by all the microcontrollers like the IP address domain.
From the ESP32-P4 base, we can build out the "Controller Base" definition which will include all of the build flags like the IP domain, controller name, ROS domain ID.
From the "Controller Base" definition, we can define an extended definition for "Controller OTA" that contains the information like the upload protocol, IP address, port, and login information to enable OTA programming.
