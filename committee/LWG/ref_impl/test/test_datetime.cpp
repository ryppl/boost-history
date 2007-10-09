
//g++-4.0 -I ../ test_datetime.cpp -o test_date


#include <date_time>
#include <iostream>

using std::system_time;
using std::seconds;
using std::minutes;
using std::hours;
using std::milliseconds;
using std::microseconds;
using std::nanoseconds;


void do_conversions()
{
  {
    minutes m(1);
    seconds s(m);
    std::cout << m.get_count() << " minute is " << s.get_count() << " seconds" << std::endl;
  }

  {
    hours h(1);
    seconds s(h);
    std::cout << h.get_count() << " hour is " << s.get_count() << " seconds" << std::endl;
  }

  {
    hours h(1);
    minutes m(h);
    std::cout << h.get_count() << " hour is " << m.get_count() << " minutes" << std::endl;
  }

  {
    seconds s(1);
    milliseconds ms(s);
    std::cout << s.get_count()  << " second is " 
              << ms.get_count() << " milli seconds" << std::endl;
  }
  {
    seconds s(1);
    microseconds ms(s);
    std::cout << s.get_count()  << " second is " 
              << ms.get_count() << " micro seconds" << std::endl;
  }

  {
    seconds s(1);
    nanoseconds ns(s);
    std::cout << s.get_count()  << " second is " 
              << ns.get_count() << " nano seconds" << std::endl;
  }


};



int
main()
{

  system_time now = std::get_system_time();
  
  //nanosec since 1-1-1970
  std::cout << now.get_rep() << std::endl;

  system_time later = now + nanoseconds(5);
  std::cout << later.get_rep() << std::endl;
  later = now + seconds(10);
  std::cout << later.get_rep() << std::endl;


  nanoseconds ns(5);
  ns += nanoseconds(3);
  ns = -ns;

  std::cout << ns.get_count() << std::endl;

  nanoseconds ns2 = std::get_system_time() - now;
  std::cout << ns2.get_count() << std::endl;


  nanoseconds ns3 = nanoseconds(3) + microseconds(3) - seconds(3);
  std::cout << ns3.get_count() << std::endl;

  //  nanoseconds ns4 = seconds(3) + nanoseconds(2); //illegal
  
  seconds s(1);
  seconds s2 = s + seconds(3) + seconds(5);
  std::cout << s2.get_count() << std::endl;

  system_time now2 = std::get_system_time();
  std::cout << "now2:       " <<  now2.get_rep() << std::endl;
  now2 += seconds(1);
  std::cout << "now2 + 1s:  " <<  now2.get_rep() << std::endl;
  now2 += minutes(1);
  std::cout << "now2 + 1m:  " <<  now2.get_rep() << std::endl;
  now2 += milliseconds(1);
  std::cout << "now2 + 1ms: " <<  now2.get_rep() << std::endl;


  do_conversions();
  
  return 0;

}
