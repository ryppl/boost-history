// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef DOXYGEN_DOCS_ONLY
template<typename Id, typename Signature, typename ArchivePair>
class call<Id, Signature, ArchivePair
#ifndef DOXYGEN_DOCS_ONLY
    , typename enable_if_c<boost::function_traits<Signature>::arity==BOOST_ARITY_NUM_ARGS>::type,
   BOOST_ARITY_ENABLE_DISABLE_VOID
#endif
   >
   : public sync_returning_call<typename boost::function_traits<Signature>::result_type>
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
    BOOST_ARITY_STORABLE_TYPES(Signature)
    BOOST_ARITY_TYPES(Signature)

    BOOST_PP_REPEAT_FROM_TO(1,BOOST_ARITY_NUM_ARGS_INC,BOOST_RPC_REF_COPY,_)

public:
    call(typename boost::call_traits<Id>::param_type id BOOST_ARITY_COMMA
        BOOST_ARITY_TYPE_PARMS)
    BOOST_PP_IF(BOOST_ARITY_NUM_ARGS,:,BOOST_PP_EMPTY())
    BOOST_PP_ENUM_SHIFTED(BOOST_ARITY_NUM_ARGS_INC,BOOST_RPC_INIT_REF_COPY,_)
    {
        set(id BOOST_ARITY_COMMA BOOST_PP_ENUM_SHIFTED_PARAMS(BOOST_ARITY_NUM_ARGS_INC,a));
    }
    void set(typename boost::call_traits<Id>::param_type id BOOST_ARITY_COMMA
        BOOST_ARITY_TYPE_PARMS)
    {
        call_base::reset();
        params = detail::serialize<Id, ArchivePair,Signature>
            (id BOOST_ARITY_COMMA BOOST_PP_ENUM_SHIFTED_PARAMS(BOOST_ARITY_NUM_ARGS_INC,a));
    }
    void reset()
    {
        call_base::reset();
    }
protected:
    virtual const std::string &parameters() const {return params;}
    virtual void result_string(const std::string &str, const call_options &options)
    {
        std::stringstream stream(str, std::ios::in | std::ios::out | std::ios::binary);
        typename ArchivePair::iarchive_type archive(stream);
#ifdef BOOST_ARITY_NON_VOID_RETURN_TYPE
        if (options.marshal_option >= call_options::return_only)
            archive & return_val;
#endif
        if (options.marshal_option >= boost::rpc::call_options::all_out_parameters)
        {
            BOOST_PP_REPEAT_FROM_TO(1, BOOST_ARITY_NUM_ARGS_INC, BOOST_RPC_ARCHIVE_OUT_PARAM, archive)
        }
    };
private:
    std::string params;

#endif
};
