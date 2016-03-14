# Opensoft-2016
Repository for LBS Opensoft

## Installing ghostscript
```shell
sudo apt-get install ghostscript
```

## Installing libharu
```shell
cd libharu
cmake .
make
sudo make install
sudo ldconfig -v
```

## Running pdf2png.py
```shell
python pdf2png.py
```
The output images will be saved in Images/

Image names input001.png, input002.png, ...

## Compiling create_table.cc
```shell
g++ -c create_table.cc -o create_table.o
gcc create_table.o -L"/usr/local/lib" -lhpdf -o create_table
```


