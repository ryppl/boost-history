///////////////////////////////////////////////////////////////////////////////
//  cycle_through_shared_containers.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_CYCLE_THROUGH_SHARED_CONTAINERS_HPP_ER_2009
#define BOOST_CYCLE_THROUGH_SHARED_CONTAINERS_HPP_ER_2009
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <functional>
#include <iterator>
#include <boost/mpl/assert.hpp>
#include <boost/assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/generator/shared_container.hpp>
#include <boost/generator/keyword.hpp>
#include <boost/iterator/cycle_iterator.hpp>//in the vault
namespace boost{
namespace generator{

    //DODO kwd

    /// \class cycle_through_shared_containers
    /// \brief Useful for experimenal design (fixed pool of covariates)
    template<
        typename Cont = const std::vector<double>,
        typename Id = mpl::void_
    >
    class cycle_through_shared_containers{
    public:
        typedef shared_container<Cont>                      shared_t;
        typedef std::vector<shared_t>                       collec_shared_t;
    private:
        typedef typename range_size<collec_shared_t>::type  size_type;
        typedef typename
            range_iterator<collec_shared_t>::type           base_iter_t;
        typedef cycle_iterator<base_iter_t,size_type>       cycle_t;
    public:
        typedef shared_t                                    result_type;

        template<typename R>
        cycle_through_shared_containers(const R& r,size_type offset)
        {
            set(
                r,
                offset
            );
        }
        template<typename ArgumentPack>
        cycle_through_shared_containers(const ArgumentPack& args)
        {
            typedef kwd<Id> kwd;
            set(
                args[kwd::range_containers],
                args[kwd::range_containers_offset]
            );
        }

        cycle_through_shared_containers(
            const cycle_through_shared_containers& that)
        {
            set(that.collec_shared_,that.offset());
        }

        cycle_through_shared_containers&
        operator=(const cycle_through_shared_containers& that)
        {
            if(&that!=this){
                set(that.collec_shared_,that.offset());
            }
            return *this;
        }

        template<typename R>
        void set(const R& range_of_ranges,size_type offset = 0){
            //range_of_ranges either ranges or shared containers;
            collec_shared_.clear();

            copy(
                begin(range_of_ranges),
                end(range_of_ranges),
                back_inserter(collec_shared_)
            );
            cycle_ = make_cycle_iterator(
                begin(collec_shared_),
                end(collec_shared_),
                offset
            );
        }

        size_type offset()const { return cycle_.offset(); }

        result_type operator()(){
            shared_t result = (*cycle_);
            std::advance(cycle_,1);
            return result;
        };

    private:
        cycle_through_shared_containers();
        collec_shared_t collec_shared_;
        cycle_t cycle_;
    };

}
}

#endif
