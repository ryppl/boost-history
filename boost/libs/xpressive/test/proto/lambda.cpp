///////////////////////////////////////////////////////////////////////////////
// lambda.hpp
//
//  Copyright 2006 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <sstream>
#include <boost/mpl/if.hpp>
#include <boost/mpl/print.hpp>
#include <boost/fusion/tuple.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/xpressive/proto/proto.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

using namespace boost;

struct lambda_domain {};
template<int I> struct placeholder {};

template<typename Tuple>
struct lambda_context;

template<typename T> T make();

template<typename T>
char check_reference(T &);

template<typename T>
char (&check_reference(T const &))[2];

template<typename T>
struct param
  : add_reference<typename add_const<T>::type>
{};

template<typename Tuple>
struct lambda_context_result
{
    typedef lambda_context<Tuple> ctx_type;

    template<typename Sig>
    struct result;

    template<typename This, typename Arg>
    struct result<This(proto::tag::terminal, Arg &)>
    {
        typedef Arg &type;
    };

    template<typename This, int I>
    struct result<This(proto::tag::terminal, placeholder<I> const &)>
    {
        typedef typename fusion::result_of::value_at_c<Tuple, I>::type type;
    };

#define UN_OP_RESULT(Op, Arg)\
    typedef typename proto::meta::eval<Arg, ctx_type>::type arg_type;\
    BOOST_TYPEOF_NESTED_TYPEDEF_TPL(nested, Op ::make<arg_type>())\
    typedef typename mpl::if_c<\
        1==sizeof(check_reference(Op ::make<arg_type>()))\
      , typename nested::type &\
      , typename nested::type\
    >::type type;\
    static type call(typename param<arg_type>::type arg)\
    {\
        return Op arg;\
    }\
    /**/

#define BIN_OP_RESULT(Left, Op, Right)\
    typedef typename proto::meta::eval<Left, ctx_type>::type left_type;\
    typedef typename proto::meta::eval<Right, ctx_type>::type right_type;\
    BOOST_TYPEOF_NESTED_TYPEDEF_TPL(nested, ::make<left_type>() Op ::make<right_type>())\
    typedef typename mpl::if_c<\
        1==sizeof(check_reference(::make<left_type>() Op ::make<right_type>()))\
      , typename nested::type &\
      , typename nested::type\
    >::type type;\
    static type call(typename param<left_type>::type left, typename param<right_type>::type right)\
    {\
        return left Op right;\
    }\
    /**/

    template<typename This, typename Arg>
    struct result<This(proto::tag::unary_minus, Arg &)>
    {
        UN_OP_RESULT(-, Arg)
    };

    template<typename This, typename Left, typename Right>
    struct result<This(proto::tag::add, Left &, Right &)>
    {
        BIN_OP_RESULT(Left, +, Right)
    };

    template<typename This, typename Left, typename Right>
    struct result<This(proto::tag::subtract, Left &, Right &)>
    {
        BIN_OP_RESULT(Left, -, Right)
    };

    template<typename This, typename Left, typename Right>
    struct result<This(proto::tag::multiply, Left &, Right &)>
    {
        BIN_OP_RESULT(Left, *, Right)
    };

    template<typename This, typename Left, typename Right>
    struct result<This(proto::tag::divide, Left &, Right &)>
    {
        BIN_OP_RESULT(Left, /, Right)
    };

    template<typename This, typename Left, typename Right>
    struct result<This(proto::tag::left_shift, Left &, Right &)>
    {
        BIN_OP_RESULT(Left, <<, Right)
    };

    template<typename This, typename Left, typename Right>
    struct result<This(proto::tag::right_shift, Left &, Right &)>
    {
        BIN_OP_RESULT(Left, >>, Right)
    };
};

template<typename Tuple>
struct lambda_context
  : lambda_context_result<Tuple>
{
    typedef lambda_context<Tuple> this_type;
    typedef lambda_context_result<Tuple> base_type;
    template<typename Sig> struct result_ : base_type::template result<Sig> {};

    lambda_context(Tuple const &args)
      : args_(args)
    {}

    template<typename Arg>
    Arg &
    operator()(proto::tag::terminal, Arg &arg)
    {
        return arg;
    }

    template<int I>
    typename fusion::result_of::at_c<Tuple, I>::type
    operator()(proto::tag::terminal, placeholder<I>)
    {
        return fusion::get<I>(this->args_);
    }

    template<typename Tag, typename Arg>
    typename result_<this_type(Tag, Arg &)>::type
    operator()(Tag, Arg &arg)
    {
        return result_<this_type(Tag, Arg &)>::call(arg.eval(*this));
    }

    template<typename Tag, typename Left, typename Right>
    typename result_<this_type(Tag, Left &, Right &)>::type
    operator()(Tag, Left &left, Right &right)
    {
        return result_<this_type(Tag, Left &, Right &)>::call(left.eval(*this), right.eval(*this));
    }

private:
    Tuple args_;
};

template<typename T>
struct lambda
  : proto::extends<T, lambda<T>, lambda_domain>
{
    typedef proto::extends<T, lambda<T>, lambda_domain> base_type;
    
    lambda()
      : base_type()
    {}

    lambda(T const &t)
      : base_type(t)
    {}

    using base_type::operator =;

    // hide base_type::operator() by defining our own which
    // evaluates the lambda expression.
    template<typename A0>
    typename proto::meta::eval<T, lambda_context<fusion::tuple<A0 const &> > >::type
    operator()(A0 const &a0) const
    {
        fusion::tuple<A0 const &> args(a0);
        lambda_context<fusion::tuple<A0 const &> > ctx(args);
        return this->eval(ctx);
    }

    template<typename A0, typename A1>
    typename proto::meta::eval<T, lambda_context<fusion::tuple<A0 const &, A1 const &> > >::type
    operator()(A0 const &a0, A1 const &a1) const
    {
        fusion::tuple<A0 const &, A1 const &> args(a0, a1);
        lambda_context<fusion::tuple<A0 const &, A1 const &> > ctx(args);
        return this->eval(ctx);
    }
};

namespace boost { namespace proto { namespace meta
{
    template<typename Expr, typename Tag>
    struct generate<lambda_domain, Expr, Tag>
    {
        typedef lambda<Expr> type;
        static type make(Expr const &expr)
        {
            return lambda<Expr>(expr);
        }
    };
}}}

lambda<proto::meta::terminal<placeholder<0> >::type> const _1;
lambda<proto::meta::terminal<placeholder<1> >::type> const _2;

void test_lambda()
{
    BOOST_CHECK_EQUAL(11, ( (_1 + 2) / 4 )(42));
    BOOST_CHECK_EQUAL(-11, ( (-(_1 + 2)) / 4 )(42));
    BOOST_CHECK_CLOSE(2.58, ( (4 - _2) * 3 )(42, 3.14), 0.1);

    std::stringstream sout;
    (sout << _1 << " -- " << _2)(42, "Life, the Universe and Everything!");
    BOOST_CHECK_EQUAL("42 -- Life, the Universe and Everything!", sout.str());
}

using namespace unit_test;
///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("test expression template domains");

    test->add(BOOST_TEST_CASE(&test_lambda));

    return test;
}
