
#include <iostream>

 

#include <boost/type_traits/is_same.hpp>
#include <boost/static_assert.hpp>
#include <boost/detail/lightweight_test.hpp>


#include <boost/overload.hpp>

#include "test/functors.hpp"


namespace ovld = boost::overloads;
namespace ovdtl = boost::overloads::detail;
using boost::is_same;




#include "test/trait_by_index.hpp"
#include "test/trait_by_signature.hpp"
#include "test/trait_by_functor.hpp"
#include "test/signature_deduction.hpp"
#include "test/idx_and_sig_methods.hpp"
#include "test/free_and_member_functions.hpp"
#include "test/signature_with_ptr_ref.hpp"

int main()
{
    test05();
    test06();
    test07();
    test08();
    return boost::report_errors();
}

