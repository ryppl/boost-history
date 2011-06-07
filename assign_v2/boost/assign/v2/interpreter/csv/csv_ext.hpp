//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_INTERPRETER_CSV_EXT_ER_2011_HPP
#define BOOST_ASSIGN_V2_INTERPRETER_CSV_EXT_ER_2011_HPP
#include <boost/assign/v2/interpreter/crtp.hpp>
#include <boost/assign/v2/support/config/enable_cpp0x.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <boost/assign/v2/support/mpl/variadic_args_to_indices.hpp>
#include <boost/assign/v2/support/pp/ignore.hpp>
#include <boost/assign/v2/support/traits/type/add_ref_const.hpp>
#include <boost/type_traits.hpp>
#include <boost/utility/enable_if.hpp>
#include <utility>
#include <tuple>

namespace boost{
namespace assign{
namespace v2{
//[syntax_interpreter_csv_ext
namespace interpreter_aux{

    template<
        int I,
        typename... Cast,
        typename D, typename C, typename F, typename MTag, typename DTag,
        typename... Args
    >
    D const&
    csv(
        interpreter_crtp<D, C, F, MTag, DTag> const& interpreter,
        Args&&... args
    );

//<-
    template<int I, int K, typename...Cast>
    struct csv_tail
    {
        template<typename D, typename T, typename ...Args>
        static D const& call( D const& interpreter, T&& /*drop*/, Args&&...args )
        {
            typedef csv_tail<I, K + 1, Cast...> next_;
            return next_::call(
                interpreter,
                std::forward<Args>( args )...
            );
        }
    };

    template<int I, typename...Cast>
    struct csv_tail<I, I, Cast...>
    {
        template<typename D, typename ...Args>
        static D const& call( D const& interpreter, Args&&...args)
        {
            return csv<I, Cast...>(
                interpreter, std::forward<Args>( args )...
            );
        }
    };

    template<
        int I, typename Head, typename Tail,
        bool exit = ( pack_holder_size<int, Head>::value == I )
    > struct csv_cast{};

    template<int I, typename...Head, typename U, typename...Tail>
    struct csv_cast<
        I,
        pack_holder<Head...>,
        pack_holder<U, Tail...>,
        false
    > : csv_cast<
            I,
            pack_holder<Head..., U>,
            pack_holder<Tail...>
        >
    {};

    template<int I, typename... Head, typename... Tail>
    struct csv_cast<I, pack_holder<Head...>, pack_holder<Tail...>, true >
    {
        typedef pack_holder<Head...> type;
    };

    template<typename NextTuple, int ...Values, typename Tuple, typename T>
    NextTuple
    csv_next_tuple(// TODO weird warning : unused parameter 'tu'
            indices<int, Values...>,
            Tuple&& tu,
            T&& t
    )
    {
        return NextTuple(
            get<Values>( std::forward<Tuple>( tu ) )...,
            std::forward<T>( t )
        );
    }

    template<typename T> struct csv_tuple_elem{};
    template<typename T> struct csv_tuple_elem<T&>{ typedef T& type; };
    template<typename T> struct csv_tuple_elem<T&&>{
        typedef T type;
    };

    template<typename ...Args>
    struct csv_tuple
    {
        typedef std::tuple<
            typename csv_tuple_elem<Args>::type...
        > type;
    };

    template<int I, typename HeadPack, typename ArgsPack,
        bool exit = pack_holder_size<int, HeadPack>::value == I
    >
    struct csv_tuple_head{};

    template<int I, typename ...Head, typename...Args>
    struct csv_tuple_head<
        I, pack_holder<Head...>, pack_holder<Args...>, true
    >
    {
        typedef typename csv_tuple<Head&&...>::type type;

        static type call(type&& tu, Args&&... /*unused*/)
        {
            return tu;
        }
    };

    template<int I, typename ...Head, typename T, typename...Args>
    struct csv_tuple_head<
        I,
        pack_holder<Head...>,
        pack_holder<T, Args...>,
        false
    >
    {
        typedef typename csv_tuple<Head&&...>::type tuple_;
        typedef csv_tuple_head<
            I,
            pack_holder<Head..., T>,
            pack_holder<Args...>
        > next_;
        typedef typename next_::type type;

        static type call(tuple_&& tu, T&& t, Args&&... args)
        {
            typedef typename head_indices<
                int, sizeof...(Head)
            >::type indices_;
            typedef typename csv_tuple<Head&&..., T&&>::type next_tuple_;
            return next_::call(
                csv_next_tuple< next_tuple_ >(
                    indices_(),
                    std::forward<tuple_>( tu ),
                    std::forward<T>( t )
                ),
                std::forward<Args>( args )...
            );
        }

    };

    template<typename D, typename...Cast>
    void csv_invoke_helper(D const& interpreter, Cast...args)
    {
        interpreter( args...);
    }

    template<typename D, int...Values, typename...Cast, typename Tuple>
    void csv_invoke(
        D const& interpreter,
        indices<int, Values...>,
        pack_holder<Cast...>,
        Tuple&& tu
    )
    {
        csv_invoke_helper<D, Cast...>(
            interpreter,
            get<Values>( std::forward<Tuple>( tu ) )...
        );
    }

    template<
        int I,
        typename... Cast,
        typename D, typename C, typename F, typename MTag, typename DTag
    >
    D const&
    csv(
        interpreter_crtp<D, C, F, MTag, DTag> const& interpreter
    )
    {
        return static_cast<D const&>( interpreter );
    }

    template<
        int I,
        typename... Cast,
        typename D, typename C, typename F, typename MTag, typename DTag,
        typename... Args
    >
    D const&
    csv(
        interpreter_crtp<D, C, F, MTag, DTag> const& interpreter,
        Args&&... args
    )
    {
        {
            typedef pack_holder<
                typename type_traits::add_ref_const<Cast>::type...
            > head_;
            typedef pack_holder<Args...> tail_;
            typedef typename csv_cast<I, head_, tail_>::type cast_;

            typedef csv_tuple_head<
                I, pack_holder<>, pack_holder<Args...>
            > make_tuple_;

            typedef typename head_indices<int, I>::type indices_;
            typedef std::tuple<> init_;
            csv_invoke(
                interpreter,
                indices_(),
                cast_(),
                make_tuple_::call
                (
                    init_(),
                    std::forward<Args>( args )...
                )
            );
        }
        {
            typedef csv_tail<I, 0, Cast...> tail_;
            return tail_::call(
                static_cast<D const&>( interpreter ),
                std::forward<Args>( args )...
            );
        }
    }

//->

}// interpreter_aux

using interpreter_aux::csv;

//]
}// v2
}// assign
}// boost

#else
#include <boost/assign/v2/interpreter/cpp03/csv_ext.hpp>
#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X

#endif // BOOST_ASSIGN_V2_INTERPRETER_CSV_EXT_ER_2011_HPP
