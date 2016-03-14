# Opensoft-2016
Repository for LBS Opensoft

## Installing libharu
<code>
cd libharu
cmake .
make
sudo make install
sudo ldconfig -v
</code>

## Compiling create_table.cc
<code>
g++ -c create_table.cc -o create_table.o
gcc create_table.o -L"/usr/local/lib" -lhpdf -o create_table
</code>
