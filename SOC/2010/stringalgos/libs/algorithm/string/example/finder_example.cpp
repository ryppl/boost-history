#include <string>
#include <vector>
#include <iostream>

#include <boost/algorithm/string/string_search.hpp>
#include <boost/algorithm/string/finder.hpp>
#include <boost/algorithm/string/case_conv.hpp>

#include <boost/range/algorithm/copy.hpp>

int main ()
{

//Example 1: KMP

	typedef boost::finder_t<std::wstring, std::wstring, boost::knuth_morris_pratt> finder;
	finder f;
	
	f.set_string(L"The world is mine"); // set the string to search for: "The world is mine"
	f.set_substring(L"mine"); // set the pattern to search for: "mine" 
    std::wstring::difference_type match = f.find_first_index(); // searches pattern "mine" in text
                                                               // "The world is mine" 
    if (match != static_cast<std::string::difference_type>(-1))
        std::wcout << L"Found a match at position " << match << std::endl;

    f.set_substring(L"world"); // Note: the string stays the same, only the substring was changed
    match = f.find_first_index(); // searches pattern "world" in text "The world is mine"
    if (match != static_cast<std::string::difference_type>(-1))
        std::wcout << L"Found a match at position " << match << std::endl;

    // Turn "Hello world" into "Hello WORLD"
    std::wstring mystr(L"Hello world");
    f.set_string(mystr);
    match = f.find_first_index(); // searches pattern "world" in text "Hello world"
    boost::to_upper( boost::make_iterator_range(mystr.begin()+match,mystr.begin()+match+5) );
    std::wcout << mystr << std::endl;
    
    // Turn "Hello WORLD" into "HELLO WORLD"
    f.set_string(&mystr);
    f.set_substring(L"Hello");
    finder::string_range_type match2 = f.find_first(); // because we've passed the string as a
                                                       // pointer, no internal copy of the text
                                                       // was made, and we can use find_first()
                                                       // to get a range of the match
    boost::to_upper(match2);
    std::wcout << mystr << std::endl;

// Example 2: Searching with suffix arrays

    typedef boost::finder_t<std::vector<wchar_t>, std::wstring, boost::suffix_array_search,
        boost::is_equal> finder2;
    //the pattern's type is a vector of wchar_t-s, whereas the text's type is a wstring
    finder2 f2;
    f2.set_string(
        L"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer interdum elit ac orci "
        L"fermentum in pretium lectus facilisis. Vestibulum rutrum convallis justo nec fringilla. "
        L"Sed magna justo, sollicitudin sed viverra in, lacinia quis metus. Integer volutpat, nisl "
        L"rhoncus condimentum accumsan, est mi fermentum lacus, sed imperdiet sem erat a risus. "
        ); // Note: this works by making an internal copy (or move if the compiler supports it) of
           // the given text
    f2.set_substring(L"consectetur");

    boost::to_upper(f2.find_first()); // finds consectetur in the internal copy
                                      // then makes it uppercase
    //turns all occurences of letter e into uppercase
    f2.set_substring(L"e");
    for (finder2::string_range_type range = f2.find_first();
        boost::begin(range) != boost::end(range); range = f2.find_next())
    {
        boost::to_upper(range);
    }

    //display the internal copy of the text
    boost::copy(f2.get_string_range(), std::ostream_iterator<wchar_t,wchar_t>(std::wcout));
    std::wcout << std::endl;
    
    std::cin.get();
	return 0;
}
