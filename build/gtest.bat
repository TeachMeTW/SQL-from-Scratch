git clone https://github.com/google/googletest.git
cd googletest && git checkout 3c958ac && mkdir build && cd build && cmake -G "MinGW Makefiles" .. && MinGW32-make && cd ../../