//demonstrate visit_each using fields_visitor to access subobjects.
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/empty_base.hpp>
#include <boost/mpl/arg.hpp>
#include <boost/mpl/for_each.hpp>
#include "boost/fields_visitor/fields_visitor.hpp"
#include "boost/io/filters/mout.hpp"

namespace boost {
  template<typename Visitor, typename Record>
  void visit_each(Visitor& a_visitor,Record& a_record, int)
  {
    mout()<<"visit_each::record_number="<<a_record.field_number()<<"\n";
    a_visitor(a_record);
    ++mout();
    a_visitor.visit_fields(a_record);
    --mout();
  }
}

  enum
field_numerals
{ field_n0
, field_n1
, field_n2
};

  template
  < field_numerals FieldNumber
  >
  struct
fields
;

  template
  < typename CrtcVisitor //Curiously Recurring Template Class Visitor
  >
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
        CrtcVisitor& a_visitor=static_cast<CrtcVisitor&>(*this);
        boost::visit_each(a_visitor,a_field,0);
    }

      template
      < typename CurrentField
      >
      void
    visit_fields(CurrentField& a_field)
    {
            typedef 
          typename boost::fields_visitor::field_iterator_of<CrtcVisitor,CurrentField> 
        iter_type;
        iter_type a_iter(&a_field);
        CrtcVisitor& a_visitor=static_cast<CrtcVisitor&>(*this);
        for(; !a_iter.empty(); a_iter.increment())
        {
            a_iter.accept_visitor(a_visitor);
        }
    }
      template
      < typename CurrentField
      >
      void
    operator()(CurrentField& a_field)
    {
        mout()<<"visitor::operator():general_field_number="<<a_field.field_number()<<"\n";
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
    VisitOtherFields::operator()
    //allow overloading of this->operator() with superclass's operator()s
    ;
        typedef
      fields<FieldId::value>
    current_field_type
    ;
      void
    operator()(current_field_type& a_field)
    {
        mout()<<"visitor::operator()::specific_field_number="<<a_field.field_number()<<"\n";
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
    , visit_general_field<visit_all_fields>
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
  : public boost::fields_visitor::field_registrar<visit_all_fields>
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

      field_numerals
    field_number(void)const
    {
        return FieldNumber;
    }
    
};

  template
  < class Left
  , class Right
  >
  struct
inherit_left
/**@brief
 *  ForwardOp, for use in fold, which
 *  adds field with id=Right::value to 
 *  those fields inherited from Left.
 */
  : public Left
{
      fields<Right::value>
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

  struct
record
/**@brief
 *  Record containing tuple of fields,
 */
{
        typedef
      boost::mpl::fold
      < contained_indices_type
      , boost::mpl::empty_base
      , inherit_left
        < boost::mpl::arg<1>
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
      int
    field_number(void)const
    {
        return -1;
    }
    
};

SELECTED_FIELDS_DESCRIPTION_OF_RECORD(record)

using namespace boost;

  struct
print_field_numerals
{
    template<field_numerals FieldId>
    void operator()(boost::mpl::integral_c<field_numerals,FieldId>)
    {
        mout()<<"print_field_numerals::operator():FielId="<<FieldId<<"\n";
    }
};

void test(void)
{
    {
        mout()<<"print<contained_indices_type>=\n";
        print_field_numerals print_numerals;
        boost::mpl::for_each<contained_indices_type>(print_numerals);
        mout()<<"\n";
    }
    {
        mout()<<"print<visited_indices_type>=\n";
        print_field_numerals print_numerals;
        boost::mpl::for_each<visited_indices_type>(print_numerals);
        mout()<<"\n";
    }
    visit_all_fields a_all_visitor;
    {
        record a_record;
        mout()<<"boost::visit_each(a_record)\n";
        boost::visit_each(a_all_visitor,a_record,0);
    }
}

int main(void)
{
    test();
    return 0;
}

//ChangeLog:
//  2006-01-23:
//    1) copied from simple_record_field_traversal_test.cpp
//    2) elminated the visitability_numerals
//    2) enabled graph traversal using CRTP and visit_each
//       instead of accept_each.

