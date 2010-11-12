#include <iostream>

//#include <libs/assign/v2/test/anon/anon.h>

//#include <libs/assign/v2/test/chain/chain.h>
//#include <libs/assign/v2/test/chain/convert.h>

//#include <libs/assign/v2/test/detail/functor.h>

//#include <libs/assign/v2/test/put/modulo_op.h>
//#include <libs/assign/v2/test/put/ptr.h>
//#include <libs/assign/v2/test/put/static.h>
//#include <libs/assign/v2/test/put/stl.h>
//#include <libs/assign/v2/test/put/tuple_refs.h>
//#include <libs/assign/v2/test/put/compose/common/forward_parameter.h>
//#include <libs/assign/v2/test/put/compose/functor/composer.h>
//#include <libs/assign/v2/test/put/compose/functor/forward_args.h>
//#include <libs/assign/v2/test/put/compose/functor/forward_seq_args.h>
//#include <libs/assign/v2/test/put/compose/functor/stl.h>
//#include <libs/assign/v2/test/put/compose/csv/composer.h>
//#include <libs/assign/v2/test/put/compose/csv/stl.h>

//#include <libs/assign/v2/test/put_range/convert.h>

//#include <libs/assign/v2/test/type_traits/has_push.h>
//#include <libs/assign/v2/test/type_traits/has_value_type.h>
//#include <libs/assign/v2/test/type_traits/has_static_size.h>

//#include <libs/assign/v2/test/ref/convert_traits.h>
//#include <libs/assign/v2/test/ref/wrapper.h>
//#include <libs/assign/v2/test/ref/fusion.h>
//#include <libs/assign/v2/test/ref/fusion_matrix.h>
//#include <libs/assign/v2/test/ref/csv_anon.h>
//#include <libs/assign/v2/test/ref/anon.h>

//#include <libs/assign/v2/example/foo.h>
//#include <libs/assign/v2/example/put_adaptor.h>
//#include <libs/assign/v2/example/ref_anon.h>
 //#include <libs/assign/v2/example/anon.h>
//#include <libs/assign/v2/example/put_range.h>
//#include <libs/assign/v2/example/conversion.h>
//#include <libs/assign/v2/example/chain.h>
//#include <libs/assign/v2/example/fusion.h>
//#include <libs/assign/v2/example/experimental.h>

//#include <libs/assign/v2/speed/anon_put_ref.h>

int main (int argc, char * const argv[]) {

	{
		//using namespace test_assign_v2;
    	//xxx_anon::xxx_anon::test();

    	//xxx_chain::xxx_chain::test();
    	//xxx_chain::xxx_convert::test();

    	//xxx_detail::xxx_functor::test();

    	//xxx_put::xxx_modulo_op::test();
    	//xxx_put::xxx_ptr::test();
    	//xxx_put::xxx_static::test();
    	//xxx_put::xxx_stl::test();
    	//xxx_put::xxx_tuple_refs::test();

    	//xxx_put_range::xxx_convert::test();

		//xxx_put::xxx_compose::xxx_common::xxx_forward_parameter::test();
		//xxx_put::xxx_compose::xxx_functor::xxx_composer::test();
		//xxx_put::xxx_compose::xxx_functor::xxx_forward_args::test();
    	//xxx_put::xxx_compose::xxx_functor::xxx_forward_seq_args::test();
    	//xxx_put::xxx_compose::xxx_functor::xxx_stl::test();

    	//xxx_put::xxx_compose::xxx_csv::xxx_composer::test();
    	//xxx_put::xxx_compose::xxx_csv::xxx_stl::test();

    	//xxx_ref::xxx_convert_traits::test();
    	//xxx_ref::xxx_wrapper::test();
    	//xxx_ref::xxx_anon::test();
    	//xxx_ref::xxx_csv_anon::test();
    	//xxx_ref::xxx_fusion::test();
    	//xxx_ref::xxx_fusion_matrix::test();

    	//xxx_type_traits::xxx_has_push::test();
    	//xxx_type_traits::xxx_has_value_type::test();
    	//xxx_type_traits::xxx_has_static_size::test();

	}
    {
        //using namespace example_assign_v2;
        //xxx_put_adaptor::run();
        //xxx_anon::run();
        //xxx_put_range::run();
        //xxx_ref_anon::run();
        //xxx_conversion::run();
        //xxx_chain::run();
        //xxx_fusion::run();
    }
    {
        // using namespace speed_assign_v2;
        // xxx_anon_put_ref::run(std::cout);
    }

    std::cout << "main.cpp" << std::endl;

    return 0;

}
