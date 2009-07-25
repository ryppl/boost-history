#define BOOST_UNICODE_SOURCE
#include <boost/unicode/graphemes.hpp>

BOOST_UNICODE_DECL bool boost::unicode::detail::grapheme_is_break
    [boost::unicode::ucd::grapheme_cluster_break::_count]
    [boost::unicode::ucd::grapheme_cluster_break::_count] =
{
//                 any cr lf control l  v  t  lv lvt extend prepend spacing_mark
/* any */          {1, 1, 1, 1,      1, 1, 1, 1, 1,  0,     1,      0},
/* cr */           {1, 1, 0, 1,      1, 1, 1, 1, 1,  1,     1,      1},
/* lf */           {1, 1, 1, 1,      1, 1, 1, 1, 1,  1,     1,      1},
/* control */      {1, 1, 1, 1,      1, 1, 1, 1, 1,  1,     1,      1},
/* l */            {1, 1, 1, 1,      0, 0, 1, 0, 0,  0,     1,      0},
/* v */            {1, 1, 1, 1,      1, 0, 0, 1, 1,  0,     1,      0},
/* t */            {1, 1, 1, 1,      1, 1, 0, 1, 1,  0,     1,      0},
/* lv */           {1, 1, 1, 1,      1, 0, 0, 1, 1,  0,     1,      0},
/* lvt */          {1, 1, 1, 1,      1, 1, 0, 1, 1,  0,     1,      0},
/* extend */       {1, 1, 1, 1,      1, 1, 1, 1, 1,  0,     1,      0},
/* prepend */      {0, 1, 1, 1,      0, 0, 0, 0, 0,  0,     0,      0},
/* spacing_mark */ {1, 1, 1, 1,      1, 1, 1, 1, 1,  0,     1,      0}
};
