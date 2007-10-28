// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_PHOENIX_SEQUENCE_ENVIRONMENT_HPP
#define BOOST_DATAFLOW_PHOENIX_SEQUENCE_ENVIRONMENT_HPP

#include <boost/dataflow/detail/make_ref.hpp>

#include <boost/fusion/view/transform_view.hpp>

namespace boost { namespace phoenix {

namespace detail {

    template<class Sequence>
    class environment_wrapper : public Sequence
    {
    public:
        typedef typename Sequence::types args_type;
        typedef boost::fusion::transform_view<const Sequence
            , boost::dataflow::detail::make_ref> tie_type;
        
        tie_type args() const
        {
            return tie_type(*this, boost::dataflow::detail::make_ref());
        }
    };

} // namespace detail

template<typename Sequence, typename Op>
class sequence_environment : public detail::environment_wrapper<
Sequence >
{
public:
    double operator()()
    {
        return Op().eval(*this);
    }
};

} } // namespace boost::phoenix

#endif // BOOST_DATAFLOW_DETAIL_MAKE_REF_HPP
