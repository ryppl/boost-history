// Pinhole path_filtered_iterator.hpp file
//
// Copyright Jared McIntyre 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PATH_FILTERED_ITERATOR
#define BOOST_PATH_FILTERED_ITERATOR

#include <string>
#include <boost/../../boost_sandbox/pinhole/boost/pinhole/detail/depth_first_iterator_base.hpp>
#include <boost/../../boost_sandbox/pinhole/boost/pinhole/detail/tokenizer.hpp>

namespace boost { namespace pinhole
{
    namespace detail
    {
        template<class Iterator>
        struct path_filter : depth_first_iterator_base<Iterator, path_filter<Iterator> >
        {
            typedef depth_first_iterator_base<Iterator, path_filter<Iterator> > base_type;

            explicit path_filter( Iterator end ) :
                base_type(end, end)
            {
                current_token = token_path.tokens.begin();
                end_token     = token_path.tokens.end();

                // required call by depth_first_iterator_base
                MoveIteratorToValidPosition();
            }

            explicit path_filter( std::string path, Iterator x, Iterator end ) :
                base_type(x, end)
            {
                process_path( path, token_path );
                current_token = token_path.tokens.begin();
                end_token     = token_path.tokens.end();

                // required call by depth_first_iterator_base
                MoveIteratorToValidPosition();
            }

            path_filter( const path_filter& other ) :
                base_type(other)
            {
                token_path = other.token_path;
                current_token = token_path.tokens.begin();
                detail::token_path::token_list::difference_type position = std::distance(other.token_path.tokens.begin(), other.current_token);
                for( ; position != 0; --position )
                {
                    ++current_token;
                }
                end_token = token_path.tokens.end();
            }

            inline bool validity_test(property_group* property)
            {
                return  current_token->name == property->get_name() &&
                        (true == current_token->property.empty() || 
                            (property->is_valid_property(current_token->property) && current_token->value == property->get_as_string(current_token->property)) );
            }

            inline bool visit_children(property_group* property)
            {
                if( false == validity_test(property) || ++detail::token_path::const_iterator(current_token) == end_token)
                {
                    return false;
                }
                    
                ++current_token;
                    
                return true;
            }

            inline bool is_valid(property_group* property)
            {
                // Looking for only leaf nodes
                return ++detail::token_path::const_iterator(current_token) == end_token && validity_test(property);
            }

            inline void move_up()
            {
                --current_token;
            }

        private:

            path_filter() :
                base_type() { }

            detail::token_path token_path;
            detail::token_path::const_iterator current_token;
            detail::token_path::const_iterator end_token;
        };
        }

    /** Iterates over all the keys in a map. */
    typedef detail::path_filter<children_collection::iterator> path_filtered_iterator;
    typedef detail::path_filter<children_collection::const_iterator> const_path_filtered_iterator;
}}

#endif // include guard