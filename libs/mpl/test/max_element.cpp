#include "boost/mpl/list_c.hpp"
#include "boost/mpl/max_element.hpp"
#include "boost/mpl/comparison/less.hpp"

namespace mpl = boost::mpl;

int main()
{
  using namespace mpl::placeholder;

  typedef mpl::list_c<int, 3, 4, 2, 0, -5, 8, -1, 7>::type numbers;

  typedef mpl::max_element< numbers, mpl::less<_,_> >::type max_el;
  typedef max_el::type max_number;

  std::cout << max_number();
}