<?

include_once( "common.php" );
include_once( "util/boost.php" );

echo bareHtmlHeader( "Pointer Container Library" );

echo pictureAndName( "Pointer Container Library" ); 
echo overviewLinks(); echo hr();
echo introduction(); echo hr();
echo reference(); echo hr();
echo examples(); echo hr();
echo portability(); echo hr();
echo Faq(); echo hr();
echo copyRightMessage(); 

echo htmlFooter();

//////////////////////////////////////////////////////////////////////////////
// implementation
//////////////////////////////////////////////////////////////////////////////
//
// overviewLinks();
// introduction();
// reference();
// examples();
// portability();
// Faq();
// historyAndAcknowledgement();
//
// examplesLink();
// faqLink();
//
// explanationTable();	   
// ct( $expr ); 
// rt4( $st, $a, $p, $i );
// cIs( $type )
// copyRightMessage();
// 

function overviewLinks()
{
    $list = 
        item( introLink() ) . 
        item( referenceLink() ) . 
        item( examplesLink() ) .
        item( portLink() ) . 
        item( faqLink() );

    return ulist( $list );
}



function introduction()
{
   $res = beginSection( introLink() );
   
   $motivation = p( "This library provides standard-like containers that are suitable
                     for storing pointers to both polymorphic and non-polymorphic objects.
                     For each of the standard containers there is a pointer container 
                     equivalent that takes ownership of the stored pointers in an exception 
                     safe manner. In this respect it is intended to solve 
                     the so-called " .  i( "polymorphic class problem." ) ) .
                 P( "The main advantages are " . 
                    ulist( li( "Exception-safe and fool proof pointer storage and manipulation." ) .
                           li( "Exception-guarantees are generally much better than with standard containers (at least the strong guarantee)" ) .
                           li( "Notational convinience compared to the use of containers of smart pointers." ) .
                           li( "Iterators are automatically indirected so the comparison operations can be kept
                                on object basis instead of making/adding pointer based variants." ) .
                           li( "No memory-overhead as containers of smart_pointers can have." ) .
                           li( "Faster than using containers of smart pointers." ) . 
                           li( "Provides an elegant solution to " . code( "vector< vector<T> >" ) . " performance
                                problems; simply use " . code( "ptr_vector< vector<T> >." ) .
                           li( "Offers more explicit control of lifetime issues." ) ) )     
                            
                           ) .
                 P( "The main disadvantages are " . 
                    ulist( li( "The containers cannot be used with all mutable algorithms; eg. invoking " . 
                               code( "std::unique()" ) . " is undefined " ) 
                           ) 
                    );  
                     
   $examples = p( "Below is given some example that show how the usage compares to a container of smart pointers:" . 
                  pre( "
                       
    using namespace boost;
    using namespace std;
                       
    class Poly
    {
    public:
        virtual ~Poly() {}
        void foo() { doFoo(); }
    private:    
        virtual void doFoo() 
        {
            int i;
            ++i;
        }
    };
                                                             
    //
    // one doesn't need to introduce new names or live with long ones
    //                                                         
    typedef shared_ptr<Poly> PolyPtr;
    
    //
    // one doesn't need to write this anymore
    //                   
    struct PolyPtrOps 
    {
      void operator()( const PolyPtr & a )
        { a->foo(); }
    };
    
    int main()
    {
        enum { size = 2000000 };
        vector<PolyPtr>    svec   
        ptr_vector<Poly>   pvec;
        
        for( int i = 0; i < size; ++i ) 
        {
            svec.push_back( PolyPtr( new Poly ) ); 
            pvec.push_back( new Poly );  // no extra syntax      
        }
                       
        for_each( svec.begin(), svec.end(), PolyPtrOps() );

        for_each( pvec.begin(), pvec.end(), mem_fun_ref( &Poly::foo ) );
     } ") .
     pre( " 
          //
          // Example: Composite implementation
          //
          
          class Composite
          {
             typedef ptr_list< Composite >  composite_t;
             composite_t                    elements_;
          
          public:
             
             void add( Composite* c )
             {
                 elements_.push_back( c );
             }
          
             void add( Composite& c )
             {
                 elements_.push_back( c );
             }
          
             void remove( elements::iterator c )
             {
                 elements_.erase( c );
             }
              
             void foo()
             {
                std::for_each( elements_.begin(), elements_.end(), mem_fun_ref( &Composite::foo ) ); 
             }
          
          private:
             virtual void do_foo() {}
          };
          
          class ConcreteComposite1 : public Composite
          {
              virtual void do_foo() 
              { ... }
          };
          
          class ConcreteComposite2 : public Composite
          {
              virtual void do_foo() 
              { ... }
          };
          
          ...
          
          Composite c;
          c.add( new ConcreteComposite1 );
          c.add( new ConcreteComposite2 );
          c.add( c ); // inserts clone 
          " ) );

   return $res . $motivation . $examples ;
}



function reference()
{
    $overview   = beginSection( referenceLink() );
    $overview  .= p( "There is a few design decisions that will affect how the
                     classes are used. Besides these the classes are much like
                     normal standard containers and provides almost the same interface.
                     The new conventions are: " ) .
                  ulist( li( i( "The containers are neither Copy-Constructible nor Assignable." )  .
                             " This is because cloning a lot of pointers can be a very expensive operation; instead functions are
                               provided to transfer ownership. If a 
                              deep-copy is needed anyway, every container has " . code( "clone()" ) . " member function." ) . 
                         li( i( "Stored elements need not be CopyConstructible or Assignable, but 
                                for a subset of the operations they are required to be Clonable." ) . 
                             " This is because most pylumophic objects cannot be copied directly, 
                              but they can often be so by a use of a member function. Often it does not even make
                              sense to clone an object in which case a large subset of the operations are still workable." 
                              ) .
                         li( i( "Whenever objects are inserted into a container, they are cloned before insertion. 
                                 Whenever pointers are inserted into a container, ownership is transferred to the container." ) .
                             " All containers take ownership of the stored pointers and therefor it need to have its
                               own copies. " ) .
                         li( i( "Ownership can be tranferred from a container on a per pointer basis." ) .
                             " This can of course also be convinient. Whenever it happens, an " . code( "std::auto_ptr<>" ) .
                               " is used to provide an exception-safe transfer. " ) .              
                         li( i( "Ownership can be transferred from a container to another container on a per iterator range basis." ) . 
                             " This makes it possible to exchange data safely between different pointer containers
                               without cloning the objects again. " )
                         );

     $overview   .= P( "The documentation is divided into an explanation for each container:" ) .
                    ulist( item( arrayLink() ) .
                           item( dequeLink() ) .
                           item( listLink() ) .
                           item( vectorLink() ) .
                           item( setLink() ) .
                           item( mapLink() )
                           );
     $overview   .= hr() . arrayRef() . 
                    hr() . dequeRef() .
                    hr() . listRef() .
                    hr() . vectorRef() .
                    hr() . setRef() . 
                    hr() . mapRef();
     
    return $overview . vSpace( 1 ); 
}



function examples()
{
    $header = beginSection( examplesLink() );
    $exampleList = p( "Some good examples are given in the accompanying test
                      files:" ) .
                   ulist( li( a( "../test/iterator.cpp", code( "iterator.cpp" ) ) ) .
                          " shows how to implement a container version of " . 
                          code( "std::copy()" ) . " that works with " . code( "std::ifstream_iterator<>." ) .
                          li( a( "../test/iterator.cpp", code( "string.cpp" ) ) ) .
                          " shows how to implement a container version of " . 
                          code( "std::find()" ) . " that works with " . code( "char[],wchar_t[],char*,wchar_t*." ) );
     
    return $header . $exampleList;
}



function portability()
{
    $header       = beginSection( portLink() );
    $compilerList = p( "Full support for built-in arrays require that the
                        compiler supports class template partial specialization, however,
                        a work-around for built-in types have been made." ) .
                    p( "Notice that some compilers cannot do function template ordering
                        properly. In that case one cannot rely of " . code( "result_iterator" )
                        . " and a single function definition; instead one needs to supply
                        a function overloaded for const and non-const arguments if it is required." ) .
                    p( "Full support for iterators like " . code( "std::istream_iterator<>" ) . " depends very
                        much on a conforming standard library." ) .
                    p( "Most of the tests have been run successfully on these compilers" . 
                       ulist( li( "vc7.1" ) . 
                              li( "gcc3.2" ) . 
                              li( "como4.3.0.1" ) .
                              li( "bcc6" ) ) );

    return $header . $compilerList;
}



function Faq()
{
    $res  = beginSection( faqLink() );
    $faq1 = p( i("Since a pointer container is not Copy-Constructible and Assignable I cannot 
                 put them into standard containers; what do I do?" ) .
               " Since they are Clonable, you simply put them in a pointer container." );
    $faq2 = p( i( "Calling " . code( "assign()" ) . " is very costly and I do not really need to store
                  cloned objects; I merely need to overwrite the existing ones; what do I do?;" ) .
               " Call " . code( "std::copy( first, last, c.begin() );." ) ); 
    $faq3 = "";
    return $res . olist( $faq1 . $faq2 . $faq3 );
}



function examplesLink()
{
    return "Examples";
}



function faqLink()
{
    return "FAQ";
}



function explanationTable()	   
{
 
}



function ct( $C, $expr )
{
    return code( "container_traits<$C>::" . $expr );
}



function rt4( $sc, $a, $p, $i )
{
    return td( code( $sc ) . vSpace() . code( $a ) . vSpace() . code( $p ) . vSpace() . code( $i ) );
}



function cIs( $type )
{
    return " if " . code( "C" ) . " is " . $type . vSpace(); 
}



function arrayLink()
{
    return " Class " . code( "ptr_array<>" );
}



function dequeLink()
{
    return " Class " . code( "ptr_deque<>" );
}



function listLink()
{
    return " Class " . code( "ptr_list<>" );
}



function vectorLink()
{
    return " Class " . code( "ptr_vector<>" );
}



function setLink()
{
    return " Class " . code( "ptr_set<>" );
}



function mapLink()
{
    return " Class " . code( "ptr_map<>" );
}



function arrayRef()
{ 
    return beginSection( arrayLink() );
}



function dequeRef()
{
    return beginSection( dequeLink() );
}
                    


function listRef() 
{
    return beginSection( listLink() );
}



function vectorRef() 
{
    $res      = beginSection( vectorLink() );
    $synopsis = beginSynopsis() . 
"<pre>             
namespace boost
{
    template< typename T, typename Allocator = std::allocator< T* > >
    class ptr_container 
    {
    public: // typedefs
        typedef  std::auto_ptr< T >                           auto_type;
        typedef  const auto_type                              const_auto_type;
        typedef  T                                            value_type;
        typedef  T&                                           reference;
        typedef  const T&                                     const_reference;
        typedef  <b>implementation defined</b>                       iterator;
        typedef  <b>implementation defined</b>                       const_iterator;
        typedef  <b>implementation defined</b>                       difference_type; 
        typedef  <b>implementation defined</b>                       size_type;
	typedef  Allocator                                    allocator_type;
	typedef  typename Allocator::pointer                  pointer;
	typedef  typename Allocator::const_pointer            const_pointer; 
        typedef  std::reverse_iterator< iterator >            reverse_iterator;
        typedef  std::reverse_iterator< const_iterator >      const_reverse_iterator;
        
    public: // construct/copy/destroy
        explicit ptr_vector( const allocator_type& = allocator_type() );
        ptr_vector( size_type n, const_reference x, const allocator_type& = allocator_type() );
        ptr_vector( ptr_vector& r );
        ptr_vector( release_type r );
        template< typename InputIterator >
        ptr_vector( InputIterator first, InputIterator last, const allocator_type& = allocator_type() );
        ~ptr_vector();
        ptr_vector& operator=( ptr_vector& r );	
        template< typename InputIterator >
        assign( InputIterator first, InputIterator last );
        assign( size_type n, const_reference u );
        allocator_type get_allocator() const;                                      

    public: // iterators
        iterator                begin();
        const_iterator          begin() const;
        iterator                end();
        const_iterator          end() const;
        reverse_iterator        rbegin();
        const_reverse_iterator  rbegin();
        reverse_iterator        rend();
        const_reverse_iterator  rend();
	
    public: // capacity
        size_type  size() const;
        size_type  max_size() const;
        void     resize( size_type sz, ptr_type = new value_type() );
        size_type  capacity() const;
    	bool       empty() const;	
        void       reserve( size_type n );
  
    public: // element access
        reference        operator[]( size_type n );
        const_reference  operator[]( size_type n ) const;
        reference        at( size_type n );
        const_reference  at( size_type n ) const;
        reference        front();
        const_reference  front() const;
        reference        back();
        const_reference  back() const;
    
    public: // modifiers
        void      push_back( ptr_type x );
        void      pop_back();
	iterator  insert( iterator position, const_reference x );
	void      insert( iterator position, size_type n, ptr_type x );
        template< typename InputIterator >
        void      insert( iterator position, InputIterator first, InputIterator last ); 
        iterator  erase( iterator position );
        iterator  erase( iterator first, iterator last );
        void      swap( ptr_vector& r );
        void      clear():
	
    public: // pointer container requirements
	
        ptr_vector  clone();
        auto_type   release_back();
        auto_type   release( iterator position );
  
    }; //  class 'ptr_vector'

    template < typename T, typename Allocator >
    bool operator==( const ptr_vector< T,Allocator >& x,
	             const ptr_vector< T,Allocator >& y);
    
    template < typename T, typename Allocator >
    bool operator<( const ptr_vector< T,Allocator >& x,
		    const ptr_vector< T,Allocator >& y);
    
    template < typename T, typename Allocator >
    bool operator!=( const ptr_vector< T,Allocator >& x,
	             const ptr_vector< T,Allocator >& y);
    
    template < typename T, typename Allocator>
    bool operator>( const ptr_vector< T,Allocator >& x,
		    const ptr_vector< T,Allocator >& y);
   
    template < typename T, typename Allocator>
    bool operator>=( const ptr_vector< T,Allocator >& x,
	             const ptr_vector< T,Allocator >& y);
    
    template < typename T, typename Allocator>
    bool operator<=( const ptr_vector< T,Allocator >& x,
	             const ptr_vector< T,Allocator >& y);

    template< typename T, typaname Allocator  >
    void swap( ptr_vector< T,Allocator >& x, 
               ptr_vector< T,Allocator >& y );

} // namespace 'boost'  
</pre> ";
    
    $details  = beginDetails( "construct/copy/destroy" ) .        
        code( "explicit ptr_vector( const allocator_type& = allocator_type() );" ) .
        blockQuote( 
            effects( "Constructs an empty container" ) .
            postconditions( code( "size() == 0" ) ) .
            ""//exceptionSafety( "If an exception is thrown, the constructor has no effect" )
            ) .
        code( "ptr_vector( size_type n, const_reference x, const allocator_type& = allocator_type() )" ) .
        blockQuote( 
            effects( "Constructs a container with " . code( "n" ) . " clones of " . code( "x" ) ) .
            postconditions( code( "size() == n" ) ) .
            ""//exceptionSafety( "If an exception is thrown, the constructor has no effect" )
            ) . 
        pre( "template< typename InputIterator >
ptr_vector( InputIterator first, InputIterator last, const allocator_type& = allocator_type() ); " ) .
        blockQuote( 
           requirements( code("(first,last]") . " is a valid range"  ) .
           effects( "Constructs a container with a cloned range of " . code( "(first,last]" ) ) .
           postconditions( code( "size() == std::distance( first, last )" ) ) .
           ""//exceptionSafety( "Strong guarantee" )
           ) .
        code( "~ptr_vector();" ) .
        blockQuote( 
            effects( "Deletes the containers pointers and then the data itself" ) .
            throws( "Nothing" )
            ) .
        pre( "template< typename InputIterator >
assign( InputIterator first, InputIterator last );" ) .
        blockQuote(
            requirements( code("(first,last]") . " is a valid range"  ) .
            effects( code( "clear(); insert( first, last );" ) ) .
            postconditions( code( "size() == std::distance( first, last )" ) ) .
            exceptionSafety( "strong guarantee" )
            ) .
        code( "assign( size_type n, const_reference u )" ) .
        blockQuote( 
            effects( code( "clear(); insert( begin(), n, u );" ) ) .
            postconditions( code( "size() == n" ) ) .
            exceptionSafety( "Strong guarantee" ) 
            ) .
        code( "allocator_type get_allocator() const" ) .
        blockQuote( 
            effects( "Returns a copy of the allocator of the container object" )
            ) 
    ///////////////////////////////////////////////////////////////////////////
    . beginDetails( "iterators" ) .
        code( "iterator begin();" ) .
        blockQuote(
            effects( "Returns a mutable iterator with " . code( "value_type T" ) ) .
            throws( "Nothing" )
            ) .
        code( "const_iterator begin() const;" ) .
        blockQuote(
            effects( "Returns a non-mutable iterator with " . code( "value_type T" ) ) .
            throws( "Nothing" )
            ) .
        code( "iterator end();" ) .
        blockQuote(
            effects( "Returns a mutable iterator with " . code( "value_type T" ) ) .
            throws( "Nothing" )
            ) .
        code( "const_iterator end() const;" ) .
        blockQuote(
            effects( "Returns a non-mutable iterator with " . code( "value_type T" ) ) .
            throws( "Nothing" )
            ) .
        code( "iterator rbegin();" ) .
        blockQuote(
            effects( "Returns a mutable iterator with " . code( "value_type T" ) ) .
            throws( "Nothing" )
            ) .
        code( "const_iterator rbegin() const;" ) .
        blockQuote(
            effects( "Returns a non-mutable iterator with " . code( "value_type T" ) ) .
            throws( "Nothing" )
            ) .
        code( "iterator rend();" ) .
        blockQuote(
            effects( "Returns a mutable iterator with " . code( "value_type T" ) ) .
            throws( "Nothing" )
            ) .
        code( "const_iterator rend() const;" ) .
        blockQuote(
            effects( "Returns a non-mutable iterator with " . code( "value_type T" ) ) .
            throws( "Nothing" )
            )  .
        //////////////////////////////////////////////////////////////////////
        beginDetails( "capacity" ) .
        code( "size_type size() const;" ) .
        blockQuote( 
            effects( "Returns the number of stored elements" ) .
            throws( "Nothing" ) 
            ) .
        code( "size_type max_size() const;" ) .
        blockQuote( 
            effects( "Returns the maximum number of stored elements" ) .
            throws( "Nothing" ) 
            ) .
        code( "void resize( size_type sz, const_reference x );" ) .
        blockQuote(
            effects( pre( "if ( sz > size() )
    insert( end(), sz-size(), x );
else if ( sz < size() )
    erase( begin()+sz, end() );
else
   ; //do nothing " )  ) .
            postconditions( code( "size() == sz" ) ) .
            exceptionSafety( "strong guarantee" )
            ) .
        code( "bool empty() const;" ) .
        blockQuote( 
            effects( "Returns whether the container is empty or not" ) .
            throws( "Nothing" ) 
            ) .
        beginDetails( "element access" ) .
        code( "reference front();" ) .
        blockQuote( 
            effects( code( "return *begin()" ) ) .
            throws( "Nothing" ) 
            ) .
        code( "const_reference front() const;" ) .
        blockQuote( 
            effects( code( "return *begin()" ) ) .
            throws( "Nothing" ) 
        ) .
        code( "reference back();" ) .
        blockQuote( 
            effects( code( "return *end()" ) ) .
            throws( "Nothing" ) 
            ) .
        code( "const_reference back() const;" ) .
        blockQuote( 
            effects( code( "return *end()" ) ) .
            throws( "Nothing" ) 
            ) .    
    beginDetails( "modifiers" ) .
        code( "void push_back( const_reference x );" ) .
        blockQuote(
            effects( code( "push_back( make_clone( x ) );" ) ) .
            exceptionSafety( "Strong guarantee" ) 
            ) .
        code( "void push_back( ptr_type x );" ) .
        blockQuote(
            requirements( code("x") . " is heap-allocated and cannot be " . code("0") ) .
            effects( "Inserts the pointer into container and takes ownership of it" ) .
            throws( code("bad_pointer") . " if " . code( "x" ) . " is " . code( "0" ) ) .
            exceptionSafety( "Strong guarantee" ) 
            ) .
        code( "void pop_back();" ) .
        blockQuote( 
            effects( "Removes the last element in the container" ) .
            throws( "Nothing" ) 
            ) .
        code( "iterator insert( iterator position, const_reference x );" ) .
        blockQuote( 
            requirements( code("position") . " is a valid iterator from the container" ) .
            effects( code( "return insert( position, make_clone( x ) );" ) ) .
            exceptionSafety( "Strong guarantee" ) 
            ) .
        code( "iterator insert( iterator position, ptr_type x );" ) .
        blockQuote( 
            requirements( code("position") . " is a valid iterator from the container and " . 
                          code( "x" ) ." is heap-allocated and cannot be " .code( "0" ) ) .
            effects( "Inserts " . code("x") . " before " . code( "position" ) .
                     " and returns an iterator pointing to it" ) .
            throws( code("bad_pointer") . " if " . code( "x" ) . " is " . code( "0" ) ) .
            exceptionSafety( "Strong guarantee" ) 
            ) .
        code( "void insert( iterator position, size_type n, const_reference x );" ) .
        blockQuote( 
            requirements( code("position") . " is a valid iterator from the container" ) .
            effects( "Inserts " . code( "n" ). " clones of " . code("x") . " before " . 
                     code( "position" ) ." into the container " ) .
            exceptionSafety( "Strong guarantee" ) 
            ) .
        pre( "template< typename InputIterator >
void insert( iterator position, InputIterator first, InputIterator last );" ) .
        blockQuote( 
            requirements( code("position") . " is a valid iterator from the container" ) .
            effects( "Inserts a cloned range before " . code("position") ) .
            exceptionSafety( "Strong guarantee" ) 
            ) .
        code( "iterator erase( iterator position );" ) .
        blockQuote( 
            requirements( code("position") . " is a valid iterator from the container" ) .
            effects( "Removes the element defined by ". code( "position" ) ." and returns an
                      iterator to the following element" ) .
            throws( "Nothing" ) 
            ) .
        code( "iterator erase( iterator first, iterator last );" ) .
        blockQuote( 
            requirements( code("(first,last]") . " is a valid range" ) .
            effects( "Removes the range of element defined by ". code( "position" ) ." and returns an
                 iterator to the following element" ) .
            throws( "Nothing" ) 
            ) .
        code( "void swap( ptr_vector& r );" ) .
        blockQuote( 
            effects( "Swaps the content of the two containers" ) .
            throws( "Nothing" ) 
            ) .
        code( "void clear();" ) .
        blockQuote( 
            effects( "Destroys all object of the container " ) .
            postconditions( code( "empty() == true" ) ) . 
            throws( "Nothing" ) 
            )
 
                  ;
                
    
    return $res . $synopsis . $details;
}



function setRef() 
{
    return beginSection( setLink() );
}



function mapRef()
{
    return beginSection( mapLink() );
}
     


function beginSynopsis()
{
    return h3( "Synopsis:" );
}



function beginDetails( $text )
{
    return p( h3( "Semantics: " . i( $text ) ) ) ;
}



function requirements( $text )
{
    return p( b( "Requirements: " ) . $text );
}



function effects( $text )
{
    return p( b( "Effects: " ) . $text );
}



function postconditions( $text )
{
    return p( b( "Postconditions: " ) . $text );
}


function throws( $text )
{
    return p( b( "Throws: " ) . $text );
}


function exceptionSafety( $text )
{
    return p( b( "Exception safety: " ) . $text ); 
}







?>
