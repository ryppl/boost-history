/*=============================================================================
    Spirit v1.7.0
    Copyright (c) 2003 Vaclav Vesely
    http://spirit.sourceforge.net/

    Permission to copy, use, modify, sell and distribute this software is
    granted provided this copyright notice appears in all copies. This
    software is provided "as is" without express or implied warranty, and
    with no claim as to its suitability for any purpose.
=============================================================================*/
#include <cassert>
#include <boost/cstdlib.hpp>
#include <boost/spirit/core.hpp>
#include <boost/spirit/symbols.hpp>
#include <boost/spirit/attribute.hpp>
#include <boost/spirit/phoenix/primitives.hpp>
#include <boost/spirit/phoenix/operators.hpp>
#include <boost/spirit/phoenix/statements.hpp>
#include <boost/spirit/dynamic/lazy.hpp>

using namespace boost;
using namespace spirit;
using namespace phoenix;

//-----------------------------------------------------------------------------
//  one value closure

template<typename T>
struct val_closure
    :   public spirit::closure<val_closure<T>, T>
{
    typename val_closure<T>::member1 val;
};

//-----------------------------------------------------------------------------
//  my grammer
//
//  Parsers nested blocks of numbers like:
//
//  dec {
//      1 2 3
//      bin {
//          1 10 11
//      }
//      4 5 6
//  }
//
//  And returns the overall sum.

struct my_grammar
    :   public grammar<my_grammar, val_closure<int>::context_t>
{
    //  int rules type
    template<typename ScannerT>
    struct int_rule {
        typedef rule<ScannerT, val_closure<int>::context_t> type;
    };

    // block closure
    template<typename ScannerT>
    struct block_closure:
        public spirit::closure<block_closure<ScannerT>,
            int, typename int_rule<ScannerT>::type*>
    {
        typename block_closure<ScannerT>::member1 sum;
        typename block_closure<ScannerT>::member2 int_rule;
    };

    // block rule type
    template<typename ScannerT>
    struct block_rule {
        typedef rule<ScannerT, typename block_closure<ScannerT>::context_t>
            type;
    };

    // grammer definition
    template<typename ScannerT>
    struct definition
    {
        typedef rule<ScannerT> rule_t;
        typedef typename int_rule<ScannerT>::type int_rule_t;
        typedef typename block_rule<ScannerT>::type block_rule_t;

        rule_t top;
        block_rule_t block;
        rule_t block_line;
        symbols<int_rule_t*> base;
        int_rule_t bin_number;
        int_rule_t dec_number;

        definition(my_grammar const& self)
        {
            top = block[self.val = arg1];

            block =
                    base[
                        block.sum = 0,
                        block.int_rule = arg1 // store an int rule in a closure member
                    ]
                >>  "{"
                >>  *block_line
                >>  "}"
                ;

            block_line =
                    lazy_p(*block.int_rule)[block.sum += arg1] // use the stored rule
                |   block[block.sum += arg1]
                ;

            // bind base keywords and int parsers
            base.add
                ("bin", &bin_number)
                ("dec", &dec_number)
                ;

            bin_number = bin_p[bin_number.val = arg1];
            dec_number = uint_p[dec_number.val = arg1];
        }

        rule_t const& start() const
        {
            return top;
        }
    };
};

//-----------------------------------------------------------------------------

int main()
{
    my_grammar gram;
    parse_info<> info;

    int result;
    info = parse("bin{1 dec{1 2 3} 10}", gram[var(result) = arg1], space_p);
    assert(info.full);
    assert(result = 10);

    return exit_success;
}

//-----------------------------------------------------------------------------
