## To communicate between two ROS devices

1) https://husarion.com/tutorials/ros-tutorials/5-running-ros-on-multiple-machines/


## Gazebo ROS

1) To install gazebo_ros package -  https://classic.gazebosim.org/tutorials?tut=ros_installing&cat=connect_ros


## ROS2 with PX4
1) Useful video on ROS2 wtih PX4 ([link][ros2_px4_])

[ros2_px4_]:https://youtu.be/qhLATrkA_Gw



## PX4 development environment for linux ([link][linux_px4]) <br>
Best Youtube video on PX4 fundamentals [link][px4funda] and its summary is as follows. <br>
### Part 1: High level overview of PX4 architecture

   - NuttX: It is the operating system PX4 runs on. Everything in PX4 runs on NuttX
   - Modules: These divides the tasks. They do flight modes, velocity control, state estimator, flight control, position control, ground station communication
   - Drivers: It is a translator between the Hardware (all the sensors) and PX4
   - uORB: Communication channels between all the tasks (check out all the uORB messages - [link][uORBmessages])
   - Parameters: (It can be accessed from src/modules/battery_status/module.yaml) <br>
### Part 2: Development Environment Setup

   - install PX4 autopilot ```git clone https://github.com/PX4/PX4-Autopilot.git --recursive```
   - ```cd PX4-Autopilot```
   - 


[px4funda]: https://youtu.be/-1MASuJDGn4
[linux_px4]: https://docs.px4.io/main/en/dev_setup/dev_env_linux_ubuntu.html
[uORBmessages]:https://docs.px4.io/main/en/middleware/uorb_graph.html

## MAVLINK
To increse the rate of mavlink messages - https://github.com/mavlink/mavros/issues/305

## Extras
1) Link to xml file where ID of the messages are defined - /home/pratik/catkin_ws/src/mavlink/message_definitions/v1.0


## Tmux 
1) Install tmux by ```sudo apt-get install tmux```
2) To start new terminal i) First press ```ctrl + B``` ia) to add terminal in right side ```shift + 5``` ib) to add terminal below ```shift + " ```
3) To navigate between the terminal i) First press ```ctrl + B``` then use the arrow key
4) To close the terminal type ```exit``` in that terminal

## Rpi4 8 gb RAM ([link][rpilink])
### Ubuntu Server 20.04.5 LTS (64-bit)
1) Download raspberry pi imager - https://www.raspberrypi.com/software/
2) Select -> Other general-purpose OS -> Ubuntu -> Ubuntu Server 20.04.5 LTS (64-bit)
3) Go to settings (gear icon) <br> 
    i) Set hostname: ```raspberrypi2``` <br>
    ii) Enable SSH: tick Use password authentication, <br>
    iii) username = ```raspberrypi2```, password = ```raspberry```, <br>
    iv) Configure wireless LAN: SSID - ```pratik```, password - ```hcrlab8318``` <br>

[rpilink]: https://robu.in/product/raspberry-pi-4-model-b-8gb-starter-kit/?gclid=CjwKCAjwkLCkBhA9EiwAka9QRkXvehp1eUdSgwkAgVF37yJyJTX_pxnJmlBz8wtE63cy9Ec5fIf58RoCWRkQAvD_BwE

### If Ubuntu desktop opens up although you have installed Ubuntu Server
if gui opens up try ```ctrl +alt + F3``` for text mode(terminal) <br>
```ctrl + alt + F2``` for GUI

### To enable UART Communication
1) Run the code ```sudo raspi-config```
  - If you get the error like ```sudo: raspi-config command not found```
  - Install raspi-config using the code as given below <br>
 ```wget https://archive.raspberrypi.org/debian/pool/main/r/raspi-config/raspi-config_20200601_all.deb -P /tmp```<br>
 ```sudo apt-get install libnewt0.52 whiptail parted triggerhappy lua5.1 alsa-utils -y``` <br>
 ```sudo apt-get install -fy``` <br>
 ```sudo dpkg -i /tmp/raspi-config_20200601_all.deb```
3) Go to 5 Interfacing Options -> P6 Serial -> <br>
  - Would you like a login shell to be accesible over serial? -> [No] <br>
  - Would you like the serial port hardware to be enabled? -> [Yes]
  - reboot the Rpi
4) Hardware permissions for Serial : You may want to add your user to the dialout group, to avoid errors such as: serial.serialutil.SerialException: [Errno 13] could not open port /dev/ttyACM0: [Errno 13] Permission denied: ‘/dev/ttyACM0’.
  - Use the code ```sudo adduser your_username dialout```
5) Install Python Serial library on Raspberry Pi <br>
   a) To install pip ```sudo apt install python3-pip``` <br>
   b) To install pyserial library ```python3 -m pip install pyserial``` <br>
6) Make a CAM_device.py file. a) Paste the code [link][camdevicecode] b) make it executable by ```chmod +x CAM_device.py```

  [camdevicecode]:https://github.com/pratikiitgn/Bigger_quadcopter/blob/main/RaspberryPi/CAM_device.py

### To enable wifi communication <br>
   go to ```/etc/netplans``` <br>
   type the command - ```sudo nano 50-cloud-init.yaml``` <br>
   1) First remove ```render - network```
   2) below "access-points": <br>
        "hcrlab": <br>
	(four spaces) password: "hcrlab8318" <br>
	    (consider four spaces for tab in yaml file)<br>
   3) To shutdown the RPI ```sudo shutdown now```

### To make static ip address to Rpi
https://phoenixnap.com/kb/raspberry-pi-static-ip

### To enable on/off Rpi by switch

1) It is mandwtory to have a push button [link][pushbutton]
2) Go to config file by ```sudo nano /boot/config.txt``` add following text.
    ```dtoverlay=gpio-shutdown```
3) Update the bootloder using following code
    ```sudo apt update ```
    ```sudo apt full-upgrade```
    ```sudo apt install rpi-eeprom```
    ```sudo rpi-eeprom-update```

[pushbutton]:https://www.electronicscomp.com/push-button-spst-on-off-switch?gclid=CjwKCAjwwb6lBhBJEiwAbuVUSr3fsi_ydaZahDxkuTAKQDShHD6ZyhtXRaB8YJN0UJccNHmBy7ZsCBoCr5wQAvD_BwE
## Install ROS Noetic and MAVROS [link][noeticlink] <br>
   1) Install "ROS-Base: (Bare Bones)" (Not full ROS) ```sudo apt install ros-noetic-ros-base```  <br>
   2) To source the environment type this ```source /opt/ros/noetic/setup.bash``` in your bashrc file by ```echo "source /opt/ros/noetic/setup.bash" >> ~/. bashrc``` and then source by ```source ~/.bashrc```
   3) Make a catkin workspace by ```mkdir -p ~/catkin_ws/src``` and Go to catkin_ws ```cd ~/catkin_ws/```
   4) Type this command to install catkin tools ```sudo apt install python3-catkin-tools python3-osrf-pycommon```
   5) build the workspace by ```catkin build```
 
  [noeticlink]: http://wiki.ros.org/noetic/Installation/Ubuntu
   
1) Install MAVROS from source - ([link][mavrosfromsource]) <br>
   1) ```cd ~/catkin_ws```
   2) ```catkin init```
   3) ```wstool init src```
   4) ```rosinstall_generator --rosdistro kinetic mavlink | tee /tmp/mavros.rosinstall```
   5) ```rosinstall_generator --upstream mavros | tee -a /tmp/mavros.rosinstall```
   6) ```wstool merge -t src /tmp/mavros.rosinstall```
   7) ```wstool update -t src -j4```
   8) ```rosdep install --from-paths src --ignore-src -y ``` 
   9) ```sudo ./src/mavros/mavros/scripts/install_geographiclib_datasets.sh``` (This line will through some error.) Solve this by <br> 
     i) Download the missing files as follows <br>
        ```wget https://sourceforge.net/projects/geographiclib/files/magnetic-distrib/emm2015.tar.bz2 --no-check-certificate``` <br>
	```wget https://sourceforge.net/projects/geographiclib/files/gravity-distrib/egm96.tar.bz2 --no-check-certificate``` <br>
	```wget https://sourceforge.net/projects/geographiclib/files/geoids-distrib/egm96-5.tar.bz2 --no-check-certificate``` <br>
     ii) extract, copy and paste to usr/share/GeographicLib <br>
        ``` sudo tar -xf emm2015.tar.bz2 -C /usr/share/GeographicLib/```<br>
	``` sudo tar -xf egm96.tar.bz2 -C /usr/share/GeographicLib/```<br>
	``` sudo tar -xf egm96-5.tar.bz2 -C /usr/share/GeographicLib/```<br>
   10) ```catkin build```
   11) ```source devel/setup.bash```
   12) Add  ```source devel/setup.bash``` to .bashrc file as follow. <br>
      i) go to home directory or just type ```cd``` <br>
      ii) open bashrc file ```nano ~/.bashrc``` <br>
      iii) Paste this line at the end ```source devel/setup.bash``` <br>
      iv) Save it and source the bashrc file as ```source ~/.bashrc``` <br>

2) Check mavros is running or not (Note: Make sure you connect your RPI with pixhawk using USB connection) <br>
   1) Open a terminal and type ```roslaunch mavros px4.launch```

[mavrosfromsource]:https://docs.px4.io/main/en/ros/mavros_installation.html#source-installation

## Making a custom ROS package 'hcrlab'
1) Go to catkin/src by ```cd catkin_ws/src```
2) Make a package by commands ```catkin_create_pkg hcrlab std_msgs rospy roscpp sensor_msgs``` - here we have added 'sensor_msgs' dependency which is used to get the drone's data
3) go to catkin_ws and build the package by i) cd ```cd catkin_ws``` and ```catkin build```

## TP link wifi router information
[better to go with default- make router in AP mode] <br>
Just follow the steps in this video for AP - [link][tplinkvid]

The default ip address would be - 192.168.0.254 (For more info see at the back of the device)
[tplinkvid]: https://youtu.be/ISUSlTcgWks
We have following username and password of the router <br>
username - ```hcrlab``` <br>
password - ```hcrlab8318``` <br>

## VIM 

1) Best video - (1) https://youtu.be/_NUO4JEtkDw (2) VIM + tmux - https://youtu.be/5r6yzFEXajQ
2) ```$vimtutor``` - to get started learning with VIM
3) Three modes i) Normal ii) Insert iii) Visual
4) ```h - j - k - l``` left up down right
5) ```gg``` - go to top of file
6) ``` diw``` - delete in word
7) ```di)``` - yank all text inside parentheses
8) ```dd``` delete current line
9) ```:wq``` write current file and quit
10) ```ZQ``` write current file, if modified and quit
11) ```ZQ``` Quit without checking for changes 
12) ```:w``` write current file
13) 


## BASH scripting

1) Bourne Again Shell
2) ```echo``` - to display the text you pass

## For PX4 based Customizable controller

### Usefull links

1) Main webpage - https://docs.px4.io/master/en/dev_setup/getting_started.html
2) Available messages on PX4 Autopilot - https://github.com/PX4/PX4-Autopilot/tree/master/msg
3) Online data analyser - https://logs.px4.io/
4) Compatible version of QGroundController (v 3.5.6) for real-time ploting of sensor data - https://github.com/mavlink/qgroundcontrol/releases/tag/v3.5.6
5) Serial port configuration - https://docs.px4.io/master/en/peripherals/serial_configuration.html

### Installation for Ubuntu Environment

1) Installation for Ubuntu machine - https://docs.px4.io/master/en/dev_setup/dev_env_linux_ubuntu.html, Youtube link - https://www.youtube.com/watch?v=OtValQdAdrU&t=7s

    a) First install git - ```sudo apt-get install git```\
    b) clone the git repo - ```git clone https://github.com/PX4/PX4-Autopilot.git --recursive```\
    c) Go to firmware directory type - ```bash ./PX4-Autopilot/Tools/setup/ubuntu.sh```\
    d) Now reboot the PC - ```sudo reboot now```\
    e) install SITL - go to firmware directory again and run - ```make px4_sitl jmavsim```\
    f) Try to take the drone using - ```commander takeoff```\
    g) To land use - ```commander land```

### Building for NuttX/Pixhawk Based Boards

1) Build NuttX by running this command\

* Pixhawk Cube Orange ```make cubepilot_cubeorange``` 
* Pixhawk 4: ```make px4_fmu-v5_default```
* Pixhawk 4 Mini: ```make px4_fmu-v5_default```
* CUAV V5+: ```make px4_fmu-v5_default```
* CUAV V5 nano: ```make px4_fmu-v5_default```
* Pixracer: ```make px4_fmu-v4_default```
* Pixhawk 3 Pro: ```make px4_fmu-v4pro_default```
* Pixhawk Mini: ```make px4_fmu-v3_default```
* Pixhawk 2 (Cube Black): ```make px4_fmu-v3_default```
* mRo Pixhawk 2.4.8 : ```make px4_fmu-v3_default (supports 2MB Flash)```
* Holybro pix32: ```make px4_fmu-v2_default```
* Pixfalcon: ```make px4_fmu-v2_default```
* Dropix: ```make px4_fmu-v2_default```
* Pixhawk 1: ```make px4_fmu-v2_default```
  
2) Note donw the serial ports' locations once you build the firmware. For Pixhawk CubeOrange you will get following.
	
	a) -- SERIAL_GPS1 /dev/ttyS2 \
	b) -- SERIAL_GPS2 /dev/ttyS5 \
	c) -- SERIAL_TEL1 /dev/ttyS0 \
	d) -- SERIAL_TEL2 /dev/ttyS1

4) Flash/upload the code on hardware.

	a) For Pixhawk 2.4.8 - ```make px4_fmu-v3_default upload```\
	b) For Pixhawk CubeOrange - ```make cubepilot_cubeorange upload```

6) In order to clean all the firmware - ```git submodule update --recursive```, ```make distclean```

### Making first application - https://dev.px4.io/v1.11_noredirect/en/apps/hello_sky.html

1) First Goto PX4-Autopilot -> src -> examples
2) Make a directory named [px4_simple_app]. Make a .c file named [px4_simple_app.c] and paste the following basic code

``` 
#include <px4_platform_common/log.h>

__EXPORT int px4_simple_app_main(int argc, char *argv[]);

int px4_simple_app_main(int argc, char *argv[])
{
    PX4_INFO("Hello Sky!");
    return OK;
}
```

3) Create and open a new cmake definition file named CMakeLists.txt. Copy in the text below:

```
px4_add_module(
    MODULE examples__px4_simple_app
    MAIN px4_simple_app
    STACK_MAIN 2000
    SRCS
        px4_simple_app.c
    DEPENDS
    )
```

4) A. (For Pixhawk 2.4.8) : Now go to PX4-Autopilot-> boards -> px4 -> fmu-v3 -> open default.px4board file. and enter following line of code in it\
```CONFIG_EXAMPLES_PX4_SIMPLE_APP=y```\
   B. (For Pixhawk CubeOrange) : Now go to PX4-Autopilot-> boards -> cubepilot -> cubeorange -> open default.px4board file. and enter following line of code in it\
```CONFIG_EXAMPLES_PX4_SIMPLE_APP=y```


6) Now again build the app and upload to the hardware by - ```make px4_fmu-v3_default```, ```make px4_fmu-v3_default upload```

7) To see whether or not the app is built successfully, verify it by using this link - https://dev.px4.io/v1.11_noredirect/en/apps/hello_sky.html#connect-the-console

### Subscribing to sensor data - list of topics - https://github.com/PX4/PX4-Autopilot/tree/master/msg

Individual message channels between applications are called topics. Take an example of sensor_combined topic. \
1) Include sensor_combined header file in your app- ```#include <uORB/topics/sensor_combined.h>```
2) Add ```int sensor_sub_fd = orb_subscribe(ORB_ID(sensor_combined));``` in the main function.\
    ```sensor_sub_fd``` is a topic handle and can be used to very efficiently perform a blocking wait for new data. The current thread goes to sleep and is woken up automatically by the scheduler once new data is available, not consuming any CPU cycles while waiting. To do this, we use the ```poll()``` POSIX system call. Adding ```poll()``` to the subscription looks like (pseudocode, look for the full implementation below). The final code is here - [reading_accelerometer_data]

3) Combined code for vehicle attitude with the accelerometer data - [reading_accelerometer_attitude_data]




### For MAVLink Shell/console - https://dev.px4.io/v1.11_noredirect/en/debug/mavlink_shell.html

* Option - 1 (Using mavlink shell)

1) First shutdown the QGroundControl and install dependancies by - ```sudo pip3 install pymavlink pyserial```
2) Open the terminal in Firmware directory and run this code - ```./Tools/mavlink_shell.py /dev/ttyACM0```

* Option - 2 (Using mavlink console in QGroundControl)

1) Just open QGroundControl and run codes. It is same as Option - 1

### What is file descriptor? (Not finished)

1) It is an index the file descriptor table.
2) For each process in an operating system, there is a process control box (PCB). PCB keeps track of the context of the process. One of the field within this is called as file descriptor table (FDT). FDT holds pointers to 'resources'. The resources means a) Files, b) terminal i/o, c) pipes, d) sockets, e) devices etc. 

### PX4 SITL RTPS Gazebo (Not finished)

1) Run these code from PX4-Autopilot directory - ```make px4_sitl_rtps gazebo```

### Logging data to SD card

1) Logger is able to log any uORB topic. The output format of log file is ULog -> https://docs.px4.io/master/en/dev_log/ulog_file_format.html
2) By default logging is started when we arm quadcopter and stop when we disarm thr quadcopter. 
3) To arm and disarm the quadcopter use the code - ```logger on``` and ```logger off```
4) To get a list of all supoorted logger commands and parameters use - ```logger help```
5) To create a custom message\
    a) Remove the SD card from PIXHAWK and open it on PC with SD card reader.\
    b) Create a ```etc``` folder, inside it create ```logging``` folder, inside it create text file named ```logger_topics.txt```. Finally, you will have ```etc/logging/logger_topics.txt```.\
    c) Just mention the name of msg in this txt file as follows
    ```
    adc_report
    airspeed
    ekf_gps_position
    sensor_combined
    sensor_baro
    vehicle_attitude
    vehicle_air_speed
    inpur_rc
    ```
    d) Remove the SD card from PC and insert it into Pixhawk.\
    e) Just go to mavlink console use the code ```logger start``` and ```logger stop```\
    f) Now, you can download the logged file from 'Log Download' tab on QGroundControl\
    g) The downloaded file will have extension of .ulog which is in binary format. We need to download pyulog package to read it. Install it using - ```pip3 install pyulog```\
    h) Use the command ```ulog2csv``` to convert the ulog file into .csv format - ```ulog2csv (filename.ulog)```


### To plot the customize sensor data to QGroundControl

1) This is only valid for QGroundControl v 3.5.6 only. We need to use mavlink protocol.
2) The file that is helpful is located at Firmware-> src -> examples -> px4_mavlink_debug -> px4_mavlink_debug.cpp
3) To unable it, we need to uncomment name of this app from Firmware -> boards -> px4 -> fmu-v3 -> default.cmake in example section
4) Now, open the QGroundControl v 3.5.6. Go to mavlink console and run ```px4_mavlink_debug``` code.
5) Now, you can go to widgets -> Analyze. You can see now the avaialable sensor name.

## For CAM device

1) Serial port configuration - https://docs.px4.io/master/en/peripherals/serial_configuration.html
2) Change the parameters [MAV_2_CONFIG] to TELEM 2, [MAV_2_RATE] to 38400 (considering 4 bits of data), 
3) Bit rate (B/s) = baud rate * the number of bit per baud - i.e., if buad rate = 9600,\\

    a) Bit rate – the number of binary ‘bits’, 1s or 0s to be transmitted per second\
    b) Baud rate – the number of line ‘symbols’ transmitted per second

4) Serial port mapping - (the location on the firmware is - PX4-Autopilot/boards/px4/fmu-v3/default.cmake)\
    SERIAL_PORTS\
	* GPS1:/dev/ttyS0\
	* TEL1:/dev/ttyS1\
	* TEL2:/dev/ttyS2\
	* TEL4:/dev/ttyS6\

5) Sample code - https://programmer.help/blogs/5e35463a2dbdb.html








## For Ardupilot Customizable controller

### Usefull links

1) Main webpage for developer- https://ardupilot.org/dev/docs/building-setup-linux.html
2) To clean ardupilot github repository on Ubuntu ```./waf distclean```
3) Learning base code - https://ardupilot.org/dev/docs/learning-the-ardupilot-codebase.html
4) Ardupilot Libraries for sensors, controller - https://ardupilot.org/dev/docs/apmcopter-programming-libraries.html
5) Link to build the code - https://github.com/ArduPilot/ardupilot/blob/master/BUILD.md

### Getting started with Ardupilot code

1) To download Ardupilot repo\
    $ git clone --recursive https://github.com/ArduPilot/ardupilot.git
2) Go to ardupilot directory and type following code to build the fmv3 version. After successfully building it you will see fmv3 named folder in build directory. After that run following code to burn on Pixhawk\
    ```./waf configure --board fmuv3```\
    ```./waf copter```\
2.1) Go to ardupilot directory and type following code to build the fmv2 version. After successfully building it you will see pixhawk1 named folder in build directory. After that run following code to burn on Pixhawk\
    ```./waf configure --board pxhawk1```\
    ```./waf copter```
3) To upload the code on the Pixhawk use following code in ardupilot main directory\
    ```./waf --targets bin/arducopter --upload```
    or \
    ```./waf copter --upload```

4) If you got the error  ```Waf: Entering directory `/home/user/ardupilot/build/navio2'
Command ['/usr/bin/git', 'rev-parse', '--short=8', 'HEAD'] returned 128 ``` when you are working on git based ardupilot directory.

Go to Tools/ardupilotwaf/git_submodule.py and modified following method

```
def _git_head_hash(ctx, path, short=False):
    cmd = [ctx.env.get_flat('GIT'), 'rev-parse']
    if short:
        cmd.append('--short=8')
    cmd.append('HEAD')
    out = ctx.cmd_and_log(cmd, quiet=Context.BOTH, cwd=path)
    return out.strip()
```
    
to 

```
def _git_head_hash(ctx, path, short=False):
    return "deadc0de"
```
    
    

### UART Ports on Ardupilot https://ardupilot.org/dev/docs/learning-ardupilot-uarts-and-the-console.html

1) Followings are the UART ports on the Pixhawk
* uartA - the console (usually USB, runs MAVLink telemetry) (Serial 0)
* uartB - the first GPS  (Serial 3) [+5v], [TX], [RX], [CAN2 TX], [CAN2RX], [GND]
* uartC - primary telemetry (telem1 on most autopilots)  (Serial 1) [+5v], [TX], [RX], [CTS], [RTS], [GND]
* uartD - secondary telemetry (telem2 on most autopilots)  (Serial 2) [+5v], [TX], [RX], [CTS], [RTS], [GND]
* uartE - 2nd GPS (Serial 4/5)  (Serial 4 and 5) [5v], [TX4], [RX4], [TX5], [RX5], [GND]
* uartF - User Configurable  (Serial 0)
* uartG - User Configurable  (Serial 0)
* uartH - User Configurable  (Serial 0)

2) Some basic codes
* printf - formatted print
* printf_P - formatted print with progmem string (saves memory on AVR boards)
* println - print and line feed
* write - write a bunch of bytes
* read - read some bytes
* available - check if any bytes are waiting
* txspace - check how much outgoing buffer space is available
* get_flow_control - check if the UART has flow control capabilities

### For mission planner message - ```https://ardupilot.org/dev/docs/debug-with-send-text.html```

### Useful notes


1) The "_with_bl.hex": (it will be at build->name_of_board-> bin) file contains the bootloader and is used for flashing to Ardupilot from other firmware... 
2) The "..apj": (it will be at build->name_of_board-> bin) is used by mission planner to update firmware on existing Ardupilot installations as a local firmware file

### If functions in UserCode.cpp files are disabled/showing less brightened

1) Go to APM_Config.h file and uncomment the respective functions.


    
    
## Some Ubuntu Shortcuts

1) To check the working directory -  ```pwd```
2) To list down all the directories - ```ls -al```
3) To open a file - ```gedit (name of file)```

[reading_accelerometer_data]: https://github.com/pratikiitgn/Bigger_quadcopter/blob/main/PX4_autopilot/reading_sensor_data/reading_accelerometer_data.c 
[reading_accelerometer_attitude_data]: https://github.com/pratikiitgn/Bigger_quadcopter/blob/main/PX4_autopilot/reading_sensor_data/reading_accelerometer_attitude_data.c

## Useful notes for ROS

1) To know the ros version - ```rosversion -d```
2) To make a catkin_ws\
	a) ```mkdir -p ~/catkin_ws/src```\
	b) ```cd ~/catkin_ws/```\
	c) ```catkin_make```\




## With RPi3 (MAVROS)

### Guidlines
reference video - https://www.youtube.com/watch?v=Irko6xb2qjs\\

Helpful links
https://ubuntu.com/download/raspberry-pi \\

Make sure that your SD card is formatted.

1. Download image and burn to SD card with Etcher

2. The first challenge on boot of Pi is setting up the wireless network. This will require a keyboard and ethernet connection to Pi. I've documented a bit in this gist:
https://gist.github.com/dbaldwin/fa1baac11b0ae2f000092b695c3d0b33

3. After getting the network setup you'll want to SSH into the Pi. At least that's what I did.

4. Now logged into Pi I used the following commands to get ROS Melodic installed:

sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'

sudo apt-key adv --keyserver 'hkp://keyserver.ubuntu.com:80' --recv-key C1CF6E31E6BADE8868B172B4F42ED6FBAB17C654

sudo apt update

sudo apt install ros-melodic-ros-base

echo "source /opt/ros/melodic/setup.bash" >> ~/.bashrc

source ~/.bashrc

5. Install MAVROS

sudo apt-get install ros-melodic-mavros ros-melodic-mavros-extras

6. Install Geographic datasets
wget https://raw.githubusercontent.com/mavlink/mavros/master/mavros/scripts/install_geographiclib_datasets.sh
chmod a+x install_geographiclib_datasets.sh
sudo ./install_geographiclib_datasets.sh

7. Connect to Pixhawk
Plug pixhawk serial port into RPi 4 USB

8. Launch MAVROS and see Pixhawk telemetry!
roslaunch mavros px4.launch

9. There may be a permissions issue with the serial port. Make sure the user launching mavros is in the "dialout" group.
sudo usermod -a -G dialout db

### With Rpi 3 Model A+

GPIO PINS layout 

https://github.com/pratikiitgn/Bigger_quadcopter/blob/main/Rpi/Photos/GPIO_Pin_rpi.png.png


Username - ```pi```
Passward - ```raspberry```

To turn off - ```sudo shutdown -h now```\

To configure the wifi - https://forums.raspberrypi.com/viewtopic.php?t=210245\
To configure RX TX pin - https://www.electronicwings.com/raspberry-pi/raspberry-pi-uart-communication-using-python-and-c\


