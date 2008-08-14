#define N BOOST_PP_ITERATION()

#define labelN BOOST_PP_CAT(label,N)
#define sizeN BOOST_PP_CAT(size,N)
#define volumeN BOOST_PP_CAT(volume,N)
#define _labelN BOOST_PP_CAT(_label,N)
#define _sizeN BOOST_PP_CAT(_size,N)
#define _volumeN BOOST_PP_CAT(_volume,N)

#define named_constructableN BOOST_PP_CAT(named_constructable,N)

BOOST_PARAMETER_NAME(labelN)
BOOST_PARAMETER_NAME(sizeN)
BOOST_PARAMETER_NAME(volumeN)

class named_constructableN
{
public:
    template<typename FM0, typename FM1, typename FM2>
    named_constructableN(const FM0 &a0, const FM1 &a1, const FM2 &a2)
    {
        label = (a0, a1, a2)[_labelN];
        size = (a0, a1, a2)[_sizeN];
        volume = (a0, a1, a2)[_volumeN];
    }
    std::string label;
    int size;
    double volume;
};


BOOST_AUTO_TEST_CASE( BOOST_PP_CAT(test_multi_param_construction,N) )
{
    named_constructableN constructed(_labelN = "hello", _sizeN = 1, _volumeN = 1.0);
    BOOST_CHECK_EQUAL(constructed.label, "hello");
    BOOST_CHECK_EQUAL(constructed.size, 1);    
    BOOST_CHECK_EQUAL(constructed.volume, 1.0);    

}