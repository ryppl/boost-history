/*
#if defined(BOOST_MPL_LIMIT_METAFUNCTION_ARITY) && BOOST_MPL_LIMIT_METAFUNCTION_ARITY < 6
#undef BOOST_MPL_LIMIT_METAFUNCTION_ARITY
#define BOOST_MPL_LIMIT_METAFUNCTION_ARITY 10
#elif !defined(BOOST_MPL_LIMIT_METAFUNCTION_ARITY)
#define BOOST_MPL_LIMIT_METAFUNCTION_ARITY 10
#endif
*/


#if 0

#include <boost/type_traits.hpp>


#include <boost/mpl/lambda.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/vector.hpp>


#include <iostream>
#include <string>


using namespace std;
using namespace boost;

/*
template <class F, class X>
struct twice : boost::mpl::apply<F, typename boost::mpl::apply<F, X>::type> {};

using namespace boost::mpl;
using namespace boost::mpl::placeholders;

template <class S1, class S2>
struct cross_product
{
    template <class OldS, class T>
    struct inner_cross_product
    {
        typedef typename boost::mpl::fold<S2, OldS,
            boost::mpl::push_back<boost::mpl::_1, std::pair<T, boost::mpl::_2> > >::type type;
    };

    typedef typename boost::mpl::fold<S1, boost::mpl::vector0<>,
        inner_cross_product<boost::mpl::_1,boost::mpl::_2> >::type type;
};

int main ()
{
    using namespace boost;
    using namespace boost::mpl::placeholders;
    typedef twice<boost::add_pointer<boost::mpl::_1>,int>::type A;
    static_assert(is_same<A, int**>::value,"a");
    static_assert(
        boost::mpl::equal<
            boost::mpl::transform<
                boost::mpl::vector_c<int, 1,2,3>,
                boost::mpl::plus<_,boost::mpl::int_<1> >
            >::type,
            boost::mpl::vector_c<int,2,3,4>
        >::value, "c");
    static_assert(
        boost::mpl::equal<
            boost::mpl::transform<
                boost::mpl::vector_c<int,1,2,3>,
                boost::mpl::vector_c<int,1,1,1>,
                boost::mpl::plus<_,_>
            >::type,
            boost::mpl::vector_c<int,2,3,4>
        >::value, "b");
    static_assert(
        boost::mpl::equal<
            boost::mpl::fold<
                boost::mpl::vector<int>, boost::mpl::vector0<>,
                boost::mpl::push_back<boost::mpl::_1,std::pair<boost::mpl::_2,boost::mpl::_2> > >::type,
            boost::mpl::vector<std::pair<int,int>>
        >::value, "c");
    static_assert(
        boost::mpl::equal<
            cross_product<
                boost::mpl::vector<char>, boost::mpl::vector<char>
            >::type,
            boost::mpl::vector<std::pair<char,char>>
        >::value, "ohai"
    );


    std::cin.get();
    return 0;
}
*/






#include <boost/mpl/lambda.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/fusion/adapted/mpl.hpp>


template <class T, class U, class V> struct A : T::template X<U> {};

#if 0
template <class Range1T, class Range2T, class AlgorithmT/*,
    class ComparatorT = ::boost::algorithm::is_equal,*/
    //class AllocatorT = /*typename AlgorithmT::default_allocator_type*/std::allocator<std::size_t>/*,
    //class AdditionalBehaviorT = boost::algorithm::finder_no_additional_behavior*/
>
class simplified_finder_t2 :
    //public boost::algorithm::detail::finder_typedefs<Range1T,Range2T,ComparatorT,AllocatorT>,
    private AlgorithmT::template algorithm<
        simplified_finder_t<Range1T, Range2T, AlgorithmT>,
        typename boost::range_const_iterator<Range1T>::type,
        typename boost::range_iterator<Range2T>::type//,
        //ComparatorT,AllocatorT>
    >
{ };
#endif

#endif

//#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
//#define BOOST_MPL_LIMIT_METAFUNCTION_ARITY 10
#include <boost/algorithm/string/benchmark_finder.hpp>

#include <boost/algorithm/string/finder.hpp>
#include <boost/algorithm/string/string_search.hpp>

#include <boost/mpl/lambda.hpp>
#include <boost/fusion/algorithm/transformation/transform.hpp>
#include <boost/fusion/container/vector/convert.hpp>

#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/distance.hpp>
#include <boost/cstdint.hpp>

#include <boost/mpl/vector.hpp>

#include <fstream>
#include <cstdlib>
#include <limits>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>

#include <boost/assign/list_of.hpp>

#include <boost/foreach.hpp>

#include <boost/spirit/home/phoenix/function.hpp>
#include <boost/spirit/home/phoenix/core/argument.hpp>
#include <boost/spirit/home/phoenix/core/reference.hpp>
#include <boost/spirit/home/phoenix/operator.hpp>

#include <boost/exception/exception.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/throw_exception.hpp>

#include <boost/format.hpp>


unsigned int mrand (unsigned int min, unsigned int max)
{
    static boost::mt19937 rng;
    assert(min <= max);
    boost::uniform_int<> distrib(min, max);
    boost::variate_generator<boost::mt19937&, boost::uniform_int<> > gen(rng, distrib);
    return gen();
}

std::string rand_str (int max)
{
    std::string ret;
    max = mrand(max/2,max);
    for (int i = 0; i < max; ++i) ret += (unsigned char)mrand(0,255);
    assert(ret.size() == max);
    return ret;
}

//get a random substring out of a string
std::string rand_substr(std::string const &str)
{
    unsigned int size = mrand(1,str.size());
    unsigned int offset = mrand(0, str.size()-size-1);
    return std::string(str.begin()+offset, str.begin()+offset+size);
}

std::string rand_obfuscate_end (std::string const &str)
{
    unsigned int start;
    if (str.size() < 10) start = 0;
    else start = str.size() - 10;
    unsigned int offset = mrand(start, str.size()-1);
    return str.substr(0, offset) + (char)mrand(0,255) + str.substr(offset+1);
}

std::string rand_obfuscate_beginning (std::string const &str)
{
    unsigned int end = 10;
    if (end >= str.size()) end = str.size()-1;
    unsigned int offset = mrand(0, end);
    return str.substr(0, offset) + (char)mrand(0,256) + str.substr(offset+1);
}

char find_most_frequent(std::string const &str)
{
    static const unsigned int char_range_size =
        std::numeric_limits<char>::max() -
        std::numeric_limits<char>::min() + 1;
    std::vector<unsigned int> freq(char_range_size);
    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
    {
        //!\todo reuse old one
        //++freq[*it - std::numeric_limits<char>::min()];
        try
        {
            ++freq.at((int)*it - std::numeric_limits<char>::min());
        }
        catch (std::exception const &)
        {
                BOOST_THROW_EXCEPTION(std::runtime_error(
                (boost::format("Tried to increment index %1% of the vector") % ((int)*it - std::numeric_limits<char>::min())).str()
                ));
        }
        
    }
    unsigned int max_idx = 0, max = freq[0];
    for (unsigned int i = 1; i < char_range_size; ++i)
        if (freq[i] > max) { max = freq[i]; max_idx = i; }
    return max_idx + std::numeric_limits<char>::min();
}


void unit_test()
{
    assert(find_most_frequent("hello world") == 'l');
    assert(find_most_frequent("aaabb") == 'a');
    assert(find_most_frequent("aaabbccc") != 'b');
    assert(find_most_frequent("aaaaaaaaaaa") == 'a');
    assert(find_most_frequent("abababb") == 'b');
}

int main ()
{
    using namespace boost;
    using namespace boost::mpl;
    using namespace boost::fusion;
    using namespace boost::phoenix;
    using namespace boost::phoenix::arg_names;
    using namespace boost::random;
    using namespace boost::assign;

    std::string substr("hello world"), str("hello world");

    boost::benchmark_finder<std::string, std::string,
        boost::mpl::vector<
            boost::naive_search,
            boost::knuth_morris_pratt,
            boost::boyer_moore,
            //boost::suffix_array_search,
            boost::rabin_karp32/*,
            boost::rabin_karp64*/
        >,
       boost::is_equal> b;

    b.set_substring(&substr); b.set_string(&str);
    std::cout << "Doing almost no work:" << std::endl;
    for (unsigned int i=0; i<5000; ++i) b.find_first();
    b.output_stats(std::cout);

    b.clear();

    std::string const &benchmark_path = "E:/gsoc-boost/benchmarks";

    std::vector<std::string> benchmark_files = list_of
        ("dblp.xml.200MB") ("dna.200MB") ("english.200MB") ("pitches.50MB")
        ("proteins.200MB") ("sources.200MB") ("random1.50MB");
    boost::for_each(benchmark_files, arg1 = benchmark_path + "/" + arg1);

    unit_test();

    //Category 1: The text
    //  Test 1: Against long nonmatching substring
    //  Test 2: Against almost matching substrings (differing in ending only)
    //  Test 3: Against almost matching substrings (differing in beginning only)
    //  Test 4: Against matching substrings
    //  Test 5: Against the most frequent character
    bool substr_change = false;
    try {
        //!\todo move back to starting at step 1
        for (unsigned int test = 1; test <= 5; ++test)
        {

            BOOST_FOREACH(std::string fn, benchmark_files)
            {
                try {
                    b.clear();
                    if (test == 1) { substr = rand_str(10000); b.set_substring(&substr); }
                } catch (std::exception const &e) { BOOST_THROW_EXCEPTION(e); }

                std::cout << "Test " << test << " (" << fn << "):" << std::endl;
            
                std::ifstream bf(fn, std::ios::binary);
                if (!bf) { std::cerr << "Error opening " << fn << std::endl; return -1; }

                std::vector<char> buf(1<<20);
                while (bf.read(&buf[0], mrand(1000,1000000)))
                {
                    //Note: Only reading the first 10MB of the file!!
                    if (bf.tellg() > 10*1024*1024) break;

                    try {
                        assert(bf.gcount() <= buf.size());
                        str.assign(buf.begin(), buf.begin()+static_cast<std::size_t>(bf.gcount()));
                    } catch (std::exception const &e) { BOOST_THROW_EXCEPTION(e); }

                    substr_change = false;
                    try {
                        switch (test)
                        {
                        case 1:
                            break;
                        case 2:
                            substr = rand_obfuscate_end(rand_substr(str));
                            substr_change = true;
                            break;
                        case 3:
                            substr = rand_obfuscate_beginning(rand_substr(str));
                            substr_change = true;
                            break;
                        case 4:
                            substr = rand_substr(str);
                            substr_change = true;
                            break;
                        case 5:
                            substr = find_most_frequent(str);
                            substr_change = true;
                            break;
                        }
                    } catch (std::exception const &e) { BOOST_THROW_EXCEPTION(e); }
                    try {
                        if (substr_change) b.set_substring(&substr);
                        b.set_string(&str);
                    } catch (std::exception const &e) { BOOST_THROW_EXCEPTION(e); }
                    while (boost::distance(b.find_next()));
                } // end while (read)
                b.output_stats(std::cout);
            } // end foreach file
        } // end for(test)
    } // end try
    catch (boost::exception const &e)
    {
        std::cerr << boost::diagnostic_information(e);
    }
    catch (std::exception const &e)
    {
        std::cerr << boost::diagnostic_information(e);
    }
    return 0;
}
