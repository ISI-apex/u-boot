# You should set up CROSS_COMPILE to use aarch64 compiler
make clean
make CROSS_COMPILE=arm-none-eabi- CONFIG_LD_GCC=y
cp u-boot.bin R52-u-boot.bin
