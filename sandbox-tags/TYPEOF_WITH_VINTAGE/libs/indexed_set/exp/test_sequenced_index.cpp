#define BOOST_INDEXED_SET_ENABLE_INVARIANT_CHECKING
#define BOOST_INDEXED_SET_ENABLE_SAFE_MODE

#include <boost/indexed_set.hpp>
#include <boost/indexed_set/sequenced_index.hpp>
#include <algorithm>
#include <iostream>
#include <iterator>

using namespace boost::indexed_sets;

int main()
{
  typedef indexed_set<
    int,
    index_list<
      non_unique<identity<int> >,
      sequenced<>,
      sequenced<>
    >
  > sequenced_set;
  typedef nth_index_type<sequenced_set,1>::type sequenced_index;

  sequenced_set    ss;
  sequenced_index& si=get<1>(ss);

  si.push_front(0);                       /* 0        */
  si.push_front(4);                       /* 40       */
  ss.insert(2);                           /* 402      */
  ss.insert(5);                           /* 4025     */
  si.push_front(3);                       /* 34025    */
  si.push_back(6);                        /* 340256   */
  si.push_back(1);                        /* 3402561  */
  si.insert(project<1>(ss,ss.find(2)),8); /* 34082561 */

  ss.erase(6);                            /* 3408251  */

  std::copy(ss.begin(),ss.end(),std::ostream_iterator<int>(std::cout));
  std::cout<<std::endl;
  std::copy(si.begin(),si.end(),std::ostream_iterator<int>(std::cout));
  std::cout<<std::endl;

  si.reverse();                           /* 1528043  */
  std::copy(si.begin(),si.end(),std::ostream_iterator<int>(std::cout));
  std::cout<<std::endl;

  si.splice(
    project<1>(ss,ss.find(5)),si,
    project<1>(ss,ss.find(8)),
    project<1>(ss,ss.find(3)));           /* 1804523  */
  std::copy(si.begin(),si.end(),std::ostream_iterator<int>(std::cout));
  std::cout<<std::endl;

  {
    sequenced_set    ss;
    sequenced_index& si=get<1>(ss);
    for(int i=32;i--;){
      si.push_back(i);
    }
    std::cout<<"filled"<<std::endl;

    try{si.sort();}catch(...){}
    std::copy(si.begin(),si.end(),std::ostream_iterator<int>(std::cout,","));
    std::cout<<std::endl;

    try{si.sort(std::greater<int>());}catch(...){}
    std::copy(si.begin(),si.end(),std::ostream_iterator<int>(std::cout,","));
    std::cout<<std::endl;
  }

  ss.clear();
  ss.insert(2);
  ss.insert(5);
  ss.insert(7);                           /* 257 */

  sequenced_set ss1;
  sequenced_index& si1=get<1>(ss1);
  for(int i=0;i<10;++i)ss1.insert(i);

  si.merge(si1);                          /* 0122345567789  */
  std::copy(si.begin(),si.end(),std::ostream_iterator<int>(std::cout));
  std::cout<<std::endl;

  return 0;
}
