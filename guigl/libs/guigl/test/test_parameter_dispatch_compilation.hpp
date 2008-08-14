#define N BOOST_PP_ITERATION()

#define labelN BOOST_PP_CAT(label,N)
#define sizeN BOOST_PP_CAT(size,N)
#define volumeN BOOST_PP_CAT(volume,N)
#define _labelN BOOST_PP_CAT(_label,N)
#define _sizeN BOOST_PP_CAT(_size,N)
#define _volumeN BOOST_PP_CAT(_volume,N)

#define named_constructable_implN BOOST_PP_CAT(named_constructable_impl,N)
#define named_constructableN BOOST_PP_CAT(named_constructable,N)

BOOST_PARAMETER_NAME(labelN)
BOOST_PARAMETER_NAME(sizeN)
BOOST_PARAMETER_NAME(volumeN)

struct named_constructable_implN
{
    template <class ArgumentPack>
    named_constructable_implN(ArgumentPack const& args)
    {
        label = args[_labelN];
        size = args[_sizeN];
        volume = args[_volumeN];
    }

    std::string label;
    int size;
    double volume;
};

class named_constructableN : public named_constructable_implN
{
public:
    BOOST_PARAMETER_CONSTRUCTOR(
        named_constructableN, (named_constructable_implN), tag
      , (required (labelN,*) (sizeN,*) (volumeN,*)) )
};


BOOST_AUTO_TEST_CASE( BOOST_PP_CAT(test_multi_param_construction,N) )
{
    named_constructableN constructed(_labelN = "hello", _sizeN = 1, _volumeN = 1.0);
    BOOST_CHECK_EQUAL(constructed.label, "hello");
    BOOST_CHECK_EQUAL(constructed.size, 1);    
    BOOST_CHECK_EQUAL(constructed.volume, 1.0);    

}