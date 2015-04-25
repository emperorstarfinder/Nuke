########################################################
#					EXECUTABLE NAME 				   #
########################################################

EXE 				:= particleTests
EXE_VIS				:= particelVisualTests

########################################################
#					SOURCES							   #
########################################################


COM_SOURCES			:= Particles.hpp ParticlePolicies.hpp 
TEST_SOURCES		:= $(COM_SOURCES) ParticleTests.cpp
VIS_SOURCES 		:= $(COM_SOURCES) ParticleVisualTests.cpp

########################################################
#					COMPILERS						   #
########################################################

CXX					:= g++

########################################################
#				INCLUDE DIRECTORIES 				   #
########################################################

INCLUDES 			:= -I/usr/include

########################################################
#					LIBRARIES 						   #
########################################################

COM_LIBS 			:= -lGL -lGLU -lglut -lm -lGLEW
TEST_LIBS 			:= $(COM_LIBS) -lgtest -lgtest_main \
						-lpthread
VIS_LIBS 			:= $(COM_LIBS) 

LIB_DIR 			:= -L/usr/lib

########################################################
#					COMPILER FLAGS 					   #
########################################################

CXX_FLAGS 		:= -std=c++11 -g

########################################################
# 					TARGET RULES 					   #
########################################################

.PHONY: all tests vis

all: tests vis

tests: $(EXE)

vis: $(EXE_VIS)

$(EXE) :
	$(CXX) $(CXX_FLAGS) $(TEST_SOURCES) -o $(EXE) $(TEST_LIBS)

$(EXE_VIS) :
	$(CXX) $(CXX_FLAGS) $(VIS_SOURCES) -o $(EXE_VIS) $(VIS_LIBS)

clean:
	rm -rf *.o
	rm -rf $(EXE)
	rm -rf $(EXE_VIS)	

