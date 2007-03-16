//derivation of shared_count_visitor_abs for collecting cycles
#ifndef BOOST_POLICY_PTR_DETAIL_COLLECTOR_HPP_LJE20050523
#define BOOST_POLICY_PTR_DETAIL_COLLECTOR_HPP_LJE20050523
namespace boost
{
namespace policy_ptr
{
namespace detail
{

  template
  < class
  >
  class
collector
/**@brief
 *  General template collector for smart pointers.  This general template
 *  does nothing.  Specializations may implement specific collector
 *  methods.
 */
{
 public:
        static
      void
    collect(void)
    {}
};

}//exit detail namespace
}//exit policy_ptr namespace
}//exit boost namespace
#endif
