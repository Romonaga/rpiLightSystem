#!/usr/bin/env bash

sudo apt install git qt5-default libqt5sql5-mysql build-essential gcc make cmake cmake-gui cmake-curses-gui libssl-dev wiringpi libi2c-dev libcppunit-dev scons doxygen
git clone https://github.com/Romonaga/DNRLogger.git
git clone https://github.com/Romonaga/rpi_ws281x.git
git clone https://github.com/Romonaga/rpi-rgb-led-matrix.git
git clone https://github.com/Romonaga/Ws2811Wrapper.git
git clone https://github.com/Romonaga/paho.mqtt.c
git clone https://github.com/Romonaga/paho.mqtt.cpp
git clone https://github.com/Romonaga/MQTTMessageBus.git
cd DNRLogger
qmake .
make
sudo make install
cd ../rpi_ws281x
scons
sudo cp *.h /usr/local/include
sudo cp *.a /usr/local/lib
cd ../rpi-rgb-led-matrix
make
sudo cp include/*.h /usr/local/include
sudo cp lib/*.a /usr/local/lib
cd ../Ws2811Wrapper
qmake .
make
sudo make install
cd ../paho.mqtt.c
make
sudo make install
cd ../paho.mqtt.cpp
cmake -Bbuild -H. -DPAHO_BUILD_DOCUMENTATION=TRUE -DPAHO_BUILD_SAMPLES=TRUE
sudo cmake --build build/ --target install
sudo ldconfig
cd ../MQTTMessageBus
qmake .
make
sudo make install
cd ../../rpiLightSystem
qmake .
make -j2



