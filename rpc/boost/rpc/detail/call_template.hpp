// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef DOXYGEN_DOCS_ONLY
template<typename Id, typename Signature, typename ArchivePair>
class call<Id, Signature, ArchivePair
    , typename enable_if_c<boost::function_traits<Signature>::arity==BOOST_ARITY_NUM_ARGS>::type
   >
   : public async_returning_call<typename boost::function_traits<Signature>::result_type>
#endif // DOXYGEN_DOCS_ONLY
{
#ifdef DOXYGEN_DOCS_ONLY
public:
    /// Prepares the call with provided id and parameters.
    call(typename boost::call_traits<Id>::param_type id, Args..);
    /// Prepares the call with provided id and parameters.
    void set(typename boost::call_traits<Id>::param_type id, Agrs...);
    void reset();
protected:
    virtual const std::string &parameters() const;
    virtual void result_string(const std::string &str, const call_options &options);
};
#else
    BOOST_ARITY_TYPEDEF(BOOST_ARITY_storable_TYPENAME, BOOST_ARITY_argN_storable_type_NAME)
    BOOST_ARITY_CMAMC(typedef, BOOST_ARITY_function_traits_argN_type_TYPENAME, Signature, BOOST_ARITY_argN_type_NAME, ;)

    BOOST_PP_REPEAT_FROM_TO(1,BOOST_ARITY_NUM_ARGS_INC,BOOST_RPC_ARGUMENT,_)

public:
    call(typename boost::call_traits<Id>::param_type id
        BOOST_PP_ENUM_TRAILING(BOOST_ARITY_NUM_ARGS,BOOST_RPC_PARAM_TYPE_ARG,BOOST_PP_EMPTY))
    BOOST_PP_IF(BOOST_ARITY_NUM_ARGS,:,BOOST_PP_EMPTY())
    BOOST_PP_ENUM_SHIFTED(BOOST_ARITY_NUM_ARGS_INC,BOOST_RPC_INIT_ARGUMENT,_)
    {
        params = detail::serialize<Id, ArchivePair,Signature>()
            (id BOOST_ARITY_COMMA BOOST_PP_ENUM_SHIFTED_PARAMS(BOOST_ARITY_NUM_ARGS_INC,a));
    }
    std::auto_ptr<handler_base> spawn_handler()
    {
        std::auto_ptr<handler_base> handler_ptr(
            new handler<Signature, ArchivePair>(
            BOOST_ARITY_ENUM(BOOST_RPC_VAR_arg_storableN_NAME,BOOST_PP_EMPTY())));
        return handler_ptr;
    }
protected:
    virtual const std::string &parameters() const {return params;}
private:
    std::string params;

#endif
};
