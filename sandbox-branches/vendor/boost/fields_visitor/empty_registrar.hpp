//defines empty (do nothing ) registrar
#ifndef BOOST_FIELDS_VISITOR_EMPTY_REGISTRAR_LJE20050318
#define BOOST_FIELDS_VISITOR_EMPTY_REGISTRAR_LJE20050318
namespace boost
{
namespace fields_visitor
{
namespace registrar_heirarchy_detail
{
  struct
empty_registrar
/**@brief
 *  "Top" element in a registrar_heirarchy<VisitorSequence>.
 *  (See registrar_heirarchy.hpp)
 */
{
      template<typename SelectedField>
    empty_registrar
      ( SelectedField*a_field
      )
    {
      #if 0 && defined(CONTAINER_SINGLE_OBJ_ID)
        debug_utility::trace_scope ts("empty_registrar(SelectedField*a_field)");
        mout()<<":id_get="<<a_field->id_get()<<"\n";
      #endif
    }
};

}//exit registrar_heirarchy_detail
}//exit fields_visitor namespace
}//exit boost namespace
#endif
