#include <boost/indexed_set.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <iterator>

using namespace boost::indexed_sets;

int main()
{
  typedef indexed_set<
    boost::shared_ptr<int>,
    index_list<
      unique<identity<int> >
    >
  > ptr_int_set;

  ptr_int_set pis;

  pis.insert(boost::shared_ptr<int>(new int(0)));
  pis.insert(boost::shared_ptr<int>(new int(5)));
  pis.insert(boost::shared_ptr<int>(new int(7)));
  pis.insert(boost::shared_ptr<int>(new int(3)));
  pis.insert(boost::shared_ptr<int>(new int(4)));
  pis.insert(boost::shared_ptr<int>(new int(8)));
  pis.insert(boost::shared_ptr<int>(new int(1)));
  pis.insert(boost::shared_ptr<int>(new int(9)));
  pis.insert(boost::shared_ptr<int>(new int(2)));
  pis.insert(boost::shared_ptr<int>(new int(6)));

  for(ptr_int_set::iterator it=pis.begin();it!=pis.end();++it){
    std::cout<<**it;
  }

  return 0;
}
