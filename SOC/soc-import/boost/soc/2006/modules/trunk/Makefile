# Invoke STLFilt to clean up what we're getting here.
CXX = gfilt

CXXFLAGS += -I/usr/local/src/boost_1_33_1 -I.

mfront:  driver/driver.o main.o parser/parse.o
	$(CXX) -o mfront $^

