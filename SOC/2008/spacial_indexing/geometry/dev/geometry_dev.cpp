#include <vector>
#include <string>
#include <iostream>

#include <boost/numeric/conversion/bounds.hpp>
#include <boost/limits.hpp>

#include <geometry/geometry.hpp>



int main() 
{

  std::cout << "numeric::bounds versus numeric_limits example.\n";

  std::cout << "The maximum value for float:\n";
  std::cout << boost::numeric::bounds<float>::highest() << "\n";
  std::cout << std::numeric_limits<float>::max() << "\n";

  std::cout << "The minimum value for float:\n";
  std::cout << boost::numeric::bounds<float>::lowest() << "\n";
  std::cout << -std::numeric_limits<float>::max() << "\n";

  std::cout << "The smallest positive value for float:\n";
  std::cout << boost::numeric::bounds<float>::smallest() << "\n";
  std::cout << std::numeric_limits<float>::min() << "\n";

  try
  {
	  //typedef geometry::point<double, 2> XYZ ;
	  typedef geometry::point_xy<double> XYZ;
	  XYZ xyz(1,2);
	  XYZ p2(1,4);
	  std::cout << ((p2 < xyz) ? "kleiner" : "groter") << std::endl;
  }
  catch(...)
  {
	  std::cout << "error!";
  }

  return 0;
}

