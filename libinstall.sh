#!/bin/bash

# leptonica-1.73.tar.gz & tesseract-master.tar.gz are to be put in the Backend folder

clear
#Removing LibLept (if installed)
sudo apt-get --purge autoremove liblept4 -y

#Installing Ghostscript
sudo apt-get install ghostscript -y

#Installing Leptonica-1.73
tar -zxvf /Backend/leptonica-1.73.tar.gz
cd leptonica-1.73
./autobuild
./configure
make
sudo make install
cd ..

#Installing Tessetact-OCR
tar -zxvf /Backend/tesseract-master.tar.gz
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


