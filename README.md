# Opensoft-2016
Repository for LBS Opensoft

## Installing libharu
<code>
cd libharu <br>
cmake . <br>
make <br>
sudo make install <br>
sudo ldconfig -v <br>
</code>

## Compiling create_table.cc
<code>
g++ -c create_table.cc -o create_table.o <br>
gcc create_table.o -L"/usr/local/lib" -lhpdf -o create_table <br>
</code>
