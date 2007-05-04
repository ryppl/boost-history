template<typename Id, typename ArchivePair, typename Signature>
typename boost::enable_if_c<boost::function_traits<Signature>::arity==BOOST_ARITY_NUM_ARGS, std::string>::type
serialize(typename boost::call_traits<Id>::param_type id BOOST_ARITY_COMMA BOOST_ARITY_TYPE_PARMS_LONG(Signature))
{
    std::stringstream stream(std::ios::in | std::ios::out | std::ios::binary);
    typename ArchivePair::oarchive_type archive(stream);
    archive & id;
    BOOST_ARITY_ON_PARMS(BOOST_ARITY_NUM_ARGS,archive & )
    return stream.str();
}
