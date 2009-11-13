//////////////////////////////////////////////////////////////////////////////
// detail::fusion::map::serialization::saver.hpp                            //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_FUSION_MAP_SERIALIZATION_SAVER_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_FUSION_MAP_SERIALIZATION_SAVER_HPP_ER_2009
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
template<typename Oa>
struct saver
{

	saver(Oa& ar):ar_(ar){}
    saver(const saver& that):ar_(that.ar_){}

    template <typename Pair>
    void operator()(Pair const& data) const
    {
        this->ar_ << data.second;
    }

	private:
	Oa& ar_;    
};

template <typename Stuff,typename Oa>
void save(Stuff const& stuff,Oa& ar)
{
	typedef saver<Oa> saver_;
    boost::fusion::for_each(stuff, saver_(ar));
}


}// map
}// fusion
}// detail
}// statistics
}// boost

#endif