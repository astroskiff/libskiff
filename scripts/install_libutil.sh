git clone https://github.com/astroskiff/libutil
cd libutil
mkdir libutil_build
cd libutil_build
cmake -DCMAKE_BUILD_TYPE=Release ../
make -j9
sudo make install
cd ..