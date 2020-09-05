# rpiLightSystem
<b>RPI LED Light System, is simply, a way to control multiple LED lights over the internet.

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

<b>These instructions are for Rasperry Pi Buster. It is assumed you are using the latest version of buster on a fresh install.</b>

<b>* First Some machine prep.</b> <BR>
Lets be clear, regardless if you are driving ws2811, or the 2121 boards, you will need to understand a few things.
    
   1. rpiLightSystem should not be ran with a GUI, it should be headless with the bare requirements to run the system.  
   2. The Pi running rpiLights, should have the following disabled.
   
        a) Sound card.
            cd /etc/modprobe.d
            sudo nano alsa-blacklist.conf
            Enter the following line
            blacklist snd_bcm2835
            Save the file.
            
         b) I would also suggest editing /boot/config.txt look for line dtparam=audio=on and set it to dtparam=audio=off, if the line does not exist, add it.
         
         c) One Wire GPIO, should be turned off, in raspi-config, under interfaces, it will cause interference with the Panels.
         
   3. If you want to make use of the LUX, in raspi-config, under interfaces, turn on i2c.
   4. If you want to use the spi to drive the LEDS, in raspi-config, under interfaces, turn on spi.
   5. The ws281X LEDS, will consume far less CPU, then the 2121 will, so if you are using these panels, number 1, is even more important.
   6. Drving the 2121 boards from teh Pi can be done, but expect flickering and colors that are not as true.  To overcome this, you can create the required buffer chips to boost the signal lines from 3.3V to 5V.  Or you can purchase a Matrix Hat that will provide the buffers for you.
   7. The rpiLightSystem that will be the server for the rest of the rpiLightSystems, should be a P4, simply for the extra horse power.
   
## Build instructions for rpiLightSystem.

There are 2 ways you can build the rpiLightSystem.  

You will find a script in the buildRpiLightSystem, if you run this script, it will attempt to install the required packages, clone the required repositories, then build the project.

Or you can follow the below steps that achive the same thing.


1. sudo apt install git qt5-default libqt5sql5-mysql build-essential gcc make cmake cmake-gui cmake-curses-gui libssl-dev wiringpi libi2c-dev libcppunit-dev scons doxygen  
2 Clone or download DNRLogger from this repo.  https://github.com/Romonaga/DNRLogger
    1. qmake .
    2. make
    3. sudo make install
3 Time to build the WS2811 support.
    1. clone or download the rpi_ws281x lib from  this repo. https://github.com/Romonaga/rpi_ws281x
    <b>** You must use the one from this repo, it has changes the wrapper needs. I am hoping my changes make it into the main repo at some point. **</b>
    2. run scons, this will build the lib.
        1. sudo  cp *.h /usr/local/include/.
        2. sudo cp libws2811.a /usr/local/lib/.
4. Time to build the rpi-rgb-led-matrix 
    1. make 
    2. sudo cp lib/*.h /usr/local/include 
    3. sudo cp lib/*.a /usr/local/lib 
5. Lets build my wrapper around that fantastic lib!
6. clone or download Ws2811Wrapper from this repo https://github.com/Romonaga/Ws2811Wrapper
    1. run qmake .
    2. make
    3. sudo make install
7. Ok now for the harder one, support for MQQT (mosquitto)
    1. clone or download both paho repos. 
    2. We fist need to build and install the c version, before we can do the c++. https://github.com/Romonaga/paho.mqtt.c
        1. The read me is clear, you will need some support libs.  While I will outline docs are best.
            1. sudo apt install build-essential gcc make cmake cmake-gui cmake-curses-gui libssl-dev doxygen graphviz
        2. Now we can run make
        3. sudo make install
    3. This next one is not so forward, however the README.md does explain very well how to build this. https://github.com/Romonaga/paho.mqtt.cpp     
        1. cmake -Bbuild -H. -DPAHO_BUILD_DOCUMENTATION=TRUE -DPAHO_BUILD_SAMPLES=TRUE
        2. $ sudo cmake --build build/ --target install
        3. $ sudo ldconfig
 8. Time to build the projects MQTT wrapper.
    1. clone or download the MQTTMessageBus from this repo https://github.com/Romonaga/MQTTMessageBus
        1. qmake .
        2. make
        3. sudo make install
 8 If you made it this far, well, it seems you can now build the rpiLightsSystem project.
    1. clone or download the rpiLightsSystem https://github.com/Romonaga/rpiLightSystem
    2. qmake .
    3. make

## Now that the rpiLightsystem has been built.  It is now time to setup the website.

The website as well as the supporting servers like mysql, Mosquitto, do not have to run on the same machine as rpiLightSystem.  At our lab, we are running these processes off of our linux server.  However, the system is designed to run on a Raspberry PI.  It is important to note that if you have multiple rpiLightSystems you wish to control, you only need to set-up one device as the master to host the website as well as mysql and mosquitto.

These directions are assuming you wish to setup the website, mysql, mosquitto on a raspberrypi. It also assumes that this machine is configured to access the network. I again can not stress enough that you should do this with a fresh Raspberry Pi.

1. Install Apache 
    * sudo apt install apache2
    
2. Install PHP Support
    * sudo apt install php7.3
    * sudo apt install php7.3-dev
    * Next we want to extend the session life.  
        * sudo nano /etc/php/7.3/apache2/php.ini
            * session.gc_maxlifetime = 43200
            * session.cache_expire = 720

3. Install Mosquitto We need not only the server, but also support libs.
    * sudo apt install mosquitto
    * sudo apt install mosquitto-client
    * sudo apt install libmosquitto1
    * sudo apt install libmosquitto-dev
    
4. Install Mosquitto support for PHP.
    * pecl install Mosquitto-alpha
    * sudo nano /etc/php/7.3/apache2/php.ini
        * add extension=mosquitto.so at the bottom of this file.
    * sudo service apache2 restart
        
5. Now we will install mysql.  I will do my best to explain how to get this running.  While I will do my best to help you do this from the CLI, truth be told, setting up the server and loading the database is far easier if you use mysql workbench.

    * sudo apt install mariadb-server
    * sudo apt mysql_secure_installation This will secure your SQL server, please respond to the questions presented. Please remember your password!
    * sudo apt install php-mysql
   

At this point, we have completed the install of the requiered componants, now lets configure the system for use with rpiLightSystem.

MySQL server is setup, it is setup with the defaults, some of these defaults can get in our way.  It is up to you to decide if you wish to follow directions here or setup the mysql server to your standards.

Default out of the box, Mysql server will only listen in on localhost.  This is fantastic, if the only machine you wish to access the server is the machine it is installed on.  Seeing this is home use, I thnk we can relax the security a bit.  First lets let it listen on any ipaddress. 

sudo nano /etc/mysql/mariadb.conf.d/50-server.cnf look for a line that says. bind-address, it is mostly likly set to bind-address= 127.0.0.1 simply place a # in front so it looks like this #bind-address = 127.0.0.1 save the file.

Restart mysql server. sudo service mysql restart  At this point you should be able to access the mysql server from other machines.

This next part is simplifed if you use mysql workbench, However, that requires a GUI to run, and if you remember, no GUI should be ran.  So....

lets work on getting the database working.

Lets setup the database, as well as an account that can be used by ledLightSystem.  One should not use the root password of the db server.
* sudo mysql -u root -p (You did remember your root password for mysql?)
* create database LedLightSystem; (press enter)
* CREATE USER 'USERID' IDENTIFIED BY 'PASSWORD';
* GRANT ALL PRIVILEGES ON LedLightSystem . * TO 'USERID';
    
Now we will load the database with the sql script.
* sudo mysql -u root -p LedLightSystem < rpiLightSystem/db/LedLightSystem.sql
* sudo mysql -u root -p LedLightSystem < rpiLightSystem/db/LedLightSystemData.sql

<B>Let us take stock of where we are, or should be at this point.</B>
1. We have built rpiLightSystem.
2. We have installed the Web Server.
3. We have installed PHP support for Web Server.
4. We have installed MQTT Server.
5. We have installed support for PHP MQTT.

If all of the above is true, we are ready to move on.

From the rpiLightSystem source code folder you wil find a file called rpilightsystem.conf in folder /etc.  This file will require a few edits, as well as it will need to be copied to the /etc folder of the raspberrypi that is hosting the website.

<b> The setting should be clear. </b>

DBServer= Hostname of server running mysql  
DBUserID= UserID to use  
DBPassword= password for user  
DataBase= database name  
MQTTBroker= Hostname of machine running MQTT server.  

Please make sure that after uou have made the changes, that you put it into /etc

Now, the actual website.

First off, the default location on the rpi is /var/www/html.  This will have a default website from the install.  First, lets remove the index.html as it will get in our way.  sudo rm /var/www/html/index.*

Now, lets copy the Web Site.

First, you should never cloan from me, while I do have a fork of Myra's code, I do not keep it up to date as often as I should, this is why it is best to pull from her. https://github.com/Myrana/LightsWebpage

now all that is needed is to copy that to your web folder.
Please cd to the directory where you pulled the repository.
sudo cp -r * /var/www/html/.


At this point you should be able to access the website.....

The rpilightsystem.conf file is used not only by the rpiLightSystem, but also the website used to control the lights.  As such, the server that is hosting the website needs this file in /etc.

If you have followed the steps correctly, and your rpilightsystem.conf file is correctly configured, we can now move on to setup of the website.
The system has no default users, as such, there is a setup.php that can be called, it is not accessable from the nav bar, but you can type it into the url.

This will allow you to setup the default user of the system.  this will allow you to create new users as well as configure the rest of the system.
This file should be removed once the system is setup sudo rm /var/www/html/setup.php will remove the file.

Once the default user is created, you should be able to log in and configure your light strip, or matrix, or however many rpiLightSystems you are running.


....................... Not the end ...............................







IGNORE THIS SECTION HOLDING AREA
//sudo apt install libportaudocpp0
//sudo apt install portaudio19-dev


   


    

