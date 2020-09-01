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


<b> Now that the rpiLightsystem has been built.  It is now time to setup the website.</b>

The website as well as the supporting servers like mysql, Mosquitto, do not have to run on the same machine as rpiLightSystem.  At our lab, we are running these things off of a linux server.  However, the system is designed to run on a Raspberry PI.  It is important to note that if you have multiple rpiLightSystems you wish to control, you only need to set-up one device as the master to host the website as well as mysql and mosquitto.

These directions are assuming you wish to setup the website, mysql, mosquitto on a raspberrypi. It also assumes that this machine is configured to access the network. 

1. Install Apache 
    * sudo apt install apache2
    
2. Install PHP Support
    * sudo apt install php7.3
    * sudo apt install php7.3-dev

3. Install Mosquitto We need not only the server, but also support libs.
    * sudo apt install mosquitto
    * sudo apt install mosquitto-client
    * sudo apt install libmosquitto1
    * sudo apt install libmosquitto-dev
    
4. Install Mosquitto support for PHP.
    * pecl install Mosquitto-alpha
    * sudo nano /etc/php7.3/php.ini
        * add extension=mosquitto.so at the bottom of this file.
        
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
* CREATE USER '<USERID>' IDENTIFIED BY '<PASSWORD>';
* GRANT ALL PRIVILEGES ON LedLightSystem . * TO '<USERID>';
    
Now we will load the database with the sql script.
* sudo mysql -u root -p LedLightSystem < <PATHTOFILE>/LedLightSystem.sql
* sudo mysql -u root -p LedLightSystem < <PATHTOFILE>/LedLightSystemData.sql

    







   


    

