// Pinhole tokenizer.hpp file
//
// Copyright Jared McIntyre 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PINHOLE_DETAIL_TOKENIZER
#define BOOST_PINHOLE_DETAIL_TOKENIZER

#include <vector>
#include <list>
#include <boost/algorithm/string.hpp>
#include "../exceptions.hpp"
#include"../find.hpp"

namespace boost { namespace pinhole
{
    namespace detail
    {
        struct token_path
        {
            struct token
            {
                std::string name;
                std::string property;
                std::string value;
            };

            typedef std::list<token> token_list;
            typedef token_list::iterator iterator;
            typedef token_list::const_iterator const_iterator;


            token_list tokens;
            bool relative_path;
        };

        inline size_t process_path( const std::string& string_path, boost::pinhole::detail::token_path& processed_path )
        {
            typedef std::vector<std::string> split_vector_type;
            typedef boost::split_iterator<std::string::const_iterator> string_split_iterator;
            typedef boost::iterator_range<std::string::const_iterator> string_range;

            processed_path.relative_path = false;
            processed_path.tokens.clear();

            if( string_path.empty() )
            {
                return 0;
            }

            std::string::const_iterator itr( string_path.begin() );
            std::string::const_iterator itrEnd( string_path.end() );

            if( *itr == '/' )
            {
                ++itr;
            }
            else
            {
                processed_path.relative_path = true;
            }

            size_t itemCount = 0;

            string_range range(itr, itrEnd);
            string_split_iterator splitItr= boost::make_split_iterator( range, boost::first_finder("/", is_equal()));
            while( splitItr!=string_split_iterator() )
            {
                boost::pinhole::detail::token_path::token newToken;
                const std::string strSubToken( copy_range<std::string>(*splitItr) );
                ++splitItr;

                if( strSubToken.empty() )
                {
                    if( false == splitItr.eof() )
                    {
                        throw invalid_path("part of the path was empty");
                    }
                }
                else
                {
                    string_range dot_delim = boost::find_first( strSubToken, "." );
                    if( dot_delim )
                    {
                        string_range sub_range(dot_delim.end(), strSubToken.end());
                        string_range equal_delim = boost::find_first( sub_range, "=" );
                        if( equal_delim )
                        {
                            if( dot_delim.end() == equal_delim.begin() )
                            {
                                throw invalid_path(".= found (no property name defined)");
                            }

                            newToken.name.assign(strSubToken.begin(), dot_delim.begin());
                            newToken.property.assign(dot_delim.end(), equal_delim.begin());
                            newToken.value.assign(equal_delim.end(), strSubToken.end());
                        }
                        else
                        {
                            throw invalid_path(". delimeter found with no matchind =");
                        }
                    }
                    else if( boost::find_first( strSubToken, "=" ) )
                    {
                        throw invalid_path("= delimeter found with no matchind .");
                    }
                    else
                    {
                        newToken.name = strSubToken;
                    }

                    processed_path.tokens.push_back( newToken );
                    ++itemCount;
                }
            }

            return itemCount;
        }
    }
}}

#endif // include guard