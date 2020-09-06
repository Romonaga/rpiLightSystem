#!/usr/bin/env bash

set -e
RED='\033[0;31m'
NC='\033[0m' # No Color
YELLOW='\033[1;33m'
LIGHTBLUE='\033[1;34m'


echo -e "${LIGHTBLUE}Installing Required Libs${NC}"
sudo apt install git qt5-default libqt5sql5-mysql build-essential gcc make cmake cmake-gui cmake-curses-gui libssl-dev wiringpi libi2c-dev libcppunit-dev scons doxygen

echo -e "${RED}Checking to see if we need to Clean Up From Prior Runs${NC}"

if [ -d DNRLogger ]; then
	sudo rm -r DNRLogger
fi

if [ -d rpi_ws281x ]; then
	sudo rm -r rpi_ws281x
fi

if [ -d rpi-rgb-led-matrix ]; then
	sudo rm -r rpi-rgb-led-matrix
fi

if [ -d Ws2811Wrapper ]; then

	sudo rm -r Ws2811Wrapper
fi

if [ -d paho.mqtt.c ]; then

	sudo rm -r paho.mqtt.c
fi

if [ -d paho.mqtt.cpp ]; then

	sudo rm -r paho.mqtt.cpp
fi

if [ -d MQTTMessageBus ]; then

	sudo rm -r MQTTMessageBus
fi


if [ -f /usr/local/lib/libpaho-mqtt3a.so.1 ]; then
 sudo rm -r /usr/local/lib/libpaho-mqtt3*.*
fi

echo -e "${LIGHTBLUE}Cloning Repos${NC}"

echo -e "${RED}Cloning DNRLogger${NC}"
git clone https://github.com/Romonaga/DNRLogger.git
echo -e "${RED}Cloning rpi_ws2811x${NC}"
git clone https://github.com/Romonaga/rpi_ws281x.git
echo -e "${RED}Cloning rpi-rgb-led-matrix${NC}"
git clone https://github.com/Romonaga/rpi-rgb-led-matrix.git
echo -e "${RED}Cloning Ws2811Wrapper${NC}"
git clone https://github.com/Romonaga/Ws2811Wrapper.git
echo -e "${RED}Cloning paha MQTT${NC}"
git clone https://github.com/Romonaga/paho.mqtt.c
git clone https://github.com/Romonaga/paho.mqtt.cpp
echo -e "${RED}Cloning MQTT Message Bus${NC}"
git clone https://github.com/Romonaga/MQTTMessageBus.git
echo -e "${YELLOW}Building DNRLogger${NC}"
cd DNRLogger
qmake .
make
sudo make install
echo -e "${YELLOW}Building rpiWs2811${NC}"
cd ../rpi_ws281x
scons
sudo cp *.h /usr/local/include
sudo cp *.a /usr/local/lib
cd ../rpi-rgb-led-matrix
make
sudo cp include/*.h /usr/local/include
sudo cp lib/*.a /usr/local/lib
echo -e "${YELLOW}Building Ws28111Wrapper${NC}"
cd ../Ws2811Wrapper
qmake .
make
sudo make install
echo -e "${YELLOW}Building paho MQTT Support${NC}"
cd ../paho.mqtt.c
make
sudo make install
cd ../paho.mqtt.cpp
cmake -Bbuild -H. -DPAHO_BUILD_DOCUMENTATION=TRUE -DPAHO_BUILD_SAMPLES=TRUE
sudo cmake --build build/ --target install
sudo ldconfig
echo -e "${YELLOW}Building MQTT Message Bus${NC}"
cd ../MQTTMessageBus
qmake .
make
sudo make install
echo -e "${YELLOW}Building rpiLighsystem${NC}"

cd ../../
qmake .
make clean
make -j2





