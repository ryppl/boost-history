#include <boost/math/tools/ntl.hpp>
#include <boost/math/tools/test_data.hpp>
#include <boost/test/included/test_exec_monitor.hpp>
#include <boost/math/special_functions/ellint_1.hpp>
#include <fstream>
#include <boost/math/tools/test_data.hpp>
#include <boost/tr1/random.hpp>

using namespace boost::math::tools;
using namespace boost::math;
using namespace std;

float extern_val;
// confuse the compilers optimiser, and force a truncation to float precision:
float truncate_to_float(float const * pf)
{
   extern_val = *pf;
   return *pf;
}



template<class T>
T ellint_f_data(T phi, T k)
{
   return ellint_1(phi, k);
}

int test_main(int argc, char*argv [])
{
   using namespace boost::math::tools;

   NTL::RR::SetOutputPrecision(50);
   NTL::RR::SetPrecision(1000);

   parameter_info<NTL::RR> arg1, arg2;
   test_data<NTL::RR> data;

   bool cont;
   std::string line;

   if(argc < 1)
      return 1;

   do{
      if(0 == get_user_parameter_info(arg1, "phi"))
         return 1;
      if(0 == get_user_parameter_info(arg2, "k"))
         return 1;

      data.insert(&ellint_f_data<NTL::RR>, arg1, arg2);

      std::cout << "Any more data [y/n]?";
      std::getline(std::cin, line);
      boost::algorithm::trim(line);
      cont = (line == "y");
   }while(cont);

   std::cout << "Enter name of test data file [default=ellint_f.ipp]";
   std::getline(std::cin, line);
   boost::algorithm::trim(line);
   if(line == "")
      line = "ellint_f.ipp";
   std::ofstream ofs(line.c_str());
   line.erase(line.find('.'));
   ofs << std::scientific;
   write_code(ofs, data, line.c_str());

   return 0;
}

