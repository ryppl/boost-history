#ifndef BOOST_MPL_AT_C_ALWAYS_HPP_INCLUDED
#define BOOST_MPL_AT_C_ALWAYS_HPP_INCLUDED
namespace boost { namespace mpl {

template<typename Type>
struct at_c_always
{
      template
      < typename Index
      , Index I
      >
    struct at_c
    {
        typedef Type type;
    };
};
    
}}

#endif // BOOST_MPL_AT_C_ALWAYS_HPP_INCLUDED
