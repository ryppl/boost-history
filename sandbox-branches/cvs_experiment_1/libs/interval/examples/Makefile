CXX=g++
CXXFLAGS=-I../../../ -I../../../../boost -O2

CXXFLAGS+= -DBOOST_INCLUDE_MAIN

ifeq ($(CXX), g++)
  CXXFLAGS+= -DBOOST_NO_LIMITS -D_CPP_NUMERIC_LIMITS
  CXXFLAGS+= -W -Wall
else
  # CXXFLAGS+= -pto # SunPRO specific option.
endif

TARGETS= findroot_demo horner interval_speed_boost interval_speed_indet interval_speed_rvinterval interval_speed_jeremy interval_speed_profil_bias interval_speed_point

all: $(TARGETS)

interval_speed_boost: interval_speed.cpp interval_implementation_select.hpp
	$(CXX) $(CXXFLAGS)  -DUSE_BOOST $(LDFLAGS) interval_speed.cpp -o interval_speed_boost
interval_speed_indet: interval_speed.cpp interval_implementation_select.hpp
	$(CXX) $(CXXFLAGS)  -DUSE_BOOST_INDET interval_speed.cpp -o interval_speed_indet
interval_speed_rvinterval: interval_speed.cpp interval_implementation_select.hpp
	$(CXX) $(CXXFLAGS)  -DUSE_RVINTERVAL interval_speed.cpp -o interval_speed_rvinterval
interval_speed_jeremy: interval_speed.cpp interval_implementation_select.hpp
	$(CXX) $(CXXFLAGS)  -DUSE_JEREMY interval_speed.cpp -o interval_speed_jeremy
interval_speed_profil_bias: interval_speed.cpp interval_implementation_select.hpp
	$(CXX) $(CXXFLAGS)  -DUSE_PROFIL_BIAS interval_speed.cpp -o interval_speed_profil_bias
interval_speed_point: interval_speed.cpp interval_implementation_select.hpp
	$(CXX) $(CXXFLAGS)  -DUSE_POINT interval_speed.cpp -o interval_speed_point

clean:
	rm -f $(TARGETS) func2d.data

