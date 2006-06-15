# Invoke STLFilt to clean up what we're getting here.
CXX = gfilt

CXXFLAGS += -I/usr/local/src/boost -I.
LDFLAGS += -L/usr/local/lib

OBJECTS =  driver/config.o driver/driver.o main.o parser/generator.o map/map.o 

mfront.bin: $(OBJECTS)
	$(CXX) -o mfront.bin $^ $(LDFLAGS) -lboost_filesystem -lboost_wave -lboost_regex

clean:
	$(RM) $(OBJECTS)

