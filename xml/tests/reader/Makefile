CPPFLAGS:= -I../../ -I/usr/include/libxml2
LIBS:= -lxml2

TARGETS:= simple

all: $(TARGETS)

%: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $(LIBS) $<

clean:
	rm -f *.o

distclean: clean
	rm -f $(TARGETS)
