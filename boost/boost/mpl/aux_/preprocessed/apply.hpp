// preprocessed 'apply.hpp' header file,
// see 'boost/mpl/apply.hpp' for the original,
// including copyright information

#include "boost/mpl/aux_/count_if_not.hpp"
#include "boost/mpl/aux_/none.hpp"

namespace boost {
namespace mpl {

namespace aux {
template<int> struct apply_impl_chooser;
}

template<typename F> struct apply0 { 
typedef typename F::type type; 
}; 

namespace aux { 
template<> struct apply_impl_chooser< 0 > { 
template< typename F, typename T1, typename T2, typename T3, typename T4, typename T5 > struct result_ { 
    typedef typename mpl:: apply0 < F >::type type; 
}; 
}; 
} 

template< typename F, typename T1 > struct apply1 { 
typedef typename F ::template apply< T1 >::type type; 
}; 

namespace aux { 
template<> struct apply_impl_chooser< 1 > { 
template< typename F, typename T1, typename T2, typename T3, typename T4, typename T5 > struct result_ { 
    typedef typename mpl:: apply1 < F, T1 >::type type; 
}; 
}; 
} 

template< typename F, typename T1, typename T2 > struct apply2 { 
typedef typename F ::template apply< T1, T2 >::type type; 
};

namespace aux { 
template<> struct apply_impl_chooser< 2 > { 
template< typename F, typename T1, typename T2, typename T3, typename T4, typename T5 > struct result_ { 
    typedef typename mpl:: apply2 < F, T1, T2 >::type type; 
}; 
}; 
} 

template< typename F, typename T1, typename T2, typename T3 > struct apply3 { 
typedef typename F ::template apply< T1, T2, T3 >::type type; 
}; 

namespace aux { 
template<> struct apply_impl_chooser< 3 > { 
template< typename F, typename T1, typename T2, typename T3, typename T4, typename T5 > struct result_ { 
    typedef typename mpl:: apply3 < F, T1, T2, T3 >::type type; 
}; 
}; 
} 

template< typename F, typename T1, typename T2, typename T3, typename T4 > struct apply4 { 
typedef typename F ::template apply< T1, T2, T3, T4 >::type type; 
}; 

namespace aux { 
template<> struct apply_impl_chooser< 4 > { 
template< typename F, typename T1, typename T2, typename T3, typename T4, typename T5 > struct result_ { 
    typedef typename mpl:: apply4 < F, T1, T2, T3, T4 >::type type; 
}; 
}; 
} 

template< typename F, typename T1, typename T2, typename T3, typename T4, typename T5 > struct apply5 { 
typedef typename F ::template apply< T1, T2, T3, T4, T5 >::type type; 
}; 

namespace aux { 
template<> struct apply_impl_chooser< 5 > { 
template< typename F, typename T1, typename T2, typename T3, typename T4, typename T5 > struct result_ { 
    typedef typename mpl:: apply5 < F, T1, T2, T3, T4, T5 >::type type; 
}; 
}; 
} 

template< typename F, typename T1 = aux::none, typename T2 = aux::none, typename T3 = aux::none, typename T4 = aux::none, typename T5 = aux::none >
struct apply
{
private:
    typedef aux::count_if_not< aux::none, T1, T2, T3, T4, T5 > arity_;

public:
    typedef typename aux::apply_impl_chooser< arity_::value >
        ::template result_< F, T1, T2, T3, T4, T5 >::type type;
};

} // namespace mpl
} // namespace boost 
