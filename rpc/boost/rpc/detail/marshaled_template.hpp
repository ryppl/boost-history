template<typename Signature, typename ArchivePair>
class marshaled_impl<Signature, ArchivePair,
    BOOST_ARITY_NUM_ARGS, BOOST_ARITY_ENABLE_DISABLE_VOID>
    : public function<ArchivePair>
{
    BOOST_ARITY_TYPEDEF(BOOST_ARITY_storable_TYPENAME, BOOST_ARITY_argN_storable_type_NAME)
    BOOST_ARITY_CMAMC(typedef, BOOST_ARITY_function_traits_argN_type_TYPENAME, Signature, BOOST_ARITY_argN_type_NAME, ;)
public:
    marshaled_impl(const boost::function<Signature> &f) : func(f)
    {}
    virtual void call(const boost::rpc::call_options &options,
        typename ArchivePair::iarchive_type &params,
        typename ArchivePair::oarchive_type *results = 0)
    {
        BOOST_ARITY_MREPEAT(MMC, (BOOST_ARITY_argN_storable_type_NAME, BOOST_ARITY_arg_storableN_NAME, ;))
        BOOST_ARITY_MREPEAT(CMC, (params &, BOOST_ARITY_arg_storableN_NAME, ;))
        
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
