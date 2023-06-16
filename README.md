## Rpi4 8 gb RAM
1) Download raspberry pi imager - https://www.raspberrypi.com/software/
2) Select -> Other general-purpose OS -> Ubuntu -> Ubuntu Server 20.04.5 LTS (64-bit)
3) Go to settings (gear icon) -> i) Set hostname = raspberrypi2, ii) Enable SSH = tick Use password authentication, iii) username - raspberrypi2, password - raspberry, iv) Configure wireless LAN: SSID - pratik, password - hcrlab8318

## TP link wifi router information

username - ```hcrlab``` <br>
password - ```hcrlab8318``` <br>
ip address - ```192.168.0.254```, <br>
Netmask - ```255.255.255.0```, <br>
Gateway - ```0.0.0.0```, <br>
Primary DNS: ```0.0.0.0```, <br>
Secondary DNS: ```0.0.0.0```,<br>
MTU Size:```1500```, <br>
Mode: ```802.11b/g/n```, <br>


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

1) Build NuttX by running this command for Pixhawk 2.4.8 - ```make px4_fmu-v3_default```
2) For other board use following commands
* Pixhawk 4: ```make px4_fmu-v5_default```
* Pixhawk 4 Mini: ```make px4_fmu-v5_default```
* CUAV V5+: ```make px4_fmu-v5_default```
* CUAV V5 nano: ```make px4_fmu-v5_default```
* Pixracer: ```make px4_fmu-v4_default```
* Pixhawk 3 Pro: ```make px4_fmu-v4pro_default```
* Pixhawk Mini: ```make px4_fmu-v3_default```
* Pixhawk 2 (Cube Black): ```make px4_fmu-v3_default```
* mRo Pixhawk: ```make px4_fmu-v3_default (supports 2MB Flash)```
* Holybro pix32: ```make px4_fmu-v2_default```
* Pixfalcon: ```make px4_fmu-v2_default```
* Dropix: ```make px4_fmu-v2_default```
* Pixhawk 1: ```make px4_fmu-v2_default```

3) To flash on hardware - ```make px4_fmu-v3_default upload```
4) In order to clean all the firmware - ```git submodule update --recursive```, ```make distclean```

### Making first application - https://dev.px4.io/v1.11_noredirect/en/apps/hello_sky.html

1) First Goto Px4-Autopilot -> src -> examples
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

4) Now go to Px4_developer-> Firmware -> boards -> px4 -> fmu-v3 -> open default.cmake file. and enter following line of code in it\
```examples/px4_simple_app```


5) Now again build the app and upload to the hardware by - ```make px4_fmu-v3_default```, ```make px4_fmu-v3_default upload```

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


