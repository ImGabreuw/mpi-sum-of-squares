#!/bin/bash

wget https://download.open-mpi.org/release/open-mpi/v5.0/openmpi-5.0.0.tar.gz

mkdir -p ~/src
tar -xvf openmpi-5.0.0.tar.gz -C ~/src

cd ~/src/openmpi-5.0.0
./configure --prefix=$HOME/opt/openmpi
make -j"$(nproc)"
make install

echo 'export PATH=$HOME/opt/openmpi/bin:$PATH' >> ~/.bashrc
echo 'export LD_LIBRARY_PATH=$HOME/opt/openmpi/lib:$LD_LIBRARY_PATH' >> ~/.bashrc

export PATH=$HOME/opt/openmpi/bin:$PATH
export LD_LIBRARY_PATH=$HOME/opt/openmpi/lib:$LD_LIBRARY_PATH

mpicc --showme:version
mpirun --version