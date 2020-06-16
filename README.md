# rpiLightSystem
<b>RPI LED Light System, is simply, a way to control multiple LED lights over the internet.

Features.
* Customizable Lightshows
* Many precanned shows.
* Ability to save playlists.
* Ability to edit saved playlists.
* Control as many LightSystems/Lightshows as pi Memory allows.
* Fully open source.
* Simple to use, easy to setup.
* Website for controling the Lightshows is work produced by Myrana.
</b>

This is the start to a large project.  Wife wanted accent lighting, Daugher and I wanted to to be different.  

Everything that is needed to build and run this project is or will be included in this repository. This will save you time finding the repos needed.  Please follow the build instructions and install instructions for all libs that I have used.  While we have attempted to document as best as possible, it is always best to follow the directions provided by package owner.

<b>These instructions are for Rasperry Pi Buster. It is assumed you are using the latest version of buster.</b>

1. sudo apt install git
2. sudo apt install qt5-default libqt5sql5-mysql
3. sudo apt install build-essential gcc make cmake cmake-gui cmake-curses-gui
4. sudo apt install scons (Needed for Ws2811lib.
5. clone or download DNRLogger from this repo.  
    1. qmake .
    2. make
    3. sudo make install
6. Time to build the WS2811 support.
    1. clone or download the rpi_ws281x lib from  this repo.
    2. run scons, this will build the lib.
        1. sudo  cp *.h /usr/local/include/.
        2. sudo cp libws2811.a /usr/local/lib/.
7. Lets build my wrapper around that fanatic lib!
8. clone or download Ws2811Wrapper from this repo
    1. run qmake .
    2. make
    3. sudo make install
9. Ok now for the harder one, support for MQQT (mosquitto)
    1. clone or download both paho repos.
    2. We fist need to build and install the c version, before we can do the c++.
        1. The read me is clear, you will need some support libs.  While I will outline docs are best.
            1. sudo apt install build-essential gcc make cmake cmake-gui cmake-curses-gui
            2. OpenSSL sudo apt install libssl-dev
            3. sudo apt install doxygen graphviz
        2. Now we can run make
        3. sudo make install
    3. This next one is not so forward, however the README.md does explain very well how to build this.
        1. sudo apt-get install libcppunit-dev
        2. cmake -Bbuild -H. -DPAHO_BUILD_DOCUMENTATION=TRUE -DPAHO_BUILD_SAMPLES=TRUE
        3. $ sudo cmake --build build/ --target install
        4. $ sudo ldconfig
10. Time to build the projects MQTT wrapper.
    1. clone or download the MQTTMessageBus from this repo
        1. qmake .
        2. make
        3. sudo make install
11. If you made it this far, well, it seems you can now build the rpiLightsSystem project.
    1. clone or download the rpiLightsSystem
    2. qmake .
    3. make
12. Congrats, you are one step closer, we still have work to do.
    1. From the rpiLightSystems folder, you will need the file from conf, it needs to be placed in /etc please make sure you edit the file to march your system settings.
13. //TODO Website instructions.
14. //TODO mosquitto install instruction. 
15. //TODO sql instructions.





MySQL Server.

Mosquetto MQTT as well as dev libs.

  https://github.com/Romonaga/paho.mqtt.c
  https://github.com/Romonaga/paho.mqtt.cpp

PHP


Apache
  Website code forked from Myrana.

