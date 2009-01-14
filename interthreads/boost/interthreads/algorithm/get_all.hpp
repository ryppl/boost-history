//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-20009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interthreads for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERTHREADS_GET_ALL__HPP
#define BOOST_INTERTHREADS_GET_ALL__HPP

#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/zip_view.hpp>
#include <boost/fusion/include/transform.hpp>
#include <boost/mpl/transform.hpp>
#include  <boost/type_traits/remove_reference.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace interthreads {
    namespace fct {
        struct get {
            template<typename Sig>
            struct result;

            template<typename T>
            struct result<get(T)>
            {
                typedef typename boost::remove_reference<T>::type ACT;
                typedef typename ACT::result_type type;
            };
            
            template<typename ACT>
            typename ACT::result_type operator()(ACT& act) const {
                return act.get();
            }
        };
        struct intrusive_get {
            template<typename Sig>
            struct result;

            template<typename T>
            struct result<intrusive_get(T)>
            {
                typedef void type;
            };
            
            template<typename ACT_VAL>
            void operator()(ACT_VAL act_val) const {
                fusion::at_c<1>(act_val)=fusion::at_c<0>(act_val).get();
            }
        };
    }


    namespace result_of {
        template <typename Sequence>
        struct get_all {
            typedef typename fusion::result_of::transform<Sequence const, fct::get>::type type;
        };
    }
    template <typename Sequence>
    typename result_of::get_all<Sequence>::type
    get_all(Sequence& t) {
        return fusion::transform(t, fct::get());
    }

    namespace result_of {
        template <typename Sequence>
        struct set_all {
            typedef void type;
        };
    }
    template <typename SequenceHandles, typename SequenceValues>
    void set_all(SequenceHandles& handles, SequenceValues& values) {
        typedef fusion::vector<SequenceHandles&, SequenceValues&> sequences;
        sequences seqs(handles, values);
        fusion::zip_view<sequences> zip(seqs);
        fusion::for_each(zip, fct::intrusive_get());
    }
}    
}   // namespace boost

#include <boost/config/abi_suffix.hpp>

#endif
