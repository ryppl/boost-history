set (CTEST_BINARY_DIRECTORY "1.34.0/build")
# this is relative to binary directory
set (CTEST_SOURCE_DIRECTORY "../src") 

set (CTEST_CMAKE_COMMAND "cmake")
set (CTEST_COMMAND "ctest -D Nightly")

set (CTEST_INITIAL_CACHE "

CMAKE_BUILD_TYPE:STRING=Debug
BUILD_TESTING:BOOL=ON
BUILDNAME:STRING=${BUILDNAME}

"
)