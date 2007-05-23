# The set of archives that we will use for testing the Serialization
# library
set(BOOST_ARCHIVE_LIST 
  "text_archive" 
  "text_warchive" 
  "binary_archive" 
  "xml_archive" 
  "xml_warchive"
  )

# The serialization_test macro declares a set of tests of the
# serialization library. For each invocation of serialization_test,
# this macro will build separate static and dynamic tests for each
# archive type listed in BOOST_ARCHIVE_LIST.
macro(serialization_test testname)
  parse_arguments(BSL_TEST "" "NO_LIBS" ${ARGN})

  foreach(ARCHIVE ${BOOST_ARCHIVE_LIST})
    set(BSL_TEST_STATIC_LIBRARIES) 
    set(BSL_TEST_SHARED_LIBRARIES)
    if(NOT BSL_NO_LIBS)
      set(BSL_TEST_STATIC_LIBRARIES "boost_serialization-static")
      set(BSL_TEST_SHARED_LIBRARIES "boost_serialization-shared")
      if(ARCHIVE MATCHES ".*warchive")
        set(BSL_TEST_STATIC_LIBRARIES 
          "boost_wserialization-static" ${BSL_TEST_STATIC_LIBRARIES})
        set(BSL_TEST_SHARED_LIBRARIES 
          "boost_wserialization-shared" ${BSL_TEST_SHARED_LIBRARIES})
      endif(ARCHIVE MATCHES ".*warchive")
    endif(NOT BSL_NO_LIBS)

    # Tests linking against the static serialization library
    boost_test_run("${testname}_${ARCHIVE}"
      SOURCES "${testname}.cpp"
      LIBRARIES boost_test_exec_monitor-static ${BSL_TEST_STATIC_LIBRARIES}
      COMPILE_FLAGS "-DBOOST_ARCHIVE_TEST=${ARCHIVE}.hpp -DBOOST_LIB_DIAGNOSTIC=1")
    
    # Tests linking against the shared serialization library
    boost_test_run("${testname}_${ARCHIVE}_dll"
      SOURCES "${testname}.cpp"
      LIBRARIES boost_test_exec_monitor-static ${BSL_TEST_SHARED_LIBRARIES}
      COMPILE_FLAGS "-DBOOST_ARCHIVE_TEST=${ARCHIVE}.hpp -DBOOST_LIB_DIAGNOSTIC=1 -DBOOST_ALL_DYN_LINK=1")
  endforeach(ARCHIVE ${BOOST_ARCHIVE_LIST})
endmacro(serialization_test)

serialization_test(test_array)
serialization_test(test_binary)
serialization_test(test_contained_class)
serialization_test(test_cyclic_ptrs)
serialization_test(test_delete_pointer)
serialization_test(test_deque)
serialization_test(test_derived)
serialization_test(test_derived_class)
serialization_test(test_derived_class_ptr)
serialization_test(test_diamond)
serialization_test(test_exported)
serialization_test(test_class_info_load)
serialization_test(test_class_info_save)
serialization_test(test_object)
serialization_test(test_primitive)
serialization_test(test_list)
serialization_test(test_list_ptrs)
serialization_test(test_map)
serialization_test(test_mi)
serialization_test(test_multiple_ptrs)
serialization_test(test_no_rtti)
serialization_test(test_non_intrusive)
serialization_test(test_non_default_ctor)
serialization_test(test_non_default_ctor2)
serialization_test(test_null_ptr)
serialization_test(test_nvp)
serialization_test(test_recursion)
serialization_test(test_registered)
serialization_test(test_set)
serialization_test(test_simple_class)
serialization_test(test_simple_class_ptr)
serialization_test(test_split)
serialization_test(test_tracking)
serialization_test(test_unregistered)
serialization_test(test_variant)
serialization_test(test_vector)
serialization_test(test_optional)
serialization_test(test_shared_ptr)
serialization_test(test_shared_ptr_132)
# TODO
#     [ test-bsl-run_polymorphic_archive test_polymorphic : test_polymorphic_A ]

