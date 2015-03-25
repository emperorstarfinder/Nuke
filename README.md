# VoxSim
VoxSim is an attempt to make a 3D voxel simulator using both CUDA and OpenGL for the scientific computation and rendering respectively. It's also going to allow simulations to be described by the user, or to be created simply, so that it can be used academically by people who don't have the required coding knowledge.

## Current Status
Currently I am in the process of learning OpenGL, which is what the souce and the executables will reflect. After that I will move on to Voxels and rendering them on the GPU so that VoxSim can come alive =D

## Building
To build VoxSim make sure that all of the prerequisits are installed and working - see Prerequisits section. The when in the project root directoy (where the Makefile is) run:
```python
$ make vsim   # To make VoxSim linux executable
$ make vtests # To make VoxSim Unit tests executable
$ make clean  # Clean both the executables
```

Note that all executables are built to the builds directory and are for linux.


