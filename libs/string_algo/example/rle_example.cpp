//  Boost string_algo library example file  ---------------------------------//

//  (C) Copyright Pavol Droba 2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

/*
    RLE compression using replace framwork. Goal is to compress a sequence of
    repeating characters into 3 bytes ( repeat mark, character and repetition count ).
    For simplification, it works only on numeric-value sequences.
*/

#include <string>
#include <iostream>
#include <limits>
#include <boost/detail/iterator.hpp>
#include <boost/string_algo/replace_impl.hpp>
#include <boost/string_algo/detail/find_base.hpp>

// replace mark specification, specialize for a specific element type
template< typename T > T repeat_mark() { return std::numeric_limits<T>::max(); };


// Compression  -----------------------------------------------------------------------


// find compress-rle functor
/*
    Find a sequence which can be compressed. It has to be at least 3-character long
    sequence of repetitive characters 
*/
template< typename InputT, typename InputPolicy >
struct find_compressF : public boost::string_algo::detail::find_baseF<InputT, InputPolicy>
{
    typedef boost::string_algo::detail::find_baseF<InputT, InputPolicy> base_type;

    typedef typename base_type::input_type input_type;
    typedef typename base_type::input_iterator_type input_iterator_type;
    typedef typename base_type::input_reference_type input_reference_type;
    typedef typename boost::detail::iterator_traits<input_iterator_type>::value_type value_type;
    typedef boost::iterator_range<input_iterator_type> result_type;

    // Construction
    find_compressF() {}

    // Operation
    result_type operator()( input_reference_type Input ) const
    {
        return operator()( Input, Input.begin() );
    }

    result_type operator()( 
        input_reference_type Input, 
        input_iterator_type From ) const
    {
        // begin of matchin segment
        input_iterator_type MStart=Input.end();
        // Repetition counter
        value_type Cnt=0;

        // Search for a sequence of repetitive characters
        for(input_iterator_type It=From; It!=Input.end();)
        {
            input_iterator_type It2=It++;

            if ( It==Input.end() || Cnt>=std::numeric_limits<value_type>::max() )
            {
                return result_type( MStart, It );
            }

            if ( *It==*It2 )
            {
                if ( MStart==Input.end() )
                {
                    // Mark the start
                    MStart=It2;
                }

                // Increate repetition counter
                Cnt++;
            }
            else
            {
                if ( MStart!=Input.end() )
                {
                    if ( Cnt>2 )
                        return result_type( MStart, It );
                    else
                    {
                        MStart=Input.end();
                        Cnt=0;
                    }
                }
            }
        }

        return result_type( Input.end(), Input.end() );
    }
};

// rle compress format
/*
    Tranform a sequence into repeat mark, characted and count 
*/
template< typename SeqT >
struct format_compressF
{
    typedef SeqT result_type;
    typedef const result_type result_reference_type;
    typedef typename SeqT::value_type value_type;

    // Construction
    format_compressF() {};

    // Operation
    template< typename ReplaceT >
    result_type operator()( const ReplaceT& Replace ) const
    {
        SeqT r;
        r.push_back( repeat_mark<value_type>() );
        r.push_back( *(Replace.begin()) );
        r.push_back( value_type( Replace.size() ) );
        
        return r;
    }
};

// Decompression  -----------------------------------------------------------------------


// find decompress-rle functor
/*
    find a repetition block
*/
template< typename InputT, typename InputPolicy >
struct find_decompressF : public boost::string_algo::detail::find_baseF<InputT, InputPolicy>
{
    typedef boost::string_algo::detail::find_baseF<InputT, InputPolicy> base_type;

    typedef typename base_type::input_type input_type;
    typedef typename base_type::input_iterator_type input_iterator_type;
    typedef typename base_type::input_reference_type input_reference_type;
    typedef typename boost::detail::iterator_traits<input_iterator_type>::value_type value_type;
    typedef boost::iterator_range<input_iterator_type> result_type;

    // Construction
    find_decompressF() {}

    // Operation
    result_type operator()( input_reference_type Input ) const
    {
        return operator()( Input, Input.begin() );
    }

    result_type operator()( 
        input_reference_type Input, 
        input_iterator_type From ) const
    {
        for(input_iterator_type It=From; It!=Input.end(); It++)
        {
            if( *It==repeat_mark<value_type>() )
            {
                // Repeat mark found, extract body
                input_iterator_type It2=It++; 
                
                if ( It==Input.end() ) break;
                    It++; 
                if ( It==Input.end() ) break;
                    It++;
                
                return result_type( It2, It );
            }
        }

        return result_type( Input.end(), Input.end() );
    }
};

// rle decompress format
/*
    transform a repetition block into a seuqence of characters
*/
template< typename SeqT >
struct format_decompressF
{
    typedef SeqT result_type;
    typedef const result_type result_reference_type;
    typedef typename SeqT::value_type value_type;

    // Construction
    format_decompressF() {};

    // Operation
    template< typename ReplaceT >
    result_type operator()( const ReplaceT& Replace ) const
    {
        // extract info
        typename ReplaceT::const_iterator It=Replace.begin();

        value_type Value=*(++It);
        value_type Repeat=*(++It);
        
        SeqT r;
        for( value_type Index=0; Index<Repeat; Index++ ) r.push_back( Value );

        return r;
    }
};

using namespace std;
using namespace boost;

void rle_example()
{
    cout << "* RLE Compression Example *" << endl << endl;

    string original("123_AA_*ZZZZZZZZZZZZZZ*34");

    // copy compression

    string compress=string_algo::replace_all_copy( 
        original, 
        find_compressF<string,string_algo::const_input_policy>(), 
        format_compressF<string>() );

    cout << "Compressed string: " << compress << endl;

    string decompress=string_algo::replace_all_copy( 
        compress, 
        find_decompressF<string,string_algo::const_input_policy>(), 
        format_decompressF<string>() );

    cout << "Decompressed string: " << decompress << endl;

    // in-place compression

    string_algo::replace_all( 
        original, 
        find_compressF<string,string_algo::mutable_input_policy>(), 
        format_compressF<string>() );
    
    cout << "Compressed string: " << original << endl;

    string_algo::replace_all( 
        original, 
        find_decompressF<string,string_algo::mutable_input_policy>(), 
        format_decompressF<string>() );

    cout << "Decompressed string: " << original << endl;

    cout << endl;
}
