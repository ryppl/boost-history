#include <boost/algorithm/string/benchmark_finder.hpp>

#include <boost/algorithm/string/find.hpp>
#include <boost/algorithm/string/finder.hpp>
#include <boost/algorithm/string/string_search.hpp>

//#include <boost/algorithm/string/string_search/knuth_morris_pratt2.hpp>

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
#include <ctime>

//replace to a constant to compare multiple benchmarks
unsigned int get_seed() { return 123; }

unsigned int mrand (unsigned int min, unsigned int max)
{
    static boost::mt19937 rng;
    static bool init = false;
    if (!init) { init = true; rng.seed(static_cast<unsigned int>(get_seed())); }
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
    unsigned int offset = mrand(0, str.size()-size);
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
        (std::numeric_limits<char>::max)() -
        (std::numeric_limits<char>::min)() + 1;
    std::vector<unsigned int> freq(char_range_size);
    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
    {
        //!\todo reuse old one
        //++freq[*it - std::numeric_limits<char>::min()];
        try
        {
            ++freq.at((int)*it - (std::numeric_limits<char>::min)());
        }
        catch (std::exception const &)
        {
                BOOST_THROW_EXCEPTION(std::runtime_error(
                (boost::format("Tried to increment index %1% of the vector") % ((int)*it - (std::numeric_limits<char>::min)())).str()
                ));
        }
        
    }
    unsigned int max_idx = 0, max = freq[0];
    for (unsigned int i = 1; i < char_range_size; ++i)
        if (freq[i] > max) { max = freq[i]; max_idx = i; }
    return max_idx + (std::numeric_limits<char>::min)();
}

unsigned int count_different_chars(std::string const &str, std::string const &str2)
{
    assert(str.size() == str2.size());
    std::string::size_type size = str.size();
    unsigned int ret = 0;
    for (unsigned int i = 0; i < size; ++i)
        if (str[i] != str2[i]) ++ret;
    return ret;
}

void unit_test()
{
    assert(find_most_frequent("hello world") == 'l');
    assert(find_most_frequent("aaabb") == 'a');
    assert(find_most_frequent("aaabbccc") != 'b');
    assert(find_most_frequent("aaaaaaaaaaa") == 'a');
    assert(find_most_frequent("abababb") == 'b');
    std::string str( rand_str(10000));
    for (unsigned int i = 0; i < 100; ++i)
    {
        std::string substr( rand_substr(str) );
        std::string::difference_type offset = str.find(substr);
        std::string::iterator begin = boost::begin( boost::algorithm::find_first(str, substr) );
        assert(begin == str.begin()+offset && begin != str.end());
    }
    for (unsigned int i = 0; i < 100; ++i)
    {
        std::string str2 = rand_str(10);
        std::string obfuscate_beginning = rand_obfuscate_beginning(str2),
            obfuscate_end = rand_obfuscate_end(str2);
        assert(count_different_chars(str2,obfuscate_beginning)<=1);
        assert(count_different_chars(str2,obfuscate_end)<=1);
    }
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

    boost::benchmark_finder_t<std::string, std::string,
        boost::mpl::vector<
            boost::naive_search,
            boost::knuth_morris_pratt,
            boost::boyer_moore,
            boost::suffix_array_search,
            boost::rabin_karp32,
            boost::rabin_karp64
        >,
       boost::is_equal> b;

    unit_test();

    b.set_substring(&substr); b.set_string(&str);
    std::cout << "Doing almost no work:" << std::endl;
    for (unsigned int i=0; i<5000; ++i) b.find_first();
    b.output_stats(std::cout);

    b.clear();

    //Use your own files here to benchmark.
    std::string const &benchmark_path = "E:/gsoc-boost/benchmarks";

    std::vector<std::string> benchmark_files = list_of
        ("dblp.xml.200MB") ("dna.200MB") ("english.200MB") ("pitches.50MB")
        ("proteins.200MB") ("sources.200MB") ("random1.50MB") ("binary.50MB");
    boost::for_each(benchmark_files, arg1 = benchmark_path + "/" + arg1);


    //Category 1: The substring and string change every iteration
    //  Test 1: Against long nonmatching substring
    //  Test 2: Against almost matching substrings (differing in ending only)
    //  Test 3: Against almost matching substrings (differing in beginning only)
    //  Test 4: Against matching substrings
    //  Test 5: Against the most frequent character
    //Category 2: The string stays constant, the substrings change
    //Category 3: The substring changes, the string stays constant
    bool substr_change = false;
    try {
        //Category 1
        for (unsigned int test = 1; test <= 4; ++test)
        {

            BOOST_FOREACH(std::string fn, benchmark_files)
            {
                try {
                    b.clear();
                    if (test == 1) { substr = rand_str(10000); b.set_substring(&substr); }
                } catch (std::exception const &e) { BOOST_THROW_EXCEPTION(e); }

                std::cout << "Test " << test << " (" << fn << "):" << std::endl;
            
                std::ifstream bf(fn.c_str(), std::ios::binary);
                if (!bf) { std::cerr << "Error opening " << fn << std::endl; return -1; }

                const unsigned int MAX_STR_SIZE = 10*1024*1024;//10;//1000000;
                const unsigned int MIN_STR_SIZE = 10*1024;//1000;
                std::vector<char> buf(MAX_STR_SIZE+32);
                while (bf.read(&buf[0], mrand(MIN_STR_SIZE,MAX_STR_SIZE)))
                {
                    //Note: Only reading the first x MB of the file!!
                    //if (bf.tellg() > 30*1024*1024) break;

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
                    //std::cerr << '+';
                    while (boost::distance(b.find_next())); //std::cerr << "+";
                    //std::cerr << "+";
                } // end while (read)
                b.output_stats(std::cout);
            } // end foreach file
        } // end for(test)

        //Category 2
        std::vector<std::string> str_list;

        std::string current_str;
        std::string repeated_substr = rand_str(1000),
            repeated_substr2 = rand_obfuscate_beginning(repeated_substr),
            repeated_substr3 = rand_obfuscate_end(repeated_substr);
        for (unsigned int i=0; i<5000; ++i) current_str += repeated_substr + rand_str(10);
        str_list.push_back(current_str); current_str.clear();
        for (unsigned int i=0; i<5000; ++i) current_str += repeated_substr2 + rand_str(10) + repeated_substr;
        str_list.push_back(current_str); current_str.clear();
        for (unsigned int i=0; i<5000; ++i) current_str += repeated_substr3 + rand_str(10) + repeated_substr;
        str_list.push_back(current_str);

        b.set_substring(&repeated_substr);
        for (unsigned int str_id = 0; str_id < str_list.size(); ++str_id)
        //for (unsigned int str_id = 0; str_id < 1; ++str_id)
        {
            b.clear();
            std::cout << "Testing against string #" << str_id << ':' << std::endl;
            b.set_string(&str_list[str_id]);
            while (boost::distance(b.find_next()));
            b.output_stats(std::cout);
        }
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
