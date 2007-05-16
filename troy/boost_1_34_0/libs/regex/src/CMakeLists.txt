
# there is lots of ICU gore that is not included in this
# CMakeLists.txt file due to a rating of NC-17 by the motion picture
# association of america.  See the jamfiles for now.

boost_library(boost_regex
  c_regex_traits.cpp
   cpp_regex_traits.cpp
   cregex.cpp
   fileiter.cpp
   icu.cpp
   instances.cpp
   posix_api.cpp
   regex.cpp
   regex_debug.cpp
   regex_raw_buffer.cpp
   regex_traits_defaults.cpp
   static_mutex.cpp
   w32_regex_traits.cpp
   wc_regex_traits.cpp
   wide_posix_api.cpp
   winstances.cpp 
   usinstances.cpp 
   SHARED_COMPILE_FLAGS -DBOOST_REGEX_DYN_LINK=1)








