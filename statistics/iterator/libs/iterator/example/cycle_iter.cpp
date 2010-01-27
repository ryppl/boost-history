#include <vector>
#include <boost/range.hpp>
#include <boost/iterator/cycle_iterator.hpp>
#include <boost/iterator/cycle_iterator_ext.hpp>

void example_iter(std::ostream& os){

    using namespace boost;
    typedef double val_;
    typedef std::vector<val_> vec_;
    typedef range_size<vec_>::type size_;
    typedef range_iterator<vec_>::type base_it_;
    const unsigned n = 10;
    vec_ vec(n);

    typedef cycle_iterator<base_it_,size_> it_;
    it_ it(boost::begin(vec),boost::end(vec),0);
    it_ it2;
    //it2 = it;
    
    typedef cycle_iterator_ext<base_it_,size_> it_ext_;
    it_ext_ it_ext(boost::begin(vec),boost::end(vec),0);

    it_ext_ it_ext2;
    it_ext2 = it_ext;
    
    os << "end";
    
}