#ifndef boost_numeric_blasbindings_traits_ublas_matrix_hpp
#define boost_numeric_blasbindings_traits_ublas_matrix_hpp

#include <boost/numeric/blasbindings/storage_traits.hpp>
#include <boost/numeric/blasbindings/traits.hpp>

namespace boost { namespace numeric { namespace blasbindings {

  template < typename M >
  struct traits< boost::numeric::ublas::matrix_range< M > >
  {
    typedef typename M::pointer                             pointer ;
    typedef typename M::const_pointer                       const_pointer ;

    typedef boost::numeric::ublas::matrix_range< M >        matrix_type ;
    typedef typename matrix_type::size_type                 size_type ;
    typedef typename matrix_type::value_type                value_type ;


    static pointer storage(matrix_type& m) 
    { return traits< M >::storage( m.data() ) + traits< M >::stride1( m.data() ) * m.start2() + m.start1() ; }       // supposes column_major !!!

    static size_type size1(matrix_type& m)
    { return m.size1() ; }

    static size_type size2(matrix_type& m)
    { return m.size2() ; }

    static size_type stride1(matrix_type& m) 
    { return traits< M >::stride1( m.data() ) ; }     

    static size_type stride2(matrix_type& m) 
    { return traits< M >::stride2( m.data() ) ; }     
  };

  template < typename M >
  struct traits< boost::numeric::ublas::matrix_range< M > const >
  {
    typedef typename M::const_pointer                       pointer ;
    typedef typename M::const_pointer                       const_pointer ;

    typedef boost::numeric::ublas::matrix_range< M > const  matrix_type ;
    typedef typename matrix_type::size_type                 size_type ;
    typedef typename matrix_type::value_type                value_type ;


    static pointer storage(matrix_type& m) 
    { return traits< M const >::storage( m.data() ) + traits< M const >::stride1( m.data() ) * m.start2() + m.start1() ; }       // supposes column_major !!!

    static size_type size1(matrix_type& m)
    { return m.size1() ; }

    static size_type size2(matrix_type& m)
    { return m.size2() ; }

    static size_type stride1(matrix_type& m) 
    { return traits< M const >::stride1( m.data() ) ; }     

    static size_type stride2(matrix_type& m) 
    { return traits< M const >::stride2( m.data() ) ; }     
  };

  template < typename M >
  struct traits< boost::numeric::ublas::matrix_slice< M > >
  {
    typedef typename M::pointer                             pointer ;
    typedef typename M::const_pointer                       const_pointer ;

    typedef boost::numeric::ublas::matrix_slice< M >        matrix_type ;
    typedef typename matrix_type::size_type                 size_type ;
    typedef typename matrix_type::value_type                value_type ;


    static pointer storage(matrix_type& m) 
    { return traits< M >::storage( m.data() ) + traits< M >::stride1( m.data() ) * m.start2() + m.start1() ; }       // supposes column_major !!!

    static size_type size1(matrix_type& m)
    { return m.size1() ; }

    static size_type size2(matrix_type& m)
    { return m.size2() ; }

    static size_type stride1(matrix_type& m) 
    { return traits< M >::stride1( m.data() ) * m.stride1() ; }     

    static size_type stride2(matrix_type& m) 
    { return traits< M >::stride2( m.data() ) * m.stride2() ; }     
  };

  template < typename M >
  struct traits< boost::numeric::ublas::matrix_slice< M > const >
  {
    typedef typename M::const_pointer                       pointer ;
    typedef typename M::const_pointer                       const_pointer ;

    typedef boost::numeric::ublas::matrix_slice< M > const  matrix_type ;
    typedef typename matrix_type::size_type                 size_type ;
    typedef typename matrix_type::value_type                value_type ;


    static pointer storage(matrix_type& m) 
    { return traits< M const >::storage( m.data() ) + traits< M const >::stride1( m.data() ) * m.start2() + m.start1() ; }       // supposes column_major !!!

    static size_type size1(matrix_type& m)
    { return m.size1() ; }

    static size_type size2(matrix_type& m)
    { return m.size2() ; }

    static size_type stride1(matrix_type& m) 
    { return traits< M const >::stride1( m.data() ) * m.stride1() ; }     

    static size_type stride2(matrix_type& m) 
    { return traits< M const >::stride2( m.data() ) * m.stride2() ; }     
  };

}}}

#endif // boost_numeric_blasbindings_traits_ublas_matrix_hpp
