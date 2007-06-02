cmake_minimum_required(VERSION 2.4.4 FATAL_ERROR)

project(Boost)

##########################################################################
# Global configurartion                                                  #
##########################################################################

# Boost version
set(BOOST_VERSION_MAJOR 1)
set(BOOST_VERSION_MINOR 34)
set(BOOST_VERSION_SUBMINOR 0)
set(BOOST_VERSION "${BOOST_VERSION_MAJOR}.${BOOST_VERSION_MINOR}.${BOOST_VERSION_SUBMINOR}")

# Which library variants will we build?
option(BUILD_STATIC "Whether to build static libraries" ON)
option(BUILD_SHARED "Whether to build shared libraries" ON)
option(BUILD_DEBUG "Whether to build debugging libraries" ON)
option(BUILD_RELEASE "Whether to build release libraries" ON)
option(BUILD_SINGLE_THREADED "Whether to build single-threaded libraries" ON)
option(BUILD_MULTI_THREADED "Whether to build multi-threaded libraries" ON)

# Always include the Boost source directory, so that we can find
# include files in boost/
include_directories(${Boost_SOURCE_DIR})

# Boost.Build version 2 does this due to trouble with autolinking 
# during building and testing. TODO: Try to remove this
add_definitions(-DBOOST_ALL_NO_LIB=1)

##########################################################################

# Make it easy to find Boost-specific CMake modules
list(APPEND CMAKE_MODULE_PATH ${Boost_SOURCE_DIR}/tools/build/CMake)

include(boost)
include(BoostConfig)

# The default set of library variants that we will be building
boost_add_default_variant(STATIC SHARED)
boost_add_default_variant(DEBUG RELEASE)
boost_add_default_variant(SINGLE_THREADED MULTI_THREADED)

##########################################################################
# Boost installation                                                     #
##########################################################################

# Installation of Boost headers
install(DIRECTORY boost 
        DESTINATION include
        PATTERN "CVS" EXCLUDE
        REGEX ".svn" EXCLUDE)
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

add_subdirectory(libs)
