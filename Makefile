# directory structure
DIR_MAIN    = ./
DIR_BUILD   = $(DIR_MAIN)build/
DIR_H       = $(DIR_BUILD)include/
DIR_OBJ     = $(DIR_BUILD)obj/
DIR_CXX     = $(DIR_BUILD)src/
DIR_MAC     = $(DIR_MAIN)macros/

# search paths
vpath %.h   $(DIR_H)
vpath %.cxx $(DIR_CXX)
vpath %.o   $(DIR_OBJ)

# file lists
BIN_FEMTO  = thermiCorr
HSRC_FEMTO = Accessibility.cxx Configurator.cxx HBTFit.cxx Messages.cxx Parser.cxx Storage.cxx
SRC_FEMTO  = $(HSRC_FEMTO:%=$(DIR_CXX)%) $(BIN_FEMTO:%=$(DIR_CXX)%.cxx)
OBJ_FEMTO  = $(SRC_FEMTO:$(DIR_CXX)%.cxx=$(DIR_OBJ)%.o)

BIN_Q2  = Q2test
HSRC_Q2 = Accessibility.cxx Compliance.cxx Configurator.cxx Messages.cxx Parser.cxx Storage.cxx
SRC_Q2  = $(HSRC_Q2:%=$(DIR_CXX)%) $(BIN_Q2:%=$(DIR_CXX)%.cxx)
OBJ_Q2  = $(SRC_Q2:$(DIR_CXX)%.cxx=$(DIR_OBJ)%.o)

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
 
all: $(BIN_FEMTO:%=$(DIR_OBJ)%) $(BIN_Q2:%=$(DIR_OBJ)%)
	cp $^ $(DIR_MAIN)
	echo
	echo "Ready!"
	echo "Type \"./thermiCorr\" to fit and extract HBT radii,"
	echo "Type \"./Q2test\" to perform compliance test."
	echo

$(DIR_OBJ)thermiCorr: $(OBJ_FEMTO)
	echo "Linking:   $@ ($(LD))"
	$(LD) $^ -o $@ $(LFLAGS)

$(DIR_OBJ)Q2test: $(OBJ_Q2)
	echo "Linking:   $@ ($(LD))"
	$(LD) $^ -o $@ $(LFLAGS)

$(DIR_OBJ)%.o: %.cxx
	@[ -d $(DIR_OBJ) ] || mkdir -p $(DIR_OBJ)
	echo "Compiling: $< ($(CXX))"
	$(CXX) -c $< -o $@ $(CXXFLAGS)

clean:
	rm -f $(DIR_OBJ)*.o
	rm -f $(DIR_MAC)*.d
	rm -f $(DIR_MAC)*.so
	rm -f $(DIR_MAC)*.pcm
	rm -f $(DIR_OBJ)$(BIN_FEMTO) $(DIR_MAIN)$(BIN_FEMTO)
	rm -f $(DIR_OBJ)$(BIN_Q2) $(DIR_MAIN)$(BIN_Q2)
	echo "*.o, *.so, *.d, *.pcm and binary files removed."

.SILENT :