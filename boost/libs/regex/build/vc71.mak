#
# auto generated makefile for VC6 compiler
#
# usage:
# make
#   brings libraries up to date
# make install
#   brings libraries up to date and copies binaries to your VC6 /lib and /bin directories (recomended)
#

#
# Add additional compiler options here:
#
CXXFLAGS=
#
# Add additional include directories here:
#
INCLUDES=
#
# add additional linker flags here:
#
XLFLAGS=
#
# add additional static-library creation flags here:
#
XSFLAGS=

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF "$(MSVCDIR)" == ""
!ERROR Variable MSVCDIR not set.
!ENDIF


ALL_HEADER= ../../../boost/regex/concepts.hpp ../../../boost/regex/config.hpp ../../../boost/regex/icu.hpp ../../../boost/regex/mfc.hpp ../../../boost/regex/pattern_except.hpp ../../../boost/regex/regex_traits.hpp ../../../boost/regex/static_mutex.hpp ../../../boost/regex/user.hpp ../../../boost/regex/v4/basic_regex.hpp ../../../boost/regex/v4/basic_regex_creator.hpp ../../../boost/regex/v4/basic_regex_parser.hpp ../../../boost/regex/v4/c_regex_traits.hpp ../../../boost/regex/v4/char_regex_traits.hpp ../../../boost/regex/v4/cpp_regex_traits.hpp ../../../boost/regex/v4/cregex.hpp ../../../boost/regex/v4/error_type.hpp ../../../boost/regex/v4/fileiter.hpp ../../../boost/regex/v4/instances.hpp ../../../boost/regex/v4/iterator_category.hpp ../../../boost/regex/v4/iterator_traits.hpp ../../../boost/regex/v4/match_flags.hpp ../../../boost/regex/v4/match_results.hpp ../../../boost/regex/v4/mem_block_cache.hpp ../../../boost/regex/v4/perl_matcher.hpp ../../../boost/regex/v4/perl_matcher_common.hpp ../../../boost/regex/v4/perl_matcher_non_recursive.hpp ../../../boost/regex/v4/perl_matcher_recursive.hpp ../../../boost/regex/v4/primary_transform.hpp ../../../boost/regex/v4/protected_call.hpp ../../../boost/regex/v4/regbase.hpp ../../../boost/regex/v4/regex.hpp ../../../boost/regex/v4/regex_cstring.hpp ../../../boost/regex/v4/regex_format.hpp ../../../boost/regex/v4/regex_fwd.hpp ../../../boost/regex/v4/regex_grep.hpp ../../../boost/regex/v4/regex_iterator.hpp ../../../boost/regex/v4/regex_kmp.hpp ../../../boost/regex/v4/regex_match.hpp ../../../boost/regex/v4/regex_merge.hpp ../../../boost/regex/v4/regex_raw_buffer.hpp ../../../boost/regex/v4/regex_replace.hpp ../../../boost/regex/v4/regex_search.hpp ../../../boost/regex/v4/regex_split.hpp ../../../boost/regex/v4/regex_stack.hpp ../../../boost/regex/v4/regex_token_iterator.hpp ../../../boost/regex/v4/regex_traits.hpp ../../../boost/regex/v4/regex_traits_defaults.hpp ../../../boost/regex/v4/regex_workaround.hpp ../../../boost/regex/v4/states.hpp ../../../boost/regex/v4/sub_match.hpp ../../../boost/regex/v4/syntax_type.hpp ../../../boost/regex/v4/w32_regex_traits.hpp ../../../boost/regex/config/borland.hpp ../../../boost/regex/config/cwchar.hpp ../../../boost/regex/config/regex_library_include.hpp

all : main_dir libboost_regex-vc71-s-1_32_dir ./vc71/libboost_regex-vc71-s-1_32.lib libboost_regex-vc71-mt-s-1_32_dir ./vc71/libboost_regex-vc71-mt-s-1_32.lib libboost_regex-vc71-sgd-1_32_dir ./vc71/libboost_regex-vc71-sgd-1_32.lib libboost_regex-vc71-mt-sgd-1_32_dir ./vc71/libboost_regex-vc71-mt-sgd-1_32.lib boost_regex-vc71-mt-gd-1_32_dir ./vc71/boost_regex-vc71-mt-gd-1_32.lib boost_regex-vc71-mt-1_32_dir ./vc71/boost_regex-vc71-mt-1_32.lib libboost_regex-vc71-mt-1_32_dir ./vc71/libboost_regex-vc71-mt-1_32.lib libboost_regex-vc71-mt-gd-1_32_dir ./vc71/libboost_regex-vc71-mt-gd-1_32.lib

clean :  libboost_regex-vc71-s-1_32_clean libboost_regex-vc71-mt-s-1_32_clean libboost_regex-vc71-sgd-1_32_clean libboost_regex-vc71-mt-sgd-1_32_clean boost_regex-vc71-mt-gd-1_32_clean boost_regex-vc71-mt-1_32_clean libboost_regex-vc71-mt-1_32_clean libboost_regex-vc71-mt-gd-1_32_clean

install : all
	copy vc71\libboost_regex-vc71-s-1_32.lib "$(MSVCDIR)\lib"
	copy vc71\libboost_regex-vc71-mt-s-1_32.lib "$(MSVCDIR)\lib"
	copy vc71\libboost_regex-vc71-sgd-1_32.lib "$(MSVCDIR)\lib"
	copy vc71\libboost_regex-vc71-sgd-1_32.pdb "$(MSVCDIR)\lib"
	copy vc71\libboost_regex-vc71-mt-sgd-1_32.lib "$(MSVCDIR)\lib"
	copy vc71\libboost_regex-vc71-mt-sgd-1_32.pdb "$(MSVCDIR)\lib"
	copy vc71\boost_regex-vc71-mt-gd-1_32.lib "$(MSVCDIR)\lib"
	copy vc71\boost_regex-vc71-mt-gd-1_32.dll "$(MSVCDIR)\bin"
	copy vc71\boost_regex-vc71-mt-gd-1_32.pdb "$(MSVCDIR)\lib"
	copy vc71\boost_regex-vc71-mt-1_32.lib "$(MSVCDIR)\lib"
	copy vc71\boost_regex-vc71-mt-1_32.dll "$(MSVCDIR)\bin"
	copy vc71\libboost_regex-vc71-mt-1_32.lib "$(MSVCDIR)\lib"
	copy vc71\libboost_regex-vc71-mt-gd-1_32.lib "$(MSVCDIR)\lib"
	copy vc71\libboost_regex-vc71-mt-gd-1_32.pdb "$(MSVCDIR)\lib"

main_dir :
	@if not exist "vc71\$(NULL)" mkdir vc71


########################################################
#
# section for libboost_regex-vc71-s-1_32.lib
#
########################################################
vc71/libboost_regex-vc71-s-1_32/c_regex_traits.obj: ../src/c_regex_traits.cpp $(ALL_HEADER)
	cl $(INCLUDES) /c /nologo /ML /W3 /GX /O2 /GB /GF /Gy /I..\..\..\ /DWIN32 /DNDEBUG /D_MBCS /D_LIB /FD /Zc:wchar_t  $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-s-1_32/ -Fdvc71/libboost_regex-vc71-s-1_32.pdb ../src/c_regex_traits.cpp

vc71/libboost_regex-vc71-s-1_32/cpp_regex_traits.obj: ../src/cpp_regex_traits.cpp $(ALL_HEADER)
	cl $(INCLUDES) /c /nologo /ML /W3 /GX /O2 /GB /GF /Gy /I..\..\..\ /DWIN32 /DNDEBUG /D_MBCS /D_LIB /FD /Zc:wchar_t  $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-s-1_32/ -Fdvc71/libboost_regex-vc71-s-1_32.pdb ../src/cpp_regex_traits.cpp

vc71/libboost_regex-vc71-s-1_32/cregex.obj: ../src/cregex.cpp $(ALL_HEADER)
	cl $(INCLUDES) /c /nologo /ML /W3 /GX /O2 /GB /GF /Gy /I..\..\..\ /DWIN32 /DNDEBUG /D_MBCS /D_LIB /FD /Zc:wchar_t  $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-s-1_32/ -Fdvc71/libboost_regex-vc71-s-1_32.pdb ../src/cregex.cpp

vc71/libboost_regex-vc71-s-1_32/fileiter.obj: ../src/fileiter.cpp $(ALL_HEADER)
	cl $(INCLUDES) /c /nologo /ML /W3 /GX /O2 /GB /GF /Gy /I..\..\..\ /DWIN32 /DNDEBUG /D_MBCS /D_LIB /FD /Zc:wchar_t  $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-s-1_32/ -Fdvc71/libboost_regex-vc71-s-1_32.pdb ../src/fileiter.cpp

vc71/libboost_regex-vc71-s-1_32/instances.obj: ../src/instances.cpp $(ALL_HEADER)
	cl $(INCLUDES) /c /nologo /ML /W3 /GX /O2 /GB /GF /Gy /I..\..\..\ /DWIN32 /DNDEBUG /D_MBCS /D_LIB /FD /Zc:wchar_t  $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-s-1_32/ -Fdvc71/libboost_regex-vc71-s-1_32.pdb ../src/instances.cpp

vc71/libboost_regex-vc71-s-1_32/posix_api.obj: ../src/posix_api.cpp $(ALL_HEADER)
	cl $(INCLUDES) /c /nologo /ML /W3 /GX /O2 /GB /GF /Gy /I..\..\..\ /DWIN32 /DNDEBUG /D_MBCS /D_LIB /FD /Zc:wchar_t  $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-s-1_32/ -Fdvc71/libboost_regex-vc71-s-1_32.pdb ../src/posix_api.cpp

vc71/libboost_regex-vc71-s-1_32/regex.obj: ../src/regex.cpp $(ALL_HEADER)
	cl $(INCLUDES) /c /nologo /ML /W3 /GX /O2 /GB /GF /Gy /I..\..\..\ /DWIN32 /DNDEBUG /D_MBCS /D_LIB /FD /Zc:wchar_t  $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-s-1_32/ -Fdvc71/libboost_regex-vc71-s-1_32.pdb ../src/regex.cpp

vc71/libboost_regex-vc71-s-1_32/regex_debug.obj: ../src/regex_debug.cpp $(ALL_HEADER)
	cl $(INCLUDES) /c /nologo /ML /W3 /GX /O2 /GB /GF /Gy /I..\..\..\ /DWIN32 /DNDEBUG /D_MBCS /D_LIB /FD /Zc:wchar_t  $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-s-1_32/ -Fdvc71/libboost_regex-vc71-s-1_32.pdb ../src/regex_debug.cpp

vc71/libboost_regex-vc71-s-1_32/regex_raw_buffer.obj: ../src/regex_raw_buffer.cpp $(ALL_HEADER)
	cl $(INCLUDES) /c /nologo /ML /W3 /GX /O2 /GB /GF /Gy /I..\..\..\ /DWIN32 /DNDEBUG /D_MBCS /D_LIB /FD /Zc:wchar_t  $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-s-1_32/ -Fdvc71/libboost_regex-vc71-s-1_32.pdb ../src/regex_raw_buffer.cpp

vc71/libboost_regex-vc71-s-1_32/regex_traits_defaults.obj: ../src/regex_traits_defaults.cpp $(ALL_HEADER)
	cl $(INCLUDES) /c /nologo /ML /W3 /GX /O2 /GB /GF /Gy /I..\..\..\ /DWIN32 /DNDEBUG /D_MBCS /D_LIB /FD /Zc:wchar_t  $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-s-1_32/ -Fdvc71/libboost_regex-vc71-s-1_32.pdb ../src/regex_traits_defaults.cpp

vc71/libboost_regex-vc71-s-1_32/static_mutex.obj: ../src/static_mutex.cpp $(ALL_HEADER)
	cl $(INCLUDES) /c /nologo /ML /W3 /GX /O2 /GB /GF /Gy /I..\..\..\ /DWIN32 /DNDEBUG /D_MBCS /D_LIB /FD /Zc:wchar_t  $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-s-1_32/ -Fdvc71/libboost_regex-vc71-s-1_32.pdb ../src/static_mutex.cpp

vc71/libboost_regex-vc71-s-1_32/usinstances.obj: ../src/usinstances.cpp $(ALL_HEADER)
	cl $(INCLUDES) /c /nologo /ML /W3 /GX /O2 /GB /GF /Gy /I..\..\..\ /DWIN32 /DNDEBUG /D_MBCS /D_LIB /FD /Zc:wchar_t  $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-s-1_32/ -Fdvc71/libboost_regex-vc71-s-1_32.pdb ../src/usinstances.cpp

vc71/libboost_regex-vc71-s-1_32/w32_regex_traits.obj: ../src/w32_regex_traits.cpp $(ALL_HEADER)
	cl $(INCLUDES) /c /nologo /ML /W3 /GX /O2 /GB /GF /Gy /I..\..\..\ /DWIN32 /DNDEBUG /D_MBCS /D_LIB /FD /Zc:wchar_t  $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-s-1_32/ -Fdvc71/libboost_regex-vc71-s-1_32.pdb ../src/w32_regex_traits.cpp

vc71/libboost_regex-vc71-s-1_32/wc_regex_traits.obj: ../src/wc_regex_traits.cpp $(ALL_HEADER)
	cl $(INCLUDES) /c /nologo /ML /W3 /GX /O2 /GB /GF /Gy /I..\..\..\ /DWIN32 /DNDEBUG /D_MBCS /D_LIB /FD /Zc:wchar_t  $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-s-1_32/ -Fdvc71/libboost_regex-vc71-s-1_32.pdb ../src/wc_regex_traits.cpp

vc71/libboost_regex-vc71-s-1_32/wide_posix_api.obj: ../src/wide_posix_api.cpp $(ALL_HEADER)
	cl $(INCLUDES) /c /nologo /ML /W3 /GX /O2 /GB /GF /Gy /I..\..\..\ /DWIN32 /DNDEBUG /D_MBCS /D_LIB /FD /Zc:wchar_t  $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-s-1_32/ -Fdvc71/libboost_regex-vc71-s-1_32.pdb ../src/wide_posix_api.cpp

vc71/libboost_regex-vc71-s-1_32/winstances.obj: ../src/winstances.cpp $(ALL_HEADER)
	cl $(INCLUDES) /c /nologo /ML /W3 /GX /O2 /GB /GF /Gy /I..\..\..\ /DWIN32 /DNDEBUG /D_MBCS /D_LIB /FD /Zc:wchar_t  $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-s-1_32/ -Fdvc71/libboost_regex-vc71-s-1_32.pdb ../src/winstances.cpp

libboost_regex-vc71-s-1_32_dir :
	@if not exist "vc71\libboost_regex-vc71-s-1_32\$(NULL)" mkdir vc71\libboost_regex-vc71-s-1_32

libboost_regex-vc71-s-1_32_clean :
	del vc71\libboost_regex-vc71-s-1_32\*.obj
	del vc71\libboost_regex-vc71-s-1_32\*.idb
	del vc71\libboost_regex-vc71-s-1_32\*.exp
	del vc71\libboost_regex-vc71-s-1_32\*.pch

./vc71/libboost_regex-vc71-s-1_32.lib : vc71/libboost_regex-vc71-s-1_32/c_regex_traits.obj vc71/libboost_regex-vc71-s-1_32/cpp_regex_traits.obj vc71/libboost_regex-vc71-s-1_32/cregex.obj vc71/libboost_regex-vc71-s-1_32/fileiter.obj vc71/libboost_regex-vc71-s-1_32/instances.obj vc71/libboost_regex-vc71-s-1_32/posix_api.obj vc71/libboost_regex-vc71-s-1_32/regex.obj vc71/libboost_regex-vc71-s-1_32/regex_debug.obj vc71/libboost_regex-vc71-s-1_32/regex_raw_buffer.obj vc71/libboost_regex-vc71-s-1_32/regex_traits_defaults.obj vc71/libboost_regex-vc71-s-1_32/static_mutex.obj vc71/libboost_regex-vc71-s-1_32/usinstances.obj vc71/libboost_regex-vc71-s-1_32/w32_regex_traits.obj vc71/libboost_regex-vc71-s-1_32/wc_regex_traits.obj vc71/libboost_regex-vc71-s-1_32/wide_posix_api.obj vc71/libboost_regex-vc71-s-1_32/winstances.obj
	link -lib /nologo /out:vc71/libboost_regex-vc71-s-1_32.lib $(XSFLAGS)  vc71/libboost_regex-vc71-s-1_32/c_regex_traits.obj vc71/libboost_regex-vc71-s-1_32/cpp_regex_traits.obj vc71/libboost_regex-vc71-s-1_32/cregex.obj vc71/libboost_regex-vc71-s-1_32/fileiter.obj vc71/libboost_regex-vc71-s-1_32/instances.obj vc71/libboost_regex-vc71-s-1_32/posix_api.obj vc71/libboost_regex-vc71-s-1_32/regex.obj vc71/libboost_regex-vc71-s-1_32/regex_debug.obj vc71/libboost_regex-vc71-s-1_32/regex_raw_buffer.obj vc71/libboost_regex-vc71-s-1_32/regex_traits_defaults.obj vc71/libboost_regex-vc71-s-1_32/static_mutex.obj vc71/libboost_regex-vc71-s-1_32/usinstances.obj vc71/libboost_regex-vc71-s-1_32/w32_regex_traits.obj vc71/libboost_regex-vc71-s-1_32/wc_regex_traits.obj vc71/libboost_regex-vc71-s-1_32/wide_posix_api.obj vc71/libboost_regex-vc71-s-1_32/winstances.obj

########################################################
#
# section for libboost_regex-vc71-mt-s-1_32.lib
#
########################################################
vc71/libboost_regex-vc71-mt-s-1_32/c_regex_traits.obj: ../src/c_regex_traits.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MT /W3 /GX /O2 /GB /GF /Gy /I..\..\..\ /D_MT /DWIN32 /DNDEBUG /D_MBCS /D_LIB /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-s-1_32/ -Fdvc71/libboost_regex-vc71-mt-s-1_32.pdb ../src/c_regex_traits.cpp

vc71/libboost_regex-vc71-mt-s-1_32/cpp_regex_traits.obj: ../src/cpp_regex_traits.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MT /W3 /GX /O2 /GB /GF /Gy /I..\..\..\ /D_MT /DWIN32 /DNDEBUG /D_MBCS /D_LIB /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-s-1_32/ -Fdvc71/libboost_regex-vc71-mt-s-1_32.pdb ../src/cpp_regex_traits.cpp

vc71/libboost_regex-vc71-mt-s-1_32/cregex.obj: ../src/cregex.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MT /W3 /GX /O2 /GB /GF /Gy /I..\..\..\ /D_MT /DWIN32 /DNDEBUG /D_MBCS /D_LIB /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-s-1_32/ -Fdvc71/libboost_regex-vc71-mt-s-1_32.pdb ../src/cregex.cpp

vc71/libboost_regex-vc71-mt-s-1_32/fileiter.obj: ../src/fileiter.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MT /W3 /GX /O2 /GB /GF /Gy /I..\..\..\ /D_MT /DWIN32 /DNDEBUG /D_MBCS /D_LIB /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-s-1_32/ -Fdvc71/libboost_regex-vc71-mt-s-1_32.pdb ../src/fileiter.cpp

vc71/libboost_regex-vc71-mt-s-1_32/instances.obj: ../src/instances.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MT /W3 /GX /O2 /GB /GF /Gy /I..\..\..\ /D_MT /DWIN32 /DNDEBUG /D_MBCS /D_LIB /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-s-1_32/ -Fdvc71/libboost_regex-vc71-mt-s-1_32.pdb ../src/instances.cpp

vc71/libboost_regex-vc71-mt-s-1_32/posix_api.obj: ../src/posix_api.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MT /W3 /GX /O2 /GB /GF /Gy /I..\..\..\ /D_MT /DWIN32 /DNDEBUG /D_MBCS /D_LIB /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-s-1_32/ -Fdvc71/libboost_regex-vc71-mt-s-1_32.pdb ../src/posix_api.cpp

vc71/libboost_regex-vc71-mt-s-1_32/regex.obj: ../src/regex.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MT /W3 /GX /O2 /GB /GF /Gy /I..\..\..\ /D_MT /DWIN32 /DNDEBUG /D_MBCS /D_LIB /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-s-1_32/ -Fdvc71/libboost_regex-vc71-mt-s-1_32.pdb ../src/regex.cpp

vc71/libboost_regex-vc71-mt-s-1_32/regex_debug.obj: ../src/regex_debug.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MT /W3 /GX /O2 /GB /GF /Gy /I..\..\..\ /D_MT /DWIN32 /DNDEBUG /D_MBCS /D_LIB /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-s-1_32/ -Fdvc71/libboost_regex-vc71-mt-s-1_32.pdb ../src/regex_debug.cpp

vc71/libboost_regex-vc71-mt-s-1_32/regex_raw_buffer.obj: ../src/regex_raw_buffer.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MT /W3 /GX /O2 /GB /GF /Gy /I..\..\..\ /D_MT /DWIN32 /DNDEBUG /D_MBCS /D_LIB /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-s-1_32/ -Fdvc71/libboost_regex-vc71-mt-s-1_32.pdb ../src/regex_raw_buffer.cpp

vc71/libboost_regex-vc71-mt-s-1_32/regex_traits_defaults.obj: ../src/regex_traits_defaults.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MT /W3 /GX /O2 /GB /GF /Gy /I..\..\..\ /D_MT /DWIN32 /DNDEBUG /D_MBCS /D_LIB /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-s-1_32/ -Fdvc71/libboost_regex-vc71-mt-s-1_32.pdb ../src/regex_traits_defaults.cpp

vc71/libboost_regex-vc71-mt-s-1_32/static_mutex.obj: ../src/static_mutex.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MT /W3 /GX /O2 /GB /GF /Gy /I..\..\..\ /D_MT /DWIN32 /DNDEBUG /D_MBCS /D_LIB /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-s-1_32/ -Fdvc71/libboost_regex-vc71-mt-s-1_32.pdb ../src/static_mutex.cpp

vc71/libboost_regex-vc71-mt-s-1_32/usinstances.obj: ../src/usinstances.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MT /W3 /GX /O2 /GB /GF /Gy /I..\..\..\ /D_MT /DWIN32 /DNDEBUG /D_MBCS /D_LIB /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-s-1_32/ -Fdvc71/libboost_regex-vc71-mt-s-1_32.pdb ../src/usinstances.cpp

vc71/libboost_regex-vc71-mt-s-1_32/w32_regex_traits.obj: ../src/w32_regex_traits.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MT /W3 /GX /O2 /GB /GF /Gy /I..\..\..\ /D_MT /DWIN32 /DNDEBUG /D_MBCS /D_LIB /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-s-1_32/ -Fdvc71/libboost_regex-vc71-mt-s-1_32.pdb ../src/w32_regex_traits.cpp

vc71/libboost_regex-vc71-mt-s-1_32/wc_regex_traits.obj: ../src/wc_regex_traits.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MT /W3 /GX /O2 /GB /GF /Gy /I..\..\..\ /D_MT /DWIN32 /DNDEBUG /D_MBCS /D_LIB /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-s-1_32/ -Fdvc71/libboost_regex-vc71-mt-s-1_32.pdb ../src/wc_regex_traits.cpp

vc71/libboost_regex-vc71-mt-s-1_32/wide_posix_api.obj: ../src/wide_posix_api.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MT /W3 /GX /O2 /GB /GF /Gy /I..\..\..\ /D_MT /DWIN32 /DNDEBUG /D_MBCS /D_LIB /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-s-1_32/ -Fdvc71/libboost_regex-vc71-mt-s-1_32.pdb ../src/wide_posix_api.cpp

vc71/libboost_regex-vc71-mt-s-1_32/winstances.obj: ../src/winstances.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MT /W3 /GX /O2 /GB /GF /Gy /I..\..\..\ /D_MT /DWIN32 /DNDEBUG /D_MBCS /D_LIB /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-s-1_32/ -Fdvc71/libboost_regex-vc71-mt-s-1_32.pdb ../src/winstances.cpp

libboost_regex-vc71-mt-s-1_32_dir :
	@if not exist "vc71\libboost_regex-vc71-mt-s-1_32\$(NULL)" mkdir vc71\libboost_regex-vc71-mt-s-1_32

libboost_regex-vc71-mt-s-1_32_clean :
	del vc71\libboost_regex-vc71-mt-s-1_32\*.obj
	del vc71\libboost_regex-vc71-mt-s-1_32\*.idb
	del vc71\libboost_regex-vc71-mt-s-1_32\*.exp
	del vc71\libboost_regex-vc71-mt-s-1_32\*.pch

./vc71/libboost_regex-vc71-mt-s-1_32.lib : vc71/libboost_regex-vc71-mt-s-1_32/c_regex_traits.obj vc71/libboost_regex-vc71-mt-s-1_32/cpp_regex_traits.obj vc71/libboost_regex-vc71-mt-s-1_32/cregex.obj vc71/libboost_regex-vc71-mt-s-1_32/fileiter.obj vc71/libboost_regex-vc71-mt-s-1_32/instances.obj vc71/libboost_regex-vc71-mt-s-1_32/posix_api.obj vc71/libboost_regex-vc71-mt-s-1_32/regex.obj vc71/libboost_regex-vc71-mt-s-1_32/regex_debug.obj vc71/libboost_regex-vc71-mt-s-1_32/regex_raw_buffer.obj vc71/libboost_regex-vc71-mt-s-1_32/regex_traits_defaults.obj vc71/libboost_regex-vc71-mt-s-1_32/static_mutex.obj vc71/libboost_regex-vc71-mt-s-1_32/usinstances.obj vc71/libboost_regex-vc71-mt-s-1_32/w32_regex_traits.obj vc71/libboost_regex-vc71-mt-s-1_32/wc_regex_traits.obj vc71/libboost_regex-vc71-mt-s-1_32/wide_posix_api.obj vc71/libboost_regex-vc71-mt-s-1_32/winstances.obj
	link -lib /nologo /out:vc71/libboost_regex-vc71-mt-s-1_32.lib $(XSFLAGS)  vc71/libboost_regex-vc71-mt-s-1_32/c_regex_traits.obj vc71/libboost_regex-vc71-mt-s-1_32/cpp_regex_traits.obj vc71/libboost_regex-vc71-mt-s-1_32/cregex.obj vc71/libboost_regex-vc71-mt-s-1_32/fileiter.obj vc71/libboost_regex-vc71-mt-s-1_32/instances.obj vc71/libboost_regex-vc71-mt-s-1_32/posix_api.obj vc71/libboost_regex-vc71-mt-s-1_32/regex.obj vc71/libboost_regex-vc71-mt-s-1_32/regex_debug.obj vc71/libboost_regex-vc71-mt-s-1_32/regex_raw_buffer.obj vc71/libboost_regex-vc71-mt-s-1_32/regex_traits_defaults.obj vc71/libboost_regex-vc71-mt-s-1_32/static_mutex.obj vc71/libboost_regex-vc71-mt-s-1_32/usinstances.obj vc71/libboost_regex-vc71-mt-s-1_32/w32_regex_traits.obj vc71/libboost_regex-vc71-mt-s-1_32/wc_regex_traits.obj vc71/libboost_regex-vc71-mt-s-1_32/wide_posix_api.obj vc71/libboost_regex-vc71-mt-s-1_32/winstances.obj

########################################################
#
# section for libboost_regex-vc71-sgd-1_32.lib
#
########################################################
vc71/libboost_regex-vc71-sgd-1_32/c_regex_traits.obj: ../src/c_regex_traits.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MLd /W3 /Gm /GX /Zi /Od /I..\..\..\ /DWIN32 /D_DEBUG /D_MBCS /D_LIB /FD /GX /RTC1 /Zc:wchar_t /c  $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-sgd-1_32/ -Fdvc71/libboost_regex-vc71-sgd-1_32.pdb ../src/c_regex_traits.cpp

vc71/libboost_regex-vc71-sgd-1_32/cpp_regex_traits.obj: ../src/cpp_regex_traits.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MLd /W3 /Gm /GX /Zi /Od /I..\..\..\ /DWIN32 /D_DEBUG /D_MBCS /D_LIB /FD /GX /RTC1 /Zc:wchar_t /c  $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-sgd-1_32/ -Fdvc71/libboost_regex-vc71-sgd-1_32.pdb ../src/cpp_regex_traits.cpp

vc71/libboost_regex-vc71-sgd-1_32/cregex.obj: ../src/cregex.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MLd /W3 /Gm /GX /Zi /Od /I..\..\..\ /DWIN32 /D_DEBUG /D_MBCS /D_LIB /FD /GX /RTC1 /Zc:wchar_t /c  $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-sgd-1_32/ -Fdvc71/libboost_regex-vc71-sgd-1_32.pdb ../src/cregex.cpp

vc71/libboost_regex-vc71-sgd-1_32/fileiter.obj: ../src/fileiter.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MLd /W3 /Gm /GX /Zi /Od /I..\..\..\ /DWIN32 /D_DEBUG /D_MBCS /D_LIB /FD /GX /RTC1 /Zc:wchar_t /c  $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-sgd-1_32/ -Fdvc71/libboost_regex-vc71-sgd-1_32.pdb ../src/fileiter.cpp

vc71/libboost_regex-vc71-sgd-1_32/instances.obj: ../src/instances.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MLd /W3 /Gm /GX /Zi /Od /I..\..\..\ /DWIN32 /D_DEBUG /D_MBCS /D_LIB /FD /GX /RTC1 /Zc:wchar_t /c  $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-sgd-1_32/ -Fdvc71/libboost_regex-vc71-sgd-1_32.pdb ../src/instances.cpp

vc71/libboost_regex-vc71-sgd-1_32/posix_api.obj: ../src/posix_api.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MLd /W3 /Gm /GX /Zi /Od /I..\..\..\ /DWIN32 /D_DEBUG /D_MBCS /D_LIB /FD /GX /RTC1 /Zc:wchar_t /c  $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-sgd-1_32/ -Fdvc71/libboost_regex-vc71-sgd-1_32.pdb ../src/posix_api.cpp

vc71/libboost_regex-vc71-sgd-1_32/regex.obj: ../src/regex.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MLd /W3 /Gm /GX /Zi /Od /I..\..\..\ /DWIN32 /D_DEBUG /D_MBCS /D_LIB /FD /GX /RTC1 /Zc:wchar_t /c  $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-sgd-1_32/ -Fdvc71/libboost_regex-vc71-sgd-1_32.pdb ../src/regex.cpp

vc71/libboost_regex-vc71-sgd-1_32/regex_debug.obj: ../src/regex_debug.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MLd /W3 /Gm /GX /Zi /Od /I..\..\..\ /DWIN32 /D_DEBUG /D_MBCS /D_LIB /FD /GX /RTC1 /Zc:wchar_t /c  $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-sgd-1_32/ -Fdvc71/libboost_regex-vc71-sgd-1_32.pdb ../src/regex_debug.cpp

vc71/libboost_regex-vc71-sgd-1_32/regex_raw_buffer.obj: ../src/regex_raw_buffer.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MLd /W3 /Gm /GX /Zi /Od /I..\..\..\ /DWIN32 /D_DEBUG /D_MBCS /D_LIB /FD /GX /RTC1 /Zc:wchar_t /c  $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-sgd-1_32/ -Fdvc71/libboost_regex-vc71-sgd-1_32.pdb ../src/regex_raw_buffer.cpp

vc71/libboost_regex-vc71-sgd-1_32/regex_traits_defaults.obj: ../src/regex_traits_defaults.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MLd /W3 /Gm /GX /Zi /Od /I..\..\..\ /DWIN32 /D_DEBUG /D_MBCS /D_LIB /FD /GX /RTC1 /Zc:wchar_t /c  $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-sgd-1_32/ -Fdvc71/libboost_regex-vc71-sgd-1_32.pdb ../src/regex_traits_defaults.cpp

vc71/libboost_regex-vc71-sgd-1_32/static_mutex.obj: ../src/static_mutex.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MLd /W3 /Gm /GX /Zi /Od /I..\..\..\ /DWIN32 /D_DEBUG /D_MBCS /D_LIB /FD /GX /RTC1 /Zc:wchar_t /c  $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-sgd-1_32/ -Fdvc71/libboost_regex-vc71-sgd-1_32.pdb ../src/static_mutex.cpp

vc71/libboost_regex-vc71-sgd-1_32/usinstances.obj: ../src/usinstances.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MLd /W3 /Gm /GX /Zi /Od /I..\..\..\ /DWIN32 /D_DEBUG /D_MBCS /D_LIB /FD /GX /RTC1 /Zc:wchar_t /c  $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-sgd-1_32/ -Fdvc71/libboost_regex-vc71-sgd-1_32.pdb ../src/usinstances.cpp

vc71/libboost_regex-vc71-sgd-1_32/w32_regex_traits.obj: ../src/w32_regex_traits.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MLd /W3 /Gm /GX /Zi /Od /I..\..\..\ /DWIN32 /D_DEBUG /D_MBCS /D_LIB /FD /GX /RTC1 /Zc:wchar_t /c  $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-sgd-1_32/ -Fdvc71/libboost_regex-vc71-sgd-1_32.pdb ../src/w32_regex_traits.cpp

vc71/libboost_regex-vc71-sgd-1_32/wc_regex_traits.obj: ../src/wc_regex_traits.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MLd /W3 /Gm /GX /Zi /Od /I..\..\..\ /DWIN32 /D_DEBUG /D_MBCS /D_LIB /FD /GX /RTC1 /Zc:wchar_t /c  $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-sgd-1_32/ -Fdvc71/libboost_regex-vc71-sgd-1_32.pdb ../src/wc_regex_traits.cpp

vc71/libboost_regex-vc71-sgd-1_32/wide_posix_api.obj: ../src/wide_posix_api.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MLd /W3 /Gm /GX /Zi /Od /I..\..\..\ /DWIN32 /D_DEBUG /D_MBCS /D_LIB /FD /GX /RTC1 /Zc:wchar_t /c  $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-sgd-1_32/ -Fdvc71/libboost_regex-vc71-sgd-1_32.pdb ../src/wide_posix_api.cpp

vc71/libboost_regex-vc71-sgd-1_32/winstances.obj: ../src/winstances.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MLd /W3 /Gm /GX /Zi /Od /I..\..\..\ /DWIN32 /D_DEBUG /D_MBCS /D_LIB /FD /GX /RTC1 /Zc:wchar_t /c  $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-sgd-1_32/ -Fdvc71/libboost_regex-vc71-sgd-1_32.pdb ../src/winstances.cpp

libboost_regex-vc71-sgd-1_32_dir :
	@if not exist "vc71\libboost_regex-vc71-sgd-1_32\$(NULL)" mkdir vc71\libboost_regex-vc71-sgd-1_32

libboost_regex-vc71-sgd-1_32_clean :
	del vc71\libboost_regex-vc71-sgd-1_32\*.obj
	del vc71\libboost_regex-vc71-sgd-1_32\*.idb
	del vc71\libboost_regex-vc71-sgd-1_32\*.exp
	del vc71\libboost_regex-vc71-sgd-1_32\*.pch

./vc71/libboost_regex-vc71-sgd-1_32.lib : vc71/libboost_regex-vc71-sgd-1_32/c_regex_traits.obj vc71/libboost_regex-vc71-sgd-1_32/cpp_regex_traits.obj vc71/libboost_regex-vc71-sgd-1_32/cregex.obj vc71/libboost_regex-vc71-sgd-1_32/fileiter.obj vc71/libboost_regex-vc71-sgd-1_32/instances.obj vc71/libboost_regex-vc71-sgd-1_32/posix_api.obj vc71/libboost_regex-vc71-sgd-1_32/regex.obj vc71/libboost_regex-vc71-sgd-1_32/regex_debug.obj vc71/libboost_regex-vc71-sgd-1_32/regex_raw_buffer.obj vc71/libboost_regex-vc71-sgd-1_32/regex_traits_defaults.obj vc71/libboost_regex-vc71-sgd-1_32/static_mutex.obj vc71/libboost_regex-vc71-sgd-1_32/usinstances.obj vc71/libboost_regex-vc71-sgd-1_32/w32_regex_traits.obj vc71/libboost_regex-vc71-sgd-1_32/wc_regex_traits.obj vc71/libboost_regex-vc71-sgd-1_32/wide_posix_api.obj vc71/libboost_regex-vc71-sgd-1_32/winstances.obj
	link -lib /nologo /out:vc71/libboost_regex-vc71-sgd-1_32.lib $(XSFLAGS)  vc71/libboost_regex-vc71-sgd-1_32/c_regex_traits.obj vc71/libboost_regex-vc71-sgd-1_32/cpp_regex_traits.obj vc71/libboost_regex-vc71-sgd-1_32/cregex.obj vc71/libboost_regex-vc71-sgd-1_32/fileiter.obj vc71/libboost_regex-vc71-sgd-1_32/instances.obj vc71/libboost_regex-vc71-sgd-1_32/posix_api.obj vc71/libboost_regex-vc71-sgd-1_32/regex.obj vc71/libboost_regex-vc71-sgd-1_32/regex_debug.obj vc71/libboost_regex-vc71-sgd-1_32/regex_raw_buffer.obj vc71/libboost_regex-vc71-sgd-1_32/regex_traits_defaults.obj vc71/libboost_regex-vc71-sgd-1_32/static_mutex.obj vc71/libboost_regex-vc71-sgd-1_32/usinstances.obj vc71/libboost_regex-vc71-sgd-1_32/w32_regex_traits.obj vc71/libboost_regex-vc71-sgd-1_32/wc_regex_traits.obj vc71/libboost_regex-vc71-sgd-1_32/wide_posix_api.obj vc71/libboost_regex-vc71-sgd-1_32/winstances.obj

########################################################
#
# section for libboost_regex-vc71-mt-sgd-1_32.lib
#
########################################################
vc71/libboost_regex-vc71-mt-sgd-1_32/c_regex_traits.obj: ../src/c_regex_traits.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MTd /W3 /Gm /GX /Zi /Od /I..\..\..\ /DWIN32 /D_MT /D_DEBUG /D_MBCS /D_LIB /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-sgd-1_32/ -Fdvc71/libboost_regex-vc71-mt-sgd-1_32.pdb ../src/c_regex_traits.cpp

vc71/libboost_regex-vc71-mt-sgd-1_32/cpp_regex_traits.obj: ../src/cpp_regex_traits.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MTd /W3 /Gm /GX /Zi /Od /I..\..\..\ /DWIN32 /D_MT /D_DEBUG /D_MBCS /D_LIB /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-sgd-1_32/ -Fdvc71/libboost_regex-vc71-mt-sgd-1_32.pdb ../src/cpp_regex_traits.cpp

vc71/libboost_regex-vc71-mt-sgd-1_32/cregex.obj: ../src/cregex.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MTd /W3 /Gm /GX /Zi /Od /I..\..\..\ /DWIN32 /D_MT /D_DEBUG /D_MBCS /D_LIB /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-sgd-1_32/ -Fdvc71/libboost_regex-vc71-mt-sgd-1_32.pdb ../src/cregex.cpp

vc71/libboost_regex-vc71-mt-sgd-1_32/fileiter.obj: ../src/fileiter.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MTd /W3 /Gm /GX /Zi /Od /I..\..\..\ /DWIN32 /D_MT /D_DEBUG /D_MBCS /D_LIB /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-sgd-1_32/ -Fdvc71/libboost_regex-vc71-mt-sgd-1_32.pdb ../src/fileiter.cpp

vc71/libboost_regex-vc71-mt-sgd-1_32/instances.obj: ../src/instances.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MTd /W3 /Gm /GX /Zi /Od /I..\..\..\ /DWIN32 /D_MT /D_DEBUG /D_MBCS /D_LIB /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-sgd-1_32/ -Fdvc71/libboost_regex-vc71-mt-sgd-1_32.pdb ../src/instances.cpp

vc71/libboost_regex-vc71-mt-sgd-1_32/posix_api.obj: ../src/posix_api.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MTd /W3 /Gm /GX /Zi /Od /I..\..\..\ /DWIN32 /D_MT /D_DEBUG /D_MBCS /D_LIB /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-sgd-1_32/ -Fdvc71/libboost_regex-vc71-mt-sgd-1_32.pdb ../src/posix_api.cpp

vc71/libboost_regex-vc71-mt-sgd-1_32/regex.obj: ../src/regex.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MTd /W3 /Gm /GX /Zi /Od /I..\..\..\ /DWIN32 /D_MT /D_DEBUG /D_MBCS /D_LIB /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-sgd-1_32/ -Fdvc71/libboost_regex-vc71-mt-sgd-1_32.pdb ../src/regex.cpp

vc71/libboost_regex-vc71-mt-sgd-1_32/regex_debug.obj: ../src/regex_debug.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MTd /W3 /Gm /GX /Zi /Od /I..\..\..\ /DWIN32 /D_MT /D_DEBUG /D_MBCS /D_LIB /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-sgd-1_32/ -Fdvc71/libboost_regex-vc71-mt-sgd-1_32.pdb ../src/regex_debug.cpp

vc71/libboost_regex-vc71-mt-sgd-1_32/regex_raw_buffer.obj: ../src/regex_raw_buffer.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MTd /W3 /Gm /GX /Zi /Od /I..\..\..\ /DWIN32 /D_MT /D_DEBUG /D_MBCS /D_LIB /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-sgd-1_32/ -Fdvc71/libboost_regex-vc71-mt-sgd-1_32.pdb ../src/regex_raw_buffer.cpp

vc71/libboost_regex-vc71-mt-sgd-1_32/regex_traits_defaults.obj: ../src/regex_traits_defaults.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MTd /W3 /Gm /GX /Zi /Od /I..\..\..\ /DWIN32 /D_MT /D_DEBUG /D_MBCS /D_LIB /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-sgd-1_32/ -Fdvc71/libboost_regex-vc71-mt-sgd-1_32.pdb ../src/regex_traits_defaults.cpp

vc71/libboost_regex-vc71-mt-sgd-1_32/static_mutex.obj: ../src/static_mutex.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MTd /W3 /Gm /GX /Zi /Od /I..\..\..\ /DWIN32 /D_MT /D_DEBUG /D_MBCS /D_LIB /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-sgd-1_32/ -Fdvc71/libboost_regex-vc71-mt-sgd-1_32.pdb ../src/static_mutex.cpp

vc71/libboost_regex-vc71-mt-sgd-1_32/usinstances.obj: ../src/usinstances.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MTd /W3 /Gm /GX /Zi /Od /I..\..\..\ /DWIN32 /D_MT /D_DEBUG /D_MBCS /D_LIB /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-sgd-1_32/ -Fdvc71/libboost_regex-vc71-mt-sgd-1_32.pdb ../src/usinstances.cpp

vc71/libboost_regex-vc71-mt-sgd-1_32/w32_regex_traits.obj: ../src/w32_regex_traits.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MTd /W3 /Gm /GX /Zi /Od /I..\..\..\ /DWIN32 /D_MT /D_DEBUG /D_MBCS /D_LIB /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-sgd-1_32/ -Fdvc71/libboost_regex-vc71-mt-sgd-1_32.pdb ../src/w32_regex_traits.cpp

vc71/libboost_regex-vc71-mt-sgd-1_32/wc_regex_traits.obj: ../src/wc_regex_traits.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MTd /W3 /Gm /GX /Zi /Od /I..\..\..\ /DWIN32 /D_MT /D_DEBUG /D_MBCS /D_LIB /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-sgd-1_32/ -Fdvc71/libboost_regex-vc71-mt-sgd-1_32.pdb ../src/wc_regex_traits.cpp

vc71/libboost_regex-vc71-mt-sgd-1_32/wide_posix_api.obj: ../src/wide_posix_api.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MTd /W3 /Gm /GX /Zi /Od /I..\..\..\ /DWIN32 /D_MT /D_DEBUG /D_MBCS /D_LIB /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-sgd-1_32/ -Fdvc71/libboost_regex-vc71-mt-sgd-1_32.pdb ../src/wide_posix_api.cpp

vc71/libboost_regex-vc71-mt-sgd-1_32/winstances.obj: ../src/winstances.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MTd /W3 /Gm /GX /Zi /Od /I..\..\..\ /DWIN32 /D_MT /D_DEBUG /D_MBCS /D_LIB /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-sgd-1_32/ -Fdvc71/libboost_regex-vc71-mt-sgd-1_32.pdb ../src/winstances.cpp

libboost_regex-vc71-mt-sgd-1_32_dir :
	@if not exist "vc71\libboost_regex-vc71-mt-sgd-1_32\$(NULL)" mkdir vc71\libboost_regex-vc71-mt-sgd-1_32

libboost_regex-vc71-mt-sgd-1_32_clean :
	del vc71\libboost_regex-vc71-mt-sgd-1_32\*.obj
	del vc71\libboost_regex-vc71-mt-sgd-1_32\*.idb
	del vc71\libboost_regex-vc71-mt-sgd-1_32\*.exp
	del vc71\libboost_regex-vc71-mt-sgd-1_32\*.pch

./vc71/libboost_regex-vc71-mt-sgd-1_32.lib : vc71/libboost_regex-vc71-mt-sgd-1_32/c_regex_traits.obj vc71/libboost_regex-vc71-mt-sgd-1_32/cpp_regex_traits.obj vc71/libboost_regex-vc71-mt-sgd-1_32/cregex.obj vc71/libboost_regex-vc71-mt-sgd-1_32/fileiter.obj vc71/libboost_regex-vc71-mt-sgd-1_32/instances.obj vc71/libboost_regex-vc71-mt-sgd-1_32/posix_api.obj vc71/libboost_regex-vc71-mt-sgd-1_32/regex.obj vc71/libboost_regex-vc71-mt-sgd-1_32/regex_debug.obj vc71/libboost_regex-vc71-mt-sgd-1_32/regex_raw_buffer.obj vc71/libboost_regex-vc71-mt-sgd-1_32/regex_traits_defaults.obj vc71/libboost_regex-vc71-mt-sgd-1_32/static_mutex.obj vc71/libboost_regex-vc71-mt-sgd-1_32/usinstances.obj vc71/libboost_regex-vc71-mt-sgd-1_32/w32_regex_traits.obj vc71/libboost_regex-vc71-mt-sgd-1_32/wc_regex_traits.obj vc71/libboost_regex-vc71-mt-sgd-1_32/wide_posix_api.obj vc71/libboost_regex-vc71-mt-sgd-1_32/winstances.obj
	link -lib /nologo /out:vc71/libboost_regex-vc71-mt-sgd-1_32.lib $(XSFLAGS)  vc71/libboost_regex-vc71-mt-sgd-1_32/c_regex_traits.obj vc71/libboost_regex-vc71-mt-sgd-1_32/cpp_regex_traits.obj vc71/libboost_regex-vc71-mt-sgd-1_32/cregex.obj vc71/libboost_regex-vc71-mt-sgd-1_32/fileiter.obj vc71/libboost_regex-vc71-mt-sgd-1_32/instances.obj vc71/libboost_regex-vc71-mt-sgd-1_32/posix_api.obj vc71/libboost_regex-vc71-mt-sgd-1_32/regex.obj vc71/libboost_regex-vc71-mt-sgd-1_32/regex_debug.obj vc71/libboost_regex-vc71-mt-sgd-1_32/regex_raw_buffer.obj vc71/libboost_regex-vc71-mt-sgd-1_32/regex_traits_defaults.obj vc71/libboost_regex-vc71-mt-sgd-1_32/static_mutex.obj vc71/libboost_regex-vc71-mt-sgd-1_32/usinstances.obj vc71/libboost_regex-vc71-mt-sgd-1_32/w32_regex_traits.obj vc71/libboost_regex-vc71-mt-sgd-1_32/wc_regex_traits.obj vc71/libboost_regex-vc71-mt-sgd-1_32/wide_posix_api.obj vc71/libboost_regex-vc71-mt-sgd-1_32/winstances.obj

########################################################
#
# section for boost_regex-vc71-mt-gd-1_32.lib
#
########################################################
vc71/boost_regex-vc71-mt-gd-1_32/c_regex_traits.obj: ../src/c_regex_traits.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MDd /W3 /Gm /GX /Zi /Od /I../../../ /D_DEBUG /DBOOST_REGEX_DYN_LINK /DWIN32 /D_WINDOWS /D_MBCS /DUSRDLL /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/boost_regex-vc71-mt-gd-1_32/ -Fdvc71/boost_regex-vc71-mt-gd-1_32.pdb ../src/c_regex_traits.cpp

vc71/boost_regex-vc71-mt-gd-1_32/cpp_regex_traits.obj: ../src/cpp_regex_traits.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MDd /W3 /Gm /GX /Zi /Od /I../../../ /D_DEBUG /DBOOST_REGEX_DYN_LINK /DWIN32 /D_WINDOWS /D_MBCS /DUSRDLL /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/boost_regex-vc71-mt-gd-1_32/ -Fdvc71/boost_regex-vc71-mt-gd-1_32.pdb ../src/cpp_regex_traits.cpp

vc71/boost_regex-vc71-mt-gd-1_32/cregex.obj: ../src/cregex.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MDd /W3 /Gm /GX /Zi /Od /I../../../ /D_DEBUG /DBOOST_REGEX_DYN_LINK /DWIN32 /D_WINDOWS /D_MBCS /DUSRDLL /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/boost_regex-vc71-mt-gd-1_32/ -Fdvc71/boost_regex-vc71-mt-gd-1_32.pdb ../src/cregex.cpp

vc71/boost_regex-vc71-mt-gd-1_32/fileiter.obj: ../src/fileiter.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MDd /W3 /Gm /GX /Zi /Od /I../../../ /D_DEBUG /DBOOST_REGEX_DYN_LINK /DWIN32 /D_WINDOWS /D_MBCS /DUSRDLL /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/boost_regex-vc71-mt-gd-1_32/ -Fdvc71/boost_regex-vc71-mt-gd-1_32.pdb ../src/fileiter.cpp

vc71/boost_regex-vc71-mt-gd-1_32/instances.obj: ../src/instances.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MDd /W3 /Gm /GX /Zi /Od /I../../../ /D_DEBUG /DBOOST_REGEX_DYN_LINK /DWIN32 /D_WINDOWS /D_MBCS /DUSRDLL /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/boost_regex-vc71-mt-gd-1_32/ -Fdvc71/boost_regex-vc71-mt-gd-1_32.pdb ../src/instances.cpp

vc71/boost_regex-vc71-mt-gd-1_32/posix_api.obj: ../src/posix_api.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MDd /W3 /Gm /GX /Zi /Od /I../../../ /D_DEBUG /DBOOST_REGEX_DYN_LINK /DWIN32 /D_WINDOWS /D_MBCS /DUSRDLL /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/boost_regex-vc71-mt-gd-1_32/ -Fdvc71/boost_regex-vc71-mt-gd-1_32.pdb ../src/posix_api.cpp

vc71/boost_regex-vc71-mt-gd-1_32/regex.obj: ../src/regex.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MDd /W3 /Gm /GX /Zi /Od /I../../../ /D_DEBUG /DBOOST_REGEX_DYN_LINK /DWIN32 /D_WINDOWS /D_MBCS /DUSRDLL /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/boost_regex-vc71-mt-gd-1_32/ -Fdvc71/boost_regex-vc71-mt-gd-1_32.pdb ../src/regex.cpp

vc71/boost_regex-vc71-mt-gd-1_32/regex_debug.obj: ../src/regex_debug.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MDd /W3 /Gm /GX /Zi /Od /I../../../ /D_DEBUG /DBOOST_REGEX_DYN_LINK /DWIN32 /D_WINDOWS /D_MBCS /DUSRDLL /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/boost_regex-vc71-mt-gd-1_32/ -Fdvc71/boost_regex-vc71-mt-gd-1_32.pdb ../src/regex_debug.cpp

vc71/boost_regex-vc71-mt-gd-1_32/regex_raw_buffer.obj: ../src/regex_raw_buffer.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MDd /W3 /Gm /GX /Zi /Od /I../../../ /D_DEBUG /DBOOST_REGEX_DYN_LINK /DWIN32 /D_WINDOWS /D_MBCS /DUSRDLL /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/boost_regex-vc71-mt-gd-1_32/ -Fdvc71/boost_regex-vc71-mt-gd-1_32.pdb ../src/regex_raw_buffer.cpp

vc71/boost_regex-vc71-mt-gd-1_32/regex_traits_defaults.obj: ../src/regex_traits_defaults.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MDd /W3 /Gm /GX /Zi /Od /I../../../ /D_DEBUG /DBOOST_REGEX_DYN_LINK /DWIN32 /D_WINDOWS /D_MBCS /DUSRDLL /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/boost_regex-vc71-mt-gd-1_32/ -Fdvc71/boost_regex-vc71-mt-gd-1_32.pdb ../src/regex_traits_defaults.cpp

vc71/boost_regex-vc71-mt-gd-1_32/static_mutex.obj: ../src/static_mutex.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MDd /W3 /Gm /GX /Zi /Od /I../../../ /D_DEBUG /DBOOST_REGEX_DYN_LINK /DWIN32 /D_WINDOWS /D_MBCS /DUSRDLL /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/boost_regex-vc71-mt-gd-1_32/ -Fdvc71/boost_regex-vc71-mt-gd-1_32.pdb ../src/static_mutex.cpp

vc71/boost_regex-vc71-mt-gd-1_32/usinstances.obj: ../src/usinstances.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MDd /W3 /Gm /GX /Zi /Od /I../../../ /D_DEBUG /DBOOST_REGEX_DYN_LINK /DWIN32 /D_WINDOWS /D_MBCS /DUSRDLL /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/boost_regex-vc71-mt-gd-1_32/ -Fdvc71/boost_regex-vc71-mt-gd-1_32.pdb ../src/usinstances.cpp

vc71/boost_regex-vc71-mt-gd-1_32/w32_regex_traits.obj: ../src/w32_regex_traits.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MDd /W3 /Gm /GX /Zi /Od /I../../../ /D_DEBUG /DBOOST_REGEX_DYN_LINK /DWIN32 /D_WINDOWS /D_MBCS /DUSRDLL /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/boost_regex-vc71-mt-gd-1_32/ -Fdvc71/boost_regex-vc71-mt-gd-1_32.pdb ../src/w32_regex_traits.cpp

vc71/boost_regex-vc71-mt-gd-1_32/wc_regex_traits.obj: ../src/wc_regex_traits.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MDd /W3 /Gm /GX /Zi /Od /I../../../ /D_DEBUG /DBOOST_REGEX_DYN_LINK /DWIN32 /D_WINDOWS /D_MBCS /DUSRDLL /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/boost_regex-vc71-mt-gd-1_32/ -Fdvc71/boost_regex-vc71-mt-gd-1_32.pdb ../src/wc_regex_traits.cpp

vc71/boost_regex-vc71-mt-gd-1_32/wide_posix_api.obj: ../src/wide_posix_api.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MDd /W3 /Gm /GX /Zi /Od /I../../../ /D_DEBUG /DBOOST_REGEX_DYN_LINK /DWIN32 /D_WINDOWS /D_MBCS /DUSRDLL /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/boost_regex-vc71-mt-gd-1_32/ -Fdvc71/boost_regex-vc71-mt-gd-1_32.pdb ../src/wide_posix_api.cpp

vc71/boost_regex-vc71-mt-gd-1_32/winstances.obj: ../src/winstances.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MDd /W3 /Gm /GX /Zi /Od /I../../../ /D_DEBUG /DBOOST_REGEX_DYN_LINK /DWIN32 /D_WINDOWS /D_MBCS /DUSRDLL /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/boost_regex-vc71-mt-gd-1_32/ -Fdvc71/boost_regex-vc71-mt-gd-1_32.pdb ../src/winstances.cpp

boost_regex-vc71-mt-gd-1_32_dir :
	@if not exist "vc71\boost_regex-vc71-mt-gd-1_32\$(NULL)" mkdir vc71\boost_regex-vc71-mt-gd-1_32

boost_regex-vc71-mt-gd-1_32_clean :
	del vc71\boost_regex-vc71-mt-gd-1_32\*.obj
	del vc71\boost_regex-vc71-mt-gd-1_32\*.idb
	del vc71\boost_regex-vc71-mt-gd-1_32\*.exp
	del vc71\boost_regex-vc71-mt-gd-1_32\*.pch

./vc71/boost_regex-vc71-mt-gd-1_32.lib : vc71/boost_regex-vc71-mt-gd-1_32/c_regex_traits.obj vc71/boost_regex-vc71-mt-gd-1_32/cpp_regex_traits.obj vc71/boost_regex-vc71-mt-gd-1_32/cregex.obj vc71/boost_regex-vc71-mt-gd-1_32/fileiter.obj vc71/boost_regex-vc71-mt-gd-1_32/instances.obj vc71/boost_regex-vc71-mt-gd-1_32/posix_api.obj vc71/boost_regex-vc71-mt-gd-1_32/regex.obj vc71/boost_regex-vc71-mt-gd-1_32/regex_debug.obj vc71/boost_regex-vc71-mt-gd-1_32/regex_raw_buffer.obj vc71/boost_regex-vc71-mt-gd-1_32/regex_traits_defaults.obj vc71/boost_regex-vc71-mt-gd-1_32/static_mutex.obj vc71/boost_regex-vc71-mt-gd-1_32/usinstances.obj vc71/boost_regex-vc71-mt-gd-1_32/w32_regex_traits.obj vc71/boost_regex-vc71-mt-gd-1_32/wc_regex_traits.obj vc71/boost_regex-vc71-mt-gd-1_32/wide_posix_api.obj vc71/boost_regex-vc71-mt-gd-1_32/winstances.obj
	link kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:yes /pdb:"vc71/boost_regex-vc71-mt-gd-1_32.pdb" /debug /machine:I386 /out:"vc71/boost_regex-vc71-mt-gd-1_32.dll" /implib:"vc71/boost_regex-vc71-mt-gd-1_32.lib" /LIBPATH:$(STLPORT_PATH)\lib $(XLFLAGS)  vc71/boost_regex-vc71-mt-gd-1_32/c_regex_traits.obj vc71/boost_regex-vc71-mt-gd-1_32/cpp_regex_traits.obj vc71/boost_regex-vc71-mt-gd-1_32/cregex.obj vc71/boost_regex-vc71-mt-gd-1_32/fileiter.obj vc71/boost_regex-vc71-mt-gd-1_32/instances.obj vc71/boost_regex-vc71-mt-gd-1_32/posix_api.obj vc71/boost_regex-vc71-mt-gd-1_32/regex.obj vc71/boost_regex-vc71-mt-gd-1_32/regex_debug.obj vc71/boost_regex-vc71-mt-gd-1_32/regex_raw_buffer.obj vc71/boost_regex-vc71-mt-gd-1_32/regex_traits_defaults.obj vc71/boost_regex-vc71-mt-gd-1_32/static_mutex.obj vc71/boost_regex-vc71-mt-gd-1_32/usinstances.obj vc71/boost_regex-vc71-mt-gd-1_32/w32_regex_traits.obj vc71/boost_regex-vc71-mt-gd-1_32/wc_regex_traits.obj vc71/boost_regex-vc71-mt-gd-1_32/wide_posix_api.obj vc71/boost_regex-vc71-mt-gd-1_32/winstances.obj

########################################################
#
# section for boost_regex-vc71-mt-1_32.lib
#
########################################################
vc71/boost_regex-vc71-mt-1_32/c_regex_traits.obj: ../src/c_regex_traits.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MD /W3 /GX /O2 /GB /GF /Gy /I../../../ /DBOOST_REGEX_DYN_LINK /DNDEBUG /DWIN32 /D_WINDOWS /D_MBCS /D_USRDLL /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/boost_regex-vc71-mt-1_32/ -Fdvc71/boost_regex-vc71-mt-1_32.pdb ../src/c_regex_traits.cpp

vc71/boost_regex-vc71-mt-1_32/cpp_regex_traits.obj: ../src/cpp_regex_traits.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MD /W3 /GX /O2 /GB /GF /Gy /I../../../ /DBOOST_REGEX_DYN_LINK /DNDEBUG /DWIN32 /D_WINDOWS /D_MBCS /D_USRDLL /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/boost_regex-vc71-mt-1_32/ -Fdvc71/boost_regex-vc71-mt-1_32.pdb ../src/cpp_regex_traits.cpp

vc71/boost_regex-vc71-mt-1_32/cregex.obj: ../src/cregex.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MD /W3 /GX /O2 /GB /GF /Gy /I../../../ /DBOOST_REGEX_DYN_LINK /DNDEBUG /DWIN32 /D_WINDOWS /D_MBCS /D_USRDLL /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/boost_regex-vc71-mt-1_32/ -Fdvc71/boost_regex-vc71-mt-1_32.pdb ../src/cregex.cpp

vc71/boost_regex-vc71-mt-1_32/fileiter.obj: ../src/fileiter.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MD /W3 /GX /O2 /GB /GF /Gy /I../../../ /DBOOST_REGEX_DYN_LINK /DNDEBUG /DWIN32 /D_WINDOWS /D_MBCS /D_USRDLL /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/boost_regex-vc71-mt-1_32/ -Fdvc71/boost_regex-vc71-mt-1_32.pdb ../src/fileiter.cpp

vc71/boost_regex-vc71-mt-1_32/instances.obj: ../src/instances.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MD /W3 /GX /O2 /GB /GF /Gy /I../../../ /DBOOST_REGEX_DYN_LINK /DNDEBUG /DWIN32 /D_WINDOWS /D_MBCS /D_USRDLL /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/boost_regex-vc71-mt-1_32/ -Fdvc71/boost_regex-vc71-mt-1_32.pdb ../src/instances.cpp

vc71/boost_regex-vc71-mt-1_32/posix_api.obj: ../src/posix_api.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MD /W3 /GX /O2 /GB /GF /Gy /I../../../ /DBOOST_REGEX_DYN_LINK /DNDEBUG /DWIN32 /D_WINDOWS /D_MBCS /D_USRDLL /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/boost_regex-vc71-mt-1_32/ -Fdvc71/boost_regex-vc71-mt-1_32.pdb ../src/posix_api.cpp

vc71/boost_regex-vc71-mt-1_32/regex.obj: ../src/regex.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MD /W3 /GX /O2 /GB /GF /Gy /I../../../ /DBOOST_REGEX_DYN_LINK /DNDEBUG /DWIN32 /D_WINDOWS /D_MBCS /D_USRDLL /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/boost_regex-vc71-mt-1_32/ -Fdvc71/boost_regex-vc71-mt-1_32.pdb ../src/regex.cpp

vc71/boost_regex-vc71-mt-1_32/regex_debug.obj: ../src/regex_debug.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MD /W3 /GX /O2 /GB /GF /Gy /I../../../ /DBOOST_REGEX_DYN_LINK /DNDEBUG /DWIN32 /D_WINDOWS /D_MBCS /D_USRDLL /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/boost_regex-vc71-mt-1_32/ -Fdvc71/boost_regex-vc71-mt-1_32.pdb ../src/regex_debug.cpp

vc71/boost_regex-vc71-mt-1_32/regex_raw_buffer.obj: ../src/regex_raw_buffer.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MD /W3 /GX /O2 /GB /GF /Gy /I../../../ /DBOOST_REGEX_DYN_LINK /DNDEBUG /DWIN32 /D_WINDOWS /D_MBCS /D_USRDLL /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/boost_regex-vc71-mt-1_32/ -Fdvc71/boost_regex-vc71-mt-1_32.pdb ../src/regex_raw_buffer.cpp

vc71/boost_regex-vc71-mt-1_32/regex_traits_defaults.obj: ../src/regex_traits_defaults.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MD /W3 /GX /O2 /GB /GF /Gy /I../../../ /DBOOST_REGEX_DYN_LINK /DNDEBUG /DWIN32 /D_WINDOWS /D_MBCS /D_USRDLL /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/boost_regex-vc71-mt-1_32/ -Fdvc71/boost_regex-vc71-mt-1_32.pdb ../src/regex_traits_defaults.cpp

vc71/boost_regex-vc71-mt-1_32/static_mutex.obj: ../src/static_mutex.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MD /W3 /GX /O2 /GB /GF /Gy /I../../../ /DBOOST_REGEX_DYN_LINK /DNDEBUG /DWIN32 /D_WINDOWS /D_MBCS /D_USRDLL /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/boost_regex-vc71-mt-1_32/ -Fdvc71/boost_regex-vc71-mt-1_32.pdb ../src/static_mutex.cpp

vc71/boost_regex-vc71-mt-1_32/usinstances.obj: ../src/usinstances.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MD /W3 /GX /O2 /GB /GF /Gy /I../../../ /DBOOST_REGEX_DYN_LINK /DNDEBUG /DWIN32 /D_WINDOWS /D_MBCS /D_USRDLL /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/boost_regex-vc71-mt-1_32/ -Fdvc71/boost_regex-vc71-mt-1_32.pdb ../src/usinstances.cpp

vc71/boost_regex-vc71-mt-1_32/w32_regex_traits.obj: ../src/w32_regex_traits.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MD /W3 /GX /O2 /GB /GF /Gy /I../../../ /DBOOST_REGEX_DYN_LINK /DNDEBUG /DWIN32 /D_WINDOWS /D_MBCS /D_USRDLL /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/boost_regex-vc71-mt-1_32/ -Fdvc71/boost_regex-vc71-mt-1_32.pdb ../src/w32_regex_traits.cpp

vc71/boost_regex-vc71-mt-1_32/wc_regex_traits.obj: ../src/wc_regex_traits.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MD /W3 /GX /O2 /GB /GF /Gy /I../../../ /DBOOST_REGEX_DYN_LINK /DNDEBUG /DWIN32 /D_WINDOWS /D_MBCS /D_USRDLL /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/boost_regex-vc71-mt-1_32/ -Fdvc71/boost_regex-vc71-mt-1_32.pdb ../src/wc_regex_traits.cpp

vc71/boost_regex-vc71-mt-1_32/wide_posix_api.obj: ../src/wide_posix_api.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MD /W3 /GX /O2 /GB /GF /Gy /I../../../ /DBOOST_REGEX_DYN_LINK /DNDEBUG /DWIN32 /D_WINDOWS /D_MBCS /D_USRDLL /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/boost_regex-vc71-mt-1_32/ -Fdvc71/boost_regex-vc71-mt-1_32.pdb ../src/wide_posix_api.cpp

vc71/boost_regex-vc71-mt-1_32/winstances.obj: ../src/winstances.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MD /W3 /GX /O2 /GB /GF /Gy /I../../../ /DBOOST_REGEX_DYN_LINK /DNDEBUG /DWIN32 /D_WINDOWS /D_MBCS /D_USRDLL /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/boost_regex-vc71-mt-1_32/ -Fdvc71/boost_regex-vc71-mt-1_32.pdb ../src/winstances.cpp

boost_regex-vc71-mt-1_32_dir :
	@if not exist "vc71\boost_regex-vc71-mt-1_32\$(NULL)" mkdir vc71\boost_regex-vc71-mt-1_32

boost_regex-vc71-mt-1_32_clean :
	del vc71\boost_regex-vc71-mt-1_32\*.obj
	del vc71\boost_regex-vc71-mt-1_32\*.idb
	del vc71\boost_regex-vc71-mt-1_32\*.exp
	del vc71\boost_regex-vc71-mt-1_32\*.pch

./vc71/boost_regex-vc71-mt-1_32.lib : vc71/boost_regex-vc71-mt-1_32/c_regex_traits.obj vc71/boost_regex-vc71-mt-1_32/cpp_regex_traits.obj vc71/boost_regex-vc71-mt-1_32/cregex.obj vc71/boost_regex-vc71-mt-1_32/fileiter.obj vc71/boost_regex-vc71-mt-1_32/instances.obj vc71/boost_regex-vc71-mt-1_32/posix_api.obj vc71/boost_regex-vc71-mt-1_32/regex.obj vc71/boost_regex-vc71-mt-1_32/regex_debug.obj vc71/boost_regex-vc71-mt-1_32/regex_raw_buffer.obj vc71/boost_regex-vc71-mt-1_32/regex_traits_defaults.obj vc71/boost_regex-vc71-mt-1_32/static_mutex.obj vc71/boost_regex-vc71-mt-1_32/usinstances.obj vc71/boost_regex-vc71-mt-1_32/w32_regex_traits.obj vc71/boost_regex-vc71-mt-1_32/wc_regex_traits.obj vc71/boost_regex-vc71-mt-1_32/wide_posix_api.obj vc71/boost_regex-vc71-mt-1_32/winstances.obj
	link kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:yes /pdb:"vc71/boost_regex-vc71-mt-1_32.pdb" /debug /machine:I386 /out:"vc71/boost_regex-vc71-mt-1_32.dll" /implib:"vc71/boost_regex-vc71-mt-1_32.lib" /LIBPATH:$(STLPORT_PATH)\lib $(XLFLAGS)  vc71/boost_regex-vc71-mt-1_32/c_regex_traits.obj vc71/boost_regex-vc71-mt-1_32/cpp_regex_traits.obj vc71/boost_regex-vc71-mt-1_32/cregex.obj vc71/boost_regex-vc71-mt-1_32/fileiter.obj vc71/boost_regex-vc71-mt-1_32/instances.obj vc71/boost_regex-vc71-mt-1_32/posix_api.obj vc71/boost_regex-vc71-mt-1_32/regex.obj vc71/boost_regex-vc71-mt-1_32/regex_debug.obj vc71/boost_regex-vc71-mt-1_32/regex_raw_buffer.obj vc71/boost_regex-vc71-mt-1_32/regex_traits_defaults.obj vc71/boost_regex-vc71-mt-1_32/static_mutex.obj vc71/boost_regex-vc71-mt-1_32/usinstances.obj vc71/boost_regex-vc71-mt-1_32/w32_regex_traits.obj vc71/boost_regex-vc71-mt-1_32/wc_regex_traits.obj vc71/boost_regex-vc71-mt-1_32/wide_posix_api.obj vc71/boost_regex-vc71-mt-1_32/winstances.obj

########################################################
#
# section for libboost_regex-vc71-mt-1_32.lib
#
########################################################
vc71/libboost_regex-vc71-mt-1_32/c_regex_traits.obj: ../src/c_regex_traits.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MD /W3 /GX /O2 /GB /GF /Gy /I../../../ /DBOOST_REGEX_STATIC_LINK /DNDEBUG /DWIN32 /D_WINDOWS /D_MBCS /D_USRDLL /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-1_32/ -Fdvc71/libboost_regex-vc71-mt-1_32.pdb ../src/c_regex_traits.cpp

vc71/libboost_regex-vc71-mt-1_32/cpp_regex_traits.obj: ../src/cpp_regex_traits.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MD /W3 /GX /O2 /GB /GF /Gy /I../../../ /DBOOST_REGEX_STATIC_LINK /DNDEBUG /DWIN32 /D_WINDOWS /D_MBCS /D_USRDLL /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-1_32/ -Fdvc71/libboost_regex-vc71-mt-1_32.pdb ../src/cpp_regex_traits.cpp

vc71/libboost_regex-vc71-mt-1_32/cregex.obj: ../src/cregex.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MD /W3 /GX /O2 /GB /GF /Gy /I../../../ /DBOOST_REGEX_STATIC_LINK /DNDEBUG /DWIN32 /D_WINDOWS /D_MBCS /D_USRDLL /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-1_32/ -Fdvc71/libboost_regex-vc71-mt-1_32.pdb ../src/cregex.cpp

vc71/libboost_regex-vc71-mt-1_32/fileiter.obj: ../src/fileiter.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MD /W3 /GX /O2 /GB /GF /Gy /I../../../ /DBOOST_REGEX_STATIC_LINK /DNDEBUG /DWIN32 /D_WINDOWS /D_MBCS /D_USRDLL /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-1_32/ -Fdvc71/libboost_regex-vc71-mt-1_32.pdb ../src/fileiter.cpp

vc71/libboost_regex-vc71-mt-1_32/instances.obj: ../src/instances.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MD /W3 /GX /O2 /GB /GF /Gy /I../../../ /DBOOST_REGEX_STATIC_LINK /DNDEBUG /DWIN32 /D_WINDOWS /D_MBCS /D_USRDLL /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-1_32/ -Fdvc71/libboost_regex-vc71-mt-1_32.pdb ../src/instances.cpp

vc71/libboost_regex-vc71-mt-1_32/posix_api.obj: ../src/posix_api.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MD /W3 /GX /O2 /GB /GF /Gy /I../../../ /DBOOST_REGEX_STATIC_LINK /DNDEBUG /DWIN32 /D_WINDOWS /D_MBCS /D_USRDLL /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-1_32/ -Fdvc71/libboost_regex-vc71-mt-1_32.pdb ../src/posix_api.cpp

vc71/libboost_regex-vc71-mt-1_32/regex.obj: ../src/regex.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MD /W3 /GX /O2 /GB /GF /Gy /I../../../ /DBOOST_REGEX_STATIC_LINK /DNDEBUG /DWIN32 /D_WINDOWS /D_MBCS /D_USRDLL /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-1_32/ -Fdvc71/libboost_regex-vc71-mt-1_32.pdb ../src/regex.cpp

vc71/libboost_regex-vc71-mt-1_32/regex_debug.obj: ../src/regex_debug.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MD /W3 /GX /O2 /GB /GF /Gy /I../../../ /DBOOST_REGEX_STATIC_LINK /DNDEBUG /DWIN32 /D_WINDOWS /D_MBCS /D_USRDLL /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-1_32/ -Fdvc71/libboost_regex-vc71-mt-1_32.pdb ../src/regex_debug.cpp

vc71/libboost_regex-vc71-mt-1_32/regex_raw_buffer.obj: ../src/regex_raw_buffer.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MD /W3 /GX /O2 /GB /GF /Gy /I../../../ /DBOOST_REGEX_STATIC_LINK /DNDEBUG /DWIN32 /D_WINDOWS /D_MBCS /D_USRDLL /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-1_32/ -Fdvc71/libboost_regex-vc71-mt-1_32.pdb ../src/regex_raw_buffer.cpp

vc71/libboost_regex-vc71-mt-1_32/regex_traits_defaults.obj: ../src/regex_traits_defaults.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MD /W3 /GX /O2 /GB /GF /Gy /I../../../ /DBOOST_REGEX_STATIC_LINK /DNDEBUG /DWIN32 /D_WINDOWS /D_MBCS /D_USRDLL /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-1_32/ -Fdvc71/libboost_regex-vc71-mt-1_32.pdb ../src/regex_traits_defaults.cpp

vc71/libboost_regex-vc71-mt-1_32/static_mutex.obj: ../src/static_mutex.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MD /W3 /GX /O2 /GB /GF /Gy /I../../../ /DBOOST_REGEX_STATIC_LINK /DNDEBUG /DWIN32 /D_WINDOWS /D_MBCS /D_USRDLL /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-1_32/ -Fdvc71/libboost_regex-vc71-mt-1_32.pdb ../src/static_mutex.cpp

vc71/libboost_regex-vc71-mt-1_32/usinstances.obj: ../src/usinstances.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MD /W3 /GX /O2 /GB /GF /Gy /I../../../ /DBOOST_REGEX_STATIC_LINK /DNDEBUG /DWIN32 /D_WINDOWS /D_MBCS /D_USRDLL /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-1_32/ -Fdvc71/libboost_regex-vc71-mt-1_32.pdb ../src/usinstances.cpp

vc71/libboost_regex-vc71-mt-1_32/w32_regex_traits.obj: ../src/w32_regex_traits.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MD /W3 /GX /O2 /GB /GF /Gy /I../../../ /DBOOST_REGEX_STATIC_LINK /DNDEBUG /DWIN32 /D_WINDOWS /D_MBCS /D_USRDLL /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-1_32/ -Fdvc71/libboost_regex-vc71-mt-1_32.pdb ../src/w32_regex_traits.cpp

vc71/libboost_regex-vc71-mt-1_32/wc_regex_traits.obj: ../src/wc_regex_traits.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MD /W3 /GX /O2 /GB /GF /Gy /I../../../ /DBOOST_REGEX_STATIC_LINK /DNDEBUG /DWIN32 /D_WINDOWS /D_MBCS /D_USRDLL /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-1_32/ -Fdvc71/libboost_regex-vc71-mt-1_32.pdb ../src/wc_regex_traits.cpp

vc71/libboost_regex-vc71-mt-1_32/wide_posix_api.obj: ../src/wide_posix_api.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MD /W3 /GX /O2 /GB /GF /Gy /I../../../ /DBOOST_REGEX_STATIC_LINK /DNDEBUG /DWIN32 /D_WINDOWS /D_MBCS /D_USRDLL /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-1_32/ -Fdvc71/libboost_regex-vc71-mt-1_32.pdb ../src/wide_posix_api.cpp

vc71/libboost_regex-vc71-mt-1_32/winstances.obj: ../src/winstances.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MD /W3 /GX /O2 /GB /GF /Gy /I../../../ /DBOOST_REGEX_STATIC_LINK /DNDEBUG /DWIN32 /D_WINDOWS /D_MBCS /D_USRDLL /FD /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-1_32/ -Fdvc71/libboost_regex-vc71-mt-1_32.pdb ../src/winstances.cpp

libboost_regex-vc71-mt-1_32_dir :
	@if not exist "vc71\libboost_regex-vc71-mt-1_32\$(NULL)" mkdir vc71\libboost_regex-vc71-mt-1_32

libboost_regex-vc71-mt-1_32_clean :
	del vc71\libboost_regex-vc71-mt-1_32\*.obj
	del vc71\libboost_regex-vc71-mt-1_32\*.idb
	del vc71\libboost_regex-vc71-mt-1_32\*.exp
	del vc71\libboost_regex-vc71-mt-1_32\*.pch

./vc71/libboost_regex-vc71-mt-1_32.lib : vc71/libboost_regex-vc71-mt-1_32/c_regex_traits.obj vc71/libboost_regex-vc71-mt-1_32/cpp_regex_traits.obj vc71/libboost_regex-vc71-mt-1_32/cregex.obj vc71/libboost_regex-vc71-mt-1_32/fileiter.obj vc71/libboost_regex-vc71-mt-1_32/instances.obj vc71/libboost_regex-vc71-mt-1_32/posix_api.obj vc71/libboost_regex-vc71-mt-1_32/regex.obj vc71/libboost_regex-vc71-mt-1_32/regex_debug.obj vc71/libboost_regex-vc71-mt-1_32/regex_raw_buffer.obj vc71/libboost_regex-vc71-mt-1_32/regex_traits_defaults.obj vc71/libboost_regex-vc71-mt-1_32/static_mutex.obj vc71/libboost_regex-vc71-mt-1_32/usinstances.obj vc71/libboost_regex-vc71-mt-1_32/w32_regex_traits.obj vc71/libboost_regex-vc71-mt-1_32/wc_regex_traits.obj vc71/libboost_regex-vc71-mt-1_32/wide_posix_api.obj vc71/libboost_regex-vc71-mt-1_32/winstances.obj
	link -lib /nologo /out:vc71/libboost_regex-vc71-mt-1_32.lib $(XSFLAGS)  vc71/libboost_regex-vc71-mt-1_32/c_regex_traits.obj vc71/libboost_regex-vc71-mt-1_32/cpp_regex_traits.obj vc71/libboost_regex-vc71-mt-1_32/cregex.obj vc71/libboost_regex-vc71-mt-1_32/fileiter.obj vc71/libboost_regex-vc71-mt-1_32/instances.obj vc71/libboost_regex-vc71-mt-1_32/posix_api.obj vc71/libboost_regex-vc71-mt-1_32/regex.obj vc71/libboost_regex-vc71-mt-1_32/regex_debug.obj vc71/libboost_regex-vc71-mt-1_32/regex_raw_buffer.obj vc71/libboost_regex-vc71-mt-1_32/regex_traits_defaults.obj vc71/libboost_regex-vc71-mt-1_32/static_mutex.obj vc71/libboost_regex-vc71-mt-1_32/usinstances.obj vc71/libboost_regex-vc71-mt-1_32/w32_regex_traits.obj vc71/libboost_regex-vc71-mt-1_32/wc_regex_traits.obj vc71/libboost_regex-vc71-mt-1_32/wide_posix_api.obj vc71/libboost_regex-vc71-mt-1_32/winstances.obj

########################################################
#
# section for libboost_regex-vc71-mt-gd-1_32.lib
#
########################################################
vc71/libboost_regex-vc71-mt-gd-1_32/c_regex_traits.obj: ../src/c_regex_traits.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MDd /W3 /Gm /GX /Zi /Od /I../../../ /DBOOST_REGEX_STATIC_LINK /D_DEBUG /DWIN32 /D_WINDOWS /D_MBCS /DUSRDLL /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-gd-1_32/ -Fdvc71/libboost_regex-vc71-mt-gd-1_32.pdb ../src/c_regex_traits.cpp

vc71/libboost_regex-vc71-mt-gd-1_32/cpp_regex_traits.obj: ../src/cpp_regex_traits.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MDd /W3 /Gm /GX /Zi /Od /I../../../ /DBOOST_REGEX_STATIC_LINK /D_DEBUG /DWIN32 /D_WINDOWS /D_MBCS /DUSRDLL /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-gd-1_32/ -Fdvc71/libboost_regex-vc71-mt-gd-1_32.pdb ../src/cpp_regex_traits.cpp

vc71/libboost_regex-vc71-mt-gd-1_32/cregex.obj: ../src/cregex.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MDd /W3 /Gm /GX /Zi /Od /I../../../ /DBOOST_REGEX_STATIC_LINK /D_DEBUG /DWIN32 /D_WINDOWS /D_MBCS /DUSRDLL /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-gd-1_32/ -Fdvc71/libboost_regex-vc71-mt-gd-1_32.pdb ../src/cregex.cpp

vc71/libboost_regex-vc71-mt-gd-1_32/fileiter.obj: ../src/fileiter.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MDd /W3 /Gm /GX /Zi /Od /I../../../ /DBOOST_REGEX_STATIC_LINK /D_DEBUG /DWIN32 /D_WINDOWS /D_MBCS /DUSRDLL /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-gd-1_32/ -Fdvc71/libboost_regex-vc71-mt-gd-1_32.pdb ../src/fileiter.cpp

vc71/libboost_regex-vc71-mt-gd-1_32/instances.obj: ../src/instances.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MDd /W3 /Gm /GX /Zi /Od /I../../../ /DBOOST_REGEX_STATIC_LINK /D_DEBUG /DWIN32 /D_WINDOWS /D_MBCS /DUSRDLL /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-gd-1_32/ -Fdvc71/libboost_regex-vc71-mt-gd-1_32.pdb ../src/instances.cpp

vc71/libboost_regex-vc71-mt-gd-1_32/posix_api.obj: ../src/posix_api.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MDd /W3 /Gm /GX /Zi /Od /I../../../ /DBOOST_REGEX_STATIC_LINK /D_DEBUG /DWIN32 /D_WINDOWS /D_MBCS /DUSRDLL /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-gd-1_32/ -Fdvc71/libboost_regex-vc71-mt-gd-1_32.pdb ../src/posix_api.cpp

vc71/libboost_regex-vc71-mt-gd-1_32/regex.obj: ../src/regex.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MDd /W3 /Gm /GX /Zi /Od /I../../../ /DBOOST_REGEX_STATIC_LINK /D_DEBUG /DWIN32 /D_WINDOWS /D_MBCS /DUSRDLL /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-gd-1_32/ -Fdvc71/libboost_regex-vc71-mt-gd-1_32.pdb ../src/regex.cpp

vc71/libboost_regex-vc71-mt-gd-1_32/regex_debug.obj: ../src/regex_debug.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MDd /W3 /Gm /GX /Zi /Od /I../../../ /DBOOST_REGEX_STATIC_LINK /D_DEBUG /DWIN32 /D_WINDOWS /D_MBCS /DUSRDLL /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-gd-1_32/ -Fdvc71/libboost_regex-vc71-mt-gd-1_32.pdb ../src/regex_debug.cpp

vc71/libboost_regex-vc71-mt-gd-1_32/regex_raw_buffer.obj: ../src/regex_raw_buffer.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MDd /W3 /Gm /GX /Zi /Od /I../../../ /DBOOST_REGEX_STATIC_LINK /D_DEBUG /DWIN32 /D_WINDOWS /D_MBCS /DUSRDLL /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-gd-1_32/ -Fdvc71/libboost_regex-vc71-mt-gd-1_32.pdb ../src/regex_raw_buffer.cpp

vc71/libboost_regex-vc71-mt-gd-1_32/regex_traits_defaults.obj: ../src/regex_traits_defaults.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MDd /W3 /Gm /GX /Zi /Od /I../../../ /DBOOST_REGEX_STATIC_LINK /D_DEBUG /DWIN32 /D_WINDOWS /D_MBCS /DUSRDLL /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-gd-1_32/ -Fdvc71/libboost_regex-vc71-mt-gd-1_32.pdb ../src/regex_traits_defaults.cpp

vc71/libboost_regex-vc71-mt-gd-1_32/static_mutex.obj: ../src/static_mutex.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MDd /W3 /Gm /GX /Zi /Od /I../../../ /DBOOST_REGEX_STATIC_LINK /D_DEBUG /DWIN32 /D_WINDOWS /D_MBCS /DUSRDLL /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-gd-1_32/ -Fdvc71/libboost_regex-vc71-mt-gd-1_32.pdb ../src/static_mutex.cpp

vc71/libboost_regex-vc71-mt-gd-1_32/usinstances.obj: ../src/usinstances.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MDd /W3 /Gm /GX /Zi /Od /I../../../ /DBOOST_REGEX_STATIC_LINK /D_DEBUG /DWIN32 /D_WINDOWS /D_MBCS /DUSRDLL /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-gd-1_32/ -Fdvc71/libboost_regex-vc71-mt-gd-1_32.pdb ../src/usinstances.cpp

vc71/libboost_regex-vc71-mt-gd-1_32/w32_regex_traits.obj: ../src/w32_regex_traits.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MDd /W3 /Gm /GX /Zi /Od /I../../../ /DBOOST_REGEX_STATIC_LINK /D_DEBUG /DWIN32 /D_WINDOWS /D_MBCS /DUSRDLL /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-gd-1_32/ -Fdvc71/libboost_regex-vc71-mt-gd-1_32.pdb ../src/w32_regex_traits.cpp

vc71/libboost_regex-vc71-mt-gd-1_32/wc_regex_traits.obj: ../src/wc_regex_traits.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MDd /W3 /Gm /GX /Zi /Od /I../../../ /DBOOST_REGEX_STATIC_LINK /D_DEBUG /DWIN32 /D_WINDOWS /D_MBCS /DUSRDLL /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-gd-1_32/ -Fdvc71/libboost_regex-vc71-mt-gd-1_32.pdb ../src/wc_regex_traits.cpp

vc71/libboost_regex-vc71-mt-gd-1_32/wide_posix_api.obj: ../src/wide_posix_api.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MDd /W3 /Gm /GX /Zi /Od /I../../../ /DBOOST_REGEX_STATIC_LINK /D_DEBUG /DWIN32 /D_WINDOWS /D_MBCS /DUSRDLL /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-gd-1_32/ -Fdvc71/libboost_regex-vc71-mt-gd-1_32.pdb ../src/wide_posix_api.cpp

vc71/libboost_regex-vc71-mt-gd-1_32/winstances.obj: ../src/winstances.cpp $(ALL_HEADER)
	cl $(INCLUDES) /nologo /MDd /W3 /Gm /GX /Zi /Od /I../../../ /DBOOST_REGEX_STATIC_LINK /D_DEBUG /DWIN32 /D_WINDOWS /D_MBCS /DUSRDLL /FD /GX /RTC1 /Zc:wchar_t /c $(CXXFLAGS) -Y- -Fo./vc71/libboost_regex-vc71-mt-gd-1_32/ -Fdvc71/libboost_regex-vc71-mt-gd-1_32.pdb ../src/winstances.cpp

libboost_regex-vc71-mt-gd-1_32_dir :
	@if not exist "vc71\libboost_regex-vc71-mt-gd-1_32\$(NULL)" mkdir vc71\libboost_regex-vc71-mt-gd-1_32

libboost_regex-vc71-mt-gd-1_32_clean :
	del vc71\libboost_regex-vc71-mt-gd-1_32\*.obj
	del vc71\libboost_regex-vc71-mt-gd-1_32\*.idb
	del vc71\libboost_regex-vc71-mt-gd-1_32\*.exp
	del vc71\libboost_regex-vc71-mt-gd-1_32\*.pch

./vc71/libboost_regex-vc71-mt-gd-1_32.lib : vc71/libboost_regex-vc71-mt-gd-1_32/c_regex_traits.obj vc71/libboost_regex-vc71-mt-gd-1_32/cpp_regex_traits.obj vc71/libboost_regex-vc71-mt-gd-1_32/cregex.obj vc71/libboost_regex-vc71-mt-gd-1_32/fileiter.obj vc71/libboost_regex-vc71-mt-gd-1_32/instances.obj vc71/libboost_regex-vc71-mt-gd-1_32/posix_api.obj vc71/libboost_regex-vc71-mt-gd-1_32/regex.obj vc71/libboost_regex-vc71-mt-gd-1_32/regex_debug.obj vc71/libboost_regex-vc71-mt-gd-1_32/regex_raw_buffer.obj vc71/libboost_regex-vc71-mt-gd-1_32/regex_traits_defaults.obj vc71/libboost_regex-vc71-mt-gd-1_32/static_mutex.obj vc71/libboost_regex-vc71-mt-gd-1_32/usinstances.obj vc71/libboost_regex-vc71-mt-gd-1_32/w32_regex_traits.obj vc71/libboost_regex-vc71-mt-gd-1_32/wc_regex_traits.obj vc71/libboost_regex-vc71-mt-gd-1_32/wide_posix_api.obj vc71/libboost_regex-vc71-mt-gd-1_32/winstances.obj
	link -lib /nologo /out:vc71/libboost_regex-vc71-mt-gd-1_32.lib $(XSFLAGS)  vc71/libboost_regex-vc71-mt-gd-1_32/c_regex_traits.obj vc71/libboost_regex-vc71-mt-gd-1_32/cpp_regex_traits.obj vc71/libboost_regex-vc71-mt-gd-1_32/cregex.obj vc71/libboost_regex-vc71-mt-gd-1_32/fileiter.obj vc71/libboost_regex-vc71-mt-gd-1_32/instances.obj vc71/libboost_regex-vc71-mt-gd-1_32/posix_api.obj vc71/libboost_regex-vc71-mt-gd-1_32/regex.obj vc71/libboost_regex-vc71-mt-gd-1_32/regex_debug.obj vc71/libboost_regex-vc71-mt-gd-1_32/regex_raw_buffer.obj vc71/libboost_regex-vc71-mt-gd-1_32/regex_traits_defaults.obj vc71/libboost_regex-vc71-mt-gd-1_32/static_mutex.obj vc71/libboost_regex-vc71-mt-gd-1_32/usinstances.obj vc71/libboost_regex-vc71-mt-gd-1_32/w32_regex_traits.obj vc71/libboost_regex-vc71-mt-gd-1_32/wc_regex_traits.obj vc71/libboost_regex-vc71-mt-gd-1_32/wide_posix_api.obj vc71/libboost_regex-vc71-mt-gd-1_32/winstances.obj

