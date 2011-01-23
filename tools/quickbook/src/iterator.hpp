/*=============================================================================
    Copyright (c) 2010 Daniel James

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_QUICKBOOK_ITERATOR_HPP)
#define BOOST_SPIRIT_QUICKBOOK_ITERATOR_HPP

#include <boost/operators.hpp>
#include <boost/iterator/iterator_traits.hpp>

namespace quickbook
{
    struct file_position
    {
        file_position() : line(0), column(0) {}
        file_position(int l, int c) : line(l), column(c) {}
    
        int line;
        int column;
    };
    
    template <typename Iterator>
    struct position_iterator
        : boost::forward_iterator_helper<
            position_iterator<Iterator>,
            typename boost::iterator_value<Iterator>::type,
            typename boost::iterator_difference<Iterator>::type,
            typename boost::iterator_pointer<Iterator>::type,
            typename boost::iterator_reference<Iterator>::type
        >
    {
        position_iterator() {}
        explicit position_iterator(Iterator base)
            : base_(base), previous_('\0'), position_() {}
        explicit position_iterator(Iterator base, file_position const& position)
            : base_(base), previous_('\0'), position_(position) {}
    
        friend bool operator==(
            position_iterator const& x,
            position_iterator const& y)
        {
            return x.base_ == y.base_;
        }
        
        position_iterator& operator++()
        {
            char val = *base_;
    
            if (val == '\r') {
                ++position_.line;
                position_.column = 0;           
            }
            else if (val == '\n') {
                if (previous_ != '\r') {
                    ++position_.line;
                    position_.column = 0;
                }
            }
            else {
                ++position_.column;
            }
    
            previous_ = val;
            ++base_;

            return *this;
        }
    
        typename boost::iterator_reference<Iterator>::type operator*() const
        {
            return *base_;
        }
        
        file_position const& get_position() const {
            return position_;
        }
    
        Iterator base() const {
            return base_;
        }
    
    private:
        Iterator base_;
        char previous_;
        file_position position_;
    };
}

#endif
