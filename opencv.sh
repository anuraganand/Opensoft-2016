# INSTALL openCV latest version.

# version="$(wget -q -O - http://sourceforge.net/projects/opencvlibrary/files/opencv-unix | egrep -m1 -o '\"[0-9](\.[0-9]+)+' | cut -c2-)"
# # echo "Installing OpenCV" $version
# # mkdir OpenCV
# # cd OpenCV
# # echo "Removing any pre-installed ffmpeg and x264"
# # sudo apt-get -qq remove ffmpeg x264 libx264-dev
# # echo "Installing Dependenices"
# # sudo apt-get -qq install libopencv-dev build-essential checkinstall cmake pkg-config yasm libjpeg-dev libjasper-dev libavcodec-dev libavformat-dev libswscale-dev libdc1394-22-dev libxine-dev libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev libv4l-dev python-dev python-numpy libtbb-dev libqt4-dev libgtk2.0-dev libfaac-dev libmp3lame-dev libopencore-amrnb-dev libopencore-amrwb-dev libtheora-dev libvorbis-dev libxvidcore-dev x264 v4l-utils ffmpeg cmake qt5-default checkinstall
# # echo "Downloading OpenCV" $version
# # wget -O OpenCV-$version.zip http://sourceforge.net/projects/opencvlibrary/files/opencv-unix/$version/opencv-"$version".zip/download
# # echo "Installing OpenCV" $version
# # unzip OpenCV-$version.zip
# cd opencv-$version
# mkdir build
# cd build
# cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D OPENCV_EXTRA_MODULES_PATH=/home/agnivo/opencv/opencv_contrib/modules  -D WITH_TBB=ON -D BUILD_NEW_PYTHON_SUPPORT=ON -D WITH_V4L=ON -D INSTALL_C_EXAMPLES=ON -D INSTALL_PYTHON_EXAMPLES=ON -D BUILD_EXAMPLES=ON -D WITH_QT=ON -D WITH_OPENGL=ON ..
# make -j2
# sudo checkinstall
# sudo sh -c 'echo "/usr/local/lib" > /etc/ld.so.conf.d/opencv.conf'
# sudo ldconfig
# echo "OpenCV" $version "ready to be used"

# Install OpenCV 2.4.11

# install dependencies
sudo apt-get update
sudo apt-get install -y build-essential
sudo apt-get install -y cmake
sudo apt-get install -y libgtk2.0-dev
sudo apt-get install -y pkg-config
sudo apt-get install -y python-numpy python-dev
sudo apt-get install -y libavcodec-dev libavformat-dev libswscale-dev
sudo apt-get install -y libjpeg-dev libpng-dev libtiff-dev libjasper-dev
 
sudo apt-get -qq install libopencv-dev build-essential checkinstall cmake pkg-config yasm libjpeg-dev libjasper-dev libavcodec-dev libavformat-dev libswscale-dev libdc1394-22-dev libxine-dev libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev libv4l-dev python-dev python-numpy libtbb-dev libqt4-dev libgtk2.0-dev libmp3lame-dev libopencore-amrnb-dev libopencore-amrwb-dev libtheora-dev libvorbis-dev libxvidcore-dev x264 v4l-utils
 
# download opencv-2.4.11
wget http://downloads.sourceforge.net/project/opencvlibrary/opencv-unix/2.4.11/opencv-2.4.11.zip
unzip opencv-2.4.11.zip
cd opencv-2.4.11
mkdir release
cd release
 
# compile and install
cmake -G "Unix Makefiles" -D CMAKE_CXX_COMPILER=/usr/bin/g++ CMAKE_C_COMPILER=/usr/bin/gcc -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D WITH_TBB=ON -D BUILD_NEW_PYTHON_SUPPORT=ON -D WITH_V4L=ON -D INSTALL_C_EXAMPLES=ON -D INSTALL_PYTHON_EXAMPLES=ON -D BUILD_EXAMPLES=ON -D WITH_QT=ON -D WITH_OPENGL=ON -D BUILD_FAT_JAVA_LIB=ON -D INSTALL_TO_MANGLED_PATHS=ON -D INSTALL_CREATE_DISTRIB=ON -D INSTALL_TESTS=ON -D ENABLE_FAST_MATH=ON -D WITH_IMAGEIO=ON -D BUILD_SHARED_LIBS=OFF -D WITH_GSTREAMER=ON ..
make all -j4 # 4 cores
sudo make install
