// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef DOXYGEN_DOCS_ONLY
template<typename Signature, typename ArchivePair>
class handler<Signature, ArchivePair
    , typename enable_if_c<boost::function_traits<Signature>::arity==BOOST_ARITY_NUM_ARGS>::type,
   BOOST_ARITY_ENABLE_DISABLE_VOID
   >
   : public async_returning_handler<typename boost::function_traits<Signature>::result_type>
#endif // !DOXYGEN_DOCS_ONLY
{
#ifdef DOXYGEN_DOCS_ONLY
public:
    /// Initializes the handler with the specified arguments.
    handler(...)
    /// True if this handler has "out" arguments.
    bool has_out_parameters()
    {
        bool has_out = false;
        BOOST_PP_REPEAT_FROM_TO(1,BOOST_ARITY_NUM_ARGS_INC,BOOST_RPC_ARGUMENT_CHECK_OUT,BOOST_PP_EMPTY())
        return has_out;
    }
    /// Assigns the promises of the handler to storage variables.
    void assign_promises()
    {
        BOOST_PP_REPEAT_FROM_TO(1,BOOST_ARITY_NUM_ARGS_INC,BOOST_RPC_ARGUMENT_ASSIGN_PROMISE,BOOST_PP_EMPTY())
    }
    /// Handler will process the marshalled result string.
    virtual void result_string(const std::string &str)
};
#else
    typedef async_returning_handler<typename boost::function_traits<Signature>::result_type> base_type;
    typedef typename base_type::ptr ptr;
    
    BOOST_ARITY_TYPEDEF(BOOST_ARITY_storable_TYPENAME, BOOST_ARITY_argN_storable_type_NAME)
    BOOST_ARITY_CMAMC(typedef, BOOST_ARITY_function_traits_argN_type_TYPENAME, Signature, BOOST_ARITY_argN_type_NAME, ;)

    BOOST_ARITY_REPEAT(BOOST_RPC_TYPE_PARGUMENT_VAR,BOOST_RPC_VAR_arg_storableN_NAME)
public:
    handler(BOOST_ARITY_ENUM(BOOST_RPC_TYPE_ARGUMENT_CREF_ARG,BOOST_RPC_VAR_arg_storableN_NAME))
        BOOST_ARITY_COLON
        BOOST_ARITY_ENUM(BOOST_RPC_VAR_arg_storableN_INIT,BOOST_RPC_VAR_arg_storableN_NAME)
    {}
    bool has_out_parameters()
    {
        bool has_out = false;
        BOOST_PP_REPEAT_FROM_TO(1,BOOST_ARITY_NUM_ARGS_INC,BOOST_RPC_ARGUMENT_CHECK_OUT,BOOST_PP_EMPTY())
        return has_out;
    }
    void assign_promises()
    {
        BOOST_PP_REPEAT_FROM_TO(1,BOOST_ARITY_NUM_ARGS_INC,BOOST_RPC_ARGUMENT_ASSIGN_PROMISE,BOOST_PP_EMPTY())
    }
    virtual void result_string(const std::string &str)
    {
        std::stringstream stream(str, std::ios::in | std::ios::out | std::ios::binary);
        typename ArchivePair::iarchive_type archive(stream);
        extract_return_val(archive, base_type::options);
#ifdef BOOST_ARITY_NON_VOID_RETURN_TYPE
        base_type::return_prom.set(base_type::return_val);
#else
		base_type::return_prom.set();
#endif
        if (base_type::options.marshal_option >= boost::rpc::call_options::all_out_parameters)
        {
            BOOST_PP_REPEAT_FROM_TO(1, BOOST_ARITY_NUM_ARGS_INC, BOOST_RPC_ARCHIVE_OUT_PARAM, archive)
        }
    };
#endif
};