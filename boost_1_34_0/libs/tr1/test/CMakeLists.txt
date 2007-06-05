include_directories(${Boost_SOURCE_DIR}/boost/tr1/tr1)

if (BORLAND)
  # Borland's broken include mechanism needs these extra headers:
  include_directories(${Boost_SOURCE_DIR}/boost/tr1/tr1/bcc32)  
endif (BORLAND)

macro(boost_glob_test_compile path glob_par compile_flags)
	  file(GLOB file_list ${path}/${glob_par})
	  foreach (current_file ${file_list})
	  	GET_FILENAME_COMPONENT(file_name ${current_file} NAME_WE)
	  	boost_test_compile(${file_name} SOURCES ${path}/${file_name}.cpp COMPILE_FLAGS "${compile_flags}")
	  endforeach (current_file)
endmacro(boost_glob_test_compile)

macro(boost_glob_test_compile_fail patch glob_par compile_flags)
	  file(GLOB file_list ${glob_par})
	  foreach (current_file ${file_list})
	  	GET_FILENAME_COMPONENT(file_name ${current_file} NAME_WE)
	  	boost_test_compile_fail(${file_name} SOURCES ${path}/${file_name}.cpp COMPILE_FLAGS "${compile_flags}")
	  endforeach (current_file)
endmacro(boost_glob_test_compile_fail)

macro(boost_glob_test_run path glob_par compile_flags)
	  file(GLOB file_list ${path}/${glob_par})
	  foreach (current_file ${file_list})
	  	GET_FILENAME_COMPONENT(file_name ${current_file} NAME_WE)
	  	boost_test_run(${file_name} SOURCES ${path}/${file_name}.cpp COMPILE_FLAGS "${compile_flags}")
	  endforeach (current_file)
endmacro(boost_glob_test_run)


boost_glob_test_compile(. test*.cpp "-DTEST_STD_HEADERS=1")

boost_glob_test_run(. run*.cpp -DTEST_STD_HEADERS=1)

boost_glob_test_compile_fail(config tr1_has_tr1*fail.cpp "")

boost_glob_test_compile(config tr1_has_tr1*pass.cpp "")

boost_glob_test_run(type_traits *.cpp "-DTEST_STD=1")

boost_glob_test_compile(std_headers *.cpp "")
