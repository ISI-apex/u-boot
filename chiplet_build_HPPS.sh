# You should set up CROSS_COMPILE to use aarch64 compiler
make clean
make -j4 CROSS_COMPILE=aarch64-poky-linux-
cp u-boot HPPS-u-boot
cp u-boot.bin HPPS-u-boot.bin
