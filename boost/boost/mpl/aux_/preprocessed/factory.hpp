// preprocessed 'factory.hpp' header file,
// see 'boost/mpl/list/factory.hpp' for the original,
// including copyright information

#include "boost/mpl/list/traits.hpp"
#include "boost/mpl/aux_/count_if_not.hpp"

namespace boost {
namespace mpl {

namespace aux {

template<long N>
struct list_factory_part1
{
    template< typename Tag, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14 >
    struct part2
    { 
        typedef typename mpl::list_traits<Tag>::null_node type;

    };
};

template<> struct list_factory_part1< 1 > { 
template< typename Tag, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14 > struct part2 { 
    typedef typename mpl::list_traits<Tag>::template make_node<T0, typename mpl::list_traits<Tag>::null_node >::type type; 
};
};

template<> struct list_factory_part1< 2 > { 
template< typename Tag, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14 > struct part2 { 
    typedef typename mpl::list_traits<Tag>::template make_node<T0, typename mpl::list_traits<Tag>::template make_node<T1, typename mpl::list_traits<Tag>::null_node >::type >::type type; 
}; 
}; 

template<> struct list_factory_part1< 3 > { 
template< typename Tag, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14 > struct part2 { 
    typedef typename mpl::list_traits<Tag>::template make_node<T0, typename mpl::list_traits<Tag>::template make_node<T1, typename mpl::list_traits<Tag>::template make_node<T2, typename mpl::list_traits<Tag>::null_node >::type >::type >::type type; 
}; 
}; 

template<> struct list_factory_part1< 4 > { 
template< typename Tag, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14 > struct part2 { 
    typedef typename mpl::list_traits<Tag>::template make_node<T0, typename mpl::list_traits<Tag>::template make_node<T1, typename mpl::list_traits<Tag>::template make_node<T2, typename mpl::list_traits<Tag>::template make_node<T3, typename mpl::list_traits<Tag>::null_node >::type >::type >::type >::type type; 
}; 
}; 

template<> struct list_factory_part1< 5 > { 
template< typename Tag, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14 > struct part2 { 
    typedef typename mpl::list_traits<Tag>::template make_node<T0, typename mpl::list_traits<Tag>::template make_node<T1, typename mpl::list_traits<Tag>::template make_node<T2, typename mpl::list_traits<Tag>::template make_node<T3, typename mpl::list_traits<Tag>::template make_node<T4, typename mpl::list_traits<Tag>::null_node >::type >::type >::type >::type >::type type; 
}; 
}; 

template<> struct list_factory_part1< 6 > { 
template< typename Tag, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14 > struct part2 { 
    typedef typename mpl::list_traits<Tag>::template make_node<T0, typename mpl::list_traits<Tag>::template make_node<T1, typename mpl::list_traits<Tag>::template make_node<T2, typename mpl::list_traits<Tag>::template make_node<T3, typename mpl::list_traits<Tag>::template make_node<T4, typename mpl::list_traits<Tag>::template make_node<T5, typename mpl::list_traits<Tag>::null_node >::type >::type >::type >::type >::type >::type type; 
}; 
}; 

template<> struct list_factory_part1< 7 > { 
template< typename Tag, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14 > struct part2 { 
    typedef typename mpl::list_traits<Tag>::template make_node<T0, typename mpl::list_traits<Tag>::template make_node<T1, typename mpl::list_traits<Tag>::template make_node<T2, typename mpl::list_traits<Tag>::template make_node<T3, typename mpl::list_traits<Tag>::template make_node<T4, typename mpl::list_traits<Tag>::template make_node<T5, typename mpl::list_traits<Tag>::template make_node<T6, typename mpl::list_traits<Tag>::null_node >::type >::type >::type >::type >::type >::type >::type type; 
}; 
}; 

template<> struct list_factory_part1< 8 > { 
template< typename Tag, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14 > struct part2 { 
    typedef typename mpl::list_traits<Tag>::template make_node<T0, typename mpl::list_traits<Tag>::template make_node<T1, typename mpl::list_traits<Tag>::template make_node<T2, typename mpl::list_traits<Tag>::template make_node<T3, typename mpl::list_traits<Tag>::template make_node<T4, typename mpl::list_traits<Tag>::template make_node<T5, typename mpl::list_traits<Tag>::template make_node<T6, typename mpl::list_traits<Tag>::template make_node<T7, typename mpl::list_traits<Tag>::null_node >::type >::type >::type >::type >::type >::type >::type >::type type; 
}; 
}; 

template<> struct list_factory_part1< 9 > { 
template< typename Tag, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14 > struct part2 { 
    typedef typename mpl::list_traits<Tag>::template make_node<T0, typename mpl::list_traits<Tag>::template make_node<T1, typename mpl::list_traits<Tag>::template make_node<T2, typename mpl::list_traits<Tag>::template make_node<T3, typename mpl::list_traits<Tag>::template make_node<T4, typename mpl::list_traits<Tag>::template make_node<T5, typename mpl::list_traits<Tag>::template make_node<T6, typename mpl::list_traits<Tag>::template make_node<T7, typename mpl::list_traits<Tag>::template make_node<T8, typename mpl::list_traits<Tag>::null_node >::type >::type >::type >::type >::type >::type >::type >::type >::type type; 
}; 
}; 

template<> struct list_factory_part1< 10 > { 
template< typename Tag, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14 > struct part2 { 
    typedef typename mpl::list_traits<Tag>::template make_node<T0, typename mpl::list_traits<Tag>::template make_node<T1, typename mpl::list_traits<Tag>::template make_node<T2, typename mpl::list_traits<Tag>::template make_node<T3, typename mpl::list_traits<Tag>::template make_node<T4, typename mpl::list_traits<Tag>::template make_node<T5, typename mpl::list_traits<Tag>::template make_node<T6, typename mpl::list_traits<Tag>::template make_node<T7, typename mpl::list_traits<Tag>::template make_node<T8, typename mpl::list_traits<Tag>::template make_node<T9, typename mpl::list_traits<Tag>::null_node >::type >::type >::type >::type >::type >::type >::type >::type >::type >::type type; 
}; 
}; 

template<> struct list_factory_part1< 11 > { 
template< typename Tag, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14 > struct part2 { 
    typedef typename mpl::list_traits<Tag>::template make_node<T0, typename mpl::list_traits<Tag>::template make_node<T1, typename mpl::list_traits<Tag>::template make_node<T2, typename mpl::list_traits<Tag>::template make_node<T3, typename mpl::list_traits<Tag>::template make_node<T4, typename mpl::list_traits<Tag>::template make_node<T5, typename mpl::list_traits<Tag>::template make_node<T6, typename mpl::list_traits<Tag>::template make_node<T7, typename mpl::list_traits<Tag>::template make_node<T8, typename mpl::list_traits<Tag>::template make_node<T9, typename mpl::list_traits<Tag>::template make_node<T10, typename mpl::list_traits<Tag>::null_node >::type >::type >::type >::type >::type >::type >::type >::type >::type >::type >::type type; 
}; 
}; 

template<> struct list_factory_part1< 12 > { 
template< typename Tag, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14 > struct part2 { 
    typedef typename mpl::list_traits<Tag>::template make_node<T0, typename mpl::list_traits<Tag>::template make_node<T1, typename mpl::list_traits<Tag>::template make_node<T2, typename mpl::list_traits<Tag>::template make_node<T3, typename mpl::list_traits<Tag>::template make_node<T4, typename mpl::list_traits<Tag>::template make_node<T5, typename mpl::list_traits<Tag>::template make_node<T6, typename mpl::list_traits<Tag>::template make_node<T7, typename mpl::list_traits<Tag>::template make_node<T8, typename mpl::list_traits<Tag>::template make_node<T9, typename mpl::list_traits<Tag>::template make_node<T10, typename mpl::list_traits<Tag>::template make_node<T11, typename mpl::list_traits<Tag>::null_node >::type >::type >::type >::type >::type >::type >::type >::type >::type >::type >::type >::type type; 
}; 
}; 

template<> struct list_factory_part1< 13 > { 
template< typename Tag, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14 > struct part2 { 
    typedef typename mpl::list_traits<Tag>::template make_node<T0, typename mpl::list_traits<Tag>::template make_node<T1, typename mpl::list_traits<Tag>::template make_node<T2, typename mpl::list_traits<Tag>::template make_node<T3, typename mpl::list_traits<Tag>::template make_node<T4, typename mpl::list_traits<Tag>::template make_node<T5, typename mpl::list_traits<Tag>::template make_node<T6, typename mpl::list_traits<Tag>::template make_node<T7, typename mpl::list_traits<Tag>::template make_node<T8, typename mpl::list_traits<Tag>::template make_node<T9, typename mpl::list_traits<Tag>::template make_node<T10, typename mpl::list_traits<Tag>::template make_node<T11, typename mpl::list_traits<Tag>::template make_node<T12, typename mpl::list_traits<Tag>::null_node >::type >::type >::type >::type >::type >::type >::type >::type >::type >::type >::type >::type >::type type; 
}; 
}; 

template<> struct list_factory_part1< 14 > { 
template< typename Tag, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14 > struct part2 { 
    typedef typename mpl::list_traits<Tag>::template make_node<T0, typename mpl::list_traits<Tag>::template make_node<T1, typename mpl::list_traits<Tag>::template make_node<T2, typename mpl::list_traits<Tag>::template make_node<T3, typename mpl::list_traits<Tag>::template make_node<T4, typename mpl::list_traits<Tag>::template make_node<T5, typename mpl::list_traits<Tag>::template make_node<T6, typename mpl::list_traits<Tag>::template make_node<T7, typename mpl::list_traits<Tag>::template make_node<T8, typename mpl::list_traits<Tag>::template make_node<T9, typename mpl::list_traits<Tag>::template make_node<T10, typename mpl::list_traits<Tag>::template make_node<T11, typename mpl::list_traits<Tag>::template make_node<T12, typename mpl::list_traits<Tag>::template make_node<T13, typename mpl::list_traits<Tag>::null_node >::type >::type >::type >::type >::type >::type >::type >::type >::type >::type >::type >::type >::type >::type type; 
}; 
};

}  // namespace aux

template< typename Tag, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14 >
struct list_factory
{
    typedef typename aux::list_factory_part1<
        aux::count_if_not<
              typename mpl::list_traits<Tag>::null_argument
            , T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14 >::value
            >::template part2< Tag, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14 >::type type;
};

} // namespace mpl
} // namespace boost 
