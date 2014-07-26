# install ino
wget http://arduino.googlecode.com/files/arduino-1.0.5-linux64.tgz
tar -xvf arduino-1.0.5-linux64.tgz
sudo mv arduino-1.0.5/ /usr/local/share/arduino
sudo apt-get install aptitude
sudo aptitude update && sudo aptitude -y install ia32-libs
sudo pip install ino jinja2 pyserial
