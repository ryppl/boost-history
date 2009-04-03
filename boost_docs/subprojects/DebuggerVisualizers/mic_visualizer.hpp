// Copyright Filip Konvièka 2007 - 2009. Use, modification and distribution are subject
// to the Boost Software License, Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef MIC_VISUALIZER_HPP
#define MIC_VISUALIZER_HPP

#if !defined(_DEBUG)||!defined(_MSC_VER)||(_MSC_VER<1400)

#define VISUALIZE_MULTI_INDEX_CONTAINER(Type) /* empty def */

#else

namespace msvc_helpers {
  template<typename T>
  struct multi_index_helper {
  };
  template<typename T>
  struct multi_index_helper_2 {
  };
  template<typename T>
  struct multi_index_helper_3 {
  };
}

#include <boost/version.hpp>

#if ( BOOST_VERSION >= 103500 )

#define __VISUALIZE_MIC_INDEX_DEF(Type)                         \
          boost::multi_index::detail::multi_index_node_type<    \
            Type::value_type,                                   \
            Type::index_specifier_type_list,                    \
            Type::allocator_type>::type

/// Enables MSVC visualizers for multi_index_container Type
#define VISUALIZE_MULTI_INDEX_CONTAINER(Type)                   \
namespace msvc_helpers {                                        \
template<>                                                      \
struct multi_index_helper_2<Type> {                             \
  typedef __VISUALIZE_MIC_INDEX_DEF(Type) index_node_type;      \
  typedef                                                       \
    boost::base_from_member<                                    \
      boost::detail::allocator::rebind_to<                      \
        index_node_type::allocator_type,                        \
        index_node_type                                         \
      >::type>                                                  \
  base_type;                                                    \
  typedef                                                       \
    boost::multi_index::detail::header_holder<                  \
      boost::multi_index::detail::prevent_eti<                  \
        Type::allocator_type,                                   \
         boost::detail::allocator::rebind_to<                   \
          Type::allocator_type,                                 \
          index_node_type                                       \
        >::type                                                 \
      >::type::pointer,                                         \
      Type>                                                     \
   /* boost::multi_index::detail::header_holder<             */ \
   /* index_node_type, Type>                                 */ \
  header_holder_type;                                           \
  typedef                                                       \
    boost::multi_index::detail::multi_index_base_type<          \
      Type::value_type,                                         \
      Type::index_specifier_type_list,                          \
      Type::allocator_type>::type                               \
  index_type;                                                   \
  struct index_type_helper : public index_type {};              \
  /* mimic multi_index_container layout */                      \
  base_type          base;                                      \
  header_holder_type header;                                    \
  index_type_helper  index;                                     \
  unsigned int       node_count;                                \
  /* ensure that the debugger sees this template instance */    \
  static void get() {                                           \
    multi_index_helper_2<Type> *ensure_inst=0;                  \
  }                                                             \
};                                                              \
template<>                                                      \
struct multi_index_helper_3<__VISUALIZE_MIC_INDEX_DEF(Type)> {  \
  /* ensure that the debugger sees this template instance */    \
  static void get() {                                           \
    multi_index_helper_3<                                       \
      __VISUALIZE_MIC_INDEX_DEF(Type)> *ensure_inst=0;          \
  }                                                             \
};                                                              \
template<>                                                      \
struct multi_index_helper<__VISUALIZE_MIC_INDEX_DEF(Type)> {    \
  typedef __VISUALIZE_MIC_INDEX_DEF(Type) index_node_type;      \
  /* mimic index_node_base<Type::value_type> layout */          \
  Type::value_type value;                                       \
  /* ensure that the debugger sees this template instance */    \
  static void get() {                                           \
    multi_index_helper<index_node_type> *ensure_inst=0;         \
  }                                                             \
};                                                              \
}

#else

// Version for 1.34

#define __VISUALIZE_MIC_INDEX_DEF(Type)                         \
          boost::multi_index::detail::multi_index_node_type<    \
            Type::value_type,                                   \
            Type::index_specifier_type_list,                    \
            std::allocator<Type::value_type> >::type

/// Enables MSVC visualizers for multi_index_container Type
#define VISUALIZE_MULTI_INDEX_CONTAINER(Type)                   \
namespace msvc_helpers {                                        \
template<>                                                      \
struct multi_index_helper_2<Type> {                             \
  typedef __VISUALIZE_MIC_INDEX_DEF(Type) index_node_type;      \
  typedef                                                       \
    boost::base_from_member<                                    \
      boost::detail::allocator::rebind_to<                      \
        std::allocator<Type::value_type>,                       \
        index_node_type                                         \
      >::type>                                                  \
  base_type;                                                    \
  typedef                                                       \
    boost::multi_index::detail::header_holder<                  \
      index_node_type, Type>                                    \
  header_holder_type;                                           \
  typedef                                                       \
    boost::multi_index::detail::multi_index_base_type<          \
      Type::value_type,                                         \
      Type::index_specifier_type_list,                          \
      std::allocator<Type::value_type> >::type                  \
  index_type;                                                   \
  /* mimic multi_index_container layout */                      \
  base_type          base;                                      \
  header_holder_type header;                                    \
  index_type         index;                                     \
  unsigned int       node_count;                                \
  /* ensure that the debugger sees this template instance */    \
  static void get() {                                           \
    multi_index_helper_2<Type> *ensure_inst=0;                  \
  }                                                             \
};                                                              \
template<>                                                      \
struct multi_index_helper_3<__VISUALIZE_MIC_INDEX_DEF(Type)> {  \
  /* ensure that the debugger sees this template instance */    \
  static void get() {                                           \
    multi_index_helper_3<                                       \
      __VISUALIZE_MIC_INDEX_DEF(Type)> *ensure_inst=0;          \
  }                                                             \
};                                                              \
template<>                                                      \
struct multi_index_helper<__VISUALIZE_MIC_INDEX_DEF(Type)> {    \
  typedef __VISUALIZE_MIC_INDEX_DEF(Type) index_node_type;      \
  /* mimic index_node_base<Type::value_type> layout */          \
  Type::value_type value;                                       \
  /* ensure that the debugger sees this template instance */    \
  static void get() {                                           \
    multi_index_helper<index_node_type> *ensure_inst=0;         \
  }                                                             \
};                                                              \
}

#endif

#endif

#endif

