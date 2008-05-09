#include <boost/regex.hpp>
#include <string>
#include <iostream>
#include <map>
#include <exception>
#include <iterator>
#include <algorithm>

#ifdef _MSC_VER

boost::regex warning_message("(.*) : warning C4150: deletion of pointer to incomplete type 'template_profiler::incomplete'; no destructor called");

#elif defined(__GNUC__)

boost::regex warning_message("(.*): warning: division by zero in `template_profiler_value / 0'");

#else

#error Unknown Compiler

#endif

struct print {
    int* cummulative;
    typedef void result_type;
    template<class T>
    void operator()(const T& t) {
        *cummulative += t.second;
        std::cout << t.first << ' ' << t.second << " " << *cummulative << std::endl;
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
    while(std::getline(std::cin, line)) {
        boost::smatch match;
        if(boost::regex_match(line, match, warning_message)) {
            ++messages[match[1]];
            ++total_matches;
        }
    }
    std::vector<std::pair<std::string, int> > copy(messages.begin(), messages.end());
    std::sort(copy.begin(), copy.end(), compare());
    std::cout << "Total instantiations: " << total_matches << std::endl;
    int cummulative = 0;
    print p = { &cummulative };
    std::for_each(copy.begin(), copy.end(), p);
}
