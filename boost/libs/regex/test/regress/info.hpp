
#ifndef BOOST_REGEX_REGRESS_INFO_HPP
#define BOOST_REGEX_REGRESS_INFO_HPP
#include <iostream>
#include <string>
#include <boost/regex.hpp>
//
// class test info, 
// store information about the test we are about to conduct:
//
template <class charT>
class test_info
{
public:
   typedef std::basic_string<charT> string_type;
private:
   struct data_type
   {
      std::string file;
      int line;
      string_type expression;
      boost::regex_constants::syntax_option_type options;
      string_type search_text;
      boost::regex_constants::match_flag_type match_options;
      const int* answer_table;
      string_type format_string;
      string_type result_string;
      bool need_to_print;
   };
   static data_type& data()
   {
      static data_type d;
      return d;
   }
public:
   test_info(){};
   static void set_info(
      const char* file,
      int line,
      const string_type& ex,
      boost::regex_constants::syntax_option_type opt,
      const string_type& search_text = string_type(),
      boost::regex_constants::match_flag_type match_options = boost::match_default,
      const int* answer_table = 0,
      const string_type& format_string = string_type(),
      const string_type& result_string = string_type())
   {
      data_type& dat = data();
      dat.file = file;
      dat.line = line;
      dat.expression = ex;
      dat.options = opt;
      dat.search_text = search_text;
      dat.match_options = match_options;
      dat.answer_table = answer_table;
      dat.format_string = format_string;
      dat.result_string = result_string;
      dat.need_to_print = true;
   }

   static const string_type& expression()
   {
      return data().expression;
   }
   static boost::regex_constants::syntax_option_type syntax_options()
   {
      return data().options;
   }
   static const string_type& search_text()
   {
      return data().search_text;
   }
   static boost::regex_constants::match_flag_type match_options()
   {
      return data().match_options;
   }
   static const int* answer_table()
   {
      return data().answer_table;
   }
   static const string_type& format_string()
   {
      return data().format_string;
   }
   static const string_type& result_string()
   {
      return data().result_string;
   }  
   static bool need_to_print()
   {
      return data().need_to_print;
   }
   static const std::string& file()
   {
      return data().file;
   }
   static int line()
   {
      return data().line;
   }
   static void clear()
   {
      data().need_to_print = false;
   }
};

template <class charT>
std::ostream& operator<<(std::ostream& os, const test_info<charT>&)
{
   if(test_info<charT>::need_to_print())
   {
      os << test_info<charT>::file() << ":" << test_info<charT>::line() << ": Error in test here:" << std::endl;
      test_info<charT>::clear();
   }
   return os;
}
//
// define some test macros:
//
extern int error_count;

#define BOOST_REGEX_TEST_ERROR(msg, charT)\
   ++error_count;\
   std::cerr << test_info<charT>();\
   std::cerr << "  " << __FILE__ << ":" << __LINE__ << ":" << msg << std::endl

#endif

