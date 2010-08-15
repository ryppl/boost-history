#include <string>
#include <vector>
#include <iostream>

#include <boost/algorithm/string/string_search.hpp>
#include <boost/algorithm/string/finder.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/finder_aliases.hpp>

#include <boost/range/algorithm/copy.hpp>

int main ()
{

//Example 1: KMP

	//typedef boost::finder_t<std::string, std::string, boost::knuth_morris_pratt> finder;
    //OR
    typedef boost::knuth_morris_pratt_finder finder;
	finder f;
	
	f.set_string("The world is mine"); // set the string to search for: "The world is mine"
	f.set_substring("mine"); // set the pattern to search for: "mine" 
    std::string::difference_type match = f.find_first_index(); // searches pattern "mine" in text
                                                                // "The world is mine"
                                                                // returns an index
    if (match != static_cast<std::string::difference_type>(-1))
        std::cout << "Found a match at position " << match << std::endl;

    f.set_substring("world"); // Note: the string stays the same, only the substring was changed
    match = f.find_first_index(); // searches pattern "world" in text "The world is mine"
    if (match != static_cast<std::string::difference_type>(-1))
        std::cout << "Found a match at position " << match << std::endl;

    // Turn "Hello world" into "Hello WORLD"
    std::string mystr("Hello world");
    f.set_string(mystr);
    match = f.find_first_index(); // searches pattern "world" in text "Hello world"
    finder::string_range_type match2(mystr.begin()+match, mystr.begin()+match+5);
    boost::to_upper(match2);
    std::cout << mystr << std::endl;
    
    // Turn "Hello WORLD" into "HELLO WORLD"
    f.set_string(&mystr);
    f.set_substring("Hello");
    match2 = f.find_first(); // because we've passed the string as a
                             // pointer, no internal copy of the text
                             // was made, and we can use find_first()
                             // to get a range of the match
    boost::to_upper(match2);
    std::cout << mystr << std::endl;

// Example 2: Searching with suffix arrays

    typedef boost::finder_t<std::vector<char>, std::string, boost::suffix_array_search,
        boost::is_equal> finder2;
    //the pattern's type is a vector of char-s, whereas the text's type is a string
    finder2 f2;
    f2.set_string(
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer interdum elit ac orci "
        "fermentum in pretium lectus facilisis. Vestibulum rutrum convallis justo nec fringilla. "
        "Sed magna justo, sollicitudin sed viverra in, lacinia quis metus. Integer volutpat, nisl "
        "rhoncus condimentum accumsan, est mi fermentum lacus, sed imperdiet sem erat a risus. "
        ); // Note: this works by making an internal copy (or move if the compiler supports it) of
           // the given text
    f2.set_substring("consectetur");

    match2 = f2.find_first();
    boost::to_upper(match2); // finds consectetur in the internal copy
                             // then makes it uppercase
    
    // Changes have occurred in the internal copy of the string from the outside, the finder
    // has no way of knowing. Call use_internal_string() in order for the finder to
    // obtain a new range from its internal string
    f2.use_internal_string();

    //turns all occurrences of letter e into uppercase
    f2.set_substring("e");
    for (finder2::string_range_type range = f2.find_first();
        boost::begin(range) != boost::end(range); range = f2.find_next())
    {
        boost::to_upper(range);
    }

    //display the internal copy of the text, after updating the internal range
    f2.use_internal_string();
    boost::copy(f2.get_string_range(), std::ostream_iterator<char,char>(std::cout));
    std::cout << std::endl;

// Example 3: Using finder_t with boyer_moore
    boost::boyer_moore_finder f3;

    std::string apple("apple");
    f3.set_substring(&apple); // you need to guarantee on the lifetime of apple to do this

    std::string coolsaying(
        "If you have an apple and I have an apple and we exchange these apples then you "
        "and I will still each have one apple. But if you have an idea and I have an "
        "idea and we exchange these ideas, then each of us will have two ideas."
    );
    f3.set_string(&coolsaying); // you need to guarantee on the lifetime of coolsaying to do this

    match2 = f3.find_first();
    boost::to_upper(match2); // turn the first occurence of apple uppercase
                             // modifications occur directly in coolsaying, since no copy
                             // was made
    std::cout << coolsaying << std::endl;


    std::cin.get();
    return 0;
}
