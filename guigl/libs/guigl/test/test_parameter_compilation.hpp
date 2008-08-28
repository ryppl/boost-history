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

    template<typename ArgumentPack>
    named_constructableN(const ArgumentPack &args)
    {
        label = args[_labelN];
        size = args[_sizeN];
        volume = args[_volumeN];
        
        fusion::vector<fusion::vector<fusion::vector<int> >, fusion::vector<float>, fusion::vector<int> > v3;
        v3 = fusion::make_vector(fusion::make_vector(fusion::make_vector(1)),  fusion::make_vector(1.5), fusion::make_vector(true));
    }
    std::string label;
    int size;
    double volume;
};


BOOST_AUTO_TEST_CASE( BOOST_PP_CAT(test_multi_param_construction,N) )
{
    named_constructableN constructed((_labelN = "hello", _sizeN = 1, _volumeN = 1.0));
    named_constructableN constructed2((_sizeN = 1, _volumeN = 1.0, _labelN = "hello"));
    named_constructableN constructed3((_labelN = "hello", _volumeN = 1.0, _sizeN = 1));
    named_constructableN constructed4((_volumeN = 1.0, _sizeN = 1, _labelN = "hello"));
    BOOST_CHECK_EQUAL(constructed.label, "hello");
    BOOST_CHECK_EQUAL(constructed.size, 1);    
    BOOST_CHECK_EQUAL(constructed.volume, 1.0);    
    BOOST_CHECK_EQUAL(constructed2.label, "hello");
    BOOST_CHECK_EQUAL(constructed2.size, 1);    
    BOOST_CHECK_EQUAL(constructed2.volume, 1.0);
}