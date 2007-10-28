// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_PHOENIX_COMPONENT_PRODUCER_CONTAINER_HPP
#define BOOST_DATAFLOW_PHOENIX_COMPONENT_PRODUCER_CONTAINER_HPP

#include <boost/dataflow/phoenix/component/producer_wrapper.hpp>

#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/iterator/zip_iterator.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/fusion/adapted/boost_tuple.hpp>
#include <boost/fusion/view/joint_view.hpp>

namespace boost { namespace phoenix {
    
namespace detail
{
    template<typename Iterator>
    struct applicator_incrementer
    {
        applicator_incrementer(Iterator &it) : it(it) {}
        
        template<typename F>
        void operator()(F t)
        {
            wrap_producer<F>(*it)(it);
            it++;
        }
        
        Iterator &it;
    };
    
    template<typename Iterator, typename Iterator2>
    struct applicator_incrementer2
    {
        applicator_incrementer2(Iterator &it, Iterator2 &it2) : it(it), it2(it2) {}
        
        template<typename F>
        void operator()(F f)
        {
            wrap_producer<F>(*it)(it, it2);
            it++;
            it2++;
        }
        
        Iterator &it;
        Iterator2 &it2;
    };
    
    template<typename Iterator, typename Iterator2, typename Iterator3>
    struct applicator_incrementer3
    {
        applicator_incrementer3(Iterator &it, Iterator2 &it2, Iterator3 &it3) : it(it), it2(it2), it3(it3) {}
        
        template<typename F>
        void operator()(F f)
        {
            wrap_producer<F>(*it)(it, it2, it3);
            it++;
            it2++;
            it3++;
        }
        
        Iterator &it;
        Iterator2 &it2;
        Iterator2 &it3;
    };
    
}

template<typename Container, typename F, typename FFirst = mpl::vector0<> >
class producer_container : public Container
{
public:
    struct eval_iterator : public boost::iterator_adaptor<
        typename producer_container<Container,F,FFirst>::eval_iterator,
        typename Container::iterator>
    {
        eval_iterator() {}
        explicit eval_iterator(const typename Container::iterator &i)
        : eval_iterator::iterator_adaptor_(i) {}
        
        typename eval_iterator::reference dereference() const
        {
            typename Container::iterator it = this->base_reference();
            wrap_producer<F>(*it)(it);
            return *it;
        }

    private:
        friend class boost::iterator_core_access;
    };
    
    producer_container() {}
    
    template<typename T1>
    producer_container(const T1& t1) : Container(t1) {}
    
    eval_iterator eval_begin() { return eval_iterator(Container::begin()); }
    eval_iterator eval_end() { return eval_iterator(Container::end()); }
    
    void eval()
    {
        typename Container::iterator it=Container::begin();
        boost::mpl::for_each<FFirst>(detail::applicator_incrementer<typename Container::iterator>(it));
        for ( ; it!=Container::end(); it++)
            wrap_producer<F>(*it)(it);
    }
    template<typename Container2>
    void eval(Container2 &c2)
    {
        typename Container::iterator it=Container::begin();
        typename Container2::iterator it2=c2.begin();

        boost::mpl::for_each<FFirst>(detail::applicator_incrementer2<typename Container::iterator, typename Container2::iterator>(it, it2));
        for ( ; it!=Container::end(); it++, it2++)
            wrap_producer<F>(*it)(it, it2);
    }
    template<typename Container2, typename Container3>
    void eval(Container2 &c2, Container3 &c3)
    {
            typename Container::iterator it=Container::begin();
            typename Container2::iterator it2=c2.begin();
            typename Container3::iterator it3=c3.begin();
            
            boost::mpl::for_each<FFirst>(detail::applicator_incrementer3<
                typename Container::iterator,
                typename Container2::iterator,
                typename Container3::iterator >(it, it2, it3));
            for ( ; it!=Container::end(); it++, it2++, it3++)
                wrap_producer<F>(*it)(it, it2, it3);
    }
};

} } // namespace boost::phoenix

#endif // BOOST_DATAFLOW_PHOENIX_COMPONENT_PRODUCER_WRAPPER_HPP