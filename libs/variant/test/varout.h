

#ifndef _VAROUTH_INC_
#define _VAROUTH_INC_

#include <sstream>
#include <string>
#include <iostream>

struct to_text
{
   typedef std::string return_type;
   
   template<class Value>
   std::string operator()(const Value& value) const
   {
      std::ostringstream ost;      
      ost << "[V] " << value;
      
      return ost.str();
   }

}; //to_text


template<typename T0, typename T1, typename T2, typename T3>
std::ostream& operator<<(std::ostream& out, 
   const boost::variant<T0, T1, T2, T3>& a)
{
   out << boost::apply_visitor(to_text(), a);
   return out;
}



#endif //_VAROUTH_INC_

