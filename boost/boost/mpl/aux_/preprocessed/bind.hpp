// preprocessed 'bind.hpp' header file,
// see 'boost/mpl/bind.hpp' for the original,
// including copyright information

#include "boost/mpl/lambda/arg.hpp"
#include "boost/mpl/apply.hpp"
#include "boost/mpl/aux_/count_if_not.hpp"
#include "boost/mpl/aux_/type_wrapper.hpp"
#include "boost/mpl/aux_/yes_no.hpp"
#include "boost/mpl/aux_/none.hpp"

namespace boost {
namespace mpl {

namespace aux {

template<bool>
struct resolve_argument_impl
{
    template< typename T, typename U1, typename U2, typename U3, typename U4, typename U5 > struct result_
    {
        typedef T type;
    };
};

template<> struct resolve_argument_impl<true>
{
    template< typename T, typename U1, typename U2, typename U3, typename U4, typename U5 > struct result_
    {
        typedef typename mpl::apply5 < T, U1, U2, U3, U4, U5 >::type type;
    };
};

template<typename T> struct is_nested_bind;

template<typename T>
struct resolve_bind_argument
{
    template< typename U1, typename U2, typename U3, typename U4, typename U5 > struct result_
    {
        typedef typename resolve_argument_impl< is_nested_bind<T>::value >
            ::template result_< T, U1, U2, U3, U4, U5 >::type type;
    };
};

} // namespace aux 

namespace aux {
aux::no_tag is_nested_bind_helper(...);

template<typename T> struct is_nested_bind
{
    static const bool value =
        sizeof(is_nested_bind_helper(type_wrapper<T>())) == sizeof(aux::yes_tag);
};
} 

namespace aux {
template<int> struct bind_impl_chooser;

} 

template<typename F > struct bind0 { 
    typedef bind0 type; 
    template< typename U1 = aux::none, typename U2 = aux::none, typename U3 = aux::none, typename U4 = aux::none, typename U5 = aux::none > struct apply { 
    private: public: typedef typename mpl:: apply0 < F >::type type; 
    }; 
}; 

namespace aux { 
template< typename F > aux::yes_tag is_nested_bind_helper( aux::type_wrapper< bind0 <F > > ); 
} 

namespace aux { 
template<> struct bind_impl_chooser< 0 > { 
template< typename F, typename T1, typename T2, typename T3, typename T4, typename T5 > struct result_ { 
    typedef bind0 < F > type; 
}; 
}; 
} 

namespace aux { 
template<> struct resolve_bind_argument< mpl::arg< 1 > > { 
template< typename U1, typename U2, typename U3, typename U4, typename U5 > struct result_ { 
    typedef U1 type; 
    typedef char arity_constraint[sizeof(aux::reject_if_none<type>)]; 
}; 
}; 
} 

template<typename F, typename T1 > struct bind1 { 
typedef bind1 type; 
template< typename U1 = aux::none, typename U2 = aux::none, typename U3 = aux::none, typename U4 = aux::none, typename U5 = aux::none > struct apply { 
private: typedef typename aux::resolve_bind_argument< T1 >::template result_< U1, U2, U3, U4, U5 >::type t1 ; 
public: typedef typename mpl:: apply1 < F, t1 >::type type; 
}; 
}; 

namespace aux { 
template< typename F, typename T1 > aux::yes_tag is_nested_bind_helper( aux::type_wrapper< bind1 <F, T1 > > ); 
} 
namespace aux { 
template<> struct bind_impl_chooser< 1 > { 
template< typename F, typename T1, typename T2, typename T3, typename T4, typename T5 > struct result_ { 
    typedef bind1 < F, T1 > type; 
}; 
}; 
} 

namespace aux { 
template<> struct resolve_bind_argument< mpl::arg< 2 > > { 
template< typename U1, typename U2, typename U3, typename U4, typename U5 > struct result_ { 
    typedef U2 type; 
    typedef char arity_constraint[sizeof(aux::reject_if_none<type>)]; 
}; 
}; 
} 

template<typename F, typename T1, typename T2 > struct bind2 { 
typedef bind2 type; 
template< typename U1 = aux::none, typename U2 = aux::none, typename U3 = aux::none, typename U4 = aux::none, typename U5 = aux::none > struct apply { 
private: typedef typename aux::resolve_bind_argument< T1 >::template result_< U1, U2, U3, U4, U5 >::type t1 ; 
typedef typename aux::resolve_bind_argument< T2 >::template result_< U1, U2, U3, U4, U5 >::type t2 ; 
public: typedef typename mpl:: apply2 < F, t1, t2 >::type type; 
}; 
}; 

namespace aux { 
template< typename F, typename T1, typename T2 > aux::yes_tag is_nested_bind_helper( aux::type_wrapper< bind2 <F, T1, T2 > > ); 
} 
namespace aux { 
template<> struct bind_impl_chooser< 2 > { 
template< typename F, typename T1, typename T2, typename T3, typename T4, typename T5 > struct result_ { 
    typedef bind2 < F, T1, T2 > type; 
}; 
}; 
} 

namespace aux { 
template<> struct resolve_bind_argument< mpl::arg< 3 > > { 
template< typename U1, typename U2, typename U3, typename U4, typename U5 > struct result_ { 
    typedef U3 type; 
    typedef char arity_constraint[sizeof(aux::reject_if_none<type>)]; 
}; 
}; 
} 

template<typename F, typename T1, typename T2, typename T3 > struct bind3 { 
typedef bind3 type; 
template< typename U1 = aux::none, typename U2 = aux::none, typename U3 = aux::none, typename U4 = aux::none, typename U5 = aux::none > struct apply { 
private: typedef typename aux::resolve_bind_argument< T1 >::template result_< U1, U2, U3, U4, U5 >::type t1 ; 
typedef typename aux::resolve_bind_argument< T2 >::template result_< U1, U2, U3, U4, U5 >::type t2 ; 
typedef typename aux::resolve_bind_argument< T3 >::template result_< U1, U2, U3, U4, U5 >::type t3 ; 
public: typedef typename mpl:: apply3 < F, t1, t2, t3 >::type type; 
}; 
}; 

namespace aux { 
template< typename F, typename T1, typename T2, typename T3 > aux::yes_tag is_nested_bind_helper( aux::type_wrapper< bind3 <F, T1, T2, T3 > > ); 
} 

namespace aux { 
template<> struct bind_impl_chooser< 3 > { 
template< typename F, typename T1, typename T2, typename T3, typename T4, typename T5 > struct result_ { 
    typedef bind3 < F, T1, T2, T3 > type; 
}; 
}; 
} 

namespace aux { 
template<> struct resolve_bind_argument< mpl::arg< 4 > > { 
template< typename U1, typename U2, typename U3, typename U4, typename U5 > struct result_ { 
    typedef U4 type; 
    typedef char arity_constraint[sizeof(aux::reject_if_none<type>)]; 
}; 
}; 
} 

template<typename F, typename T1, typename T2, typename T3, typename T4 > struct bind4 { 
typedef bind4 type; 
template< typename U1 = aux::none, typename U2 = aux::none, typename U3 = aux::none, typename U4 = aux::none, typename U5 = aux::none > struct apply { 
private: typedef typename aux::resolve_bind_argument< T1 >::template result_< U1, U2, U3, U4, U5 >::type t1 ; 
typedef typename aux::resolve_bind_argument< T2 >::template result_< U1, U2, U3, U4, U5 >::type t2 ; 
typedef typename aux::resolve_bind_argument< T3 >::template result_< U1, U2, U3, U4, U5 >::type t3 ; 
typedef typename aux::resolve_bind_argument< T4 >::template result_< U1, U2, U3, U4, U5 >::type t4 ; 
public: typedef typename mpl:: apply4 < F, t1, t2, t3, t4 >::type type; 
}; 
}; 

namespace aux { 
template< typename F, typename T1, typename T2, typename T3, typename T4 > aux::yes_tag is_nested_bind_helper( aux::type_wrapper< bind4 <F, T1, T2, T3, T4 > > ); 
} 

namespace aux { 
template<> struct bind_impl_chooser< 4 > { 
template< typename F, typename T1, typename T2, typename T3, typename T4, typename T5 > struct result_ { 
    typedef bind4 < F, T1, T2, T3, T4 > type; 
}; 
}; 
} 

namespace aux { 
template<> struct resolve_bind_argument< mpl::arg< 5 > > { 
template< typename U1, typename U2, typename U3, typename U4, typename U5 > struct result_ { 
    typedef U5 type; 
    typedef char arity_constraint[sizeof(aux::reject_if_none<type>)]; 
}; 
}; 
} 

template<typename F, typename T1, typename T2, typename T3, typename T4, typename T5 > struct bind5 { 
typedef bind5 type; 
template< typename U1 = aux::none, typename U2 = aux::none, typename U3 = aux::none, typename U4 = aux::none, typename U5 = aux::none > struct apply { 
private: typedef typename aux::resolve_bind_argument< T1 >::template result_< U1, U2, U3, U4, U5 >::type t1 ; 
typedef typename aux::resolve_bind_argument< T2 >::template result_< U1, U2, U3, U4, U5 >::type t2 ; 
typedef typename aux::resolve_bind_argument< T3 >::template result_< U1, U2, U3, U4, U5 >::type t3 ; 
typedef typename aux::resolve_bind_argument< T4 >::template result_< U1, U2, U3, U4, U5 >::type t4 ; 
typedef typename aux::resolve_bind_argument< T5 >::template result_< U1, U2, U3, U4, U5 >::type t5 ; 
public: typedef typename mpl:: apply5 < F, t1, t2, t3, t4, t5 >::type type; 
}; 
}; 

namespace aux { 
template< typename F, typename T1, typename T2, typename T3, typename T4, typename T5 > aux::yes_tag is_nested_bind_helper( aux::type_wrapper< bind5 <F, T1, T2, T3, T4, T5 > > ); 
} 

namespace aux { 
template<> struct bind_impl_chooser< 5 > { 
template< typename F, typename T1, typename T2, typename T3, typename T4, typename T5 > struct result_ { 
    typedef bind5 < F, T1, T2, T3, T4, T5 > type; 
}; 
}; 
} 

template< typename F, typename T1 = aux::none, typename T2 = aux::none, typename T3 = aux::none, typename T4 = aux::none, typename T5 = aux::none >
struct bind
{
private:
    typedef aux::count_if_not< aux::none, T1, T2, T3, T4, T5 > arity_;

public:
    typedef typename aux::bind_impl_chooser< arity_::value >
        ::template result_< F, T1, T2, T3, T4, T5 >::type type;
};

template< typename F, typename T >
struct bind1st
{
    template<typename U> struct apply
       : mpl::apply2<F,T,U>
    {
    };
};

template< typename F, typename T >
struct bind2nd
{
    template<typename U> struct apply
        : mpl::apply2<F,U,T>
    {
    };

};

} // namespace mpl
} // namespace boost 
