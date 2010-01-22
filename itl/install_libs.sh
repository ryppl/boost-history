#!/bin/sh

# Installs boost::date_time and boost::test so all examples and tests can be run
# for the Interval Template Library (ITL). You do not need to run this script,
# if you have a working installation of the boost libraries.

for option
do
    case $option in
    -help | --help | -h)
      want_help=yes ;;
    esac
done

if test "x$want_help" = xyes; then 
  echo "Usage: install_libs [toolset or -h --help]";
  echo "Argument: -h --help : Show usage" ;
  echo "          toolset : The compiler 'toolset' will be used with bjam.";
  echo "                    If the first argument is omitted 'gcc' is used as toolset.";
  echo "Installs boost::date_time and boost::test so all examples and tests can be run";
  echo "for the Interval Template Library (ITL). You do not need to run this script, ";
  echo "if you have a working installation of the boost libraries.";
else
  TOOLSET=$1;
  if test "x$TOOLSET" = x; then
    TOOLSET="gcc";
  fi
  bjam toolset=$TOOLSET --with-date_time --build-type=complete stage
  bjam toolset=$TOOLSET --with-test --build-type=complete stage
fi


