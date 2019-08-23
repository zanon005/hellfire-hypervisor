TOOLS_DIR=$HOME/riscv-tools

# Install required packages
sudo apt-get install autoconf automake autotools-dev curl \
  		libmpc-dev libmpfr-dev libgmp-dev gawk build-essential \
  		bison flex texinfo gperf libtool patchutils bc zlib1g-dev

mkdir -p $TOOLS_DIR
pushd $TOOLS_DIR

# Download and install the toolchain. This can take hours!
git clone https://github.com/riscv/riscv-gnu-toolchain.git
cd riscv-gnu-toolchain
git submodule update --init --recursive
./configure --prefix=$TOOLS_DIR/riscv-gnu-toolchain-bins
make
cd ..

# Download and install the RV8 simulator.
git clone https://github.com/rv8-io/rv8.git
cd rv8
git submodule update --init --recursive
make
sudo make install
cd ..

popd

# Configure path.
echo  >> ~/.profile
echo "export PATH=\"\$PATH:\"$TOOLS_DIR/riscv-gnu-toolchain-bins\"\"" >> ~/.profile
echo  >> ~/.profile

echo "***********************************************************"
echo "You need to logout to the changes on you PATH make effect. "
echo "***********************************************************"
