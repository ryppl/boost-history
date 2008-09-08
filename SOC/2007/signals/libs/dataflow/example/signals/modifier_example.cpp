// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//[ modifier_example

#include <boost/dataflow/signals/component/modifier.hpp>
#include <boost/dataflow/signals/component/storage.hpp>
#include <boost/dataflow/signals/connection.hpp>

#include <boost/bind.hpp>
#include <boost/fusion/include/transform.hpp>

using namespace boost;

// A somewhat generic multiplier (it will multiply any type with a double).
// We will use this as a transform.
struct multiply
{
    template<typename FArgs>
    struct result;
    
    template<typename F, typename T>
    struct result<F(const T &)>
    {
        typedef T type;
    };
    
    multiply(double factor)
        : m_factor(factor)
    {}
    
    template<typename T>
    T operator()(T t) const
    {
        return static_cast<T>(t * m_factor);
    }
    
    double m_factor;
};

// The implementation of the multiplier component functionality.
template<typename Signature>
struct multiplier_impl
{
    multiplier_impl(double factor)
        : m_factor(factor)
    {}
    
    // This is where the work happens.
    // seq will be a fusion sequence with all of the arguments, and we will
    // return the transformed values (using multiply as the transform).
    template<typename Seq>
    Seq operator()(const Seq &seq)
    {
        return boost::fusion::transform(seq, multiply(m_factor));
    }
protected:
    double m_factor;
};

// The actual component will inherit from modifier.
template<typename Signature> 
class multiplier
    : public boost::signals::modifier<
        // the first parameter is the derived class.
        multiplier<Signature>,
        // the second parameter is the implementation class.
        multiplier_impl<Signature>,
        // the fourth parameter determines the signal Signature the component
        // consumes and produces.
        Signature>
{
public:
    typedef boost::signals::modifier<multiplier<Signature>, multiplier_impl<Signature>, Signature>
        base_type;

    // the constructor parameter will be forwarded to the implementation class
    multiplier(double multiplier)
        : base_type(multiplier)
    {}
};

int main()
{
    {
        signals::storage<void (int, double)> result;
        multiplier<void (int, double)> multiply_by_3 (3);
        
        // result will store the result of the multiplication
        multiply_by_3 >>= result;
        multiply_by_3(1, 1.5);
        
        BOOST_ASSERT(result.at<0>() == 3);
        BOOST_ASSERT(result.at<1>() == 4.5);
    }
    {
        signals::storage<void (float, int, double)> result;
        multiplier<void (float, int, double)> multiply_by_half (0.5);
        
        // result will store the result of the multiplication
        multiply_by_half >>= result;
        multiply_by_half(1.0f, 9, 9.0);
        
        BOOST_ASSERT(result.at<0>() == 0.5f);
        BOOST_ASSERT(result.at<1>() == 4);
        BOOST_ASSERT(result.at<2>() == 4.5);
    }
    
    return 0;
}

//]