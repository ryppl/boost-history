# while developing, nice to start your nightly builds with the very latest 
# code
set(CTEST_START_WITH_EMPTY_BINARY_DIRECTORY TRUE)

set(DROP_METHOD "xmlrpc")
#set(DROP_SITE "http://public.kitware.com:8081")
set(DROP_SITE "http://dart.resophonic.com:8081")
set(DROP_LOCATION "boost_1_34_0")

set(NOTES_DIR ${CMAKE_BINARY_DIR}/Testing/Notes)
file(MAKE_DIRECTORY ${NOTES_DIR})


# put various stuff into build notes
exec_program(uname ARGS -a > ${NOTES_DIR}/uname.txt OUTPUT_VARIABLE NOWHERE)
exec_program(${CMAKE_CXX_COMPILER} --version > ${NOTES_DIR}/compiler-version.txt OUTPUT_VARIABLE NOWHERE)

exec_program(svn ARGS info ${CMAKE_SOURCE_DIR} | grep Revision | cut -c 11- OUTPUT_VARIABLE SVN_REVISION)

exec_program(${CMAKE_CXX_COMPILER} ARGS ${CMAKE_CXX_COMPILER_ARG1} -dumpversion OUTPUT_VARIABLE COMPILER_VERSION)
exec_program(uname ARGS -s OUTPUT_VARIABLE OSTYPE)
exec_program(uname ARGS -r OUTPUT_VARIABLE OSVERSION)
exec_program(uname ARGS -p OUTPUT_VARIABLE ARCH)
if(${ARCH} STREQUAL "unknown")
  exec_program(uname ARGS -m OUTPUT_VARIABLE ARCH)
endif(${ARCH} STREQUAL "unknown")


set(BUILDNAME ${OSTYPE}-${OSVERSION}/${ARCH}/gcc-${COMPILER_VERSION}/${CMAKE_BUILD_TYPE})
message(STATUS "Testing is ON.  Build name = '${BUILDNAME}'")
