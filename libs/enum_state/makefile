#
# Build smart_enum<> test programs.
#

TESTS		=  test_bounded_enum.exe test_lambda_enum.exe \
		   test_sequential_enum.exe test_wrapped_enum.exe

CXX		= g++

CPPFLAGS       += -I/home/simons/projects/boost-current -I/home/simons/projects/boost-sandbox
CXXFLAGS       += -Wall -ansi -pedantic -Wno-long-long
LDFLAGS	       +=

OBJS	       += test.o
LIBS	       +=

.SUFFIXES:
.SUFFIXES:	.cpp .exe

.cpp.exe:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LIBS) $< -o $@

.PHONY:		clean distclean realclean depend

all::		$(TESTS)

check::		$(TESTS)
	@for n in $(TESTS); do echo -en "$${n%%.exe}:\t"; .\/$$n; done

realclean distclean clean::
	@rm -f $(TESTS)
	@echo All dependent files have been removed.

depend::
	makedepend -Y $(CPPFLAGS) -fmakefile '-s# Dependencies' *.cpp 2>/dev/null
	@sed <makefile >makefile.bak -e 's/\.\///g'
	@mv makefile.bak makefile

# Dependencies

test_bounded_enum.o: /home/simons/projects/boost-current/boost/static_assert.hpp
test_bounded_enum.o: /home/simons/projects/boost-current/boost/config.hpp
test_bounded_enum.o: /home/simons/projects/boost-current/boost/config/user.hpp
test_bounded_enum.o: /home/simons/projects/boost-current/boost/config/select_compiler_config.hpp
test_bounded_enum.o: /home/simons/projects/boost-current/boost/config/compiler/gcc.hpp
test_bounded_enum.o: /home/simons/projects/boost-current/boost/config/select_stdlib_config.hpp
test_bounded_enum.o: /home/simons/projects/boost-current/boost/config/select_platform_config.hpp
test_bounded_enum.o: /home/simons/projects/boost-current/boost/config/suffix.hpp
test_bounded_enum.o: /home/simons/projects/boost-current/boost/test/test_tools.hpp
test_bounded_enum.o: /home/simons/projects/boost-current/boost/test/cpp_main.cpp
test_bounded_enum.o: /home/simons/projects/boost-current/boost/detail/catch_exceptions.hpp
test_bounded_enum.o: /home/simons/projects/boost-current/boost/cstdlib.hpp
test_bounded_enum.o: /home/simons/projects/boost-current/boost/test/test_main.cpp
test_bounded_enum.o: /home/simons/projects/boost-sandbox/boost/enum_state.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/function.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/function/function_base.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/config.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/config/user.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/config/select_compiler_config.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/config/compiler/gcc.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/config/select_stdlib_config.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/config/select_platform_config.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/config/suffix.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/type_traits.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/type_traits/fwd.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/type_traits/ice.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/type_traits/conversion_traits.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/type_traits/arithmetic_traits.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/type_traits/cv_traits.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/type_traits/reference_traits.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/type_traits/utility.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/type_traits/array_traits.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/type_traits/composite_traits.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/type_traits/transform_traits.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/type_traits/is_class.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/type_traits/alignment_traits.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/preprocessor/list/for_each_i.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/preprocessor/list/adt.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/preprocessor/tuple/elem.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/preprocessor/logical/not.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/preprocessor/logical/nor.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/preprocessor/logical/bool.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/preprocessor/inc.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/preprocessor/for.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/preprocessor/if.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/preprocessor/tuple/eat.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/preprocessor/tuple/to_list.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/preprocessor/cat.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/static_assert.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/config.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/type_traits/object_traits.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/type_traits/function_traits.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/type_traits/same_traits.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/ref.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/config.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/utility/addressof.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/function/function0.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/function/function_template.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/mem_fn.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/bind/mem_fn_template.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/bind/mem_fn_cc.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/function/function1.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/function/function2.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/function/function3.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/function/function4.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/function/function5.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/function/function6.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/function/function7.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/function/function8.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/function/function9.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/function/function10.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/lambda/lambda.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/lambda/core.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/tuple/tuple.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/tuple/detail/tuple_basic.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/lambda/detail/lambda_config.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/lambda/detail/lambda_fwd.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/lambda/detail/arity_code.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/lambda/detail/actions.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/lambda/detail/lambda_traits.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/lambda/detail/function_adaptors.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/lambda/detail/return_type_traits.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/lambda/detail/select_functions.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/lambda/detail/lambda_functor_base.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/lambda/detail/lambda_functors.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/lambda/detail/ret.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/lambda/detail/operator_actions.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/lambda/detail/operator_lambda_func_base.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/lambda/detail/operator_return_type_traits.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/lambda/detail/is_instance_of.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/preprocessor/enum_shifted_params.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/preprocessor/enum_shifted.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/preprocessor/enum.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/preprocessor/comma_if.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/preprocessor/comma.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/preprocessor/empty.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/preprocessor/repeat.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/preprocessor/tuple.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/preprocessor/tuple/reverse.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/preprocessor/dec.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/preprocessor/repeat_2nd.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/lambda/detail/operators.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/lambda/if.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/lambda/switch.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/lambda/detail/control_constructs_common.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/lambda/loops.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/lambda/exceptions.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/test/test_tools.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/test/cpp_main.cpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/detail/catch_exceptions.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/cstdlib.hpp
test_lambda_enum.o: /home/simons/projects/boost-current/boost/test/test_main.cpp
test_lambda_enum.o: /home/simons/projects/boost-sandbox/boost/enum_state.hpp
test_sequential_enum.o: /home/simons/projects/boost-current/boost/static_assert.hpp
test_sequential_enum.o: /home/simons/projects/boost-current/boost/config.hpp
test_sequential_enum.o: /home/simons/projects/boost-current/boost/config/user.hpp
test_sequential_enum.o: /home/simons/projects/boost-current/boost/config/select_compiler_config.hpp
test_sequential_enum.o: /home/simons/projects/boost-current/boost/config/compiler/gcc.hpp
test_sequential_enum.o: /home/simons/projects/boost-current/boost/config/select_stdlib_config.hpp
test_sequential_enum.o: /home/simons/projects/boost-current/boost/config/select_platform_config.hpp
test_sequential_enum.o: /home/simons/projects/boost-current/boost/config/suffix.hpp
test_sequential_enum.o: /home/simons/projects/boost-current/boost/test/test_tools.hpp
test_sequential_enum.o: /home/simons/projects/boost-current/boost/test/cpp_main.cpp
test_sequential_enum.o: /home/simons/projects/boost-current/boost/detail/catch_exceptions.hpp
test_sequential_enum.o: /home/simons/projects/boost-current/boost/cstdlib.hpp
test_sequential_enum.o: /home/simons/projects/boost-current/boost/test/test_main.cpp
test_sequential_enum.o: /home/simons/projects/boost-sandbox/boost/enum_state.hpp
test_wrapped_enum.o: /home/simons/projects/boost-current/boost/static_assert.hpp
test_wrapped_enum.o: /home/simons/projects/boost-current/boost/config.hpp
test_wrapped_enum.o: /home/simons/projects/boost-current/boost/config/user.hpp
test_wrapped_enum.o: /home/simons/projects/boost-current/boost/config/select_compiler_config.hpp
test_wrapped_enum.o: /home/simons/projects/boost-current/boost/config/compiler/gcc.hpp
test_wrapped_enum.o: /home/simons/projects/boost-current/boost/config/select_stdlib_config.hpp
test_wrapped_enum.o: /home/simons/projects/boost-current/boost/config/select_platform_config.hpp
test_wrapped_enum.o: /home/simons/projects/boost-current/boost/config/suffix.hpp
test_wrapped_enum.o: /home/simons/projects/boost-current/boost/test/test_tools.hpp
test_wrapped_enum.o: /home/simons/projects/boost-current/boost/test/cpp_main.cpp
test_wrapped_enum.o: /home/simons/projects/boost-current/boost/detail/catch_exceptions.hpp
test_wrapped_enum.o: /home/simons/projects/boost-current/boost/cstdlib.hpp
test_wrapped_enum.o: /home/simons/projects/boost-current/boost/test/test_main.cpp
test_wrapped_enum.o: /home/simons/projects/boost-sandbox/boost/enum_state.hpp
