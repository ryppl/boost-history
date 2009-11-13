//////////////////////////////////////////////////////////////////////////////
// detail::fusion::map::serialization::loader.hpp                           //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_FUSION_MAP_SERIALIZATION_LOADER_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_FUSION_MAP_SERIALIZATION_LOADER_HPP_ER_2009
#include <boost/fusion/support/pair.hpp>
#include <boost/fusion/include/pair.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/include/for_each.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace fusion{
namespace map{

// Implementation is suggested here
// http://www.boost.org/doc/libs/1_40_0/libs/fusion/doc/html/fusion/quick_start.html
// under AssociativeContainer
template<typename Ia>
struct loader
{

	loader(Ia& ar):ar_(ar){}
    loader(const loader& that):ar_(that.ar_){}

    template <typename Pair>
    void operator()(Pair& data) const
    {
        (this->ar_ >> data.second);
    }

	private:
	Ia& ar_;    
};

template <typename Stuff,typename Ia>
void load(Stuff & stuff, Ia& ar)
{
	typedef loader<Ia> loader_;
    boost::fusion::for_each(stuff, loader_(ar));
}

}// map
}// fusion
}// detail
}// statistics
}// boost

#endif