# Opensoft-2016
Repository for LBS Opensoft

## Installing libharu
```shell
cd libharu
cmake . <br>
make <br>
sudo make install <br>
sudo ldconfig -v <br>
```

## Compiling create_table.cc
```shell
g++ -c create_table.cc -o create_table.o <br>
gcc create_table.o -L"/usr/local/lib" -lhpdf -o create_table <br>
```
