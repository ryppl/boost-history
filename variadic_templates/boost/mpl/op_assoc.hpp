#ifndef BOOST_MPL_OP_ASSOC_HPP_VARIADIC_TEMPLATES
#define BOOST_MPL_OP_ASSOC_HPP_VARIADIC_TEMPLATES

namespace boost { namespace mpl {

enum op_assoc
//Indicates how some binary operator, op, associates.
{ assoc_left  //associates to left : a op b op c = (a op b) op c
, assoc_right //associates to right: a op b op c = a op (b op c)
};

}}//exit boost::mpl namespace

#endif //include guard
