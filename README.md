# Bigger_quadcopter


## For Ardupilot Customizable controller

link to page - https://ardupilot.org/dev/docs/building-setup-linux.html


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
Icon = (provide oath of your application's icon)\
5) Make file executable by $ sudo chmod +x /usr/share/applications/(name of application).desktop\
