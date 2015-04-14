# Nuke
Nuke is a multiplayer online game where each user has a 'vehicle' (not sure exactly what this will be yet - I'm thinking some sort of ship) with offensive and defensive abilities. The aim is not yet defined, but currently the idea is to get more points than the other players, or if a team aspect is implemented for the team to get more points than the other team.

## Current Status
Currently I am in the process of learning OpenGL, which is what the souce and the executables will reflect. This will be a slow process until I know enough to implement some cool features.
u
## Environments
I'm developing this in Ubuntu 14.04 with gcc version 4.8. The intention is to port the game to Windows, OSX, iOS and Android, but this will happen when the Ubuntu test version is worth porting.

## Building
To build Nuke make sure that all of the prerequisits are installed and working - see Prerequisits section. The when in the project root directoy (where the Makefile is) run:
```python
$ make nuke   # To make Nuke linux executable
$ make nuketests # To make Nuke Unit tests executable
$ make clean  # Clean both the executables
```

## Prerequisits 
You will need OpenGL and GLES to run the Linux executables.


