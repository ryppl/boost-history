template<typename Id, typename ArchivePair, typename Signature>
typename boost::enable_if_c<boost::function_traits<Signature>::arity==BOOST_ARITY_NUM_ARGS, std::string>::type
serialize(typename boost::call_traits<Id>::param_type id
          BOOST_ARITY_ENUM_TRAILING_MAM(BOOST_ARITY_function_traits_argN_type_TYPENAME, Signature, BOOST_ARITY_aN_NAME))
{
    std::stringstream stream(std::ios::in | std::ios::out | std::ios::binary);
    typename ArchivePair::oarchive_type archive(stream);
    archive & id;
    BOOST_ARITY_CMC(archive &, BOOST_ARITY_aN_NAME, ;)
    return stream.str();
}
