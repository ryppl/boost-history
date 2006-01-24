//demonstrate difference between visitable and not-visitable fields.
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/empty_base.hpp>
#include <boost/mpl/arg.hpp>
#include <boost/mpl/for_each.hpp>
#include "boost/fields_visitor/fields_visitor.hpp"
#include <iostream>

  enum
visitability_numerals
{ visitable_not
, visitable_yes
};

  enum
field_numerals
{ field_n0
, field_n1
, field_n2
};

  template
  < visitability_numerals Visitability
  , field_numerals FieldNumber
  >
  struct
fields
;

  template
  < field_numerals FieldNumber
  >
  struct
fields
  < visitable_not
  , FieldNumber
  >
{
      field_numerals
    field_number(void)const
    {
        return FieldNumber;
    }
    
};

  struct
visit_general_field
/**@brief
 *  A visitor able to visit any field.
 */
{
      template
      < typename CurrentField
      >
      void
    visit_field(CurrentField& a_field)
    {
        std::cout<<"visiting_general_field:field_number="<<a_field.field_number()<<"\n";
    }

};

  template
  < class VisitOtherFields
  , class FieldId //boost::mpl::integral_c<field_numerals,FieldNumeral>
  >
  struct
visit_specific_field
  : public VisitOtherFields
/**@brief
 *  Adds (to VisitOtherFields) ability to visit 
 *  _specifically_ a field with id=FieldId::value
 */
{
        using
    VisitOtherFields::visit_field
    //allow overloading of this->visit_field with superclass's visit_fields
    ;
        typedef
      fields<visitable_yes,FieldId::value>
    current_field_type
    ;
      void
    visit_field(current_field_type& a_field)
    {
        std::cout<<"visiting_specific_field:FieldId="<<FieldId::value<<"\n";
    }
};

    typedef
  boost::mpl::vector_c
  < field_numerals
  , field_n0
  , field_n2
  >
visited_indices_type
//indices of fields visited specificly
;

  struct
visit_all_fields
  : public
    boost::mpl::fold
    < visited_indices_type
    , visit_general_field
    , visit_specific_field
      < boost::mpl::arg<1>
      , boost::mpl::arg<2>
      >
    >::type
{
};

  template
  < field_numerals FieldNumber
  >
  struct
fields
  < visitable_yes
  , FieldNumber
  >
/**@brief
 *  Simply show what needs to be done to change visitable_not
 *  to visitable_yes field, i.e.:
 *    1) Add field_registrar<SomeVisitor> superclass
 *       for some fields visitor type, SomeVisitor.
 *    2) In all CTOR's, pass this pointer to that superclass.
 */
  : public fields
  < visitable_not
  , FieldNumber
  >
  , public boost::fields_visitor::field_registrar<visit_all_fields>
{
        typedef
      boost::fields_visitor::field_registrar<visit_all_fields>
    registrars_type
    ;
    
    fields(void)
      : registrars_type(this)
    {}
    
    fields(fields const&)
      : registrars_type(this)
    {}

};

  template
  < class Visitability
  , class Left
  , class Right
  >
  struct
inherit_left
/**@brief
 *  ForwardOp for use in inherit_linearly which
 *  adds field with id=Right::value to 
 *  those fields inherited from Left.
 */
  : public Left
{
      fields<Visitability::value,Right::value>
    my_field
    ;
};
    
    typedef
  boost::mpl::vector_c
  < field_numerals
  , field_n0
  , field_n1
  , field_n1
  , field_n2
  >
contained_indices_type
//indices of fields contained in record
;

  template
  < visitability_numerals Visitability
  >
  struct
record
/**@brief
 *  Record containing tuple of fields,
 *  visitable or not, depending in Visitability.
 */
{
        typedef
      typename boost::mpl::fold
      < contained_indices_type
      , boost::mpl::empty_base
      , inherit_left
        < boost::mpl::integral_c<visitability_numerals,Visitability>
        , boost::mpl::arg<1>
        , boost::mpl::arg<2>
        >
      >::type
    fields_type
    //type of tuple of contained fields
    ;
      fields_type
    my_fields
    //actual tuple
    ;
};

SELECTED_FIELDS_DESCRIPTION_OF_RECORD(record<visitable_yes>)

using namespace boost;

  template
  < class Visitor
  , class Record
  >
  unsigned
accept_each(Visitor& a_visitor, Record& a_record)
{
    typedef typename boost::fields_visitor::field_iterator_of<Visitor,Record> iter_type;
    iter_type a_iter(&a_record);
    unsigned n_iter=0;
    for(; !a_iter.empty(); a_iter.increment(),++n_iter)
    {
        a_iter.accept_visitor(a_visitor);
    }
    return n_iter;
}

  struct
print_field_numerals
{
    template<field_numerals FieldId>
    void operator()(boost::mpl::integral_c<field_numerals,FieldId>)
    {
        std::cout<<"print_field_numerals::operator():FielId="<<FieldId<<"\n";
    }
};

void test(void)
{
    {
        std::cout<<"print<contained_indices_type>=\n";
        print_field_numerals print_numerals;
        boost::mpl::for_each<contained_indices_type>(print_numerals);
        std::cout<<"\n";
    }
    {
        std::cout<<"print<visited_indices_type>=\n";
        print_field_numerals print_numerals;
        boost::mpl::for_each<visited_indices_type>(print_numerals);
        std::cout<<"\n";
    }
    visit_all_fields a_all_visitor;
    {
        record<visitable_not> a_record;
        std::cout<<"accept_each:visitable_not\n";
        unsigned n=accept_each(a_all_visitor,a_record);
        std::cout<<"number of fields="<<n<<"\n\n";
    }
    {
        record<visitable_yes> a_record;
        std::cout<<"accept_each:visitable_yes\n";
        unsigned n=accept_each(a_all_visitor,a_record);
        std::cout<<"number of fields="<<n<<"\n\n";
    }
}

int main(void)
{
    test();
    return 0;
}
