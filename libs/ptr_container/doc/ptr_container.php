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
echo literature(); echo hr();
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
        item( faqLink() ) .
        item( literatureLink() );

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
                 P( "The advantages are " . 
                    ulist( li( "Exception-safe and fool proof pointer storage and manipulation." ) .
                           li( "Exception-guarantees are generally much better than with standard containers (very often the strong guarantee)" ) .
                           li( "Notational convinience compared to the use of containers of smart pointers." ) .
                           li( "Iterators are automatically indirected so the comparison operations can be kept
                                on object basis instead of making/adding pointer based variants." ) .
                           li( "No memory-overhead as containers of smart_pointers can have." ) .
                           li( "Usually faster than using containers of smart pointers." ) . 
                           li( "Provides an elegant solution to potential " . code( "vector< vector<T> >" ) . " performance
                                problems; simply use " . code( "ptr_vector< vector<T> >." ) .
                           li( "Offers more explicit control of lifetime issues." ) .
                           li( "Can be used for types that are neither Assignable nor CopyConstructible. " ) ) )         
                           ) . 
                  p( "The disadvantages are: " .
                     ulist( li( "Some standard algorithms like " . code( "unique()" ) .
                                 " need special care when using them with raw pointers" ) ) );
  
                     
   $examples = p( "Below is given a small example that show how the usage compares to a container of smart pointers:" . 
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
     } " ) ); 
   
   return $res . $motivation . $examples ;
}



function reference()
{
    $overview   = beginSection( referenceLink() );
    $overview  .= p( "There are a few design decisions that will affect how the
                     classes are used. Besides these the classes are much like
                     normal standard containers and provides almost the same interface.
                     The new conventions are: " ) .
                  ulist( 
                         li( i( "All default iterators apply an extra layer of indirection. " ) .
                             " This is done to make the containers easier and safer to use. It promotes
                               a kind of pointer-less programming and the user of a class needs not worry about
                               pointers except when allocating them. Iterators that provide access to the naked pointers
                               are also provided since they might be useful in rare cases. Whenever eg. " . code( "begin()" ) .
                               " returns an " . code( "iterator, ptr_begin()" ) . " will return an iterator that allows 
                               one to iterate over the stored pointers." ) .  
                         li( i( "The containers are neither Copy-Constructible nor Assignable." )  .
                             " This is because cloning a lot of pointers can be a very expensive operation; instead functions are
                               provided to transfer ownership. If a 
                              deep-copy is needed anyway, every container has " . code( "clone()" ) . " member function." ) . 
                         li( i( "Stored elements need not be CopyConstructible or Assignable, but 
                                for a subset of the operations they are required to be Clonable." ) . 
                             " This is because most polymophic objects cannot be copied directly, 
                              but they can often be so by a use of a member function. Often it does not even make
                              sense to clone an object in which case a large subset of the operations are still workable." 
                              ) .
                         li( i( "Whenever objects are inserted into a container, they are cloned before insertion. 
                                 Whenever pointers are inserted into a container, ownership is transferred to the container." ) .
                             " All containers take ownership of the stored pointers and therefore a cntainer needs to have its 
                               own copies. " ) .
                         li( i( "Ownership can be tranferred from a container on a per pointer basis." ) .
                             " This can of course also be convenient. Whenever it happens, an " . code( "std::auto_ptr<>" ) .
                               " is used to provide an exception-safe transfer. " ) .              
                         li( i( "Ownership can be transferred from a container to another container on a per iterator range basis." ) . 
                             " This makes it possible to exchange data safely between different pointer containers
                               without cloning the objects again. " ) .
                         li( i( "A container can be cheaply returned from functions either by making a clone or by giving up ownership
                                 of the container." ) . " Two special member functions, " . code( "clone()" ) . " and " . 
                             code( "release()," ) . " both return an " . code( "auto_ptr<Container>" ) . " which can be assigned
                             to another container. This effectively reduces the cost of returning a container to one heap-allocation
                             plus a call to " . code("swap()." ) )
                         
                         );

     $overview   .= p( "The documentation is divided into a common section and an explanation for each container. 
                        The so-called \"pseudo class\" sections
                        shows the interface that some or all of the classes have in common and the indvisual parts shows the interface
                        that is only part of some of the individual classes. Before you proceed, 
                        please make sure you understand the Clonable concept." ) .
                    ulist( item( clonableLink() ) .
                           item( ptrContainerLink() ) .
                           item( ptrSequenceLink() ) .
                           item( dequeLink() ) .
                           item( listLink() ) .
                           item( vectorLink() ) .
                           item( ptrAssociativeContainerLink() ) .
                           item( setLink() ) .
                           item( multisetLink() ) .
                           item( mapLink() ) . 
                           item( multimapLink() ) .
                           item( iteratorLink() ) . 
                           item( mapIteratorLink() ) .
                           item( exceptionLink() )
                           );
     $usage = p( "The recommended usage pattern of the container classes are the 
                same as the for normal standard containers." . code( "ptr_vector") .  ", " .  
              code("ptr_list")  .  " and " .  code("ptr_deque") .  " offer the programmer 
                different complexity tradeoffs and should be used accordingly. " . code( "ptr_vector" ) . 
                " is the type of sequence that should be used by default. " . code("ptr_list") . " should be used when there 
             are frequent insertions and deletions from the middle of the sequence " . i( "and" ) . " if the
                 container is fairly large (eg. more than 100 elements). " . code("ptr_deque" ) ." is 
              the data structure of choice when most insertions and deletions take place at 
            the beginning or at the end of the sequence.  An associative container supports  
            unique keys if it may contain at most one element for each key.  Otherwise, it 
            supports equivalent keys. " .  code( "ptr_set" ) . " and " . code( "ptr_map" ) . " support unique keys. 
                " . code( "ptr_multiset" ) . " and " . code( "ptr_multimap" ) . 
                " support equivalent keys." );

     $overview   .= $usage . vSpace( 1 ) . 
                    hr() . clonableRef() .
                    hr() . ptrContainerRef() .
                    hr() . ptrSequenceRef() . 
                    hr() . dequeRef() .
                    hr() . listRef() .
                    hr() . vectorRef() .
                    hr() . ptrAssociativeContainerRef() .
                    hr() . setRef() . 
                    hr() . multisetRef() .
                    hr() . mapRef() . 
                    hr() . multimapRef() .
                    hr() . iteratorRef() .
                    hr() . mapIteratorRef() .
                    hr() . exceptionRef();
     
     return $overview;
}   



function examples()
{
    $header = beginSection( examplesLink() );
    $exampleList = p( "Some examples are given in the accompanying test
                      files:" ) .
                   ulist( li( a( "../test/incomplete_type_test.cpp", code( "incomplete_type_test.cpp" ) ) .
                          " shows how to implement the Composite pattern." ) .
                          li( a( "../test/associative_test_data.hpp", code( "associative_test_data.hpp" ) ) .
                          " shows the common interface for all associative containers." ) .
                          li( a( "../test/sequence_test_data.hpp", code( "sequence_test_data.hpp" ) ) .
                          " shows the common interface for all sequences." ) );

     
    return $header . $exampleList;
}



function portability()
{
    $header       = beginSection( portLink() );
    $compilerList = p( "This library does not rely on any difficult template code, and so it should work
                        with most compilers---even the older onces." ) .
                    p( "Most of the tests have been run successfully on these compilers:" . 
                       ulist( li( "vc7.1" ) . 
                              li( "gcc3.3.1" ) . 
                              li( "como4.3.3" ) .
                              li( "bcc6" ) ) );

    return $header . $compilerList;
}



function Faq()
{
    $res  = beginSection( faqLink() );
    $faq1 = p( i("Since a pointer container is not Copy-Constructible and Assignable, I cannot 
                 put them into standard containers; what do I do?" ) .
               " Since they are Clonable, you simply put them in a pointer container." );
    $faq2 = p( i( "Calling " . code( "assign()" ) . " is very costly and I do not really need to store
                  cloned objects; I merely need to overwrite the existing ones; what do I do?;" ) .
               " Call " . code( "std::copy( first, last, c.begin() );." ) ); 
    $faq3 = p( i( "Why is there no equivalent of " . code( "boost::array<T,size>" ) . " for storing pointers?" ) .
               " For heap-allocated pointers there would be little performance benefit of having such a class." );
    $faq4 = p( i( "Why does the classes have some mutating algorithms as member functions?" ) . 
               " Some mutating algorithms are inherently unsafe and error-prone to use with pointers. These few
                 often-used algorithms are implemented so the user does not need to care about those pitfalls." ); 
    $faq5 = p( i( "Which mutating algorithms are safe to use with pointers?" ) . 
               " Any mutating algorithm that moves elements around by swapping them. An important example is " . 
               code( "std::sort()" ) . "; examples of unsafe algorithms are " . code( "std::unique()" ) . " and " .
               code( "std::remove(). That is why these algorithms are provided as member functions. " ) );
    $faq6 = p( i( "Why does " . code( "ptr_map<T>::insert()/replace() " ) . " take two arguments (the key and the pointer) 
                   instead of one " . code( "std::pair<>" ) . "? And why is the key passed by non-const reference? " ) .
                " This is the only way the function can be implemented in an exception-safe manner; since 
                  the copy-constructor of the key might throw, and since function arguments are not guaranteed to
                  be evaluated from left to right, we need to ensure that evaluating the first argument does not throw.
                  Passing the key as a reference achieves just that." );
    $faq7 = p( i( "When instantiating a ptr_container with a type " . code( "T," ) . " is " . code( " T " ) . " then
                  allowed to be incomplete at that point? " ) . 
               " Yes." );
    $faq8 = p( i( "Why are inserting member functions overloaded for both pointers and references?" ) .
               " Assuming only pointer arguments were allowed, the inexperienced programmer might forget
                 to call " . code( "make_clone()" ) . " on an object. So the code would not compile. To fix it he
                 just takes the address of the object and now he is happy because the code compiles. So to avoid
                  that from hapening, we add the overloaded version. Notice that containers of smart pointers does
                  not have this problem. " );
    $faq9 = p( i( "Why do iterator-range inserts give the strong exception-safety guarantee? Is this not very inefficient?" ) .
               " It is because it is actually affordable to do so; the overhead is one heap-allocation which 
                 is relatively small compared to cloning N objects. " ); 
               
                  
    return $res . olist( $faq1 . $faq2 . $faq3 . $faq4 . $faq5 . $faq6 . $faq7 . $faq8 );
}



function literature()
{   
    $header       = beginSection( literatureLink() );
    return $header;
}



function examplesLink()
{
    return "Examples";
}



function faqLink()
{
    return "FAQ";
}



function literatureLink()
{
    return "References";
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



function clonableLink()
{
    return "The Clonable concept";
}



function ptrContainerLink()
{
    return "Pseudo class " . code( "ptr_container<>" ); 
}



function ptrSequenceLink()
{
    return "Pseudo class " . code( "ptr_sequence<>" );
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



function ptrAssociativeContainerLink()
{
    return "Pseudo class " . code( "ptr_associative_container<>" );
}



function setLink()
{
    return " Class " . code( "ptr_set<>" );
}



function multisetLink()
{
    return " Class " . code( "ptr_multiset<>" );
}



function mapLink()
{
    return " Class " . code( "ptr_map<>" );
}



function multimapLink()
{
    return " Class " . code( "ptr_multimap<>" );
}



function iteratorLink()
{
    return " Iterator operations";
}



function mapIteratorLink()
{
    return " Map iterator operations";
}



function exceptionLink()
{
    return "Exception classes";
}



function clonableRef()
{
    $res = beginSection( clonableLink() );
    
    $res .= p( "The Clonable concept is introduced to formalize the requirements for copying heap-allocated 
             objects. A type " . code( "T" ) ." might be Clonable even though it is not Assignable or 
             CopyConstructible. Notice that many operations on the containers does not even require the stored
             type to be Clonable." ) . 
            p( "Let " . code( t ) . " be an object of type " . code( "T" ) . ", then T is " . 
             i( "Clonable" ) ." if " . code( "new T( t )" ) . " is a valid expression or if there 
             exist a free-standing function called " . 
             code( "make_clone()" ) . " that given an object of type " . code( "T" ) . " returns a 
             heap-allocated object of type " . code( "T" ) . ", that is, the function must have the following prototype: " ) .
             p( code( "T* make_clone( const T& );" ) ) .
            p( "Notice that normal CopyConstructible classes are automatically Clonable unless " . code("operator new" ) ." is
                hidden." ) . 
            p( "This layer of indirection provided by " . code( "make_clone()" ) . " is necessary to support 
               classes that are not copyable by default. Notice that the implementation relies on argument-dependent lookup
               to find the right version of " . code( "make_clone()." ) . "This means that one does not need to overload
               or specialize the function is the " . code( "boost" ) . " namespace, but it can be placed together with
               the rest of the interface of the class. If you are implementing a class inline in headers, remember 
               to forward declare " . code( "make_clone()." ) );
        
  
    
    return $res;
}



function ptrContainerRef()
{
    $res      = beginSection( ptrContainerLink() );
    $res     .= "This section describe all the common operations for all the pointer containers. If there are differences in
                 complexity or exception-safety, the differences will be stated under the documentation of the particular
                 container.";
     
    $synopsis = beginSynopsis() . 
"<pre>             
namespace boost
{      
    template< typename T, typename Allocator = std::allocator< T* > >
    class ptr_container 
    {
    public: // typedefs
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
        typedef  <b>implementation defined</b>                       ptr_iterator;
        typedef  <b>implementation defined</b>                       ptr_const_iterator;
        typedef  std::reverse_iterator< ptr_iterator >        ptr_reverse_iterator;
        typedef  std::reverse_iterator< ptr_const_iterator >  ptr_const_reverse_iterator;        
    
    public: // construct/copy/destroy
        explicit ptr_container( const allocator_type& = allocator_type() );
        ptr_container( size_type n, const_reference x, const allocator_type& = allocator_type() );
        ptr_container( auto_ptr< ptr_container > r );
        template< typename InputIterator >
        ptr_container( InputIterator first, InputIterator last, const allocator_type& = allocator_type() );
        ~ptr_container();
        void operator=( std::auto_ptr< ptr_container > r )  
        allocator_type get_allocator() const;                                      
    
    public: // iterators
        iterator                begin();
        const_iterator          begin() const;
        iterator                end();
        const_iterator          end() const;
        reverse_iterator        rbegin();
        const_reverse_iterator  rbegin() const;
        reverse_iterator        rend();
        const_reverse_iterator  rend() const;
        
        ptr_iterator                ptr_begin();
        ptr_const_iterator          ptr_begin() const;
        ptr_iterator                ptr_end();
        ptr_const_iterator          ptr_end() const;
        ptr_reverse_iterator        ptr_rbegin();
        ptr_const_reverse_iterator  ptr_rbegin() const;
        ptr_reverse_iterator        ptr_rend();
        ptr_const_reverse_iterator  ptr_rend() const;

    
    public: // capacity
        size_type  size() const;
        size_type  max_size() const;
        bool       empty() const;	
    
    public: // modifiers
        iterator  erase( iterator position );
        iterator  erase( iterator first, iterator last );
        void      swap( ptr_container& r );
        void      clear():
    
    public: // pointer container requirements
    
        void                            replace( iterator, T* x );    
        std::auto_ptr< ptr_container >  clone() const;    
        std::auto_ptr< ptr_container >  release();
        std::auto_ptr< T >              release( iterator position );
    
    }; //  class 'ptr_container'
    
    template < typename T, typename Allocator >
    bool operator==( const ptr_container< T,Allocator >& x,
                 const ptr_container< T,Allocator >& y);
    
    template < typename T, typename Allocator >
    bool operator<( const ptr_container< T,Allocator >& x,
            const ptr_container< T,Allocator >& y);
    
    template < typename T, typename Allocator >
    bool operator!=( const ptr_container< T,Allocator >& x,
                 const ptr_container< T,Allocator >& y);
    
    template < typename T, typename Allocator>
    bool operator>( const ptr_container< T,Allocator >& x,
            const ptr_container< T,Allocator >& y);
    
    template < typename T, typename Allocator>
    bool operator>=( const ptr_container< T,Allocator >& x,
                 const ptr_container< T,Allocator >& y);
    
    template < typename T, typename Allocator>
    bool operator<=( const ptr_container< T,Allocator >& x,
                 const ptr_container< T,Allocator >& y);
    
    template< typename T, typename Allocator  >
    void swap( ptr_container< T,Allocator >& x, 
               ptr_container< T,Allocator >& y );
    
} // namespace 'boost'  
    </pre> ";
    
    $details  = beginDetails( "construct/copy/destroy" ) .        
        code( "explicit ptr_container( const allocator_type& = allocator_type() );" ) .
        blockQuote( 
            effects( "Constructs an empty container" ) .
            postconditions( code( "size() == 0" ) ) .
            ""//exceptionSafety( "If an exception is thrown, the constructor has no effect" )
            ) .
        code( "ptr_container( size_type n, const_reference x, const allocator_type& = allocator_type() )" ) .
        blockQuote( 
            effects( "Constructs a container with " . code( "n" ) . " clones of " . code( "x" ) ) .
            postconditions( code( "size() == n" ) ) .
            ""//exceptionSafety( "If an exception is thrown, the constructor has no effect" )
            ) . 
        code( "explicit ptr_container( std::auto_ptr< ptr_container > r );" ) .
        blockQuote( 
            effects( "Constructs a container by taking ownership of the supplied pointers" )  
            ) . 
        pre( "template< typename InputIterator >
    ptr_container( InputIterator first, InputIterator last, const allocator_type& = allocator_type() ); " ) .
        blockQuote( 
           requirements( code("(first,last]") . " is a valid range"  ) .
           effects( "Constructs a container with a cloned range of " . code( "(first,last]" ) ) .
           postconditions( code( "size() == std::distance( first, last )" ) ) .
           ""//exceptionSafety( "Strong guarantee" )
           ) .
        code( "~ptr_container();" ) .
        blockQuote( 
            effects( "Deletes the stored pointers and then the container itself" ) .
            throws( "Nothing" )
            ) .
        code( "void operator=( std::auto_ptr< ptr_container > r );" ) . 
        blockQuote( 
            effects( "Deletes the stored pointers and then takes ownership of the supplied pointers" ) .
            throws( "Nothing" ) 
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
        code( "reverse_iterator rbegin();" ) .
        blockQuote(
            effects( "Returns a mutable iterator with " . code( "value_type T" ) ) .
            throws( "Nothing" )
            ) .
        code( "const_reverse_iterator rbegin() const;" ) .
        blockQuote(
            effects( "Returns a non-mutable iterator with " . code( "value_type T" ) ) .
            throws( "Nothing" )
            ) .
        code( "reverse_iterator rend();" ) .
        blockQuote(
            effects( "Returns a mutable iterator with " . code( "value_type T" ) ) .
            throws( "Nothing" )
            ) .
        code( "const_reverse_iterator rend() const;" ) .
        blockQuote(
            effects( "Returns a non-mutable iterator with " . code( "value_type T" ) ) .
            throws( "Nothing" )
            )  .
        code( "ptr_iterator ptr_begin();" ) .
        blockQuote(
            effects( "Returns a mutable iterator with " . code( "value_type T" ) ) .
            throws( "Nothing" )
            ) .
        code( "ptr_const_iterator ptr_begin() const;" ) .
        blockQuote(
            effects( "Returns a non-mutable iterator with " . code( "value_type T" ) ) .
            throws( "Nothing" )
            ) .
        code( "ptr_iterator ptr_end();" ) .
        blockQuote(
            effects( "Returns a mutable iterator with " . code( "value_type T" ) ) .
            throws( "Nothing" )
            ) .
        code( "ptr_const_iterator ptr_end() const;" ) .
        blockQuote(
            effects( "Returns a non-mutable iterator with " . code( "value_type T" ) ) .
            throws( "Nothing" )
            ) .
        code( "ptr_reverse_iterator ptr_rbegin();" ) .
        blockQuote(
            effects( "Returns a mutable iterator with " . code( "value_type T" ) ) .
            throws( "Nothing" )
            ) .
        code( "ptr_const_reverse_iterator ptr_rbegin() const;" ) .
        blockQuote(
            effects( "Returns a non-mutable iterator with " . code( "value_type T" ) ) .
            throws( "Nothing" )
            ) .
        code( "ptr_reverse_iterator ptr_rend();" ) .
        blockQuote(
            effects( "Returns a mutable iterator with " . code( "value_type T" ) ) .
            throws( "Nothing" )
            ) .
        code( "ptr_const_reverse_iterator ptr_rend() const;" ) .
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
        code( "bool empty() const;" ) .
        blockQuote( 
            effects( "Returns whether the container is empty or not" ) .
            throws( "Nothing" ) 
            ) .
        ///////////////////////////////////////////////////////////////////////
    beginDetails( "modifiers" ) .
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
        code( "void swap( ptr_container& r );" ) .
        blockQuote( 
            effects( "Swaps the content of the two containers" ) .
            throws( "Nothing" ) 
            ) .
        code( "void clear();" ) .
        blockQuote( 
            effects( "Destroys all object of the container " ) .
            postconditions( code( "empty() == true" ) ) . 
            throws( "Nothing" ) 
            ) .
    /////////////////////////////////////////////////////////////////////////
    beginDetails( "pointer container requirements" ) .
        code( "std::auto_ptr< ptr_container >  clone() const;" ) .
    blockQuote( 
        effects( "Returns a deep copy of the container" ) .
        throws( code("std::bad_alloc" ) . " if there is not enough memory to make a clone of the container" ) . 
        complexity( "Linear" ) 
        ) .
        code( "std::auto_ptr< ptr_container >  release();" ) .
        blockQuote( 
        effects( "Releases ownership of the container. This is a useful way of returning
                 a container from a function." ) .
        postconditions( code( "empty() == true" ) ) .
        exceptionSafety( "Strong guarantee" ) .
        throws( code("std::bad_alloc")  . " if the return value cannot be allocated" ) 
        ) .
        code( "std::auto_ptr< T > release( iterator position );" ) .
        blockQuote( 
            requirements( code( "not empty()" ) ) . 
            effects( "Releases ownership of the pointer referred to by " . code( "position" ) ) .
            postconditions( code("size()") . " is one less " ) . 
            throws( code("bad_ptr_container_operation") . " if the container is empty" ) .
            exceptionSafety( "Strong guarantee" ) 
            )
                  ; 

    return $res . $synopsis . $details;

}



function ptrSequenceRef()
{
    $res = beginSection( ptrSequenceLink() );
    
    $res     .= "This section describe all the common operations for all the pointer sequences, that is, " . 
                code( "ptr_vector, ptr_list " ) . " and " . code( "ptr_deque." );  

$synopsis = beginSynopsis() . 
"<pre>             
namespace boost
{      

    template< typename T, typename Allocator = std::allocator< T* > >
    class ptr_sequence 
    {
    public: // construct/copy/destroy
        assign( InputIterator first, InputIterator last );
        assign( size_type n, const_reference u );
    
    public: // capacity
        void             resize( size_type sz, ptr_type = new value_type() );
    
    public: // element access
        reference        front();
        const_reference  front() const;
        reference        back();
        const_reference  back() const;
    
    public: // modifiers
        void      push_back( T* x );
        void      push_back( const_reference x );
        void      pop_back();
        iterator  insert( iterator position, const_reference x );
        void      insert( iterator position, size_type n, ptr_type x );
        template< typename InputIterator >
        void      insert( iterator position, InputIterator first, InputIterator last ); 
    
    public: // pointer container requirements
        std::auto_ptr< T >  release_back();
    
    }; //  class 'ptr_container'

} // namespace 'boost'  
</pre> ";

$details  = beginDetails( "construct/copy/destroy" ) .        
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
    //////////////////////////////////////////////////////////////////////
    beginDetails( "capacity" ) .
    code( "void resize( size_type n );" ) .
    blockQuote( 
        effects( "XX" ) .
        throws( "XX" ) 
        ) .
    //////////////////////////////////////////////////////////////////////
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
    /////////////////////////////////////////////////////////////////////
beginDetails( "modifiers" ) .
    code( "void push_back( T* x );" ) .
    blockQuote(
        requirements( code("x") . " is heap-allocated and cannot be " . code("0") ) .
        effects( "Inserts the pointer into container and takes ownership of it" ) .
        throws( code("bad_pointer") . " if " . code( "x" ) . " is " . code( "0" ) ) .
        exceptionSafety( "Strong guarantee" ) 
        ) .
    code( "void push_back( const_reference x );" ) .
    blockQuote(
        effects( code( "push_back( make_clone( x ) );" ) ) .
        exceptionSafety( "Strong guarantee" ) 
        ) .
    code( "void pop_back();" ) .
    blockQuote( 
        effects( "Removes the last element in the container if it exists" ) .
        postconditions( code( "not empty()" ) .  " implies " . code("size()") . " is one less" ) .
        exceptionSafety( "Nothrow guaarantee" ) 
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
/////////////////////////////////////////////////////////////////////////
beginDetails( "pointer container requirements" ) .
    code( "std::auto_ptr< T > release_back();" ) .
    blockQuote( 
        requirements( code("not empty()" ) ) .
        effects( "Releases ownership of the last pointer in the container" ) .
        postconditions( code("size()") . " is one less " ) .
        throws( code("bad_ptr_container_operation") . " if the container is empty" ) .
        exceptionSafety( "Strong guarantee" ) 
        ) 
              ;

return $res . $synopsis . $details;


}



function dequeRef()
{
    $res = beginSection( dequeLink() );
    $synopsis = beginSynopsis() . 
"<pre>             
namespace boost
{
    template< typename T, typename Allocator = std::allocator< T* > >
    class ptr_deque 
    {
        //
        // reversible ptr_container requirements + 
        // 
        
    public: // element access
        reference        operator[]( size_type n );
        const_reference  operator[]( size_type n ) const;
        reference        at( size_type n );
        const_reference  at( size_type n ) const;
    
   public: // modifiers
        void                push_front( T* );
        void                push_front( const_reference x );
        void                pop_front();
        std::auto_ptr< T >  release_front(); 
    
    }; //  class 'ptr_deque'
    
} // namespace 'boost'  
    </pre> ";

    $details =  beginDetails( "element access" ) .
        code( "reference operator[]( size_type n );" ) .
        blockQuote( 
            see( "ptr_vector::operator[]" ) 
            ) .
        code( "const_reference operator[]( size_type n ) const;" ) . 
        blockQuote(
            see( "ptr_vector::operator[]" ) 
            ) .
        code( "reference at( size_type n );" ) . 
        blockQuote( 
            see( "ptr_vector::at()" ) 
            ) .
        code( "const_reference at( size_type n );" ) . 
        blockQuote( 
            see( "ptr_vector::at()" ) 
            ) .
        beginDetails( "modifiers" ) .
        code( "void push_front( T* );" ) .
        blockQuote(
            requirements( code("x") . " is heap-allocated and cannot be " . code("0") ) .
            effects( "Inserts the pointer into container and takes ownership of it" ) .
            throws( code("bad_pointer") . " if " . code( "x" ) . " is " . code( "0" ) ) .
            exceptionSafety( "Strong guarantee" ) 
            ) .
        code( "void push_front( const_reference x );" ) .
        blockQuote(
            effects( code( "push_back( make_clone( x ) );" ) ) .
            exceptionSafety( "Strong guarantee" ) 
            ) .
        code( "void pop_front()" ) .
        blockQuote( 
            effects( "Removes the first element of the container if it exists" ) .
            postconditions( code( "not empty()" ) .  " implies " . code("size()") . " is one less" ) .
            exceptionSafety( "Nothrow guarantee" )
            ) .    
        code( "std::auto_ptr< T > release_front();" ) .
        blockQuote( 
            requirements( code( "not empty()" ) ) .
            effects( "Releases ownership of the first pointer in the container" ) .
            postconditions( code("size()") . " is one less " ) .
            throws( code("bad_ptr_container_operation") . " if the container is empty" ) .
            exceptionSafety( "Strong guarantee" ) 
            );
    
    return $res . $synopsis . $details; 
}
                    


function listRef() 
{
    $res = beginSection( listLink() );
    $synopsis = beginSynopsis() . 
"<pre>             
namespace boost
{
template< typename T, typename Allocator = std::allocator< T* > >
class ptr_list 
{
    //
    // reversible ptr_container requirements + 
    // 

public: // modifiers
    void                push_front( T* );
    void                pop_front();
    std::auto_ptr< T >  release_front(); 

public: // list operations
    void  splice( iterator before, ptr_list& x );
    void  splice( iterator before, ptr_list& x, iterator i );
    void  splice( iterator before, ptr_list& x, iterator first, iterator last );
    
    // Q: are they really faster as member functions???    
    void  remove( const_reference value );                           
    template< typename Predicate > 
    void  remove_if( Predicate pred );                               
      
    void  unique();    
    template< typename BinaryPredicate >
    void  unique( BinaryPredicate binary_pred );                     
    
    void  merge( ptr_list& x ); 
    template< typename Compare > 
    void  merge( ptr_list& x, Compare comp );
    
    void  sort();    
    template< typename Compare > 
    void  sort( Compare comp );                             
    
    void  reverse();

}; //  class 'ptr_list'

} // namespace 'boost'  
</pre> ";
    
    $details = beginDetails( "modifiers" ) .
        code( "void push_front( T* );" ) .
        blockQuote(
            see( code( "deque::push_front()" ) )
            ) .
        code( "void pop_front()" ) .
        blockQuote( 
            see( code( "deque::pop_front()" ) )
            ) .    
        code( "std::auto_ptr< T > release_front();" ) .
        blockQuote( 
            see( code( "deque::release_front()" ) )
            );
    
    return $res . $synopsis . $details; 
}



function vectorRef() 
{
    $res = beginSection( vectorLink() );
    $synopsis = beginSynopsis() . 
"<pre>             
namespace boost
{
    template< typename T, typename Allocator = std::allocator< T* > >
    class ptr_vector 
    {
        //
        // reversible ptr_container requirements + 
        // 

    public: // capacity
        size_type  capacity() const;
        void       reserve( size_type n );
    
    public: // element access
        reference        operator[]( size_type n );
        const_reference  operator[]( size_type n ) const;
        reference        at( size_type n );
        const_reference  at( size_type n ) const;
        
    }; //  class 'ptr_vector'
    
} // namespace 'boost'  
    </pre> ";

    $details = beginDetails( "capacity" ) .
        code( "size_type capacity() const;" ) .
        blockQuote( 
            effects( "Returns the size of the allocated buffer" ) .
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
            exceptionSafety( "Strong guarantee" )
            ) .
        beginDetails( "element access" ) .
        code( "reference operator[]( size_type n );" ) .
        blockQuote( 
            requirements( code( "n < size()" ) ) .
            effects( "Returns a reference to the n'th element" ) 
            ) .
        code( "const_reference operator[]( size_type n ) const;" ) . 
        blockQuote( 
            requirements( code( "n < size()" ) ) .
            effects( "Returns a const reference to the n'th element" ) 
            ) .
        code( "reference at( size_type n );" ) . 
        blockQuote( 
            requirements( code( "n < size()" ) ) .
            effects( "Returns a reference to the n'th element" ) .
            throws( code("std::out_of_range") . " if " . code("n >=size()" ) ) 
            ) .
        code( "const_reference at( size_type n );" ) . 
        blockQuote( 
            requirements( code( "n < size()" ) ) .
            effects( "Returns a const reference to the n'th element" ) .
            throws( code("std::out_of_range") . " if " . code("n >= size()" ) ) 
            );
    
    return $res . $synopsis . $details; 
}



function ptrAssociativeContainerRef()
{
    $res = beginSection( ptrAssociativeContainerLink() );
    
    
    return $res;
}



function setRef() 
{
    return beginSection( setLink() );
}



function multisetRef()
{
    return beginSection( multisetLink() );
}



function mapRef()
{
    return beginSection( mapLink() );
}



function multimapRef()
{
    return beginSection( multimapLink() );
}
     


function iteratorRef()
{
    return beginSection( iteratorLink() );
}



function mapIteratorRef()
{
    return beginSection( mapIteratorLink() );
}



function exceptionRef()
{
    return beginSection( exceptionLink() );
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



function complexity( $text )
{
    return p( b( "Complexity: " ) . $text );
}



function see( $link )
{
    return p( "See description in " . $link );
}



?>
