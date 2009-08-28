Current versions of the g++ compiler (as of 2009-08-28, that includes
gcc-4.5 and below) may fail to compile templates involving
boost::mpl::integral_c.  The workaround is applying the patch for
bug 40092 that's mentioned here:

  http://gcc.gnu.org/ml/gcc-patches/2009-08/msg01217.html
