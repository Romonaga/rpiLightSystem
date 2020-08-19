# rpiLightSystem
<b>RPI LED Light System, is simply, a way to control multiple LED lights over the internet.

*** I have noticed people have been cloning, my instructions are not complete, and if you have questions please feel free to open an issue.

While we have provided source code, as well as instructions for building system.  What is not provided here is the hardware, nor the electronics schematics required to drive the light strips. It is assumed that you, the user of this software, understand and have the background in working with these strips.  LumaWin, does provide kits, that are prebuilt, and ready to go with all the required hardware for installation, as well as a preconfigured Raspberry Pi.  All you need to do is set it up.

Features.
* Customizable Lightshows (done)
* Led Matrix Support (done) (Yes you can do a LED Wall with this!)
* Support For led board 2121SMD These are well known, and many know them by Adfruit. (Done!), now larger boards more affordable can be used!.
* User Drawn Pixal Art Support (done) (PITA)
* Save and Edit Matrix Art. (Done).
* Load JPG/PNG into matrix (done).
* Save User Created Art As JPG, allowing you to print and share your art!
* Scrolling Text (done) (PITA)
* Animated GIF support (Done)
* Support for 2 channels (In Testing) Lots of work still needs to be done.
* Lights By Motion. (done)
* Lights By Light Sensor. (done)
* Lights By Timer. (done)
* Lights By Lux Sensor BH1750FVI.  (WINGONEER GY-30 BH1750FVI) (done)
* Precanned shows. (always a work in progress)
* Ability to save playlists. (done)
* Ability to edit saved playlists. (done)
* Ability to Get Status From lightSystem (done)
* Control as many LightSystems/Lightshows as pi Memory allows. (not hit my limit yet)
* Fully open source. (yep, it is)
* Grouped Light Systems, control Light systems as a group. (not Started Yet)
* Simple to use, easy to setup.  
* Website for controling the Lightshows is work produced by Myrana. (always a work in progress)
* Twitch Pannel Support (Phase 1 Released and Live)

</b>



Everything that is needed to build and run this project is or will be included in this repository. This will save you time finding the repos needed.  Please follow the build instructions and install instructions for all libs that I have used.  While we have attempted to document as best as possible, it is always best to follow the directions provided by package owner.

    1. DB instructions are needed.  However, if you are willing to build the system, I have to assume you can install MySql.
    2. The etc config file needs some parms.  this should be self explanitory when you look at it.  Yes, it needs to be in etc.
    3. Apache2 needs to be installed as well. 

<b>These instructions are for Rasperry Pi Buster. It is assumed you are using the latest version of buster.</b>

<b>* First Some machine prep.</b> <BR>
Lets be clear, regardless if you are driving ws2811, or the 2121 boards, you will need to understand a few things.
    
   1. rpiLights should not be ran under a GUI, it should be headless with the bare requirements to run. 
   2. The Pi running rpiLights, should have the following disabled.
   
        a) Sound card.
            cd /etc/modprobe.d
            sudo vi alsa-blacklist.conf
            Enter the following line
            blacklist snd_bcm2835
            Save the file.
            
         b) I would also suggest editing /boot/config.txt look for line dtparam=audio=on and set it to dtparam=audio=off, if the line does not exist, add it.
         
         c) One Wire GPIO, should be turned off, in raspi-config, under interfaces, it will cause interference with the Pannels.
         
   3. If you want to make use of the LUX, in raspi-config, under interfaces, turn on i2c.
   4. If you want to use the spi to drive the LEDS, in raspi-config, under interfaces, turn on spi.
   5. The ws281X LEDS, will consume far less CPU, then the 2121 will, so if you are using these panels, number 1, is even more important.
   6. Drving the 2121 boards from teh Pi can be done, but expect flickering and colors that are not as true.  To overcome this, you can create the required buffer chips to boost the signal lines from 3.3V to 5V.  Or you can purchase a Matrix Hat that will provide the buffers for you.
   

1. sudo apt install git qt5-default libqt5sql5-mysql build-essential gcc make cmake cmake-gui cmake-curses-gui libssl-dev wiringpi libi2c-dev libcppunit-dev
2. sudo apt install scons (Needed for Ws2811lib.)
3. clone or download DNRLogger from this repo.  
    1. qmake .
    2. make
    3. sudo make install
4. Time to build the WS2811 support.
    1. clone or download the rpi_ws281x lib from  this repo. <b>** You must use the one from this repo, it has changes the wrapper needs. I am hoping my changes make it into the main repo at some point. **</b>
    2. run scons, this will build the lib.
        1. sudo  cp *.h /usr/local/include/.
        2. sudo cp libws2811.a /usr/local/lib/.
5. Lets build my wrapper around that fantastic lib!
6. clone or download Ws2811Wrapper from this repo
    1. run qmake .
    2. make
    3. sudo make install
7. Ok now for the harder one, support for MQQT (mosquitto)
    1. clone or download both paho repos.
    2. We fist need to build and install the c version, before we can do the c++.
        1. The read me is clear, you will need some support libs.  While I will outline docs are best.
            1. sudo apt install build-essential gcc make cmake cmake-gui cmake-curses-gui libssl-dev doxygen graphviz
        2. Now we can run make
        3. sudo make install
    3. This next one is not so forward, however the README.md does explain very well how to build this.      
        1. cmake -Bbuild -H. -DPAHO_BUILD_DOCUMENTATION=TRUE -DPAHO_BUILD_SAMPLES=TRUE
        2. $ sudo cmake --build build/ --target install
        3. $ sudo ldconfig
 8. Time to build the projects MQTT wrapper.
    1. clone or download the MQTTMessageBus from this repo
        1. qmake .
        2. make
        3. sudo make install
 9. If you made it this far, well, it seems you can now build the rpiLightsSystem project.
    1. clone or download the rpiLightsSystem
    2. qmake .
    3. make
10. Congrats, you are one step closer, we still have work to do.
    1. From the rpiLightSystems folder, you will need the file from conf, it needs to be placed in /etc please make sure you edit the file to march your system settings.
11. //TODO Website instructions.
12. //TODO mosquitto install instruction. 
13. //TODO sql instructions.





MySQL Server.

Mosquetto MQTT as well as dev libs.

  https://github.com/Romonaga/paho.mqtt.c
  https://github.com/Romonaga/paho.mqtt.cpp

PHP


Apache
  Website code forked from Myrana.

