project(Boost)

##########################################################################
# Global configurartion                                                  #
##########################################################################

# Boost version
set(BOOST_VERSION_MAJOR 1)
set(BOOST_VERSION_MINOR 35)
set(BOOST_VERSION_SUBMINOR 0)
set(BOOST_VERSION "${BOOST_VERSION_MAJOR}.${BOOST_VERSION_MINOR}.${BOOST_VERSION_SUBMINOR}")

# Whether to build static libraries
option(BUILD_STATIC_LIBS "Whether to build static libraries" ON)

# Whether to build shared libraries
option(BUILD_SHARED_LIBS "Whether to build shared libraries" ON)

# Always include the Boost source directory, so that we can find
# include files in boost/
include_directories(${Boost_SOURCE_DIR})

# Boost.Build version 2 does this, also, due to trouble with autolinking 
# during building and testing.
add_definitions(-DBOOST_ALL_NO_LIB=1)

##########################################################################

# Make it easy to find Boost-specific CMake modules
list(APPEND CMAKE_MODULE_PATH ${Boost_SOURCE_DIR}/tools/build/CMake)

include(boost)

##########################################################################
# Boost installation                                                     #
##########################################################################

# Installation of Boost headers
install(DIRECTORY boost DESTINATION include)
##########################################################################

##########################################################################
# Binary packages                                                        #
##########################################################################

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Boost")
set(CPACK_PACKAGE_VENDOR "Boost.org")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${Boost_SOURCE_DIR}/README")
set(CPACK_RESOURCE_FILE_README "${Boost_SOURCE_DIR}/README.txt")
set(CPACK_RESOURCE_FILE_LICENSE "${Boost_SOURCE_DIR}/LICENSE_1_0.txt")
set(CPACK_RESOURCE_FILE_WELCOME "${Boost_SOURCE_DIR}/Welcome.txt")
set(CPACK_PACKAGE_VERSION "${BOOST_VERSION}")
set(CPACK_PACKAGE_VERSION_MAJOR "${BOOST_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${BOOST_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${BOOST_VERSION_SUBMINOR}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "boost-${BOOST_VERSION_MAJOR}.${BOOST_VERSION_MINOR}.${BOOST_VERSION_SUBMINOR}")
if(WIN32 AND NOT UNIX)
  # There is a bug in NSI that does not handle full unix paths properly. Make
  # sure there is at least one set of four (4) backlasshes.
  # NOTE: No Boost icon yet
#  set(CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/Utilities/Release\\\\InstallIcon.bmp")
#  set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\MyExecutable.exe")
  set(CPACK_NSIS_DISPLAY_NAME "Boost ${BOOST_VERSION_MAJOR}.${BOOST_VERSION_MINOR}.${BOOST_VERSION_SUBMINOR}")
  set(CPACK_NSIS_HELP_LINK "http:\\\\\\\\www.boost.org")
  set(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\www.boost.org")
  set(CPACK_NSIS_CONTACT "boost-users@lists.boost.org")
  set(CPACK_NSIS_MODIFY_PATH ON)
endif(WIN32 AND NOT UNIX)
include(CPack)
##########################################################################

option(BOOST_SELFTEST "Build internal testing projects... not the boost libraries" OFF)
# Subdirectories of Boost
if (BOOST_SELFTEST)
  add_subdirectory(tools/build/CMake/testing)
else (BOOST_SELFTEST)
  add_subdirectory(libs)
endif(BOOST_SELFTEST)