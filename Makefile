###########################################
#										  #
#	Makefile for vector addition using    #
#	AspectC++ and OpenCL C++ 			  #
#										  #		
###########################################

EXE_LINUX 	  = voxsim-lin
EXE_WINDW 	  = voxsim-win
EXE_OTH		  = other
TEST_EXE_NAME = voxsim-tests

###########################################
# 			DIRECTORIES					  #
#										  #
# BUILD_DIR - Name of the directory for   #
# 			  executables				  #
# SRC_DIR	- Name of the directory where # 
# 			  all c, cpp files are		  #
# ASP_DIR   - Name of directory where 	  #
# 		      aspects are				  #
###########################################

BUILD_DIR 	= builds
SRC_DIR		= src
TST_DIR 	= tests

###########################################
#			Executable					  #
###########################################

EXE_LIN 	= $(addprefix $(BUILD_DIR)/,$(EXE_LINUX))
EXE_WIN		= $(addprefix $(BUILD_DIR)/,$(EXE_WINDW))
EXE_OTHER   = $(addprefix $(BUILD_DIR)/,$(EXE_OTH))
TEST_EXE 	= $(addprefix $(BUILD_DIR)/,$(TEST_EXE_NAME))

##########################################
# 				SOURCES					 #
##########################################

COM_MODULES  = voxsim.cpp
TST_MODULES  = test.cpp
TGT_MODULES  = 
OTH_MODULES  = world.cpp

COM_SOURCES  = $(addprefix $(SRC_DIR)/,$(COM_MODULES))
TST_SOURCES  = $(addprefix $(TST_DIR)/,$(TST_MODULES))
TGT_SOURCES  = $(addprefix $(SRC_DIR)/,$(TGT_MODULES))
OTH_SOURCES  = $(addprefix $(SRC_DIR)/,$(OTH_MODULES))

##########################################
#				DIRECTORIES 			 #
#########################################


##########################################
#				LIBRARIES 				 #
##########################################

LIBS 		= -lGL -lGLU -lglut -lm \
			  -lGLEW

#########################################
# 		INCLUDE DIRECTORIES				#
#########################################

INCLUDES 	= /usr/include

#########################################
#		LIBRARY DIRECTORIES 			#
#########################################

LIB_DIRS 	= /usr/lib

#########################################
#			COMPILE ARGS 				#
#########################################

CXX 		= g++
WXX			= x86_64-w64-mingw32-g++
CXX_FLAGS 	= 
WXX_FLAGS   =

.PHONY: all vsim vtests

# No win or tests till this works (may be a while)
all: vsim other

vsim: $(EXE_LIN)

other: $(EXE_OTHER)

win: $(EXE_WIN)				

vtests: $(TEST_EXE)

$(EXE_LIN):
	$(CXX) $(CXX_FLAGS) $(COM_SOURCES) $(TGT_SOURCES) -o $(EXE_LIN)  \
		 $(LIBS) 

$(EXE_OTHER):
	$(CXX) $(CXX_FLAGS) $(OTH_SOURCES) -o $(EXE_OTHER) $(LIBS)

$(EXE_WIN):
	$(WXX) $(WXX_FLAGS) $(COM_SOURCES) $(TGT_SOURCES) -o $(EXE_WIN)  \
		 $(LIBS) 

$(TEST_EXE):
	$(CXX) $(CXX_FLAGS) $(COM_SOURCES) $(TST_SOURCES) -o $(TEST_EXE) \
		$(LIBS)

clean:
	rm -rf $(EXE_LIN) $(EXE_WIN) $(EXE_OTHER) $(TEST_EXE)
