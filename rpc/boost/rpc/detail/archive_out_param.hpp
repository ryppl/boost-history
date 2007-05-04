#ifndef BOOST_RPC_ARCHIVE_OUT_PARAM_HPP
#define BOOST_RPC_ARCHIVE_OUT_PARAM_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits.hpp>
#include <boost/future/future.hpp>

#define BOOST_RPC_ARCHIVE_OUT_PARAM(z,I,archive) \
    boost::rpc::detail:: \
    archive_out_param<boost::function_traits<Signature>::arg##I##_type>(archive,arg_storable##I);

#define BOOST_RPC_PARAM_TYPE(I) \
    typename boost::rpc::argument<BOOST_ARITY_ARG_TYPE_NAME(I)>::type

#define BOOST_RPC_PARAM_TYPE_ARG(J,I,D) BOOST_RPC_PARAM_TYPE(I) BOOST_PP_CAT(a,BOOST_PP_INC(I))

#define BOOST_RPC_ARGUMENT(z,n,text) \
  boost::rpc:: \
  argument<typename boost::function_traits<Signature>::arg##n##_type> arg_storable##n;

#define BOOST_RPC_INIT_ARGUMENT(z,n,text) \
 BOOST_PP_CAT(arg_storable,n)(BOOST_PP_CAT(a,n))

#define BOOST_RPC_ARGUMENT_ASSIGN_PROMISE(z,I,text) \
    BOOST_PP_CAT(arg_storable,I).assign_promise();
namespace boost {
namespace rpc {

template<typename T, typename Enable=void, typename Enable2=void>
struct is_out_argument
    : public boost::is_void<int>
{};

template<typename T>
struct is_out_argument<T,
    typename boost::enable_if<boost::is_reference<T> >::type,
    typename boost::disable_if<boost::is_const<typename boost::remove_reference<T>::type> >::type>
    : public boost::is_void<void>
{};

/// Wrapper for possibly "out" arguments to a remote call
template<typename T, typename Enable=void>
class argument
{
public:
    typedef T type;
    argument(T){}
    void assign_promise(){}
};

template<typename T>
class argument<T, typename boost::enable_if<is_out_argument<T> >::type>
{
public:
    typedef argument<T> type;
    typedef typename boost::remove_reference<T>::type referred_type;
    typedef typename boost::detail::storable<T>::type storable_type;

    argument(T ref) : ref_future(promise)
    {
        passed = by_ref;
        referred_ptr = &ref;
    }
    argument(boost::future<referred_type &> &future) : ref_future(promise)
    {
        passed = by_ref_future;
        ref_future_ptr = &future;
    }
    argument(boost::future<referred_type> &future) : ref_future(promise)
    {
        passed = by_future;
        future_ptr = &future;
    }
    operator T()
    {
        switch(passed)
        {
        case by_ref:
            return *referred_ptr;
        case by_ref_future:
            return ref_future_ptr->get();
        case by_future:
            storable = future_ptr->get();
            *future_ptr = boost::future<referred_type>(promise);
            return storable;
        }
        return storable;
    }
    template<typename Archive>
    void deserialize(Archive &archive)
    {
        if (passed == by_future)
        {
            archive & storable;
            promise.set(storable);
        }
        else
        {
            archive & storable;
            promise.set(storable);
//          if (passed == by_ref_future)
//              *ref_future_ptr = boost::future<referred_type &>(promise);
        }
    }
    void assign_promise()
    {
        if (passed == by_ref)
            *referred_ptr = ref_future;
    }
private:
    union
    {
        referred_type *referred_ptr;
        boost::future<T> *ref_future_ptr;
        boost::future<referred_type> *future_ptr;
    };
    enum
    {
        by_ref,
        by_future,
        by_ref_future
    } passed;
    storable_type storable;
    boost::promise<referred_type> promise;
    boost::future<referred_type> ref_future;
};

namespace detail {

template<typename T, typename Enable=void>
struct reference_copy
{
    reference_copy(T){}
};

template<typename T>
struct reference_copy<T, typename boost::enable_if<boost::is_reference<T> >::type>
{
    T t_ref;
    reference_copy(T t_ref) : t_ref(t_ref) {}
};

#define BOOST_RPC_REF_COPY(z,n,text) \
  boost::rpc::detail:: \
  reference_copy<typename boost::function_traits<Signature>::arg##n##_type> arg_storable##n;

#define BOOST_RPC_INIT_REF_COPY(z,n,text) \
 BOOST_PP_CAT(arg_storable,n)(BOOST_PP_CAT(a,n))

template<typename T, typename Archive>
typename boost::enable_if<typename boost::is_reference<T> >::type
archive_out_param(Archive &archive, T t)
{
    archive & t;
}

template<typename T, typename Archive>
typename boost::disable_if<boost::is_reference<T> >::type
archive_out_param(Archive &archive, T t)
{
}

template<typename T, typename Archive>
typename boost::enable_if<typename boost::is_reference<T> >::type
archive_out_param (Archive &archive, reference_copy<T> t)
{
    archive & t.t_ref;
}

template<typename T, typename Archive>
typename boost::disable_if<typename boost::is_reference<T> >::type
archive_out_param (Archive &, reference_copy<T>)
{
}

template<typename T, typename Archive>
typename boost::enable_if<boost::rpc::is_out_argument<T> >::type
archive_out_param (Archive &archive, argument<T> &t)
{
    t.deserialize(archive);
}

template<typename T, typename Archive>
typename boost::disable_if<boost::rpc::is_out_argument<T> >::type
archive_out_param (Archive &, argument<T> &)
{
}

} // namespace detail
} // namespace rpc
} // namespace boost

#endif
