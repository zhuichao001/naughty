wget http://ftp.gnu.org/gnu/gcc/gcc-5.3.0/gcc-5.3.0.tar.gz
tar -xvzf gcc-5.3.0.tar.gz

cd gcc-5.3.0
./contrib/download_prerequisites

cd ..
mkdir build_gcc_5.3.0

cd build_gcc_5.3.0
../gcc-5.3.0/configure --enable-checking=release --enable-languages=c,c++ --disable-multilib  

make -j8
sudo make install
