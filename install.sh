#!/bin/bash

# leptonica-1.73.tar.gz & tesseract-master.tar.gz are to be put in the Backend folder

clear
#Removing LibLept (if installed)
sudo apt-get --purge autoremove liblept4 -y

#Installing Ghostscript
sudo apt-get install ghostscript -y

#Dependencies for ./autobuild in Leptonica-1.73
sudo apt-get install libtool -y
sudo apt-get install autotools-dev -y
sudo apt-get install automake -y

cd Backend

#Installing Leptonica-1.73
tar -zxvf leptonica-1.73.tar.gz
cd leptonica-1.73
./autobuild
./configure
make
sudo make install
cd ..

#Installing Tessetact-OCR
tar -zxvf tesseract-master.tar.gz
cd tesseract-master
./autogen.sh
./configure
make
sudo make install
cd ..

#Installing PyFPDF
cd ..
cd pyfpdf
sudo python setup.py install

sudo apt-get install python-numpy

#Installing Matplotlib and building it.
sudo apt-get install python-matplotlib -y

#Dependencies for Python-OpenCV
sudo apt-get install libjpeg8-dev libtiff5-dev libjasper-dev libpng12-dev -y
#Installing Python-OpenCV
sudo apt-get install python-opencv -y
#compile opencv.sh
cd ..
bash opencv.sh

#compile install.sh

bash compile.sh

