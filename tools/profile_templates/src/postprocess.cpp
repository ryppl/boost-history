#include <boost/regex.hpp>
#include <string>
#include <iostream>
#include <map>
#include <exception>
#include <iterator>
#include <algorithm>
#include <iomanip>

#ifdef _MSC_VER

boost::regex warning_message("(.*) : warning C4150: deletion of pointer to incomplete type 'template_profiler::incomplete'; no destructor called");

#elif defined(__GNUC__)

boost::regex warning_message("(.*): warning: division by zero in .template_profiler::value / 0.");

#else

#error Unknown Compiler

#endif

struct print {
    int* cummulative;
    int width;
    typedef void result_type;
    template<class T>
    void operator()(const T& t) {
        *cummulative += t.second;
        std::cout << std::setw(width) << t.first << std::setw(10) << t.second << std::setw(10) << *cummulative << std::endl;
    }
};

struct compare {
    template<class T>
    bool operator()(const T& lhs, const T& rhs) const {
        return(lhs.second > rhs.second);
    }
};

int main() {
    std::map<std::string, int> messages;
    std::string line;
    int total_matches = 0;
    std::ptrdiff_t max_match_length = 0;
    while(std::getline(std::cin, line)) {
        boost::smatch match;
        if(boost::regex_match(line, match, warning_message)) {
            max_match_length = (std::max)(max_match_length, match[1].length());
            ++messages[match[1]];
            ++total_matches;
        }
    }
    std::vector<std::pair<std::string, int> > copy(messages.begin(), messages.end());
    std::sort(copy.begin(), copy.end(), compare());
    std::cout << "Total instantiations: " << total_matches << std::endl;
    int cummulative = 0;
    std::cout << std::setw(max_match_length) << "Location" << std::setw(10) << "count" << std::setw(10) << "cum." << std::endl;
    std::cout << std::setfill('-') << std::setw(max_match_length + 20) << '-' << std::setfill(' ') << std::endl;
    print p = { &cummulative, max_match_length };
    std::for_each(copy.begin(), copy.end(), p);
}
