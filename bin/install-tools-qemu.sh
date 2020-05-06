#!/bin/sh

TOOLS_DIR=$HOME/riscv-tools

# Install required packages
sudo apt update
#sudo apt upgrade
sudo apt --yes --force-yes install autoconf automake autotools-dev curl \
  		libmpc-dev libmpfr-dev libgmp-dev gawk build-essential \
  		bison flex texinfo gperf libtool patchutils bc zlib1g-dev pkg-config \
		libglibmm-2.4-dev libpixman-1-dev libconfig-dev libexpat1-dev srecord


mkdir -p $TOOLS_DIR
cd $TOOLS_DIR

# Download and install the toolchain. This can take hours!
git clone https://github.com/riscv/riscv-gnu-toolchain.git
cd riscv-gnu-toolchain
git submodule update --init --recursive
./configure --prefix=$TOOLS_DIR/riscv-gnu-toolchain-bins32 --enable-gdb --with-arch=rv32gc
make

#compiles for riscv64
make clean
./configure --prefix=$TOOLS_DIR/riscv-gnu-toolchain-bins --enable-gdb 
make
cd ..

# Download and install the QEMU.
wget https://download.qemu.org/qemu-4.2.0.tar.xz
tar -xvf qemu-4.2.0.tar.xz
cd qemu-4.2.0
./configure --target-list=riscv64-softmmu,riscv32-softmmu
make
cd ..

cd ..

# Configure path.
sed -i '/riscv-tools/d' ~/.profile

echo  >> ~/.profile
echo "export PATH=\"\$PATH:\"$TOOLS_DIR/riscv-gnu-toolchain-bins32/bin:$TOOLS_DIR/riscv-gnu-toolchain-bins/bin\"\"" >> ~/.profile
echo  >> ~/.profile

echo "***********************************************************"
echo "You need to logout to the changes on you PATH make effect. "
echo "***********************************************************"
