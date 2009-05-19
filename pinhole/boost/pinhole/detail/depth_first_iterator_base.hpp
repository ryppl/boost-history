// Pinhole depth_first_iterator_base.hpp file
//
// Copyright Jared McIntyre 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DETAIL_DEPTH_FIRST_ITERATOR_BASE
#define BOOST_DETAIL_DEPTH_FIRST_ITERATOR_BASE

#include <exception>
#include <stdexcept>
#include <string>
#include <boost/../../boost_sandbox/pinhole/boost/pinhole/property_group.hpp>

#if defined(BOOST_MSVC)
    #pragma warning(push)
    #pragma warning(disable: 4561 4996)
#endif
#include <boost/iterator.hpp>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/iterator/iterator_categories.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/static_assert.hpp>
#if defined(BOOST_MSVC)
    #pragma warning(pop)
#endif

namespace boost { namespace pinhole { namespace detail
{
    template<class Iterator, class filter>
    class depth_first_iterator_base
        : public boost::iterator_adaptor<
            depth_first_iterator_base<Iterator, filter>
            , Iterator
        >
    {
    private:

        /** Required by boost::iterator_adaptor. */
        friend class boost::iterator_core_access;

        /** Required by boost::iterator_adaptor. */
        struct enabler {}; // a private type avoids misuse

        typedef std::pair<Iterator,Iterator> past_state_type;
        
    public:
        depth_first_iterator_base() :
            depth_first_iterator_base::iterator_adaptor_() { }
        
        explicit depth_first_iterator_base( Iterator x, Iterator end ) :
            depth_first_iterator_base::iterator_adaptor_(x), current_end(end) { }

        depth_first_iterator_base( const depth_first_iterator_base& other ) :
            depth_first_iterator_base::iterator_adaptor_(other)
        {
            stack = other.stack;
            current_end = other.current_end;
        }

        /** Required by boost::iterator_adaptor. */
        template <class other_iterator>
        depth_first_iterator_base(
               category_iterator<other_iterator> const& other
             , typename boost::enable_if<
                   boost::is_convertible<other_iterator*, Iterator*>
                 , enabler
               >::type = enabler()
             )
        : depth_first_iterator_base::iterator_adaptor_(other.base()) {}

    protected:

        void MoveIteratorToValidPosition()
        {
            bool bContinue = false;

            do 
            {
                if( this->base_reference() == current_end )
                {    
                    if( false == stack.empty() )
                    {
                        past_state_type& previous_par = stack.back();

                        this->base_reference() = previous_par.first;
                        current_end = previous_par.second;

                        stack.pop_back();

                        static_cast<filter*>(this)->move_up();
                    }
                }
                else
                {
                    while( false == (*this->base_reference())->get_children_collection().empty() && static_cast<filter*>(this)->visit_children(*this->base_reference()) )
                    {
                        stack.push_back( past_state_type(this->base_reference(), current_end) );
                        
                        boost::pinhole::children_collection& children = (*this->base_reference())->get_children_collection();
                        this->base_reference() = children.begin();
                        current_end = children.end();
                    }
                }

                if( this->base_reference() != current_end && false == static_cast<filter*>(this)->is_valid(*this->base_reference()) )
                {
                    bContinue = true;
                    ++(this->base_reference());
                }
                else
                {
                    bContinue = false;
                }
            } while( bContinue );
        }

    private:

        #if defined(BOOST_MSVC) // work around for extra MS secure library checks
            bool equal(iterator_adaptor_ const& x) const
            {
                return static_cast<const depth_first_iterator_base*>(&x)->stack == stack &&
                       static_cast<const depth_first_iterator_base*>(&x)->base_reference() == this->base_reference();
            }
        #endif

        void increment()
        {
            ++(this->base_reference());

            MoveIteratorToValidPosition();
        }

        std::deque< past_state_type > stack;
        Iterator current_end;
    };
}}}

#endif // include guard