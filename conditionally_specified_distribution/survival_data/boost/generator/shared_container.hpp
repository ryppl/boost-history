///////////////////////////////////////////////////////////////////////////////
// shared_container.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_SHARED_CONTAINER_HPP_ER2009
#define BOOST_SHARED_CONTAINER_HPP_ER2009
#include <algorithm>
#include <iterator>
#include <vector>
#include <boost/range.hpp>
#include <boost/shared_ptr.hpp>
//#include <boost/call_traits.hpp>

namespace boost{

    /// \class shared_container
    /// \brief Pter to a container.
    ///
    /// Can be used as a container shared across data units
    /// (experimentatl design),
    /// or to make copying a dataset cheap.
    /// Cont models Back Insertion Sequence
    template<typename Cont = const std::vector<double> >
    class shared_container{
     public:
        typedef Cont                                          container_type;
     private:
        typedef boost::shared_ptr<container_type>             pimpl_type;
        typedef typename range_iterator<container_type>::type iter_type;
     public:
        typedef typename range_size<Cont>::type               size_type;
        typedef typename iterator_value<iter_type>::type      value_type;
        typedef const container_type&                         result_type;

        template<typename R>
        shared_container(const R& container_)
        {
            container_type tmp;
            copy(
                begin(container_),
                end(container_),
                std::back_inserter(tmp)
            );
            pimpl = pimpl_type(new container_type(tmp));
        }
        shared_container(const shared_container& that):pimpl(that.pimpl){}
        shared_container& operator=(const shared_container& that){
            if(&that!=this){
                pimpl = that.pimpl;
            }
            return *this;
        }
        result_type operator()()const{
            return dereference();
        }
    private:
        result_type dereference()const{
            return (*pimpl);
        }
        pimpl_type pimpl;
    };

}

#endif // SHARED_CONTAINER_HPP_INCLUDED
