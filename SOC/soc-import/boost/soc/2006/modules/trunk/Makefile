# Invoke STLFilt to clean up what we're getting here.
CXX = gfilt

CXXFLAGS += -I/usr/local/src/boost -I.
LDFLAGS += -L/usr/local/lib


mfront.bin:  driver/driver.o main.o parser/parse.o
	$(CXX) -o mfront.bin $^ $(LDFLAGS) -lboost_filesystem -lboost_wave

clean:
	$(RM) driver/driver.o main.o parser/parse.o

