This is a document to build xorg ports for FreeBSD on RaspberryPi

1: preparation

you **must** build a FreeBSD RaspberryPi image with option
 "MALLOC_PRODUCTION=yes". if you miss this option, it will
 cause to fail building glib20 with jemalloc issue (Failed
 assertion). if you build your image by crochet-freebsd,
 you can use option "__MAKE_CONF" to specify your "make.conf".

 "WITH_NEW_XORG=yes" is available. if you want, you can set
 it in "make.conf".

2: getting ports tree

you can build image with preinstalled ports tree to use option
 "UsrPorts" in crochet-freebsd. or you can get ports tree manually
 on your RPI.

    # portsnap fetch
    # portsnap extract
    # cd /usr/ports
    # make fetchindex

3: applying changes

to complete building, you might need to apply some changes to your
 ports tree.

(1) x11/xorg-server

this is Aleksandr Rybalko's great job, and he had fixed xorg-server
 issue for FreeBSD ARM.

    # fetch --no-verify-peer https://github.com/rayddteam/x11-servers-xorg-server/archive/master.zip
    # unzip master.zip
    # rm master.zip
    # cd x11-servers-xorg-server-master
    # cp -rf * /usr/ports/x11-servers/xorg-server/
    # cd ../

(2) x11-drivers/xf86-video-scfb

this is also Aleksandr Rybalko's big work to port xf86-video-scfb driver.

    # fetch --no-verify-peer https://github.com/rayddteam/xf86-video-scfb/archive/master.zip
    # unzip master.zip
    # rm master.zip
    # cp -rf xf86-video-scfb-master /usr/ports/x11-drivers/xf86-video-scfb/

(3) x11-font/fontconfig, x11/pixman

by default building, fc-cache, provided x11-fonts/fontconfig, might
 cause to Segmentation fault (core dumped) on ARM(ports/181372).
 x11/pixman have building issue on ARM(ports/181140).
 this repository will fix both issue.

    # fetch --no-verify-peer https://github.com/taguchi-ch/freebsd-ports-xorg-raspberrypi/archive/master.zip
    # unzip master.zip
    # rm master.zip
    # cd freebsd-ports-xorg-raspberrypi-master/x11-fonts/fontconfig/
    # cp -rf * /usr/ports/x11-fonts/fontconfig/
    # cd ../../x11/pixman/
    # cp -rf * /usr/ports/x11/pixman/

(4) graphics/libGL, graphics/dri

building xorg with "WITH_NEW_XORG=yes" on ARM, you will
 meet error of graphics/libGL and/or graphics/dri.
 both issue will fix applying Olivier Cochard-Labbe's patch(ports/176703
 and 176705) to graphics/libGL only. 

    # cd ../../graphics/libGL/
    # cp -rf * /usr/ports/graphics/libGL/

(5) security/libgcrypt

security/libgcrypt had a building issue on ARM (ports/181365 by Rui Paulo).
PR had been commited and this issue fixed.

4: building xorg 

    # cd /usr/ports/x11/xorg/
    # make config-recursive

note: in configure, you do not need any video drivers. RPI video
 driver is provided by xf86-video-scfb. 

    # make install clean

5: building xf86-video-scfb

    # cd /usr/ports/x11-drivers/xf86-video-scfb/
    # ./configure
    # make install clean

6: making xorg.conf

'Xorg -configure' is not worked on RPI, yet. and you need to make
 "/etc/X11/xorg.conf" by yourself. but no problem.

Aleksandr Rybalko made a nice xorg.conf setting for you. what you
 have to do is only copying it to your "xorg.conf". 

the setting is following: 

    Section "Files"
    EndSection

    Section "Module"
        Load       "dbe"
        Disable    "dri"
        Disable    "dri2"
        Disable    "glx"
        SubSection  "extmod"
            Option  "omit xfree86-dga"
        EndSubSection
    EndSection

    Section "ServerFlags"
        Option    "AIGLX"      "false"
        Option    "NoAccel"    "True"
        Option    "NoDRI"      "True"
        Option    "DRI"        "False"
        Option    "DRI2"       "False"
    EndSection

    Section "InputDevice"
        Identifier  "Keyboard1"
        Driver      "kbd"
    EndSection

    Section "InputDevice"
        Identifier  "Mouse1"
        Driver      "mouse"
        Option      "Protocol"      "auto"
        Option      "Device"        "/dev/sysmouse"
    EndSection

    Section "Monitor"
        Identifier  "Monitor"
        Mode "1024x600"
        DotClock        25.175
        HTimings        1024 1048 1148 1200
        VTimings        600 610 620 700
        EndMode
    EndSection

    Section "Device"
    Identifier  "Generic FB"
        Driver      "scfb"
        Option    "NoAccel"    "True"
    EndSection

    Section "Screen"
        Identifier  "Screen"
        Device      "Generic FB"
        Monitor     "Monitor"
        DefaultDepth 16
        SubSection "Display"
            Depth           16
            Modes           "1024x600"
        EndSubsection
    EndSection

    Section "ServerLayout"
        Identifier  "layout"
        Screen      0 "Screen" 0 0
        InputDevice "Mouse1" "CorePointer"
        InputDevice "Keyboard1" "CoreKeyboard"
    EndSection

7: adding setting to rc.conf

    # echo 'dbus_enable="YES"' >> /etc/rc.conf
    # echo 'hald_enable="YES"' >> /etc/rc.conf

8: booting dbus and hald

    # /usr/local/etc/rc.d/dbus start
    # /usr/local/etc/rc.d/hald start

9: booting xorg

    % startx

enjoy!
