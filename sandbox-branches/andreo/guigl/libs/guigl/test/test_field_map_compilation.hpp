using namespace boost;

using namespace boost::field_map;

#define N BOOST_PP_ITERATION()

#define labelN BOOST_PP_CAT(label,N)
#define sizeN BOOST_PP_CAT(size,N)
#define volumeN BOOST_PP_CAT(volume,N)
#define named_constructableN BOOST_PP_CAT(named_constructable,N)

BOOST_FIELD_MAP_PARAMETER(labelN, std::string)
BOOST_FIELD_MAP_PARAMETER(sizeN, int)
BOOST_FIELD_MAP_PARAMETER(volumeN, double)


class named_constructableN
{
public:
    // a macro could extend the number of parameters if desired
    template<typename FM0, typename FM1, typename FM2>
    named_constructableN(const FM0 &a0, const FM1 &a1, const FM2 &a2)
    {        
        label = (a0, a1, a2)[labelN];
        size = (a0, a1, a2)[sizeN];
        volume = (a0, a1, a2)[volumeN];
    }
    std::string label;
    int size;
    double volume;
};


BOOST_AUTO_TEST_CASE( BOOST_PP_CAT(test_multi_param_construction,N) )
{    
    named_constructableN constructed(labelN = "hello", sizeN = 1, volumeN = 1.0);
    BOOST_CHECK_EQUAL(constructed.label, "hello");
    BOOST_CHECK_EQUAL(constructed.size, 1);    
    BOOST_CHECK_EQUAL(constructed.volume, 1.0);    

}