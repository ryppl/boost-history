// 
// Boost.Process 
// ~~~~~~~~~~~~~ 
// 
// Copyright (c) 2006, 2007 Julio M. Merino Vidal 
// Copyright (c) 2008, 2009 Boris Schaeling
// Copyright (c) 2010 Felipe Tanus, Boris Schaeling
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying 
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt) 
// 

#include "child.hpp" 
#include <boost/mpl/vector.hpp> 
#include <boost/mpl/front.hpp> 
#include <boost/mpl/back.hpp> 
#include <boost/mpl/at.hpp> 
#include <boost/mpl/int.hpp> 
#include <vector> 
#include <string> 

class launcher 
{ 
public: 
    bp::child operator()(const std::vector<std::string> args, 
               bp::context ctx, 
               boost::shared_ptr<bpb::stream> bstdin = bpb::close::def(), 
               boost::shared_ptr<bpb::stream> bstdout = bpb::close::def(), 
               boost::shared_ptr<bpb::stream> bstderr = bpb::close::def()) 
        const 
    { 
        ctx.stdin_behavior = bstdin; 
        ctx.stdout_behavior = bstdout; 
        ctx.stderr_behavior = bstderr; 
        return bp::create_child(get_helpers_path(), args, ctx); 
    } 
}; 

namespace mpl = boost::mpl; 

typedef mpl::vector< 
    mpl::vector<launcher, bp::context, bp::child> 
> test_types; 

BOOST_AUTO_TEST_CASE_TEMPLATE(child_test_case, T, test_types) 
{ 
    check_helpers(); 

    typedef mpl::front<T>::type Launcher; 
    typedef mpl::at<T, mpl::int_<1> >::type Context; 
    typedef mpl::back<T>::type Child; 

    using namespace child_test; 
    test_close_stdin<Launcher, Context, Child>(); 
    test_close_stdout<Launcher, Context, Child>(); 
    test_close_stderr<Launcher, Context, Child>(); 
    test_stdout<Launcher, Context, Child>(); 
    test_stderr<Launcher, Context, Child>(); 
    // test_redirect_err_to_out<Launcher, Context, Child>(); 
    test_input<Launcher, Context, Child>(); 
    test_work_directory<Launcher, Context, Child>(); 
    test_clear_environment<Launcher, Context, Child>(); 
    test_unset_environment<Launcher, Context, Child>(); 
    test_set_environment<Launcher, Context, Child>(); 
} 
