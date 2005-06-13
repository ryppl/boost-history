// (C) Copyright Jonathan Turkanis 2004
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

#ifndef BOOST_POLICY_PTR_TEST_SOURCE_SINK_HPP_INCLUDED
#define BOOST_POLICY_PTR_TEST_SOURCE_SINK_HPP_INCLUDED 

policy_ptr::to<base> base_source() 
{ 
    return policy_ptr::to<base>(new base); 
}

policy_ptr::to<derived> derived_source() 
{ 
    return policy_ptr::to<derived>(new derived); 
}

void base_sink(policy_ptr::to<base>) { }

void derived_sink(policy_ptr::to<derived>) { }

//#ifndef BOOST_POLICY_PTR_NO_ARRAY_SUPPORT //----------------------------------//
//
//policy_ptr::to<base[]> base_array_source() 
//{ 
//    return policy_ptr::to<base[]>(new base[10]); 
//}
//
//policy_ptr::to<derived[]> derived_array_source() 
//{ 
//    return policy_ptr::to<derived[]>(new derived[10]); 
//}
//
//void base_array_sink(policy_ptr::to<base[]>) { }
//
//void derived_array_sink(policy_ptr::to<derived[]>) { }
//
//#endif // #ifndef BOOST_POLICY_PTR_NO_ARRAY_SUPPORT  //-----------------------//

#endif // #ifndef BOOST_POLICY_PTR_TEST_SOURCE_SINK_HPP_INCLUDED
