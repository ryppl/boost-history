//-----------------------------------------------------------------------------
// boost variant/variant.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2002-2003
// Eric Friedman, Itay Maman
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_VARIANT_VARIANT_HPP
#define BOOST_VARIANT_VARIANT_HPP

#include <cstddef> // for std::size_t
#include <new> // for placement new
#include <typeinfo> // for typeid, std::type_info

#include "boost/variant/variant_fwd.hpp"
#include "boost/variant/detail/backup_holder.hpp"
#include "boost/variant/detail/enable_recursive_fwd.hpp"
#include "boost/variant/detail/forced_return.hpp"
#include "boost/variant/detail/initializer.hpp"
#include "boost/variant/detail/make_variant_list.hpp"
#include "boost/variant/detail/visitation_impl.hpp"

#include "boost/variant/detail/enable_if.hpp"
#include "boost/variant/detail/generic_result_type.hpp"
#include "boost/variant/detail/has_nothrow_move.hpp"
#include "boost/variant/detail/move.hpp"

#include "boost/config.hpp"
#include "boost/detail/workaround.hpp"
#include "boost/mpl/aux_/config/eti.hpp"
#include "boost/mpl/aux_/value_wknd.hpp"

#include "boost/detail/reference_content.hpp"
#include "boost/aligned_storage.hpp"
#include "boost/compressed_pair.hpp"
#include "boost/empty.hpp"
#include "boost/utility/addressof.hpp"
#include "boost/static_assert.hpp"
#include "boost/preprocessor/cat.hpp"
#include "boost/preprocessor/enum.hpp"
#include "boost/preprocessor/enum_params.hpp"
#include "boost/preprocessor/inc.hpp"
#include "boost/preprocessor/repeat.hpp"
#include "boost/type_traits/alignment_of.hpp"
#include "boost/type_traits/add_const.hpp"
#include "boost/type_traits/has_nothrow_constructor.hpp"
#include "boost/type_traits/has_nothrow_copy.hpp"
#include "boost/type_traits/is_const.hpp"
#include "boost/type_traits/is_same.hpp"
#include "boost/variant/recursive_wrapper_fwd.hpp"
#include "boost/variant/static_visitor.hpp"

#include "boost/mpl/apply_if.hpp"
#include "boost/mpl/begin_end.hpp"
#include "boost/mpl/bool.hpp"
#include "boost/mpl/distance.hpp"
#include "boost/mpl/find.hpp"
#include "boost/mpl/find_if.hpp"
#include "boost/mpl/front.hpp"
#include "boost/mpl/identity.hpp"
#include "boost/mpl/int.hpp"
#include "boost/mpl/is_sequence.hpp"
#include "boost/mpl/logical.hpp"
#include "boost/mpl/max_element.hpp"
#include "boost/mpl/protect.hpp"
#include "boost/mpl/push_front.hpp"
#include "boost/mpl/sizeof.hpp"
#include "boost/mpl/transform.hpp"
#include "boost/mpl/void.hpp"

///////////////////////////////////////////////////////////////////////////////
// Implementation Macros:
//
// BOOST_VARIANT_VISITATION_UNROLLING_LIMIT
//   Defined in boost/variant/detail/visitation_impl.hpp.
//
// BOOST_VARIANT_MINIMIZE_SIZE
//   When #defined, implementation employs all known means to minimize the
//   size of variant objects. However, often unsuccessful due to alignment
//   issues, and potentially harmful to runtime speed, so not enabled by
//   default. (TODO: Investigate further.)

#if defined(BOOST_VARIANT_MINIMIZE_SIZE)
#   include <climits> // for SCHAR_MAX
#   include "boost/mpl/apply_if.hpp"
#   include "boost/mpl/equal_to.hpp"
#   include "boost/mpl/identity.hpp"
#   include "boost/mpl/int.hpp"
#   include "boost/mpl/if.hpp"
#   include "boost/mpl/less.hpp"
#   include "boost/mpl/long.hpp"
#   include "boost/mpl/O1_size.hpp"
#endif


namespace boost {

namespace detail { namespace variant {

///////////////////////////////////////////////////////////////////////////////
// (detail) metafunction max_value
//
// Finds the maximum value of the unary metafunction F over Sequence.
//
template <typename Sequence, typename F>
struct max_value
{
private: // helpers, for metafunction result (below)

    typedef typename mpl::max_element<
          typename mpl::transform<Sequence, F>::type
        >::type max_it;

public: // metafunction result

    typedef typename BOOST_MPL_AUX_DEREF_WNKD(max_it)
        type;

};

///////////////////////////////////////////////////////////////////////////////
// (detail) class null_storage
//
// Simulates aligned_storage's interface, but with nothing underneath.
//
struct null_storage
{
public: // queries

    void* address()
    {
        return 0;
    }

#if !BOOST_WORKAROUND(BOOST_MSVC, <= 1200)

    const void* address() const
    {
        return 0;
    }

#else // MSVC6

    const void* address() const;

#endif // MSVC6 workaround

};

#if BOOST_WORKAROUND(BOOST_MSVC, <= 1200)

// MSVC6 seems not to like inline functions with const void* returns, so we
// declare the following here:

const void* null_storage::address() const
{
    return 0;
}

#endif // MSVC6 workaround

///////////////////////////////////////////////////////////////////////////////
// (detail) metafunction make_storage
//
// Provides an aligned storage type capable of holding any of the types
// specified in the given type-sequence.
//
template <typename Types, typename NeverUsesBackupFlag>
struct make_storage
{
private: // helpers, for metafunction result (below)

    typedef typename mpl::apply_if<
          NeverUsesBackupFlag
        , mpl::identity< Types >
        , mpl::push_front<
              Types
            , backup_holder<void*>
            >
        >::type types;
    typedef typename max_value<
          types, mpl::sizeof_<mpl::_1>
        >::type max_size;
    typedef typename max_value<
          types, alignment_of<mpl::_1>
        >::type max_alignment;

public: // metafunction result

#if !BOOST_WORKAROUND(BOOST_MSVC, <= 1300)

    typedef ::boost::aligned_storage<
          BOOST_MPL_AUX_VALUE_WKND(max_size)::value
        , BOOST_MPL_AUX_VALUE_WKND(max_alignment)::value
        > type;

#else // MSVC7 and below

    BOOST_STATIC_CONSTANT(std::size_t, msvc_max_size_c = max_size::value);
    BOOST_STATIC_CONSTANT(std::size_t, msvc_max_alignment_c = max_alignment::value);

    typedef ::boost::aligned_storage<
          msvc_max_size_c
        , msvc_max_alignment_c
        > type;

#endif // MSVC workaround

};

#if defined(BOOST_MPL_MSVC_60_ETI_BUG)

template<>
struct make_storage<int>
{
    typedef int type;
};

#endif // BOOST_MPL_MSVC_60_ETI_BUG workaround

///////////////////////////////////////////////////////////////////////////////
// (detail) class destroyer
//
// Internal visitor that destroys the value it visits.
//
struct destroyer
    : public static_visitor<>
{
public: // visitor interfaces

    template <typename T>
        BOOST_VARIANT_AUX_RETURN_VOID_TYPE
    internal_visit(T& operand, int) const
    {
        operand.~T();

#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x0551))
        operand; // suppresses warnings
#endif

        BOOST_VARIANT_AUX_RETURN_VOID;
    }

};

///////////////////////////////////////////////////////////////////////////////
// (detail) class template known_get
//
// Visitor that returns a reference to content of the specified type.
//
// Precondition: visited variant MUST contain logical content of type T.
//
template <typename T>
class known_get
    : public static_visitor<T&>
{

#if !BOOST_WORKAROUND(BOOST_MSVC, <= 1200)

public: // visitor interface

    T& operator()(T& operand) const
    {
        return operand;
    }

    template <typename U>
    T& operator()(U&) const
    {
        // logical error to be here: see precondition above
        BOOST_ASSERT(false);
        return ::boost::detail::variant::forced_return< T& >();
    }

#else // MSVC6

private: // helpers, for visitor interface (below)

    T& execute(T& operand, mpl::true_) const
    {
        return operand;
    }

    template <typename U>
    T& execute(U& operand, mpl::false_) const
    {
        // logical error to be here: see precondition above
        BOOST_ASSERT(false);
        return ::boost::detail::variant::forced_return< T& >();
    }

public: // visitor interface

    template <typename U>
    T& operator()(U& operand) const
    {
        typedef typename is_same< U,T >::type
            U_is_T;

        return execute(operand, U_is_T());
    }

#endif // MSVC6 workaround

};

///////////////////////////////////////////////////////////////////////////////
// (detail) class copy_into
//
// Internal visitor that copies the value it visits into the given buffer.
//
class copy_into
    : public static_visitor<>
{
private: // representation

    void* storage_;

public: // structors

    explicit copy_into(void* storage)
        : storage_(storage)
    {
    }

public: // internal visitor interface

    template <typename T>
        BOOST_VARIANT_AUX_RETURN_VOID_TYPE
    internal_visit(boost::detail::variant::backup_holder<T>& operand, long) const
    {
        new(storage_) T( operand.get() );
        BOOST_VARIANT_AUX_RETURN_VOID;
    }

    template <typename T>
        BOOST_VARIANT_AUX_RETURN_VOID_TYPE
    internal_visit(const boost::detail::variant::backup_holder<T>& operand, long) const
    {
        new(storage_) T( operand.get() );
        BOOST_VARIANT_AUX_RETURN_VOID;
    }

    template <typename T>
        BOOST_VARIANT_AUX_RETURN_VOID_TYPE
    internal_visit(const T& operand, int) const
    {
        new(storage_) T(operand);
        BOOST_VARIANT_AUX_RETURN_VOID;
    }

};

///////////////////////////////////////////////////////////////////////////////
// (detail) class assign_storage
//
// Internal visitor that assigns the given storage (which must be a
// constructed value of the same type) to the value it visits.
//
struct assign_storage
    : public static_visitor<>
{
private: // representation

    const void* rhs_storage_;

public: // structors

    explicit assign_storage(const void* rhs_storage)
        : rhs_storage_(rhs_storage)
    {
    }

public: // internal visitor interfaces

    template <typename T>
        BOOST_VARIANT_AUX_RETURN_VOID_TYPE
    internal_visit(backup_holder<T>& lhs_content, long) const
    {
        lhs_content.get()
            = static_cast< const backup_holder<T>* >(rhs_storage_)->get();
        BOOST_VARIANT_AUX_RETURN_VOID;
    }

    template <typename T>
        BOOST_VARIANT_AUX_RETURN_VOID_TYPE
    internal_visit(const backup_holder<T>& lhs_content, long) const
    {
        lhs_content.get()
            = static_cast< const backup_holder<T>* >(rhs_storage_)->get();
        BOOST_VARIANT_AUX_RETURN_VOID;
    }

    template <typename T>
        BOOST_VARIANT_AUX_RETURN_VOID_TYPE
    internal_visit(T& lhs_content, int) const
    {
        // NOTE TO USER :
        // Compile error here indicates one of variant's bounded types does
        // not meet the requirements of the Assignable concept. Thus,
        // variant is not Assignable.
        //
        // Hint: Are any of the bounded types const-qualified or references?
        //
        lhs_content = *static_cast< const T* >(rhs_storage_);
        BOOST_VARIANT_AUX_RETURN_VOID;
    }

};

///////////////////////////////////////////////////////////////////////////////
// (detail) class direct_assigner
//
// Generic static visitor that: if and only if the visited value is of the
// specified type, assigns the given value to the visited value and returns
// true; else returns false.
//
template <typename T>
class direct_assigner
    : public static_visitor<bool>
{
private: // representation

    T& rhs_;

public: // structors

    explicit direct_assigner(T& rhs)
        : rhs_(rhs)
    {
    }

#if !BOOST_WORKAROUND(BOOST_MSVC, <= 1200)

public: // visitor interface

    bool operator()(T& lhs)
    {
        lhs = rhs_;
        return true;
    }

    template <typename U>
    bool operator()(U&)
    {
        return false;
    }

#else // MSVC6

private: // helpers, for visitor interface (below)

    bool execute(T& lhs, mpl::true_)
    {
        lhs = rhs_;
        return true;
    }

    template <typename U>
    bool execute(U&, mpl::false_)
    {
        return false;
    }

public: // visitor interface

    template <typename U>
    bool operator()(U& lhs)
    {
        typedef typename is_same<U,T>::type U_is_T;
        return execute(lhs, U_is_T());
    }

#endif // MSVC6 workaround

};

///////////////////////////////////////////////////////////////////////////////
// (detail) class backup_assigner
//
// Internal visitor that "assigns" the given value to the visited value,
// using backup to recover if the destroy-copy sequence fails.
//
// NOTE: This needs to be a friend of variant, as it needs access to
// indicate_which, indicate_backup_which, etc.
//
template <typename Variant, typename RhsT>
class backup_assigner
    : public static_visitor<>
{
private: // representation

    Variant& lhs_;
    int rhs_which_;
    const RhsT& rhs_content_;

public: // structors

    backup_assigner(Variant& lhs, int rhs_which, const RhsT& rhs_content)
        : lhs_(lhs)
        , rhs_which_(rhs_which)
        , rhs_content_(rhs_content)
    {
    }

public: // visitor interface

    template <typename LhsT>
        BOOST_VARIANT_AUX_RETURN_VOID_TYPE
    internal_visit(LhsT& lhs_content, int)
    {
        // Backup lhs content...
        LhsT* backup_lhs_ptr = new LhsT(lhs_content);

        // ...destroy lhs content...
        lhs_content.~LhsT(); // nothrow

        try
        {
            // ...and attempt to copy rhs content into lhs storage:
            new(lhs_.storage_.address()) RhsT(rhs_content_);
        }
        catch (...)
        {
            // In case of failure, copy backup pointer to lhs storage...
            new(lhs_.storage_.address())
                backup_holder<LhsT>( backup_lhs_ptr ); // nothrow

            // ...indicate now using backup...
            lhs_.indicate_backup_which( lhs_.which() ); // nothrow

            // ...and rethrow:
            throw;
        }

        // In case of success, indicate success...
        lhs_.indicate_which(rhs_which_); // nothrow

        // ...and delete backup:
        delete backup_lhs_ptr; // nothrow

        BOOST_VARIANT_AUX_RETURN_VOID;
    }

};

///////////////////////////////////////////////////////////////////////////////
// (detail) class swap_with
//
// Internal visitor that swaps visited value with given storage.
//
// Precondition: Given storage MUST have same content type as visited value.
//
struct swap_with
    : public static_visitor<>
{
private: // representation

    void* toswap_;

public: // structors

    explicit swap_with(void* toswap)
        : toswap_(toswap)
    {
    }

public: // internal visitor interfaces

    template <typename T>
        BOOST_VARIANT_AUX_RETURN_VOID_TYPE
    internal_visit(backup_holder<T>& operand, long) const
    {
        operand.swap( *static_cast< backup_holder<T>* >(toswap_) );
        BOOST_VARIANT_AUX_RETURN_VOID;
    }

    template <typename T>
        BOOST_VARIANT_AUX_RETURN_VOID_TYPE
    internal_visit(boost::recursive_wrapper<T>& operand, long) const
    {
        operand.swap( *static_cast< boost::recursive_wrapper<T>* >(toswap_) );
        BOOST_VARIANT_AUX_RETURN_VOID;
    }

    template <typename T>
        BOOST_VARIANT_AUX_RETURN_VOID_TYPE
    internal_visit(T& operand, int) const
    {
        ::boost::detail::variant::move_swap(operand, *static_cast<T*>(toswap_));
        BOOST_VARIANT_AUX_RETURN_VOID;
    }

};

///////////////////////////////////////////////////////////////////////////////
// (detail) class reflect
//
// Generic static visitor that performs a typeid on the value it visits.
//
class reflect
    : public static_visitor<const std::type_info&>
{
public: // visitor interfaces

    template <typename T>
    const std::type_info& operator()(const T&) const
    {
        return typeid(T);
    }

};

///////////////////////////////////////////////////////////////////////////////
// (detail) class comparer
//
// Generic static visitor that compares the content of the given lhs variant
// with the visited rhs content using Comp.
//
// Precondition: lhs.which() == rhs.which()
//
template <typename Variant, typename Comp>
class comparer
    : public static_visitor<bool>
{
private: // representation

    const Variant& lhs_;

public: // structors

    explicit comparer(const Variant& lhs)
        : lhs_(lhs)
    {
    }

public: // visitor interfaces

    template <typename T>
    bool operator()(const T& rhs_content) const
    {
        // Since the precondition ensures lhs and rhs types are same, get T...
        known_get<const T> getter;
        const T& lhs_content = lhs_.apply_visitor(getter);

        // ...and compare lhs and rhs contents:
        return Comp()(lhs_content, rhs_content);
    }

};

///////////////////////////////////////////////////////////////////////////////
// (detail) class equal_comp
//
// Generic function object compares lhs with rhs using operator==.
//
struct equal_comp
{
    template <typename T>
    bool operator()(const T& lhs, const T& rhs) const
    {
        return lhs == rhs;
    }
};

///////////////////////////////////////////////////////////////////////////////
// (detail) class less_comp
//
// Generic function object compares lhs with rhs using operator<.
//
struct less_comp
{
    template <typename T>
    bool operator()(const T& lhs, const T& rhs) const
    {
        return lhs < rhs;
    }
};

///////////////////////////////////////////////////////////////////////////////
// (detail) class template invoke_visitor
//
// Internal visitor that invokes the given visitor using:
//  * for wrappers (e.g., recursive_wrapper), the wrapper's held value.
//  * for all other values, the value itself.
//
template <typename Visitor>
class invoke_visitor
{
private: // representation

    Visitor& visitor_;

public: // visitor typedefs

    typedef typename Visitor::result_type
        result_type;

public: // structors

    explicit invoke_visitor(Visitor& visitor)
        : visitor_(visitor)
    {
    }

#if !defined(BOOST_NO_VOID_RETURNS)

public: // internal visitor interfaces

    template <typename T>
    result_type internal_visit(T& operand, int)
    {
        return visitor_(operand);
    }

#   if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x0564))
    template <typename T>
    result_type internal_visit(const T& operand, int)
    {
        return visitor_(operand);
    }
#   endif

#else // defined(BOOST_NO_VOID_RETURNS)

private: // helpers, for internal visitor interfaces (below)

    template <typename T>
        BOOST_VARIANT_AUX_GENERIC_RESULT_TYPE(result_type)
    visit_impl(T& operand, mpl::false_)
    {
        return visitor_(operand);
    }

    template <typename T>
        BOOST_VARIANT_AUX_RETURN_VOID_TYPE
    visit_impl(T& operand, mpl::true_)
    {
        visitor_(operand);
        BOOST_VARIANT_AUX_RETURN_VOID;
    }

public: // internal visitor interfaces

    template <typename T>
        BOOST_VARIANT_AUX_GENERIC_RESULT_TYPE(result_type)
    internal_visit(T& operand, int)
    {
        typedef typename is_same<result_type, void>::type
            has_void_result_type;

        return visit_impl(operand, has_void_result_type());
    }

#endif // BOOST_NO_VOID_RETURNS) workaround

public: // internal visitor interfaces, cont.

    template <typename T>
        BOOST_VARIANT_AUX_GENERIC_RESULT_TYPE(result_type)
    internal_visit(boost::recursive_wrapper<T>& operand, long)
    {
        return internal_visit( operand.get(), 1L );
    }

    template <typename T>
        BOOST_VARIANT_AUX_GENERIC_RESULT_TYPE(result_type)
    internal_visit(const boost::recursive_wrapper<T>& operand, long)
    {
        return internal_visit( operand.get(), 1L );
    }

    template <typename T>
        BOOST_VARIANT_AUX_GENERIC_RESULT_TYPE(result_type)
    internal_visit(boost::detail::reference_content<T>& operand, long)
    {
        return internal_visit( operand.get(), 1L );
    }

    template <typename T>
        BOOST_VARIANT_AUX_GENERIC_RESULT_TYPE(result_type)
    internal_visit(const boost::detail::reference_content<T>& operand, long)
    {
        return internal_visit( operand.get(), 1L );
    }

    template <typename T>
        BOOST_VARIANT_AUX_GENERIC_RESULT_TYPE(result_type)
    internal_visit(boost::detail::variant::backup_holder<T>& operand, long)
    {
        return internal_visit( operand.get(), 1L );
    }

    template <typename T>
        BOOST_VARIANT_AUX_GENERIC_RESULT_TYPE(result_type)
    internal_visit(const boost::detail::variant::backup_holder<T>& operand, long)
    {
        return internal_visit( operand.get(), 1L );
    }

};

}} // namespace detail::variant

///////////////////////////////////////////////////////////////////////////////
// class template variant (concept inspired by Andrei Alexandrescu)
//
// See docs and boost/variant/variant_fwd.hpp for more information.
//
template <
      typename T0_
    , BOOST_VARIANT_ENUM_SHIFTED_PARAMS(typename T)
    >
class variant
{
private: // helpers, for typedefs (below)

    typedef variant wknd_self_t;

    struct is_recursive_
        : detail::variant::is_recursive_flag<T0_>
    {
    };

    typedef typename mpl::apply_if<
          is_recursive_
        , T0_
        , mpl::identity< T0_ >
        >::type unwrapped_T0_;

    struct is_sequence_based_
        : mpl::is_sequence<unwrapped_T0_>
    {
    };

#if !defined(BOOST_VARIANT_NO_TYPE_SEQUENCE_SUPPORT)

private: // helpers, for typedefs (below)

    typedef typename mpl::apply_if<
          is_sequence_based_
        , mpl::identity<unwrapped_T0_>
        , detail::variant::make_variant_list<
              unwrapped_T0_
            , BOOST_VARIANT_ENUM_SHIFTED_PARAMS(T)
            >
        >::type specified_types;

    typedef typename mpl::apply_if<
          is_recursive_
        , mpl::transform<
              specified_types
            , mpl::protect<
                  detail::variant::quoted_enable_recursive<wknd_self_t>
                >
            >
        , mpl::identity< specified_types >
        >::type recursive_enabled_types;

public: // public typedefs

    typedef typename mpl::transform<
          recursive_enabled_types
        , unwrap_recursive<mpl::_1>
        >::type types;

private: // internal typedefs

    typedef typename mpl::transform<
          recursive_enabled_types
        , mpl::protect< detail::make_reference_content<> >
        >::type internal_types;

    typedef typename mpl::front<
          internal_types
        >::type internal_T0;

#else // defined(BOOST_VARIANT_NO_TYPE_SEQUENCE_SUPPORT)

private: // helpers, for typedefs (below)

    typedef unwrapped_T0_ T0;

    #define BOOST_VARIANT_AUX_ENABLE_RECURSIVE_TYPEDEFS(z,N,_) \
        typedef typename mpl::apply_if< \
              is_recursive_ \
            , detail::variant::enable_recursive< \
                  BOOST_PP_CAT(T,N) \
                , wknd_self_t \
                > \
            , mpl::identity< BOOST_PP_CAT(T,N) > \
            >::type BOOST_PP_CAT(recursive_enabled_T,N); \
        /**/

    BOOST_PP_REPEAT(
          BOOST_VARIANT_LIMIT_TYPES
        , BOOST_VARIANT_AUX_ENABLE_RECURSIVE_TYPEDEFS
        , _
        )

    #undef BOOST_VARIANT_AUX_ENABLE_RECURSIVE_TYPEDEFS

    #define BOOST_VARIANT_AUX_UNWRAP_RECURSIVE_TYPEDEFS(z,N,_) \
        typedef typename unwrap_recursive< \
              BOOST_PP_CAT(recursive_enabled_T,N) \
            >::type BOOST_PP_CAT(public_T,N); \
        /**/

    BOOST_PP_REPEAT(
          BOOST_VARIANT_LIMIT_TYPES
        , BOOST_VARIANT_AUX_UNWRAP_RECURSIVE_TYPEDEFS
        , _
        )

    #undef BOOST_VARIANT_AUX_UNWRAP_RECURSIVE_TYPEDEFS

public: // public typedefs

    typedef typename detail::variant::make_variant_list<
          BOOST_VARIANT_ENUM_PARAMS(public_T)
        >::type types;

private: // helpers, for internal typedefs (below)

    #define BOOST_VARIANT_AUX_MAKE_REFERENCE_CONTENT_TYPEDEFS(z,N,_) \
        typedef detail::make_reference_content< \
              BOOST_PP_CAT(recursive_enabled_T,N) \
            >::type BOOST_PP_CAT(internal_T,N); \
        /**/

    BOOST_PP_REPEAT(
          BOOST_VARIANT_LIMIT_TYPES
        , BOOST_VARIANT_AUX_MAKE_REFERENCE_CONTENT_TYPEDEFS
        , _
        )

    #undef BOOST_VARIANT_AUX_MAKE_REFERENCE_CONTENT_TYPEDEFS

private: // internal typedefs

    typedef typename detail::variant::make_variant_list<
          BOOST_VARIANT_ENUM_PARAMS(internal_T)
        >::type internal_types;

private: // static precondition assertions

    // NOTE TO USER :
    // variant< type-sequence > syntax is not supported on this compiler!
    //
#   if !BOOST_WORKAROUND(BOOST_MSVC, <= 1200)
    BOOST_STATIC_ASSERT( ::boost::mpl::not_<is_sequence_based_>::value );
#   else
    BOOST_STATIC_ASSERT( !is_sequence_based_::value );
#   endif

#endif // BOOST_VARIANT_NO_TYPE_SEQUENCE_SUPPORT workaround

private: // helpers, for representation (below)

    struct T0_has_nothrow_constructor_
        : has_nothrow_constructor<internal_T0>
    {
    };

    typedef T0_has_nothrow_constructor_
        never_uses_backup_flag;

    typedef typename detail::variant::make_storage<
          internal_types, never_uses_backup_flag
        >::type storage_t;

private: // helpers, for representation (below)

    // which_ on:
    // * [0,  size<internal_types>) indicates stack content
    // * [-size<internal_types>, 0) indicates pointer to heap backup
    // if which_ >= 0:
    // * then which() -> which_
    // * else which() -> -(which_ + 1)

#if !defined(BOOST_VARIANT_MINIMIZE_SIZE)

    typedef int which_t;

#else // defined(BOOST_VARIANT_MINIMIZE_SIZE)

    // [if O1_size available, then attempt which_t size optimization...]
    // [select signed char if fewer than SCHAR_MAX types, else signed int:]
    typedef typename mpl::apply_if<
          mpl::equal_to< mpl::O1_size<internal_types>, mpl::long_<-1> >
        , mpl::identity< int >
        , mpl::if_<
              mpl::less< mpl::O1_size<internal_types>, mpl::int_<SCHAR_MAX> >
            , signed char
            , int
            >
        >::type which_t;

#endif // BOOST_VARIANT_MINIMIZE_SIZE switch

// representation -- private when possible
#if !defined(BOOST_NO_MEMBER_TEMPLATE_FRIENDS)
    private:
#else
    public:
#endif

    which_t which_;
    storage_t storage_;

    void indicate_which(int which)
    {
        which_ = static_cast<which_t>( which );
    }

    void indicate_backup_which(int which)
    {
        which_ = static_cast<which_t>( -(which + 1) );
    }

private: // helpers, for queries (below)

    bool using_backup() const
    {
        return which_ < 0;
    }

public: // queries

    int which() const
    {
        // If using heap backup...
        if (using_backup())
            // ...then return adjusted which_:
            return -(which_ + 1);

        // Otherwise, return which_ directly:
        return which_;
    }

private: // helpers, for structors (below)

    struct initializer
        : BOOST_VARIANT_AUX_INITIALIZER_T(
              recursive_enabled_types, recursive_enabled_T
            )
    {
    };

    void destroy_content()
    {
        detail::variant::destroyer visitor;
        this->internal_apply_visitor(visitor);
    }

public: // structors

    ~variant()
    {
        destroy_content();
    }

    variant()
    {
        // NOTE TO USER :
        // Compile error from here indicates that the first bound
        // type is not default-constructible, and so variant cannot
        // support its own default-construction.
        //
        new( storage_.address() ) internal_T0();
        indicate_which(0); // zero is the index of the first bounded type
    }

private: // helpers, for structors, cont. (below)

    class convert_copy_into
        : public static_visitor<int>
    {
    private: // representation

        void* storage_;

    public: // structors

        explicit convert_copy_into(void* storage)
            : storage_(storage)
        {
        }

    public: // internal visitor interfaces (below)

        template <typename T>
        int internal_visit(T& operand, int) const
        {
            // NOTE TO USER :
            // Compile error here indicates one of the source variant's types 
            // cannot be unambiguously converted to the destination variant's
            // types (or that no conversion exists).
            //
            return initializer::initialize(storage_, operand);
        }

#   if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x0564))
        template <typename T>
        result_type internal_visit(const T& operand, int) const
        {
            return initializer::initialize(storage_, operand);
        }
#   endif

        template <typename T>
        int internal_visit(boost::detail::reference_content<T>& operand, long) const
        {
            return internal_visit( operand.get(), 1L );
        }

        template <typename T>
        int internal_visit(const boost::detail::reference_content<T>& operand, long) const
        {
            return internal_visit( operand.get(), 1L );
        }

        template <typename T>
        int internal_visit(boost::detail::variant::backup_holder<T>& operand, long) const
        {
            return internal_visit( operand.get(), 1L );
        }

        template <typename T>
        int internal_visit(const boost::detail::variant::backup_holder<T>& operand, long) const
        {
            return internal_visit( operand.get(), 1L );
        }

        template <typename T>
        int internal_visit(boost::recursive_wrapper<T>& operand, long) const
        {
            return internal_visit( operand.get(), 1L );
        }

        template <typename T>
        int internal_visit(const boost::recursive_wrapper<T>& operand, long) const
        {
            return internal_visit( operand.get(), 1L );
        }

    };

    friend class convert_copy_into;

private: // helpers, for structors, below 

    template <typename T>
    void convert_construct(
          T& operand
        , int
        , mpl::false_ = mpl::false_() // is_foreign_variant
        )
    {
        // NOTE TO USER :
        // Compile error here indicates that the given type is not 
        // unambiguously convertible to one of the variant's types
        // (or that no conversion exists).
        //
        indicate_which(
              initializer::initialize(
                  storage_.address()
                , operand
                )
            );
    }

    template <typename Variant>
    void convert_construct(
          Variant& operand
        , long
        , mpl::true_// is_foreign_variant
        )
    {
        convert_copy_into visitor(storage_.address());
        indicate_which(
              operand.internal_apply_visitor(visitor)
            );
    }

    template <typename Variant>
    void convert_construct_variant(Variant& operand)
    {
        // [Determine if the given variant is itself a bounded type, or if its
        //  content needs to be converted (i.e., it is a 'foreign' variant):]
        //

        typedef typename mpl::find_if<
              types
            , is_same<
                  add_const<mpl::_1>
                , const Variant
                >
            >::type found_it;

        typedef typename mpl::end<types>::type not_found;
        typedef typename is_same<
              found_it, not_found
            >::type is_foreign_variant;

        // Convert construct from operand:
        convert_construct(
              operand, 1L
            , is_foreign_variant()
            );
    }

    template <BOOST_VARIANT_ENUM_PARAMS(typename U)>
    void convert_construct(
          boost::variant<BOOST_VARIANT_ENUM_PARAMS(U)>& operand
        , long
        )
    {
        convert_construct_variant(operand);
    }

    template <BOOST_VARIANT_ENUM_PARAMS(typename U)>
    void convert_construct(
          const boost::variant<BOOST_VARIANT_ENUM_PARAMS(U)>& operand
        , long
        )
    {
        convert_construct_variant(operand);    
    }

public: // structors, cont.

#if !BOOST_WORKAROUND(__MWERKS__, BOOST_TESTED_AT(0x3003)) \
 && !BOOST_WORKAROUND(BOOST_INTEL, BOOST_TESTED_AT(700)) \
 && !BOOST_WORKAROUND(BOOST_MSVC, <= 1300)

    template <typename T>
    variant(const T& operand)
    {
        convert_construct(operand, 1L);
    }

    template <typename T>
    variant(T& operand)
    {
        convert_construct(operand, 1L);
    }

#elif !defined(BOOST_VARIANT_AUX_NO_SFINAE) \
   && !BOOST_WORKAROUND(BOOST_INTEL, BOOST_TESTED_AT(700))

    // For compilers that cannot distinguish between T& and const T& in
    // template constructors, but do support SFINAE, we can workaround:

    template <typename T>
    variant(const T& operand)
    {
        convert_construct(operand, 1L);
    }

    template <typename T>
    variant(
          T& operand
        , typename detail::variant::enable_if<
              mpl::not_< is_const<T> >
            , void
            >::type* = 0
        )
    {
        convert_construct(operand, 1L);
    }

#else // defined(BOOST_NO_SFINAE)

    // For compilers that cannot distinguish between T& and const T& in
    // template constructors, and do NOT support SFINAE, we can't workaround:

    template <typename T>
    variant(const T& operand)
    {
        convert_construct(operand, 1L);
    }

#endif // CW8 and MSVC7 (and below) workarounds

public: // structors, cont.

    // [MSVC6 requires copy constructor appear after template constructors]
    variant(const variant& operand)
    {
        // Copy the value of operand into *this...
        detail::variant::copy_into visitor( storage_.address() );
        operand.internal_apply_visitor(visitor);

        // ...and activate the *this's primary storage on success:
        indicate_which(operand.which());
    }

private: // helpers, for modifiers (below)

#   if !defined(BOOST_NO_MEMBER_TEMPLATE_FRIENDS)
    template <typename Variant, typename RhsT>
    friend class detail::variant::backup_assigner;
#   endif

    // class assigner
    //
    // Internal visitor that "assigns" the visited value to the given variant
    // by appropriate destruction and copy-construction.
    //

    class assigner
        : public static_visitor<>
    {
    private: // representation

        variant& lhs_;
        int rhs_which_;

    public: // structors

        assigner(variant& lhs, int rhs_which)
            : lhs_(lhs)
            , rhs_which_(rhs_which)
        {
        }

    private: // helpers, for internal visitor interface (below)

        template <typename RhsT, typename B1, typename B2>
        void assign_impl(
              const RhsT& rhs_content
            , mpl::true_// has_nothrow_copy
            , B1// has_nothrow_move_constructor
            , B2// T0_has_nothrow_constructor
            )
        {
            // Destroy lhs's content...
            lhs_.destroy_content(); // nothrow

            // ...copy rhs content into lhs's storage...
            new(lhs_.storage_.address())
                RhsT( rhs_content ); // nothrow

            // ...and indicate success:
            lhs_.indicate_which(rhs_which_); // nothrow
        }

        template <typename RhsT, typename B>
        void assign_impl(
              const RhsT& rhs_content
            , mpl::false_// has_nothrow_copy
            , mpl::true_// has_nothrow_move_constructor
            , B// T0_has_nothrow_constructor
            )
        {
            // Attempt to make a temporary copy (so as to move it below)...
            RhsT temp(rhs_content);

            // ...and upon success destroy lhs's content...
            lhs_.destroy_content(); // nothrow

            // ...move the temporary copy into lhs's storage...
            new(lhs_.storage_.address())
                RhsT( detail::variant::move(temp) ); // nothrow

            // ...and indicate success:
            lhs_.indicate_which(rhs_which_); // nothrow
        }

        template <typename RhsT>
        void assign_impl(
              const RhsT& rhs_content
            , mpl::false_// has_nothrow_copy
            , mpl::false_// has_nothrow_move_constructor
            , mpl::true_// T0_has_nothrow_constructor
            )
        {
            // Destroy lhs's content...
            lhs_.destroy_content(); // nothrow

            try
            {
                // ...and attempt to copy rhs's content into lhs's storage:
                new(lhs_.storage_.address())
                    RhsT( rhs_content );
            }
            catch (...)
            {
                // In case of failure, default-construct T0 in lhs's storage...
                new (lhs_.storage_.address())
                    internal_T0; // nothrow

                // ...indicate the construction...
                lhs_.indicate_which(0);

                // ...and rethrow:
                throw;
            }

            // In the event of success, indicate success:
            lhs_.indicate_which(rhs_which_); // nothrow
        }

        template <typename RhsT>
        void assign_impl(
              const RhsT& rhs_content
            , mpl::false_// has_nothrow_copy
            , mpl::false_// has_nothrow_move_constructor
            , mpl::false_// T0_has_nothrow_constructor
            )
        {
            detail::variant::backup_assigner<wknd_self_t, RhsT>
                visitor(lhs_, rhs_which_, rhs_content);
            lhs_.internal_apply_visitor(visitor);
        }

    public: // internal visitor interfaces

        template <typename RhsT>
            BOOST_VARIANT_AUX_RETURN_VOID_TYPE
        internal_visit(const RhsT& rhs_content, int)
        {
            typedef typename has_nothrow_copy<RhsT>::type
                nothrow_copy;
            typedef typename mpl::or_< // reduces compile-time
                  nothrow_copy
                , detail::variant::has_nothrow_move_constructor<RhsT>
                >::type nothrow_move_constructor;

            assign_impl(
                  rhs_content
                , nothrow_copy()
                , nothrow_move_constructor()
                , T0_has_nothrow_constructor_()
                );

            BOOST_VARIANT_AUX_RETURN_VOID;
        }

    };

    friend class assigner;

    void variant_assign(const variant& rhs)
    {
        // If the contained types are EXACTLY the same...
        if (which_ == rhs.which_)
        {
            // ...then assign rhs's storage to lhs's content:
            detail::variant::assign_storage visitor(rhs.storage_.address());
            this->internal_apply_visitor(visitor);
        }
        else
        {
            // Otherwise, perform general (copy-based) variant assignment:
            assigner visitor(*this, rhs.which());
            rhs.internal_apply_visitor(visitor); 
        }
    }

private: // helpers, for modifiers (below)

    template <typename T>
    void assign(const T& rhs)
    {
        // If direct T-to-T assignment is not possible...
        detail::variant::direct_assigner<const T> direct_assign(rhs);
        if (this->apply_visitor(direct_assign) == false)
        {
            // ...then convert rhs to variant and assign:
            //
            // While potentially inefficient, the following construction of a
            // variant allows T as any type convertible to one of the bounded
            // types without excessive code redundancy.
            //
            variant temp(rhs);
            variant_assign( detail::variant::move(temp) );
        }
    }

public: // modifiers

    template <typename T>
    variant& operator=(const T& rhs)
    {
        assign(rhs);
        return *this;
    }

    // [MSVC6 requires copy assign appear after templated operator=]
    variant& operator=(const variant& rhs)
    {
        variant_assign(rhs);
        return *this;
    }

    void swap(variant& rhs)
    {
        // If the contained internal types are EXACTLY the same...
        if (which_ == rhs.which_)
        {
            // ...then swap the values directly:
            detail::variant::swap_with visitor(storage_.address());
            rhs.internal_apply_visitor(visitor);
        }
        else
        {
            // ...otherwise, perform general variant swap:
            variant tmp( detail::variant::move(rhs) );
            rhs = detail::variant::move(*this);
            *this = detail::variant::move(tmp);
        }
    }

public: // queries

    //
    // NOTE: member which() defined above.
    //

    bool empty() const
    {
        typedef typename mpl::begin<internal_types>::type
            begin_it;

        typedef typename mpl::find<
              internal_types, boost::empty
            >::type empty_it;

        typedef typename mpl::distance<
              begin_it, empty_it
            >::type empty_index;

        return which() == empty_index::value;
    }

    const std::type_info& type() const
    {
        detail::variant::reflect visitor;
        return this->apply_visitor(visitor);
    }

public: // prevent comparison with foreign types

#if !BOOST_WORKAROUND(BOOST_MSVC, <= 1300)

#   define BOOST_VARIANT_AUX_FAIL_COMPARISON_RETURN_TYPE \
    void

#else // MSVC7

    //
    // MSVC7 gives error about return types for above being different than
    // the true comparison operator overloads:
    //

#   define BOOST_VARIANT_AUX_FAIL_COMPARISON_RETURN_TYPE \
    bool

#endif // MSVC7 workaround

    template <typename U>
        BOOST_VARIANT_AUX_FAIL_COMPARISON_RETURN_TYPE
    operator==(const U&) const
    {
        BOOST_STATIC_ASSERT( false && sizeof(U) );
    }

    template <typename U>
        BOOST_VARIANT_AUX_FAIL_COMPARISON_RETURN_TYPE
    operator<(const U&) const
    {
        BOOST_STATIC_ASSERT( false && sizeof(U) );
    }

public: // comparison operators

    // [MSVC6 requires these operators appear after template operators]

    bool operator==(const variant& rhs) const
    {
        if (this->which() != rhs.which())
            return false;

        detail::variant::comparer<
              variant, detail::variant::equal_comp
            > visitor(*this);
        return rhs.apply_visitor(visitor);
    }

    bool operator<(const variant& rhs) const
    {
        //
        // Dirk Schreib suggested this collating order.
        //

        if (this->which() != rhs.which())
            return this->which() < rhs.which();

        detail::variant::comparer<
              variant, detail::variant::less_comp
            > visitor(*this);
        return rhs.apply_visitor(visitor);
    }

// helpers, for visitation support (below) -- private when possible
#if !defined(BOOST_NO_MEMBER_TEMPLATE_FRIENDS)

    template < BOOST_VARIANT_ENUM_PARAMS(typename U) >
    friend class variant;

private:

#else// defined(BOOST_NO_MEMBER_TEMPLATE_FRIENDS)

public:

#endif// !defined(BOOST_NO_MEMBER_TEMPLATE_FRIENDS)

    template <typename Visitor>
        BOOST_VARIANT_AUX_GENERIC_RESULT_TYPE(
              typename Visitor::result_type
            )
    internal_apply_visitor(Visitor& visitor)
    {
        typedef mpl::int_<0> first_which;
        typedef typename mpl::begin<internal_types>::type first_it;
        typedef typename mpl::end<internal_types>::type last_it;
        typedef detail::variant::visitation_impl_step<
              first_it, last_it
            > first_step;

        return detail::variant::visitation_impl(
              which_, which()
            , visitor, storage_.address(), mpl::false_()
            , never_uses_backup_flag()
            , static_cast<first_which*>(0), static_cast<first_step*>(0)
            );
    }

    template <typename Visitor>
        BOOST_VARIANT_AUX_GENERIC_RESULT_TYPE(
              typename Visitor::result_type
            )
    internal_apply_visitor(Visitor& visitor) const
    {
        typedef mpl::int_<0> first_which;
        typedef typename mpl::begin<internal_types>::type first_it;
        typedef typename mpl::end<internal_types>::type last_it;
        typedef detail::variant::visitation_impl_step<
              first_it, last_it
            > first_step;

        return detail::variant::visitation_impl(
              which_, which()
            , visitor, storage_.address(), mpl::false_()
            , never_uses_backup_flag()
            , static_cast<first_which*>(0), static_cast<first_step*>(0)
            );
    }

public: // visitation support

    template <typename Visitor>
        BOOST_VARIANT_AUX_GENERIC_RESULT_TYPE(
              typename Visitor::result_type
            )
    apply_visitor(Visitor& visitor)
    {
        detail::variant::invoke_visitor<Visitor> invoker(visitor);
        return this->internal_apply_visitor(invoker);
    }

    template <typename Visitor>
        BOOST_VARIANT_AUX_GENERIC_RESULT_TYPE(
              typename Visitor::result_type
            )
    apply_visitor(Visitor& visitor) const
    {
        detail::variant::invoke_visitor<Visitor> invoker(visitor);
        return this->internal_apply_visitor(invoker);
    }

}; // class variant

///////////////////////////////////////////////////////////////////////////////
// function template swap
//
// Swaps two variants of the same type (i.e., identical specification).
//
template < BOOST_VARIANT_ENUM_PARAMS(typename T) >
inline void swap(
      variant< BOOST_VARIANT_ENUM_PARAMS(T) >& lhs
    , variant< BOOST_VARIANT_ENUM_PARAMS(T) >& rhs
    )
{
    lhs.swap(rhs);
}

} // namespace boost

// implementation additions
#include "boost/variant/detail/variant_io.hpp"

#endif // BOOST_VARIANT_VARIANT_HPP
