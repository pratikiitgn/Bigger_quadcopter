# Bigger_quadcopter


## For Ardupilot Customizable controller

1) Main webpage for developer- https://ardupilot.org/dev/docs/building-setup-linux.html
2) To clean ardupilot stack - ./waf clean
3) Learning base code - https://ardupilot.org/dev/docs/learning-the-ardupilot-codebase.html
4) Ardupilot Libraries for sensors, controller - https://ardupilot.org/dev/docs/apmcopter-programming-libraries.html


## To make a launcher icon in Ubuntu

1) First download the AppImage of the software and then make it executable with command chmod +x (software file)
2) Download suitable icon for the app and put these two files in specific folder
3) make a desktop file by command $ sudo nano /usr/share/applications/(name of application).desktop
4) Paste following commands it\
#!/usr/bin/env xdg-open\
[Desktop Entry]\
Version=1.0\
Type = Application\
Terminal = false\
Exec=(provide path of your app location)\
Name = (provide name of your app)\
Comment = (Provide description of your app)\
Icon = (provide oath of your application's icon)
5) Make file executable by $ sudo chmod +x /usr/share/applications/(name of application).desktop


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
    $ git add (name_of_filename)
    $ git commit -m "add here your comments" (name_of_your_file)
    $ git push -u origin master
    Then after apply your username and password
    
    
## Some Ubuntu Shortcuts

1) To check the working directory - $ pwd
2) To list down all the directories - $ ls -al
3) To open a file - $ gedit (name of file)
