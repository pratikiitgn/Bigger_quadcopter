# Bigger_quadcopter

## For PX4 based Customizable controller

### Usefull links

1) Main webpage - https://docs.px4.io/master/en/dev_setup/getting_started.html
2) Available messages on PX4 Autopilot - https://github.com/PX4/PX4-Autopilot/tree/master/msg
3) 
### Installation for Ubuntu Environment

1) Installation for Ubuntu machine - https://docs.px4.io/master/en/dev_setup/dev_env_linux_ubuntu.html, Youtube link - https://www.youtube.com/watch?v=OtValQdAdrU&t=7s

    a) First install git - ```sudo apt-get install git```\
    b) clone the git repo - ```git clone https://github.com/PX4/Firmware.git --recursive```\
    c) Go to firmware directory type - ```bash ./Tools/setup/ubuntu.sh```\
    d) Now reboot the PC - sudo reboot now\
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

1) First Goto Px4_developer-> Firmware -> src -> examples
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

### Subscribing to sensor data

Individual message channels between applications are called topics. Take an example of sensor_combined topic. \
1) Include sensor_combined header file in your app- ```#include <uORB/topics/sensor_combined.h>```
2) Add ```int sensor_sub_fd = orb_subscribe(ORB_ID(sensor_combined));``` in the main function.\
    ```sensor_sub_fd``` is a topic handle and can be used to very efficiently perform a blocking wait for new data. The current thread goes to sleep and is woken up automatically by the scheduler once new data is available, not consuming any CPU cycles while waiting. To do this, we use the ```poll()``` POSIX system call. Adding ```poll()``` to the subscription looks like (pseudocode, look for the full implementation below). The final code would be as follows...

```
#include <px4_platform_common/px4_config.h>
#include <px4_platform_common/posix.h>
#include <poll.h>
#include <uORB/uORB.h>
#include <uORB/topics/sensor_combined.h>

__EXPORT int reading_sensor_data_main(int argc, char *argv[]);

int reading_sensor_data_main(int argc, char *argv[])
{
  PX4_INFO("Hello Sky!");

    /* subscribe to sensor_combined topic */
    int sensor_sub_fd = orb_subscribe(ORB_ID(sensor_combined));
    /* limit the update rate to 5 Hz */
    orb_set_interval(sensor_sub_fd, 200);

    /* one could wait for multiple topics with this technique, just using one here */
    px4_pollfd_struct_t fds[] = {
        { .fd = sensor_sub_fd,   .events = POLLIN },
    };

    int error_counter = 0;

    for (int i = 0; i < 5; i++) {
        /* wait for sensor update of 1 file descriptor for 1000 ms (1 second) */
        int poll_ret = px4_poll(fds, 1, 1000);

        /* handle the poll result */
        if (poll_ret == 0) {
            /* this means none of our providers is giving us data */
            PX4_ERR("Got no data within a second");

        } else if (poll_ret < 0) {
            /* this is seriously bad - should be an emergency */
            if (error_counter < 10 || error_counter % 50 == 0) {
                /* use a counter to prevent flooding (and slowing us down) */
                PX4_ERR("ERROR return value from poll(): %d", poll_ret);
            }

            error_counter++;

        } else {

            if (fds[0].revents & POLLIN) {
                /* obtained data for the first file descriptor */
                struct sensor_combined_s raw;
                /* copy sensors raw data into local buffer */
                orb_copy(ORB_ID(sensor_combined), sensor_sub_fd, &raw);
                PX4_INFO("Accelerometer:\t%8.4f\t%8.4f\t%8.4f",
                     (double)raw.accelerometer_m_s2[0],
                     (double)raw.accelerometer_m_s2[1],
                     (double)raw.accelerometer_m_s2[2]);
            }
        }
    }

    PX4_INFO("exiting");

    return 0;
}
```

### Reading quadcopter attitude data

1) vehicle_attitude.msg is the name of file provides the attitude of the quadcopter - https://github.com/PX4/PX4-Autopilot/blob/master/msg/vehicle_attitude.msg

2) First Goto Px4_developer-> Firmware -> src -> examples



### For MAVLink Shell/console - https://dev.px4.io/v1.11_noredirect/en/debug/mavlink_shell.html

1) First shutdown the QGroundControl and install dependancies by - ```sudo pip3 install pymavlink pyserial```
2) Open the terminal in Firmware directory and run this code - ```./Tools/mavlink_shell.py /dev/ttyACM0```

























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
    ```./waf copter```
2.1) Go to ardupilot directory and type following code to build the fmv2 version. After successfully building it you will see pixhawk1 named folder in build directory. After that run following code to burn on Pixhawk\
    ```./waf configure --board pxhawk1```\
    ```./waf copter```
3) To upload the code on the Pixhawk use following code in ardupilot main directory\
    ``` ./waf --targets bin/arducopter --upload```

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

### Useful notes


1) The "_with_bl.hex": (it will be at build->name_of_board-> bin) file contains the bootloader and is used for flashing to Ardupilot from other firmware... 
2) The "..apj": (it will be at build->name_of_board-> bin) is used by mission planner to update firmware on existing Ardupilot installations as a local firmware file


## To make a launcher icon in Ubuntu

1) First download the AppImage of the software and then make it executable with command chmod +x (software file)
2) Download suitable icon for the app and put these two files in specific folder
3) make a desktop file by command $ sudo nano /usr/share/applications/(name of application).desktop
4) Paste following commands it\
```
#!/usr/bin/env xdg-open
[Desktop Entry]
Version=1.0
Type = Application
Terminal = false
Exec=(provide path of your app location)
Name = (provide name of your app)
Comment = (Provide description of your app)
Icon = (provide oath of your application's icon)
```
5) Make file executable by ```sudo chmod +x /usr/share/applications/(name of application).desktop```


## Getting started with Git

1) Make a github account and install git on ubuntu machine
2) Configure with this code on terminal - \
    $ git config --global user.name "pratikiitgn"\
    $ git config --global user.email "pkprajapati5959@gmail.com"
3) To check the configuration \
    $ git config --list
4) Make a directory for git in home \
    $ mkdire git_workspace
5) Open git_workspace in terminal and clone the repositary\
    $ git clone (url of your repository)
6) Now, make a file in your repository in ubuntu
    $ touch test.txt
7) To check the status of GitHub repository
    $ git status
8) To add file over the github account
    $ git add (name_of_filename)\
    $ git commit -m "add here your comments" (name_of_your_file)\
    $ git push -u origin master\
    Then after apply your username and password
    
    
## Some Ubuntu Shortcuts

1) To check the working directory -  ```pwd```
2) To list down all the directories - ```ls -al```
3) To open a file - ```gedit (name of file)```
