template<typename Signature, typename ArchivePair>
class marshaled_impl<Signature, ArchivePair,
    BOOST_ARITY_NUM_ARGS, BOOST_ARITY_ENABLE_DISABLE_VOID>
    : public function<ArchivePair>
{
    BOOST_ARITY_STORABLE_TYPES(Signature)
    BOOST_ARITY_TYPES(Signature)
public:
    marshaled_impl(const boost::function<Signature> &f) : func(f)
    {}
    virtual void call(const boost::rpc::call_options &options,
        typename ArchivePair::iarchive_type &params,
        typename ArchivePair::oarchive_type *results = 0)
    {
        BOOST_ARITY_STORABLES(BOOST_ARITY_NUM_ARGS)
        BOOST_ARITY_ON_STORABLES(BOOST_ARITY_NUM_ARGS,params & )
        
#ifdef BOOST_ARITY_NON_VOID_RETURN_TYPE
        if (options.marshal_option >= boost::rpc::call_options::return_only)
        {
            typename boost::detail::storable<
                typename boost::function_traits<Signature>::result_type>::type return_val;
            return_val = func(BOOST_PP_ENUM_SHIFTED_PARAMS(BOOST_ARITY_NUM_ARGS_INC,arg_storable));
            // now serialize the return value
            BOOST_ASSERT(results);
            *results & return_val;
        }
        else
#endif
            func(BOOST_PP_ENUM_SHIFTED_PARAMS(BOOST_ARITY_NUM_ARGS_INC,arg_storable));
        if (options.marshal_option >= boost::rpc::call_options::all_out_parameters)
        {
            BOOST_ASSERT(results);
            BOOST_PP_REPEAT_FROM_TO(1, BOOST_ARITY_NUM_ARGS_INC, BOOST_RPC_ARCHIVE_OUT_PARAM, *results)
        }
    };
private:

    boost::function<Signature> func;
};
