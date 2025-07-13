# Hardware Overview

This project utilizes Commercial Off the Shelf (COTS) hardware to simplify development.
This section will go over all of the hardware required and the system architecture used to connect everything together.

---

## Microcontroller

The ESP32-P4 is a high-performance System on Chip (SoC) variant of the popular ESP32 line of microcontrollers from Espressif.
The SoC is powered by a dual-core RISC-V CPU featuring an AI accelerator, advanced memory management, and high-speed peripherals.
The ESP32-P4 is specifically designed for high-performance edge computing as required by the overarching project and was chosen for its wealth of peripherals, ease of programming, and raw performance.

The [Waveshare ESP32-P4-NANO](https://www.waveshare.com/esp32-p4-nano.htm) is a development board based on the ESP32-P4 SoC that breaks out some of the peripherals and provides a native 10/100 ethernet connection.
This board comes with a special Power Over Ethernet (POE) expansion board that enables the board to communicate with and be powered by a 802.3af-compliant router or switch.
The low Size, Weight, and Power (SWaP) specifications for this board make it ideal for a small RDTE testbench like can be provided by a 10-inch minirack.

![Waveshare ESP32-P4-NANO](../{{ images_dir }}/ESP32-P4-NANO-details-1.jpg "The Waveshare ESP32-P4-NANO development board. Courtesy of Waveshare")

## Mini Rack

A miniature 10-inch server rack was explored for this project as the RDTE test bed because it is highly configurable, portable, and can keep wires tidy and organized.
Research was conducted on Jeff Geerling's website, [Project MINI RACK](https://mini-rack.jeffgeerling.com/), to determine a suitable combination of hardware components to support the effort.
Ultimately, a [4U DeskPi RackMate T0](https://deskpi.com/products/deskpi-rackmate-t1-rackmount-10-inch-4u-server-cabinet-for-network-servers-audio-and-video-equipment) was chosen because of its cost and availability.

![4U DeskPi RackMate T0](../{{ images_dir }}/DeskPi RackMate T0.jpg "The RDTE testbed hardware enclosure. Courtesy of DeskPi")

## Router/Switch

Eventually, this project will investigate using a router and Virtual Private Network (VPN) to establish connections from various data sources outside the LAN.
This means that the RDTE testbed needs to support up to eight ethernet devices, POE, routing, firewalls, and VPN services within 2U of rack space.
Project MINI RACK lists some switches that are 10-inch rack-compatible and of those, only one (at time of component selection) fulfilled those requirements: the [MikroTik CRS112-8P-4S-IN](https://mikrotik.com/product/crs112_8p_4s_in#fndtn-specifications) with a 48V power supply.

This unit provides a maximum of 1.4A@48V (450 mA per port) which is more than sufficient for the ESP32-P4.
It also runs MikroTik RouterOS and is compatible with their entire suite of Software Defined Networking (SDN) equipment which will be useful for the project's future.
The CRS112 also supports routing, firewalls, DHCP services, and VPN over IPSec which fulfills all the requirements while only being 1U tall.

![MikroTik CRS112-8P-4S-IN](../{{ images_dir }}/crs112-8p-4s-in.png "The CRS112-8P-4S-IN POE router and switch. Courtesy of MikroTik")

## RDTE Testbed

The ESP32-P4 microcontrollers are connected to the switch using short lengths of CAT5E ethernet cable.
Another ethernet cable is used to connect the switch to the autonomy computer.
The overall rack layout with connections is shown below.

![RDTE Testbed Rack Layout](../{{ images_dir }}/rack_layout.png "The 4U rack layout for the RDTE testbed with ethernet connections shown in blue. CRS112 image courtesy of MikroTik")
