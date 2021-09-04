# Bigger_quadcopter


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
3) To upload the code on the Pixhawk use following code in ardupilot main directory\
    ``` ./waf --targets bin/arducopter --upload```

### UART Ports on Ardupilot

1) Followings are the UART ports on the Pixhawk
* uartA - the console (usually USB, runs MAVLink telemetry)
* uartB - the first GPS
* uartC - primary telemetry (telem1 on most autopilots)
* uartD - secondary telemetry (telem2 on most autopilots)
* uartE - 2nd GPS (Serial 4/5)
* uartF - User Configurable
* uartG - User Configurable
* uartH - User Configurable

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
#!/usr/bin/env xdg-open\
[Desktop Entry]\
Version=1.0\
Type = Application\
Terminal = false\
Exec=(provide path of your app location)\
Name = (provide name of your app)\
Comment = (Provide description of your app)\
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
