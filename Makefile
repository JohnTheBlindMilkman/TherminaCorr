# directory structure
DIR_MAIN    = ./
DIR_BUILD   = $(DIR_MAIN)build/
DIR_H       = $(DIR_BUILD)include/
DIR_OBJ     = $(DIR_BUILD)obj/
DIR_CXX     = $(DIR_BUILD)src/

# search paths
vpath %.h   $(DIR_H)
vpath %.cxx $(DIR_CXX)
vpath %.o   $(DIR_OBJ)

# file lists
# THERM2_EVENTS
BIN_EVENTS  = test
HSRC_EVENTS = sayHello.cxx
SRC_EVENTS  = $(HSRC_EVENTS:%=$(DIR_CXX)%) $(BIN_EVENTS:%=$(DIR_CXX)%.cxx)
OBJ_EVENTS  = $(SRC_EVENTS:$(DIR_CXX)%.cxx=$(DIR_OBJ)%.o)

# preprocessor
PREPROCESS  = -D_CXX_VER_="\"$(shell $(CXX) --version | grep $(CXX))\"" -D_ROOT_VER_="\"$(shell root-config --version)\""

# compilation
CXX         = g++
LD          = g++
CXXFLAGS    = -std=c++11 -O0 -g -Wno-deprecated -Wno-return-type -I $(DIR_H) $(PREPROCESS) `root-config --cflags`
LFLAGS      = -lm -g `root-config --libs`

#################################################################################
# RULES                                                                         #
#################################################################################
 
all: $(BIN_EVENTS:%=$(DIR_OBJ)%) $(BIN_FEMTO:%=$(DIR_OBJ)%) $(BIN_HBTFIT:%=$(DIR_OBJ)%)
	cp $^ $(DIR_MAIN)
	echo
	echo "Ready!"
	echo

$(DIR_OBJ)test: $(OBJ_EVENTS)
	echo "Linking:   $@ ($(LD))"
	$(LD) $^ -o $@ $(LFLAGS)

$(DIR_OBJ)%.o: %.cxx
	@[ -d $(DIR_OBJ) ] || mkdir -p $(DIR_OBJ)
	echo "Compiling: $< ($(CXX))"
	$(CXX) -c $< -o $@ $(CXXFLAGS)

clean:
	rm -f $(DIR_OBJ)*.o
	rm -f $(DIR_OBJ)$(BIN_EVENTS) $(DIR_MAIN)$(BIN_EVENTS)
	echo "*.o and binary files removed."