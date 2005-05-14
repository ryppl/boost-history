#include <boost/indexed_set.hpp>
#include <boost/indexed_set/sequenced_index.hpp>
#include <list>
#include <set>
#include <iostream>
#include <windows.h>

using namespace boost::indexed_sets;

typedef indexed_set<
  int,
  index_list<
    sequenced<>,
    unique<identity<int> >,
    unique<identity<int>,std::greater<int> >
  >
> int_xset;

struct greaterp
{
  bool operator()(const int* x,const int* y)const{return *x<*y;}
};

typedef std::list<int> int_list;
typedef std::set<const int*> int_set;
typedef std::set<const int*,greaterp> int_gset;

int main()
{
  __int64 t1=0,t2=0;

  for(int n=0;n<10000;++n){
    __int64 start,finish;

    QueryPerformanceCounter((LARGE_INTEGER*)&start);
    {
      int_xset is;
      for(int i=0;i<1000;++i)is.insert(is.begin(),i);
      for(int_xset::iterator it=is.begin();it!=is.end();)is.erase(it++);
    }
    QueryPerformanceCounter((LARGE_INTEGER*)&finish);
    t1+=finish-start;

    QueryPerformanceCounter((LARGE_INTEGER*)&start);
    {
      int_list il;
      int_set is;
      int_gset igs;
      for(int i=0;i<1000;++i){
        int_list::iterator it=il.insert(il.begin(),i);
        is.insert(&*it);
        igs.insert(&*it);
      }
      for(int_list::iterator it=il.begin();it!=il.end();){
        igs.erase(&*it);
        is.erase(&*it);
        il.erase(it++);
      }
    }
    QueryPerformanceCounter((LARGE_INTEGER*)&finish);
    t2+=finish-start;
  }

  std::cout<<(unsigned)t1<<std::endl;
  std::cout<<(unsigned)t2<<std::endl;
  std::cout<<(unsigned)(10000.*(double)t1/(double)t2)<<std::endl;

  return 0;
}
