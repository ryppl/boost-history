#include <libs/assign/v2/test/anon/anon.h>
#include <libs/assign/v2/test/chain/chain.h>
#include <libs/assign/v2/test/detail/functor.h>
#include <libs/assign/v2/test/put/put_wrapper.h>
#include <libs/assign/v2/test/put_range/convert.h>
#include <libs/assign/v2/test/type_traits/has_push.h>
#include <libs/assign/v2/test/type_traits/has_value_type.h>
#include <libs/assign/v2/test/type_traits/has_static_size.h>
#include <libs/assign/v2/test/ref/csv_anon.h>
#include <libs/assign/v2/test/ref/ext_anon.h>
#include <libs/assign/v2/test/ref/fusion_list.h>
#include <libs/assign/v2/test/ref/static_list.h>
#include <libs/assign/v2/test/ref/wrapper.h>

#include <libs/assign/v2/example/tutorial.h>

int main (int argc, char * const argv[]) {

	using namespace test_assign_v2;
    xxx_anon::xxx_anon::test();
    xxx_chain::xxx_chain::test();
    xxx_detail::xxx_functor::test();
    xxx_type_traits::xxx_has_value_type::test();
    xxx_type_traits::xxx_has_static_size::test();
    xxx_ref::xxx_fusion_list::test();
    xxx_ref::xxx_static_list::test();
    xxx_ref::xxx_ext_anon::test();
    xxx_ref::xxx_csv_anon::test();
    xxx_ref::xxx_wrapper::test();
    xxx_put::xxx_put_wrapper::test();
    xxx_put_range::xxx_convert::test();
	
    {
	    using namespace example_assign_v2;
	    xxx_tutorial::run();
    }
    return 0;

}
