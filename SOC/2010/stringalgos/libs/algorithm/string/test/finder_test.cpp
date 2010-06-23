//  Boost string_algo library substr_test.cpp file  ------------------//

//  Copyright Pavol Droba 2002-2003. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <boost/algorithm/string/find.hpp>
#include <boost/algorithm/string/finder.hpp>
#include <boost/algorithm/string/string_search/naive_search.hpp>
#include <boost/algorithm/string/string_search/rabin_karp.hpp>
#include <boost/algorithm/string/classification.hpp>


#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include <sstream>
#include <list>

//#define BOOST_TEST_ALTERNATIVE_INIT_API
//#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <boost/mpl/list.hpp>

#include <boost/range/iterator_range.hpp>

using namespace std;
using namespace boost;
using namespace boost::mpl;

/*template <class Finder>
void test_finder (Finder const &finder, std::pair<
*/

#include <string>
//#include <boost/range/range_value.hpp>
//#include <boost/range/range_const_iterator.hpp>
#include <boost/range/as_literal.hpp>
#include <boost/range.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/static_assert.hpp>
#include <cstdlib>


template <class T, class RangeT>
T
range_to_T(RangeT const &range)
{
    typename boost::iterator_range<boost::range_const_iterator<RangeT>::type> range2 = boost::as_literal(range);
    return T(
        boost::begin(range2),
        boost::end(range2)
        );
}

template <class SequenceT, class LiteralT>
void assign_literal (SequenceT &sequence, LiteralT literal_, unsigned int size)
{
    sequence.clear();
    sequence.insert(sequence.end(), literal_, literal_+size);
}

typedef boost::mpl::list<
    std::string,
    std::vector<char>
> types_list;

template <class IteratorT>
IteratorT advance (IteratorT from, unsigned int dist)
{
    std::advance(from, dist);
    return from;
}

//checks if this is the same range of the same container ( O(1) )
template <class Range1T, class Range2T>
bool superficial_range_equal(Range1T const &range1, Range2T const &range2)
{
    return boost::begin(range1) == boost::begin(range2) &&
        boost::end(range1) == boost::end(range2);
}

BOOST_TEST_CASE_TEMPLATE_FUNCTION(test_finders_second_type, SecondType)
{
    //boost::finder_t<SecondType::
}

BOOST_TEST_CASE_TEMPLATE_FUNCTION(test_finders_first_type, FirstType)
{
    //BOOST_TEST_CASE_TEMPLATE(test_finders_second_type, 
}

BOOST_TEST_CASE_TEMPLATE_FUNCTION(test_finders, Algorithm)
{
	/*boost::first_finder_t<std::string, std::string, Algorithm> first;
	boost::nth_finder_t<std::string, std::string, Algorithm> second; second.set_n(2);
	boost::nth_finder_t<std::string, std::string, Algorithm> third; third.set_n(3);
	boost::last_finder_t<std::string, std::string, Algorithm> last;*/
	/*typedef typename boost::finder_t<std::string, std::string, Algorithm> t1;
    typedef typename boost::finder_t<std::list<wchar_t>, std::list<wchar_t>, Algorithm> t2;
    typedef typename boost::finder_t<std::vector<char>, std::list<char>, Algorithm> t3;

    t1 f1; t2 f2; t3 f3;
    typename t1::string_range_type i1;
    typename t2::string_range_type i2;
    typename t3::string_range_type i3;
    */

    typedef std::vector<char> Sequence1T;
    typedef std::list<char> Sequence2T;
    typedef typename boost::finder_t<Sequence1T, Sequence2T, Algorithm> f_t;
    f_t::string_range_type i;

    Sequence1T s1 = range_to_T<Sequence1T>("some test substring"),
        s2 = range_to_T<Sequence1T>("another test substring");
    Sequence2T S1 = range_to_T<Sequence2T>("A string which \xFF\x3A" "contains 'some test substring' :)"),
        S2 = range_to_T<Sequence2T>("Another string containing another test substring, serves another purpose");
    boost::iterator_range<Sequence2T::const_iterator> match_range(
        advance(boost::begin(S1),27),
        advance(advance(boost::begin(S1),27), s1.size())
        ), // the match of searching s1 in S1
        match_range2(
        advance(boost::begin(S2),26),
        advance(advance(boost::begin(S2),26), s2.size())
        ) // the match of searching s2 in S2
    ; 

    ////////////////////////////////////////////////////
    //Testing constructors and simple find_first()
    ////////////////////////////////////////////////////
    f_t f1; // default ctor
    BOOST_CHECK(boost::begin(f1.get_substring_range()) == boost::end(f1.get_substring_range()));
    BOOST_CHECK(boost::begin(f1.get_string_range()) == boost::end(f1.get_string_range()) );
    BOOST_CHECK_EQUAL(f1.find_first_index(), 0);
    BOOST_CHECK_EQUAL(boost::distance(f1.find_first()),0);


    f_t f2(&s1, &S1); // ptr,ptr
    BOOST_CHECK(superficial_range_equal(f2.get_substring_range(),s1));
    BOOST_CHECK(superficial_range_equal(f2.get_string_range(), S1));
    BOOST_CHECK_EQUAL(f2.find_next_index(), 27);
    BOOST_CHECK_EQUAL(f2.find_first_index(), 27);
    BOOST_CHECK_EQUAL(f2.find_next_index(), -1);
    //BOOST_CHECK(superficial_range_equal(f2.find_first(), match_range));

    f_t f3(&s1, S1); // ptr,ref
    BOOST_CHECK(superficial_range_equal(f3.get_substring_range(),s1));
    BOOST_CHECK(boost::equal(f3.get_string_range(), S1));
    BOOST_CHECK_EQUAL(f3.find_first_index(), 27);

    f_t f4(s1, &S1); // ref,ptr
    BOOST_CHECK(boost::equal(f4.get_substring_range(), s1));
    BOOST_CHECK(superficial_range_equal(f4.get_string_range(), S1));
    BOOST_CHECK(superficial_range_equal(f4.find_first(),match_range));

    f_t f5(s1, S1); // ref, ref
    BOOST_CHECK(boost::equal(f5.get_substring_range(), s1));
    BOOST_CHECK(boost::equal(f5.get_string_range(), S1));
    BOOST_CHECK_EQUAL(f5.find_next_index(), 27);

    //!\todo fix
#   ifdef BOOST_HAS_RVALUE_REFS
    BOOST_CHECK(false);
    f_t f6( (Sequence1T(s1)), &S1); // rvalue, ptr
    BOOST_CHECK(boost::equal(f6.get_substring_range(),s1));
    BOOST_CHECK(superficial_range_equal(f6.get_string_range(),S1));
    BOOST_CHECK(f6.find_next() == match_range);

    f_t f7( (Sequence1T(s1)), S1); // rvalue, ref
    BOOST_CHECK(boost::equal(f7.get_substring_range(), s1));
    BOOST_CHECK(boost::equal(f7.get_string_range(), S1));
    BOOST_CHECK_EQUAL(f7.find_next_index(), 27);

    f_t f8( (Sequence1T(s1)), (Sequence2T(S1)) ); // rvalue, rvalue
    BOOST_CHECK(boost::equal(f8.get_substring_range(), s1));
    BOOST_CHECK(boost::equal(f8.get_string_range(), S1));
    BOOST_CHECK_EQUAL(f8.find_next_index(), 27);

    f_t f9( &s1, (Sequence2T(S1)) ); // ptr, rvalue
    BOOST_CHECK(superficial_range_equal(f9.get_substring_range(),s1));
    BOOST_CHECK(boost::equal(f9.get_string_range(), S1));
    BOOST_CHECK_EQUAL(f9.find_next_index(), 27);

    f_t f10(s1, (Sequence2T(S1)) ); // ref, rvalue
    BOOST_CHECK(boost::equal(f10.get_substring_range(), s1));
    BOOST_CHECK(boost::equal(f10.get_string_range(), S1));
    BOOST_CHECK_EQUAL(f10.find_next_index(), 27);
#   endif

    ////////////////////////////////////////////////////
    //Testing set_substring() and set_string()
    ////////////////////////////////////////////////////
    f2.set_substring(); f2.set_string();
    BOOST_CHECK_EQUAL(boost::distance(f2.get_substring_range()),0);
    BOOST_CHECK_EQUAL(boost::distance(f2.get_string_range()),0);

    f2.set_substring(s2); f2.set_string(S2); // ref
    BOOST_CHECK(boost::equal(f2.get_substring_range(),s2));
    BOOST_CHECK(boost::equal(f2.get_string_range(),S2));
    BOOST_CHECK(f2.find_first() == match_range2);

    f2.set_substring(&s2); f2.set_string(&S2); // ptr
    BOOST_CHECK(superficial_range_equal(f2.get_substring_range(),s2));
    BOOST_CHECK(superficial_range_equal(f2.get_string_range(),S2));
    BOOST_CHECK_EQUAL(f2.find_first_index(),26);

#   ifdef BOOST_HAS_RVALUE_REFS
    f2.set_substring( (Sequence1T(s2)) ); f2.set_string( (Sequence2T(S2)) );
    BOOST_CHECK(boost::equal(f2.get_substring_range(), s2));
    BOOST_CHECK(boost::equal(f2.get_string_range(), S2));
    BOOST_CHECK_EQUAL(f2.find_first_index(), 26);
#   endif
    
    ////////////////////////////////////////////////////
    //Testing correctness of various input data on the algorithm and refresh()
    ////////////////////////////////////////////////////
    //f2.set_substring(


    Sequence1T substr;
    Sequence2T str;
    f_t f; // finder

    // (0, 0)
    f.set_substring(&substr);
    f.set_string(&str);
    BOOST_CHECK_EQUAL(f.find_first_index(), 0);
    BOOST_CHECK_EQUAL(f.find_next_index(), -1);


    //
    assign_literal(substr,
	    "\1005mJ\133Rh\047",8);
    str.clear();
    f.refresh();
    BOOST_CHECK_EQUAL(f1.find_first_index(), -1);
    BOOST_CHECK_EQUAL(boost::distance(f1.find_first()), 0);

    // (0, 0) (1, 1) (2, 2) (3, 3) (4, 4) (5, 5) (6, 6) (7, 7) (8, 8) (9, 9) (10, 10)
    substr.clear();
    assign_literal(str,
	    "\055sjtIm6\052GJ",10);
    f.refresh();
    f.find_reset();
    for (unsigned int i = 0; i <= 10; ++i)
        BOOST_CHECK_EQUAL(f.find_next_index(), i);
    

    // (0, 44)
    assign_literal(substr,
	    "Zd07sA\046\042WSyj\076q\136\053Ho\043EA\056i\043C\052\1356sl"
	    "rZ\075Yizv\177y\073W\174le",44);
    assign_literal(str,
	    "Zd07sA\046\042WSyj\076q\136\053Ho\043EA\056i\043C\052\1356sl"
	    "rZ\075Yizv\177y\073W\174le",44);
    f.refresh();
    BOOST_CHECK_EQUAL(f.find_first_index(), 0);
    BOOST_CHECK_EQUAL(boost::distance(f.find_first()), 44);
    BOOST_CHECK_EQUAL(boost::distance(f.find_next()), 0);

    // (0, 1) (1, 2) (2, 3) (3, 4) (4, 5) (5, 6) (6, 7) (7, 8) (8, 9) (9, 10) (10, 11) (11, 12) (12, 13) (13, 14) (14, 15) (15, 16) (16, 17)
    assign_literal(substr,
	    "x",1);
    assign_literal(str,
	    "xxxxxxxxxxxxxxxxx",17);
    f.refresh();
    f.find_reset();
    for (unsigned int i=0; i<=16; ++i)
        BOOST_CHECK_EQUAL(f.find_next_index(), i);

    //
    assign_literal(substr,
	    "X",1);
    assign_literal(str,
	    "xxxxxxxxxxxxxxxxx",17);
    f.refresh();
    BOOST_CHECK_EQUAL(f.find_first_index(), -1);

    // (0, 2) (3, 5) (6, 8) (9, 11) (12, 14) (15, 17) (18, 20) (21, 23)
    assign_literal(substr,
	    "Yx",2);
    assign_literal(str,
	    "YxXYxXYxXYxXYxXYxXYxXYxX",24);
    f.set_substring(substr); f.set_string(str);
    f.find_reset();
    for (unsigned int i=0; i<=21; i += 3)
        BOOST_CHECK_EQUAL(f.find_next_index(), i);

    // (0, 7) (1, 8) (2, 9) (3, 10) (4, 11) (5, 12) (6, 13) (7, 14) (8, 15) (9, 16) (10, 17)
    assign_literal(substr,
	    "AAAAAAA",7);
    assign_literal(str,
	    "AAAAAAAAAAAAAAAAA",17);
    f.set_substring(substr); f.set_string(str);
    for (unsigned int i=0; i<=10; ++i)
        BOOST_CHECK_EQUAL(boost::distance(f.find_next()), 7);
    BOOST_CHECK_EQUAL(f.find_next_index(), -1);

    // (233, 581)
    assign_literal(substr,
	    "\051E\176B5Zjp\056CRO\136fmC\134p\045 d\175FC\072OW\053Fy"
	    "g\043aI ZWL\045\073\134\073\045\077k\173Ck8\200Yavc\177M\074zgu"
	    "RU4b\044\072w\140d\134b3\0501\055\042\0416\135\045g\042\057yO9bo8N"
	    "g066VA\044vtDS9tHx\050AZP\176\0446\057\200GKH7\077C"
	    "\137vzJ\043\046GVB\041iJl\077U\176ETG\047f\046h\076i\074P6\051h"
	    "Pv\072MHj8XOcPybXri\076\177b\075z8\052\0459\072xQc8"
	    "M\054i\054\074v\050Dh\140\1336\056\045\050\0465y\076\173\174Z7f\177\044\075\045R\054"
	    "\053rFv\056MO\074\041\050\135LTnn\075N\041H\177\135n03Im7\056W\053"
	    "zgn2\052B\051lhP\175IKPK\134u6c\074f3bP\057fh\140\177l"
	    "\137J\051 \043\135\136VFU\136r\042zPmGmES\134\140\136z\052K\053luI"
	    "\200e5tAUf\043oZz\175RX HvxHBlY7YJjvfO\047"
	    "\133m\056BMcp\043xVuz\134\053\176zJ\052",348);
    assign_literal(str,
	    "\041\075mS\051\052C\073rtR\136z\045\053f\072c6\073\051qYa\136\077\137\177\053\057"
	    "\177s\057b\074yUs\044JpBsUI\075jpgd\174n\041\1363\074O\055\137 "
	    "7\044Qe\173\0552g\1758F\056\175v\056Ja7\173\057t\052\051\057\177\176jgi\074"
	    "S\0566j6NfJG3\047\133p\043XD\135\135NW5\100\0552XgoaT\173"
	    "dhIl\05650j9\072E\173Ju\0439\177\1346C\046o\042M\052w\175vQ\045"
	    "X\173\076kvAiC\053fO\057\046kYL\053IIKE\0775qEr\053zpD"
	    "\076\0746\1330\177tj xC5\075\140\046\057WjPEQ\134\177\074sEf59T"
	    "rtN\047\077OU\054P\056h\077bY \050U\047m8t2s\051E\176B5Zj"
	    "p\056CRO\136fmC\134p\045 d\175FC\072OW\053Fyg\043aI ZW"
	    "L\045\073\134\073\045\077k\173Ck8\200Yavc\177M\074zguRU4b\044\072w"
	    "\140d\134b3\0501\055\042\0416\135\045g\042\057yO9bo8Ng066VA\044"
	    "vtDS9tHx\050AZP\176\0446\057\200GKH7\077C\137vzJ\043\046G"
	    "VB\041iJl\077U\176ETG\047f\046h\076i\074P6\051hPv\072MHj8"
	    "XOcPybXri\076\177b\075z8\052\0459\072xQc8M\054i\054\074v\050"
	    "Dh\140\1336\056\045\050\0465y\076\173\174Z7f\177\044\075\045R\054\053rFv\056MO"
	    "\074\041\050\135LTnn\075N\041H\177\135n03Im7\056W\053zgn2\052B\051"
	    "lhP\175IKPK\134u6c\074f3bP\057fh\140\177l\137J\051 \043\135\136"
	    "VFU\136r\042zPmGmES\134\140\136z\052K\053luI\200e5tAUf"
	    "\043oZz\175RX HvxHBlY7YJjvfO\047\133m\056BMcp"
	    "\043xVuz\134\053\176zJ\052W5c\076V0\057b\043\053\051\045Flg6\050xC"
	    "z5QWD \173\074\140\045\057LHZHzW\133\137\053\0438\0427dg\072hsC"
	    "yc\134\135O\055Sc\136\073it\053WA\055\177\051\137H\057\052\177\200O\1378C\053v"
	    "AsT\133k1\135\076\043\045\1772v\041qX2\133yERiQC\052\050\047\073jn"
	    "\176\200z8L\052P\100p0 U\137\173\134Fo7Ou5YMa99AkN "
	    "\174\174N7\053\041\077\0763Ge\042\1341IKQx\050\137U\077\136yANnnRv"
	    "\136tVA\047lm\0417K\133b\045D\134jCy\047\1336\052\0558Ne\100M\054l"
	    "zsnoC\057N \043\1764UF\200\1000ncg\173CKUS\051\052s\100cP"
	    " \100\047v\052GRj\04384f0Z\135\0761f\133\046Ph2\0775q\076KCi"
	    "\057fL\0738M\057mJ\072\046IHiSysUi\057yH3\053\075T kP5"
	    "G\042\136o0QldyV\134\0723U\173\173\076kN\177\137x6\044x\050\134\075\175\135"
	    "e\044\176RDs\133W\050\047X\135P0\042Dv25M\134g\0447J\173pBup"
	    "p1\074C\051z6\136YYN\044\174y\177\0423\077V\050e\077u\175LmyH\100S"
	    "\042\077\052\0501 R\055\044IMq2\055kD3Z\176iNZaHDhk\176\045z"
	    "YG\100\050FnEH47aV\0420hG\133\042\075JQ\044uQl\134TT49"
	    "xU\1334X7\134bk0uz\045Mej3IVxTs0oW0\076\1004C"
	    "\046\134\073Id\135TSxL5\135\042\072\133SLyLR\042\072wf\072tkV\076h"
	    "s96\042A\1747ac55\076\175o\054veL\072CvF K\054\072ku5\200"
	    "\077ymo\054XWHuOW\050\074fP\054x\050Y\133O\046\042 \074s\045\100iO"
	    "1m\135\044\075\133\177H5WybREnO\074\136U6j\054Npj\053toDD"
	    "9CD5e\176vr\133 vUUS\057\1368\133a\046Fyefd3VLGb"
	    "4\1002\176\044RvP\051\1408F\042skbDCnz\176l\133A2\174NYTA"
	    "\136\043\175\100FUY6IZ\051\175WRlLyk\0441PZ\051eQB\050\054\174R"
	    "KbFY\075FDx\1759\0540IVx\136quo\044wbjT\077r\0475\1379"
	    "cz\136\050o\047\043oi\1753\134R\041\073h\140cM5R\075\044M\200\076b\046jT"
	    "4R\1779T\054PH\0515GZUY4Y\075Y\042\175IS9ZRd\133NA\176"
	    "SRvt\073\174tl\176h0Evf\177Ncdm\054O\077\054CwCA\173\100\077"
	    "\1354\135\0450\075F22q\055\050aO 1w\073\042X\076\073D\073d\140D0\041h"
	    "Gj\045dXej\134\053su2zMl2jN\045y\055\072\135\072\074K\136\1355\140"
	    "\041f\073\1407iM9yBXLHlv9gm\053\1333\041J7\072h\056u\174\100"
	    "i\073O\1367yI\136k\054sTy\072\077b9A \136DM\074\072\075mB\0766i"
	    "G\057\173T\135nU\051M3\1374l \052ZIjSu\176k6\042u\055Xhbm"
	    "u6CW\052JOc\045\041EaJ\173\176hzu\177D\134l\047O\0515ES2W"
	    "WB\054I2\076Nz\075O\056T6\0527\051Cc\134M\073kS9\046\057jd\054v"
	    "2MA\076\174lReqk\134qsNY9F5bIc\045\056s5n4N2i"
	    "Br\137QT\051eaz\054c\052\135\0459 G\077VP\044\140O\043bkBc\055p"
	    "L\073X\045iWV\175W\137\136hM\133\050a\177hI\176vJN\137\176t\135 \173\045"
	    "\1336Z5y5NgOYx\0512\134Wa7fqzy\136vLE\045g7F\056"
	    "\136\053u MFjtQz\140\052\140IbYbO\133\140kffmc\074E\0769w"
	    "\173\047\073\133\042\177\053wu0x\175J3qpdA\051vT\057\075Jkp96B\074"
	    "\046 X\1004\140\073j\140kY\072dl\176wy\046\100gq\2008D\0508Z\054HV"
	    "\046\0558\136PY6bVQ\077\136\173 \177\042\043C\135\046w\047\057\057q\043\134S\072\133"
	    "\077J8\076\134Pfj\075\1339\140GH9 ynoOOgnU6kfM8l"
	    "\042\043k\072\044h4\0505\047\177kCW\052e\0509GVu2\056K\052Q\077E7r"
	    "uHePhj f nGd\173V\174P\175\0771xUS\1358lBH J\075"
	    "V\076jRK\056pR\057\074QLp\135\044iWkwNRX\054Be YA39"
	    "etngj\140Ac\052N\043\045S\043O0MW\077\050s\041\046\0567J\042\044\072G"
	    "x\174I\134\200cOU0\1772ZDPfup8dxc\175",2002);
    f.set_substring(&substr); f.set_string(str);
    BOOST_CHECK_EQUAL(f.find_next_index(), 233);

    // (237, 523)
    assign_literal(substr,
	    "\140\173\054\076\0736a\135t\055\0468\134\050\140\133\053\055\077Fr\177\043Z5\0576\14003"
	    "Q\134N\176 \057\076\134\176\1370KO\045\135\072i6Yu\045\072nG2\052\073hth"
	    "Xk\0729\045nq\0434t\075\176zrX\0556z\045A\0442s\055\051j\043 \175b"
	    "\1346oSd\041\057\041\077iVxRd\053\134FR\137\076\056\047\173E3C\043\075\077d"
	    "\075yx1y\1753 xF\135\134\042iu\076\176\140\100\176uf\057\041Gcr9Hm"
	    "F\055XG\077Nj\047\052JV1Q\136\055\074z\136p\1345\042NPH\053epC\072"
	    "lGI\051\073\135m2\057\047\044bG\174VK\200\177v\0464\046qy\200B\052cu\140"
	    "\051Cl\047\050CzlY\051sWv\053R\175\0561KBG\136o\045CD\055ol\041"
	    " \050v4\053\133\051\054iwvn\047\043zO\173t\041\043jxv2\100Y\041qdU"
	    "xL\134sipk\134tE\137\042R\135BE",286);
    assign_literal(str,
	    "NgTNd\176\042J\047cLB\074UAP6\077\072\051l\200\0454XkV\077u\044"
	    "\052\042y\1750\072\042KBu\174okaW\1357\100FgkyGm\043\050\136uK\050"
	    "\133o\100\056\053fpmi\077eL1\041\054\0448\046cjR8M9\135\135S\135l\076"
	    "Zf\176od3GHf2\0750G\050MxOe1\056Z50\050kYAU\174\136"
	    "\07346\0417\137\0462\177uqPz07I\133\175zc3O\175\072N\075X\200Fq"
	    "5zu4\044O\073\056S\054JDymr\077VlI9\075jn\175\173\1733bR\140"
	    "wd\054RQOC\050fyZ95n\133 z\137\041\135\175bm\100GAFfr6"
	    "Oz\173wPMm\051\057hxD\043F0O\137ZW\134QOSkhfh\140\173\054"
	    "\076\0736a\135t\055\0468\134\050\140\133\053\055\077Fr\177\043Z5\0576\14003Q\134N"
	    "\176 \057\076\134\176\1370KO\045\135\072i6Yu\045\072nG2\052\073hthXk\072"
	    "9\045nq\0434t\075\176zrX\0556z\045A\0442s\055\051j\043 \175b\1346o"
	    "Sd\041\057\041\077iVxRd\053\134FR\137\076\056\047\173E3C\043\075\077d\075yx"
	    "1y\1753 xF\135\134\042iu\076\176\140\100\176uf\057\041Gcr9HmF\055X"
	    "G\077Nj\047\052JV1Q\136\055\074z\136p\1345\042NPH\053epC\072lGI"
	    "\051\073\135m2\057\047\044bG\174VK\200\177v\0464\046qy\200B\052cu\140\051Cl"
	    "\047\050CzlY\051sWv\053R\175\0561KBG\136o\045CD\055ol\041 \050v"
	    "4\053\133\051\054iwvn\047\043zO\173t\041\043jxv2\100Y\041qdUxL\134"
	    "sipk\134tE\137\042R\135BEOd\055V7\043I6rt\175vk9o\052\075"
	    "x\055joMm5\174H\072G\100ZK\174s6LZ\075\137Y2Zj0\043\052\072S"
	    "jiuEB\100Vs\056sF\044LvaYW\140iM\077GVvnXH6j\072"
	    "\174\055Z\041T\074nkVK\044\173i\134\076\056\1744G\133\177j\133\2004d\052UBx"
	    "mg8\177a\1353\177S7\047\175S\177\135Uvqg\055\1360YY\056D\140\174\046P"
	    "\050\176o\200\135ApXGk4R\077Cpx\136lBnA4 \050\047rG3\0570"
	    "\1374\074k\043VgEsB\140V\042xVac\055\041hM3\133\176J3\0575SE"
	    "\057L\140c3\077\046\045S\1779R\200LJFw9\072\057\051bdcR3USV\050"
	    "\136eM\043\044\056Xb\054Yi vFZ mpp4\041 Sh\073SPT46"
	    "PZJ80D\043dV7af\053\177\177l\047\135zf\047\135E\041\044y\044kCU"
	    "rK\137D\136\175\051n0v4eQ\045\051K\053w52\175\041iz59\0567X\073"
	    "n\140ffPx\054KtI\043\173\047\136\134\051DmH\075\175x3\134\1352Ej\051\072"
	    "\044kp\056JD\073bk7\043M\173MhMy95qu5Lf\136\042Ue\177M"
	    "FGlOEP\177\173SKW\041\075\044V\2004Mq\054muzKj\074j\050\075B"
	    "\076\057Brx4l\074ggS\051\073\046GYI\135\052\074fU\056\041\0775e5\057X"
	    "i\041\175\140Q\077ayy4zc\1332k\0469J\052\047D\073\140dShUq\057v"
	    "a\055y\073R\2006 tJ\052n\174S\046O3\046t\200\047\045HIF\047\135\043\044n"
	    "\053D\174p\133\052OeZ\173\177\137S\133\053\140j\077EdwQcrs\077d\046hP"
	    "\075\100c\044\077\173L\140\055d\200366aV\047b\046\134\136\0750si\136FIZx"
	    "\077z2\075\173 M5\051\0518vT\100Chp0CTIo\073A\0414I\055G\042"
	    "F\134\200NUh\055\042TW6\074\052Ap8\052hs4X\044\134\0461\077\055\050u8"
	    "g\175m7\041\050\047\046m\137C4noM\0738Kk\077\042xK\100df0\136S\050"
	    "mL\177MR1V\075\057Mwwd\175\134lE\0744l\135P\177\073k\177\046Vtu"
	    "K8GS4\1401vG\075Jrw\057chmBK\0456O\137b6a\055\053\0502"
	    "\045\1340\133WA\045\057\1747pp",1242);
    f.refresh_substring();
    f.set_string(str);
    BOOST_CHECK_EQUAL(f.find_first_index(), 237);
    BOOST_CHECK_EQUAL(f.find_next_index(), -1);

    //
    assign_literal(substr,
	    "xeG\073M\051h\176W\073s0j\043AYp\177\100kcHf\174VK\075",27);
    assign_literal(str,
	    "\135c\176gq\073u\051\134x\052WTo2F A\076DHl9ko\045\074\075IL"
	    "w\076p\045\057\136 HYWP\140\175x\173\135\057\136rxx\046\073\056EN\133\136FQ"
	    "aaePrh g4mX\136\046I\0435iC\177Z6OA\174V2\135N9L"
	    "8X\0573tv\135V\077n\054c7\0754\054mUrHX\043\200io1\053K\200r"
	    "lAZYn\176\135xi\133H\200\133JA\046H\045AMmCt8s\1366D\100\075"
	    "T8\14022L\133H6\137f\176rh g4mX\136\046I\0435iC\177Z6O"
	    "A\174V2\135N9L8X\0573tw\140nK4\140R\044Qrk8\053i\052\135V"
	    "g\051EOA \1331xbt\100\073O\042\051N\134Lb\042\176j\041mf\134\176\137j"
	    "M\056hka0I\137LP6F4\045ti2W\100\076\072\057I\072zXPZ\050\045"
	    "HK\046\076k\175 5N\173qDR\043\053Bwz\057\075Frorh g4mX"
	    "\136\046I\0435iC\177Z6OA\174V2\135N9L8X\0573t\133\177mqFZ"
	    "\0557\0578\100TrOd\177D\175b\100ec\07416\177zi\073XO\056\137zDG"
	    "3E\174tE\053v\043b\074\136\053V\053\176\046\0505\057d84\041 \055\047\140\054\043\054"
	    "\047\053uV\134rz\176\072\0467MVEwb\200jlu\050 eT",414);
    f.set_substring(substr);
    f.set_string(&str);
    BOOST_CHECK_EQUAL(f.find_next_index(), -1);

    // (91, 122) (201, 232) (358, 389)
    assign_literal(substr,
	    "rh g4mX\136\046I\0435iC\177Z6OA\174V2\135N9L8X\0573"
	    "t",31);
    assign_literal(str,
	    "\077mY\176OvV\136\073N\174I\042Yz\176\041DQ\056\175KDR\0761J \135y"
	    " v\175ldK310mVs\174Uh\046j\046\055Ee\0541c\042l3OVJ"
	    "A\042bYgPVjb\042\046YC\136Q\041wq0T4\056w\046\133s3O5L"
	    "Qrh g4mX\136\046I\0435iC\177Z6OA\174V2\135N9L8X\057"
	    "3t8\1332uEAG\045\045y\075bbHFB\045\047\047j\135\0427\052zl\050\133"
	    "QwZiGX6jo \200nkk\136\073\140\047k\054ML\133f0K\041\100\134G"
	    "p3G\043\047P\054BSMqVT\175IrY\045CyErh g4mX\136\046"
	    "I\0435iC\177Z6OA\174V2\135N9L8X\0573t\077h3\052Wx \042"
	    "\0456\057nzDu xO\075\073\074\045XH\054Q tOP\043w\137\043\051kAC"
	    "dq5\073\1760MRuWV7D\176FR\200x\1759\051QD\135YfITP1"
	    "x\075\1359J0R\135R\054\047\073\1339xW1h\073l\140g\053\134m9\177r7K"
	    "kd\072\136ORWu\0433\174T\042\04227\050hB\072mrtQi\053\136\042rh"
	    " g4mX\136\046I\0435iC\177Z6OA\174V2\135N9L8X\0573t\057"
	    "KAv\075uo\043ox\135\136\0769g\073kwM\057\173\134l\075\041m \0530\100\176"
	    "TTM\174\077V\051Mn\135\076Tx\100GTX\077r\051Za\051\176sm\047\052\0732"
	    "\076nq\041\077ArD\134oV0KdaXSRS\175",470);
    f.refresh_string();
    f.set_substring(substr);
    BOOST_CHECK_EQUAL(f.find_first_index(), 91);
    BOOST_CHECK_EQUAL(f.find_next_index(), 201);
    BOOST_CHECK_EQUAL(f.find_next_index(), 358);

    // (0, 64)
    assign_literal(substr,
	    "K\175s\072\175yi3\053C9\100\055d\077CT\100Hon1TeKbeGn0"
	    "M\051sWaU\053\077o\043I\0747\1402X\05760P\044cO40\174 7ZL"
	    "8\051YD",64);
    assign_literal(str,
	    "K\175s\072\175yi3\053C9\100\055d\077CT\100Hon1TeKbeGn0"
	    "M\051sWaU\053\077o\043I\0747\1402X\05760P\044cO40\174 7ZL"
	    "8\051YD",64);
    f.refresh_string();
    f.set_substring(substr);
    BOOST_CHECK_EQUAL(boost::distance(f.find_next()), 64);

    // (0, 6) (6, 12) (12, 18)
    assign_literal(substr,
	    "Q\05618\136\137",6);
    assign_literal(str,
	    "Q\05618\136\137Q\05618\136\137Q\05618\136\137",18);
    f.set_substring(&substr);
    f.set_string(&str);
    BOOST_CHECK_EQUAL(f.find_first_index(), 0);
    BOOST_CHECK_EQUAL(f.find_next_index(), 6);
    BOOST_CHECK_EQUAL(f.find_next_index(), 12);
    BOOST_CHECK_EQUAL(f.find_next_index(), -1);

    //
    assign_literal(substr,
	    "xRl\175Uo\174\042QV9Bh0C9\052XlJI0I\200C03C5D"
	    "yS",32);
    assign_literal(str,
	    "\055\174\053l\074b\0507\140\134eRl\072\133Uf\134L6\045F\173AlXr\1350M"
	    "\044e",32);
    f.refresh();
    BOOST_CHECK_EQUAL(f.find_first_index(), -1);

    // (0, 1) (1, 2) (2, 3) (3, 4) (4, 5) (5, 6) (6, 7) (7, 8) (8, 9) (9, 10) (10, 11) (11, 12) (12, 13) (13, 14) (14, 15) (15, 16) (16, 17) (17, 18) (18, 19) (19, 20) (20, 21) (21, 22) (22, 23) (23, 24) (24, 25) (25, 26) (26, 27) (27, 28) (28, 29) (29, 30)
    assign_literal(substr,
	    "\000",1);
    assign_literal(str,
	    "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000",30);
    f.refresh();
    for (unsigned int i=0; i<=29; ++i)
        BOOST_CHECK_EQUAL(f.find_next_index(), i);
    BOOST_CHECK_EQUAL(f.find_next_index(), -1);
}

boost::unit_test::test_suite* init_unit_test_suite( int argc, char* argv[] )
{
	
	typedef boost::mpl::list<
        boost::algorithm::naive_search/*,
        boost::algorithm::rabin_karp<>*/
	> algorithm_list;
	boost::unit_test::framework::master_test_suite().add(
        BOOST_TEST_CASE_TEMPLATE(test_finders, algorithm_list)
    );
    return NULL;
}
