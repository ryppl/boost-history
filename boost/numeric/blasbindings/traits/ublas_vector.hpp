#ifndef boost_numeric_blasbindings_traits_ublas_vector_hpp
#define boost_numeric_blasbindings_traits_ublas_vector_hpp

#include <boost/numeric/blasbindings/storage_traits.hpp>
#include <boost/numeric/blasbindings/traits.hpp>

namespace boost { namespace numeric { namespace blasbindings {

  template < typename T, typename A >
  struct traits< boost::numeric::ublas::vector< T, A > >
  {
    typedef A                                     allocator ;
    typedef typename allocator::pointer           pointer ;
    typedef typename allocator::const_pointer     const_pointer ;

    typedef boost::numeric::ublas::vector< T, A > vector_type ;
    typedef typename vector_type::size_type       size_type ;
    typedef typename vector_type::value_type      value_type ;


    static pointer storage(vector_type& v) 
    { return storage_traits< A >::storage( v.data() ) ; }

    static size_type size(vector_type& v)
    { return v.size() ; }

    static size_type stride(vector_type&) 
    { return 1 ; }
  };

  template < typename T, typename A >
  struct traits< boost::numeric::ublas::vector< T, A > const >
  {
    typedef A const                                     allocator ;
    typedef typename allocator::const_pointer           pointer ;
    typedef typename allocator::const_pointer           const_pointer ;

    typedef boost::numeric::ublas::vector< T, A > const vector_type ;
    typedef typename vector_type::size_type             size_type ;
    typedef typename vector_type::value_type            value_type ;


    static pointer storage(vector_type& v) 
    { return storage_traits< allocator >::storage( v.data() ) ; }

    static size_type size(vector_type& v)
    { return v.size() ; }

    static size_type stride(vector_type&) 
    { return 1 ; }
  };

  template < typename V >
  struct traits< boost::numeric::ublas::vector_range< V > >
  {
    typedef typename V::pointer                      pointer ;
    typedef typename V::const_pointer                const_pointer ;

    typedef boost::numeric::ublas::vector_range< V > vector_type ;
    typedef typename vector_type::size_type          size_type ;
    typedef typename vector_type::value_type         value_type ;


    static pointer storage(vector_type& v) 
    { return traits< V >::storage( v.data() ) + v.start() ; }

    static size_type size(vector_type& v)
    { return v.size() ; }

    static size_type stride(vector_type& v) 
    { return traits< V >::stride( v.data() ) ; }
  };

  template < typename V >
  struct traits< boost::numeric::ublas::vector_range< V > const >
  {
    typedef typename V::const_pointer                      pointer ;
    typedef typename V::const_pointer                      const_pointer ;

    typedef boost::numeric::ublas::vector_range< V > const vector_type ;
    typedef typename vector_type::size_type                size_type ;
    typedef typename vector_type::value_type               value_type ;


    static pointer storage(vector_type& v) 
    { return traits< V const >::storage( v.data() ) + v.start() ; }

    static size_type size(vector_type& v)
    { return v.size() ; }

    static size_type stride(vector_type& v) 
    { return traits< V const >::stride( v.data() ) ; }
  };

  template < typename V >
  struct traits< boost::numeric::ublas::vector_slice< V > >
  {
    typedef typename V::pointer                      pointer ;
    typedef typename V::const_pointer                const_pointer ;

    typedef boost::numeric::ublas::vector_slice< V > vector_type ;
    typedef typename vector_type::size_type          size_type ;
    typedef typename vector_type::value_type         value_type ;

    static pointer storage(vector_type& v) 
    { return traits< V >::storage( v.data() ) + v.start() ; }

    static size_type size(vector_type& v)
    { return v.size() ; }

    static size_type stride(vector_type& v) 
    { return traits< V >::stride( v.data() ) * v.stride() ; }
  };

  template < typename V >
  struct traits< boost::numeric::ublas::vector_slice< V > const >
  {
    typedef typename V::const_pointer                      pointer ;
    typedef typename V::const_pointer                      const_pointer ;

    typedef boost::numeric::ublas::vector_slice< V > const vector_type ;
    typedef typename vector_type::size_type                size_type ;
    typedef typename vector_type::value_type               value_type ;


    static pointer storage(vector_type& v) 
    { return traits< V const >::storage( v.data() ) + v.start() ; }

    static size_type size(vector_type& v)
    { return v.size() ; }

    static size_type stride(vector_type& v) 
    { return traits< V const >::stride( v.data() ) * v.stride() ; }
  };

  template < typename T, typename F, typename A >
  struct traits< boost::numeric::ublas::matrix< T, F, A > >
  {
    typedef typename A::pointer                      pointer ;
    typedef boost::numeric::ublas::matrix< T, F, A > matrix_type ;
    typedef typename matrix_type::size_type          size_type ;
    typedef typename matrix_type::value_type         value_type ;

    static pointer storage(matrix_type& m) 
    { return storage_traits< A >::storage( m.data() ) ; }

    static size_type size1(matrix_type& m)
    { return m.size1() ; }

    static size_type size2(matrix_type& m)
    { return m.size2() ; }

    static size_type stride1(matrix_type& m)
    { return size1( m ) ; }

    static size_type stride2(matrix_type& m)
    { return size2( m ) ; }
  };

  template < typename T, typename F, typename A >
  struct traits< boost::numeric::ublas::matrix< T, F, A > const >
  {
    typedef A const                                        allocator ;
    typedef typename allocator::const_pointer              pointer ;
    typedef typename allocator::const_pointer              const_pointer ;

    typedef boost::numeric::ublas::matrix< T, F, A > const matrix_type ;
    typedef typename matrix_type::size_type                size_type ;
    typedef typename matrix_type::value_type               value_type ;


    static pointer storage(matrix_type& m) 
    { return storage_traits< allocator >::storage( m.data() ) ; }

    static size_type size1(matrix_type& m)
    { return m.size1() ; }

    static size_type size2(matrix_type& m)
    { return m.size2() ; }

    static size_type stride1(matrix_type& m)
    { return size1( m ) ; }

    static size_type stride2(matrix_type& m)
    { return size2( m ) ; }
  };

  template < typename M >
  struct traits< boost::numeric::ublas::matrix_row< M > >
  {
    typedef typename M::pointer                      pointer ;
    typedef typename M::const_pointer                      const_pointer ;

    typedef boost::numeric::ublas::matrix_row< M >   matrix_type ;
    typedef typename matrix_type::size_type          size_type ;
    typedef typename matrix_type::value_type         value_type ;


    static pointer storage(matrix_type& m) 
    { return traits< M >::storage( m.data() ) + m.index() ; }       // supposes column_major !!!

    static size_type size(matrix_type& m)
    { return m.size() ; }

    static size_type stride(matrix_type& m) 
    { return traits< M >::stride1( m.data() ) ; }     // supposes column_major
  };

  template < typename M >
  struct traits< boost::numeric::ublas::matrix_row< M > const >
  {
    typedef typename M::const_pointer                     pointer ;
    typedef typename M::const_pointer                     const_pointer ;

    typedef boost::numeric::ublas::matrix_row< M > const  matrix_type ;
    typedef typename matrix_type::size_type               size_type ;
    typedef typename matrix_type::value_type              value_type ;


    static pointer storage(matrix_type& m) 
    { return traits< M const >::storage( m.data() ) + m.index() ; }       // supposes column_major !!!

    static size_type size(matrix_type& m)
    { return m.size() ; }

    static size_type stride(matrix_type& m) 
    { return traits< M const >::stride1( m.data() ) ; }     // supposes column_major
  };

  template < typename M >
  struct traits< boost::numeric::ublas::matrix_column< M > >
  {
    typedef typename M::pointer                       pointer ;
    typedef typename M::const_pointer                 const_pointer ;

    typedef boost::numeric::ublas::matrix_column< M > matrix_type ;
    typedef typename matrix_type::size_type           size_type ;
    typedef typename matrix_type::value_type          value_type ;


    static pointer storage(matrix_type& m) 
    { return traits< M >::storage( m.data() ) + ( m.index() * traits< M >::stride1( m.data() ) ) ; }       // supposes column_major !!!

    static size_type size(matrix_type& m)
    { return m.size() ; }

    static size_type stride(matrix_type& m) 
    { return traits< M >::stride2( m.data() ) ; }     // supposes column_major
  };

  template < typename M >
  struct traits< boost::numeric::ublas::matrix_column< M > const >
  {
    typedef typename M::const_pointer                       pointer ;
    typedef typename M::const_pointer                       const_pointer ;

    typedef boost::numeric::ublas::matrix_column< M > const matrix_type ;
    typedef typename matrix_type::size_type                 size_type ;
    typedef typename matrix_type::value_type                value_type ;


    static pointer storage(matrix_type& m) 
    { return traits< M const >::storage( m.data() ) + ( m.index() * traits< M >::stride1( m.data() ) ) ; }       // supposes column_major !!!

    static size_type size(matrix_type& m)
    { return m.size() ; }

    static size_type stride(matrix_type& m) 
    { return traits< M const >::stride2( m.data() ) ; }     // supposes column_major
  };

}}}

#endif // boost_numeric_blasbindings_traits_ublas_vector_hpp
