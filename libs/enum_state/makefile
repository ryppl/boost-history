#
# Build checked_enum<> test program.
#

CXX		= g++

OPTIMFLAGS     += -O3
WARNFLAGS      += -Wall -ansi -pedantic -Wno-long-long
CPPFLAGS       += -I/home/simons/projects/boost-current -I/home/simons/projects/boost-sandbox
CXXFLAGS       += -frepo
LDFLAGS	       += -frepo

OBJS	       += test.o
LIBS	       +=

.SUFFIXES:	.o .cpp
.PHONY:		clean distclean realclean depend

.cpp.o:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(WARNFLAGS) $(OPTIMFLAGS) -c $< -o $@

all::		test

test:	$(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) $(LIBS) -o $@

check::		test
	.\/test

realclean distclean clean::
	@find . -name '*.rpo' -exec rm -f {} \;
	@find . -name '*.a'   -exec rm -f {} \;
	@find . -name '*.o'   -exec rm -f {} \;
	@find . -name '*.so'  -exec rm -f {} \;
	@rm -f test
	@echo All dependent files have been removed.

depend::
	makedepend -Y $(CPPFLAGS) -fmakefile '-s# Dependencies' *.cpp 2>/dev/null
	@sed <makefile >makefile.bak -e 's/\.\///g'
	@mv makefile.bak makefile

# Dependencies

test.o: /home/simons/projects/boost-current/boost/function.hpp
test.o: /home/simons/projects/boost-current/boost/function/function_base.hpp
test.o: /home/simons/projects/boost-current/boost/config.hpp
test.o: /home/simons/projects/boost-current/boost/config/user.hpp
test.o: /home/simons/projects/boost-current/boost/config/select_compiler_config.hpp
test.o: /home/simons/projects/boost-current/boost/config/compiler/gcc.hpp
test.o: /home/simons/projects/boost-current/boost/config/select_stdlib_config.hpp
test.o: /home/simons/projects/boost-current/boost/config/select_platform_config.hpp
test.o: /home/simons/projects/boost-current/boost/config/suffix.hpp
test.o: /home/simons/projects/boost-current/boost/type_traits.hpp
test.o: /home/simons/projects/boost-current/boost/type_traits/fwd.hpp
test.o: /home/simons/projects/boost-current/boost/type_traits/ice.hpp
test.o: /home/simons/projects/boost-current/boost/type_traits/conversion_traits.hpp
test.o: /home/simons/projects/boost-current/boost/type_traits/arithmetic_traits.hpp
test.o: /home/simons/projects/boost-current/boost/type_traits/cv_traits.hpp
test.o: /home/simons/projects/boost-current/boost/type_traits/reference_traits.hpp
test.o: /home/simons/projects/boost-current/boost/type_traits/utility.hpp
test.o: /home/simons/projects/boost-current/boost/type_traits/array_traits.hpp
test.o: /home/simons/projects/boost-current/boost/type_traits/composite_traits.hpp
test.o: /home/simons/projects/boost-current/boost/type_traits/transform_traits.hpp
test.o: /home/simons/projects/boost-current/boost/type_traits/is_class.hpp
test.o: /home/simons/projects/boost-current/boost/type_traits/alignment_traits.hpp
test.o: /home/simons/projects/boost-current/boost/preprocessor/list/for_each_i.hpp
test.o: /home/simons/projects/boost-current/boost/preprocessor/list/adt.hpp
test.o: /home/simons/projects/boost-current/boost/preprocessor/tuple/elem.hpp
test.o: /home/simons/projects/boost-current/boost/preprocessor/logical/not.hpp
test.o: /home/simons/projects/boost-current/boost/preprocessor/logical/nor.hpp
test.o: /home/simons/projects/boost-current/boost/preprocessor/logical/bool.hpp
test.o: /home/simons/projects/boost-current/boost/preprocessor/inc.hpp
test.o: /home/simons/projects/boost-current/boost/preprocessor/for.hpp
test.o: /home/simons/projects/boost-current/boost/preprocessor/if.hpp
test.o: /home/simons/projects/boost-current/boost/preprocessor/tuple/eat.hpp
test.o: /home/simons/projects/boost-current/boost/preprocessor/tuple/to_list.hpp
test.o: /home/simons/projects/boost-current/boost/preprocessor/cat.hpp
test.o: /home/simons/projects/boost-current/boost/static_assert.hpp
test.o: /home/simons/projects/boost-current/boost/config.hpp
test.o: /home/simons/projects/boost-current/boost/type_traits/object_traits.hpp
test.o: /home/simons/projects/boost-current/boost/type_traits/function_traits.hpp
test.o: /home/simons/projects/boost-current/boost/type_traits/same_traits.hpp
test.o: /home/simons/projects/boost-current/boost/ref.hpp
test.o: /home/simons/projects/boost-current/boost/utility/addressof.hpp
test.o: /home/simons/projects/boost-current/boost/function/function0.hpp
test.o: /home/simons/projects/boost-current/boost/function/function_template.hpp
test.o: /home/simons/projects/boost-current/boost/mem_fn.hpp
test.o: /home/simons/projects/boost-current/boost/bind/mem_fn_template.hpp
test.o: /home/simons/projects/boost-current/boost/bind/mem_fn_cc.hpp
test.o: /home/simons/projects/boost-current/boost/function/function1.hpp
test.o: /home/simons/projects/boost-current/boost/function/function2.hpp
test.o: /home/simons/projects/boost-current/boost/function/function3.hpp
test.o: /home/simons/projects/boost-current/boost/function/function4.hpp
test.o: /home/simons/projects/boost-current/boost/function/function5.hpp
test.o: /home/simons/projects/boost-current/boost/function/function6.hpp
test.o: /home/simons/projects/boost-current/boost/function/function7.hpp
test.o: /home/simons/projects/boost-current/boost/function/function8.hpp
test.o: /home/simons/projects/boost-current/boost/function/function9.hpp
test.o: /home/simons/projects/boost-current/boost/function/function10.hpp
test.o: /home/simons/projects/boost-current/boost/lambda/lambda.hpp
test.o: /home/simons/projects/boost-current/boost/lambda/core.hpp
test.o: /home/simons/projects/boost-current/boost/tuple/tuple.hpp
test.o: /home/simons/projects/boost-current/boost/tuple/detail/tuple_basic.hpp
test.o: /home/simons/projects/boost-current/boost/lambda/detail/lambda_config.hpp
test.o: /home/simons/projects/boost-current/boost/lambda/detail/lambda_fwd.hpp
test.o: /home/simons/projects/boost-current/boost/lambda/detail/arity_code.hpp
test.o: /home/simons/projects/boost-current/boost/lambda/detail/actions.hpp
test.o: /home/simons/projects/boost-current/boost/lambda/detail/lambda_traits.hpp
test.o: /home/simons/projects/boost-current/boost/lambda/detail/function_adaptors.hpp
test.o: /home/simons/projects/boost-current/boost/lambda/detail/return_type_traits.hpp
test.o: /home/simons/projects/boost-current/boost/lambda/detail/select_functions.hpp
test.o: /home/simons/projects/boost-current/boost/lambda/detail/lambda_functor_base.hpp
test.o: /home/simons/projects/boost-current/boost/lambda/detail/lambda_functors.hpp
test.o: /home/simons/projects/boost-current/boost/lambda/detail/ret.hpp
test.o: /home/simons/projects/boost-current/boost/lambda/detail/operator_actions.hpp
test.o: /home/simons/projects/boost-current/boost/lambda/detail/operator_lambda_func_base.hpp
test.o: /home/simons/projects/boost-current/boost/lambda/detail/operator_return_type_traits.hpp
test.o: /home/simons/projects/boost-current/boost/lambda/detail/is_instance_of.hpp
test.o: /home/simons/projects/boost-current/boost/preprocessor/enum_shifted_params.hpp
test.o: /home/simons/projects/boost-current/boost/preprocessor/enum_shifted.hpp
test.o: /home/simons/projects/boost-current/boost/preprocessor/enum.hpp
test.o: /home/simons/projects/boost-current/boost/preprocessor/comma_if.hpp
test.o: /home/simons/projects/boost-current/boost/preprocessor/comma.hpp
test.o: /home/simons/projects/boost-current/boost/preprocessor/empty.hpp
test.o: /home/simons/projects/boost-current/boost/preprocessor/repeat.hpp
test.o: /home/simons/projects/boost-current/boost/preprocessor/tuple.hpp
test.o: /home/simons/projects/boost-current/boost/preprocessor/tuple/reverse.hpp
test.o: /home/simons/projects/boost-current/boost/preprocessor/dec.hpp
test.o: /home/simons/projects/boost-current/boost/preprocessor/repeat_2nd.hpp
test.o: /home/simons/projects/boost-current/boost/lambda/detail/operators.hpp
test.o: /home/simons/projects/boost-current/boost/lambda/bind.hpp
test.o: /home/simons/projects/boost-current/boost/lambda/detail/bind_functions.hpp
test.o: /home/simons/projects/boost-current/boost/test/test_tools.hpp
test.o: /home/simons/projects/boost-current/boost/test/cpp_main.cpp
test.o: /home/simons/projects/boost-current/boost/detail/catch_exceptions.hpp
test.o: /home/simons/projects/boost-current/boost/cstdlib.hpp
test.o: /home/simons/projects/boost-current/boost/test/test_main.cpp
