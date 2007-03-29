CPPFLAGS:= -I../../ -I/usr/include/libxml2
LIBS:= -lxml2

TARGETS:= dom traverse xinclude config validate_dtd

all: $(TARGETS)

%: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $(LIBS) $<

clean:
	rm -f *.o

distclean: clean
	rm -f $(TARGETS)
