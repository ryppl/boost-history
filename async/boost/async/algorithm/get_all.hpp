//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/async for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_ASYNC_GET_ALL__HPP
#define BOOST_ASYNC_GET_ALL__HPP

#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/zip_view.hpp>
#include <boost/fusion/include/transform.hpp>
#include <boost/fusion/include/as_vector.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_const.hpp>

#include <boost/async/algorithm/get.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace async {
    namespace fct {
        struct get {
            template <typename Sig>
            struct result;

            template <typename T>
            struct result<get(T)>
            {
                typedef typename boost::remove_const<typename boost::remove_reference<T>::type>::type ACT;
                typedef typename act_traits<ACT>::move_dest_type type;
            };

            template <typename ACT>
            typename ACT::result_type operator()(ACT& act) const {
                return typename ACT::result_type();
            }
        };

        struct set {
            template<typename Sig>
            struct result;

            template<typename T>
            struct result<set(T)>
            {
                typedef void type;
            };

            template<typename ACT_VAL>
            void operator()(ACT_VAL act_val) const {
                fusion::at_c<1>(act_val)=async::get(fusion::at_c<0>(act_val));
            }
        };
    }

    namespace result_of {
        template <typename Sequence>
        struct set_all {
            typedef void type;
        };

        template <typename Sequence>
        struct get_all {
            typedef typename fusion::result_of::as_vector<
                typename fusion::result_of::transform<Sequence const, fct::get>::type
            >::type type;
        };
    }

    template <typename SequenceHandles, typename SequenceValues>
    void set_all(SequenceHandles& handles, SequenceValues& values) {
        typedef fusion::vector<SequenceHandles&, SequenceValues&> sequences;
        sequences seqs(handles, values);
        fusion::zip_view<sequences> zip(seqs);
        fusion::for_each(zip, fct::set());
    }

#if 0
namespace detail {
        template<int N>
    struct get_all_unrolled
    {
        template<typename I0>
        static void call(I0& i0)
        {
            fct::get()(*i0);
            typedef typename fusion::result_of::next<I0>::type I1;
            I1 i1(fusion::next(i0));
            fct::get()(*i1);
            typedef typename fusion::result_of::next<I1>::type I2;
            I2 i2(fusion::next(i1));
            fct::get()(*i2);
            typedef typename fusion::result_of::next<I2>::type I3;
            I3 i3(fusion::next(i2));
            fct::get()(*i3);
            return get_all_unrolled<N-4>::call(fusion::next(i3), abs_time);
        }
    };

    template<>
    struct get_all_unrolled<3>
    {
        template<typename I0>
        static bool call(I0& i0)
        {
        }
    };

    template<>
    struct get_all_unrolled<2>
    {
        template<typename I0>
        static bool call(I0& i0)
        {
        }
    };

    template<>
    struct get_all_unrolled<1>
    {
        template<typename I0>
        static bool call(I0& i0)
        {
            return fct::get()(*i0);
        }
    };

    template<>
    struct get_all_unrolled<0>
    {
        template<typename It>
        static bool call(It const&, const system_time&)
        {
            return true;
        }
    };

    template <typename Sequence>
    inline typename result_of::template get_all<Sequence>::type
    get_all(Sequence& seq, const system_time& abs_time, fusion::random_access_traversal_tag)
    {
        typedef typename fusion::result_of::begin<Sequence>::type begin;
        typedef typename fusion::result_of::end<Sequence>::type end;
        return get_all_unrolled<fusion::result_of::distance<begin, end>::type::value>::call(fusion::begin(seq), abs_time);
    }


}
#endif

    template <typename Sequence>
    typename result_of::template get_all<Sequence>::type
    get_all(Sequence& seq) {
#if 0
        return detail::get_all(seq, abs_time, typename fusion::traits::category_of<Sequence>::type());
#else
        typename result_of::template get_all<Sequence>::type res=fusion::as_vector(fusion::transform(seq, fct::get()));
        set_all(seq, res);
        return res;
#endif
    }

}
}   // namespace boost

#include <boost/config/abi_suffix.hpp>

#endif
