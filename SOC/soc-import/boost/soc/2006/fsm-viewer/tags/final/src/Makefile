PATH_TO_IRRXML=...path to irrxml-1.2/src
CPP= g++ -g
OBJ= $(PATH_TO_IRRXML)/irrXML.o viewer.o
LINKOBJ= $(PATH_TO_IRRXML)/irrXML.o viewer.o
LIBS=  --no-export-all-symbols --add-stdcall-alias
CXXINCS=  -I$(PATH_TO_IRRXML)
BIN= viewer
CXXFLAGS = $(CXXINCS)

all: $(BIN)

clean: 
	rm -f $(OBJ) $(BIN)

$(BIN): $(LINKOBJ)
	$(CPP) $(LINKOBJ) -o $(BIN) $(LIBS)

$(PATH_TO_IRRXML)/irrXML.o: $(PATH_TO_IRRXML)/irrXML.cpp
	$(CPP) -c $(PATH_TO_IRRXML)/irrXML.cpp -o $(PATH_TO_IRRXML)/irrxml-1.2/irrXML.o $(CXXFLAGS)

viewer.o: viewer.cpp
	$(CPP) -c viewer.cpp -o viewer.o $(CXXFLAGS)