# Opensoft-2016
Repository for LBS Opensoft

## Installing libharu
```shell
cd libharu
cmake .
make
sudo make install
sudo ldconfig -v
```

## Compiling create_table.cc
```shell
g++ -c create_table.cc -o create_table.o
gcc create_table.o -L"/usr/local/lib" -lhpdf -o create_table
```

## Installing ghostscript
```shell
sudo apt-get install ghostscript
```
