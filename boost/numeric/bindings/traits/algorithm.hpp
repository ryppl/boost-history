#ifndef boost_numeric_bindings_traits_algorithm_hpp
#define boost_numeric_bindings_traits_algorithm_hpp

#include <boost/numeric/bindings/traits/type_traits.hpp>

namespace boost { namespace numeric { namespace bindings { namespace traits {

  ///
  /// To be used instead of operator== for numeric types
  /// Implemented as functor instead of free function because of specialisation
  /// rationale: operator== on builtin types can not be overloaded.

  template < typename T >
  struct is_equal
  {
    is_equal(typename type_traits< T >::real_type tolerance) : tolerance_( tolerance ) {} 

    bool operator()(const T& a, const T& b)
    { return std::abs( a - b ) < tolerance_ ; }

    // bool operator()(const T& a, const T& b, typename value_traits< T >::value_type tolerance) 
    // { return std::abs( a - b ) < tolerance ; }

    typename type_traits< T >::real_type tolerance_ ;
  } ;

  /*
  template < typename T >
  struct is_equal< boost::numeric::ublas::matrix< T > >
  {
    typedef boost::numeric::ublas::matrix< T >     matrix_type ;
    typedef typename matrix_type::value_type       value_type ;
    typedef typename value_traits< value_type >::value_type tolerance_type ;

    bool operator()(const matrix_type& a, const matrix_type& b, const tolerance_type& tolerance)
    { 
      bool ret = true ;
      size_t num_rows = a.size1() ;
      size_t num_cols = a.size2() ;
      
      for(size_t i = 0 ; i < num_rows && ret ; ++i ) 
        for(size_t j = 0 ; j < num_cols && ret ; ++j ) 
          ret = ret && is_equal< value_type >()( a(i,j), b(i,j), tolerance ) ;

      return ret ;
    }
  } ;

  ///
  /// Extension of std::equal_to for which you can also specify a tolerance in the constructor
  template < typename T >
  struct equal_to : std::binary_function<T,T,bool>
  {
    typedef typename value_traits< T >::value_type value_type ;

    equal_to(const value_type & tolerance) : tolerance_( tolerance ) {}

    bool operator()(const T& a, const T& b) const
    { return is_equal< T >()(a,b,tolerance_) ; }

    value_type tolerance_ ;
  } ;
  */

}}}}

#endif // boost_numeric_bindings_traits_algorithm_hpp
