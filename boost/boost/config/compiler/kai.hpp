// Kai C++ compiler setup:

#   if __KCC_VERSION <= 4001
      // at least on Sun, the contents of <cwchar> is not in namespace std
#     define BOOST_NO_STDC_NAMESPACE
#   endif

#define BOOST_COMPILER "Kai C++ version " BOOST_STRINGIZE(__KCC_VERSION)


