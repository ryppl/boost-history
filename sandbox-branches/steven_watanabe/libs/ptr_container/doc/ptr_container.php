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
   
   $motivation = p( "This library provides standard-like containers that are for storing heap-allocated
                    objects only (or in case of a map, the mapped type must be a heap-allocated
                    object. The stored pointers can point to both polymorphic and non-polymorphic objects.
                     For each of the standard containers there is a pointer container 
                     equivalent that takes ownership of the objects in an exception 
                     safe manner. In this respect it is intended to solve 
                     the so-called " .  aLocal( polymorphicClassProblemLink() ) . "." ) .
                 P( "The advantages are " . 
                    ulist( li( "Exception-safe and fool proof pointer storage and manipulation." ) .
                           li( "Exception-guarantees are generally much better than with standard containers (very often the strong guarantee)" ) .
                           li( "Notational convenience compared to the use of containers of smart pointers." ) .
                           li( "Iterators are automatically indirected so the comparison operations can be kept
                                on object basis instead of making/adding pointer based variants." ) .
                           li( "No memory-overhead as containers of smart pointers can have (see " . aLocal( smartPointerOverheadLink() ) . "). " ) .
                           li( "Usually faster than using containers of smart pointers (see " . aLocal( smartPointerOverheadLink() ) . ")." ) . 
                           li( "Provides an elegant solution to potential " . code( "vector< vector<T> >" ) . " performance
                                problems; simply use " . code( "ptr_vector< vector<T> >." ) .
                           li( "Offers more explicit control of lifetime issues." ) .
                           li( "Can be used for types that are neither Assignable nor CopyConstructible. " ) ) )         
                           ) ; 
   /* . 
                  p( "The disadvantages are: " .
                     ulist( li( "Some standard algorithms like " . code( "unique()" ) .
                                 " need special care when using them with raw pointers" ) ) );
  */
                     
   $examples = p( "Below is given a small example that show how the usage compares to a container of smart pointers:" . 
                  pre( "                                           
    //
    // A simple polymorphic class
    //
    class Poly
    {
        int        i_;
        static int cnt_;
        
    public:
        Poly() : i_( cnt_++ )  { }
        virtual ~Poly()        { }
        void foo()             { doFoo(); }
        
    private:    
        virtual void doFoo()   { ++i_; }
        
    public:
        friend inline bool operator>( const Poly& l, const Poly r )
        {
            return l.i_ > r.i_;
        }
    };
    
    int Poly::cnt_ = 0;
  
    //
    // Normally we need something like this to compare pointers to objects
    //
    template< typename T >
    struct sptr_greater
    {
        bool operator()( const boost::shared_ptr<T>& l, const boost::shared_ptr<T>& r ) const
        {
            return *l > *r;
        }
    };
                                                             
    //
    // one doesn't need to introduce new names or live with long ones
    //                                                         
    typedef boost::shared_ptr<Poly> PolyPtr;
    

    int main()
    {
        enum { size = 2000000 };
        typedef vector<PolyPtr>          vector_t;
        typedef boost::ptr_vector<Poly>  ptr_vector_t;
        vector_t                         svec;   
        ptr_vector_t                     pvec;
        
        for( int i = 0; i < size; ++i ) 
        {
            svec.push_back( PolyPtr( new Poly ) ); 
            pvec.push_back( new Poly );  // no extra syntax      
        }
                       
        for( int i = 0; i < size; ++i )
        {
            svec[i]->foo();
            pvec[i].foo(); // automatic indirection
            svec[i] = PolyPtr( new Poly );
            pvec.replace( i, new Poly ); // direct pointer assignment not possible, original element is deleted
        }
        
        for( vector_t::iterator i = svec.begin(); i != svec.end(); ++i )
            (*i)->foo();
     
        for( ptr_vector_t::iterator i = pvec.begin(); i != pvec.end(); ++i )
            i->foo(); // automatic indirection
        
        sort( svec.begin(), svec.end(), sptr_greater<Poly>() );
        pvec.sort( std::greater<Poly>() ); // automatic indirection on predicate
        
        sort( svec.begin(), svec.end(), *_1 < *_2 ); // using lambda expressions
        pvec.sort();                                 // ascending is default sort order
        pvec.sort( std::less<Poly>() );              // same as default, no indirection on predicate
         
        // svec's pointers deleted here
        // pvec's pointers deleted here                              
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
                  olist( 
                         li( i( "Null pointers are not allowed." ) . " If the user tries to insert the null pointer, the
                                 operation will throw a " . code( "bad_pointer" ) . " exception (see " . aLocal( exampleLink(1) ) . ")." 
                             ) . 
                         li( i( "All default iterators apply an extra layer of indirection. " ) .
                             " This is done to make the containers easier and safer to use. It promotes
                               a kind of pointer-less programming and the user of a class needs not worry about
                               pointers except when allocating them (see " . aLocal( exampleLink(2) ) . ").
                               Iterators that provide access to the naked pointers
                               are also provided since they might be useful in rare cases. Whenever eg. " . code( "begin()" ) .
                               " returns an " . code( "iterator, ptr_begin()" ) . " will return an iterator that allows 
                               one to iterate over the stored pointers." ) .  
                         li( i( "The containers are neither Copy-Constructible nor Assignable." )  .
                             " This is because cloning a lot of pointers can be a very expensive operation; instead functions are
                               provided to transfer ownership. If a 
                              deep-copy is needed anyway, every container has " . code( "clone()" ) . 
                             " member function (see " . aLocal( exampleLink(3) ) . ")." ) . 
                         li( i( "Stored elements need not be CopyConstructible or Assignable, but 
                                for a subset of the operations they are required to be Clonable (see " . aLocal( clonableLink() ) . ")." ) . 
                             " This is because most polymorphic objects cannot be copied directly, 
                              but they can often be so by a use of a member function (see " . aLocal( exampleLink(4) ) . ").
                              Often it does not even make
                              sense to clone an object in which case a large subset of the operations are still workable." 
                              ) .
                         li( i( "Whenever objects are inserted into a container, they are cloned before insertion. 
                                 Whenever pointers are inserted into a container, ownership is transferred to the container." ) .
                             " All containers take ownership of the stored pointers and therefore a container needs to have its 
                               own copies (see " . aLocal( exampleLink(5) ) . ")." ) .
                         li( i( "Ownership can be transferred from a container on a per pointer basis." ) .
                             " This can of course also be convenient. Whenever it happens, an " . code( "std::auto_ptr<>" ) .
                               " is used to provide an exception-safe transfer (see " . aLocal( exampleLink(6) ) . "). " ) .              
                         li( i( "Ownership can be transferred from a container to another container on a per iterator range basis." ) . 
                             " This makes it possible to exchange data safely between different pointer containers
                               without cloning the objects again (see " . aLocal( exampleLink(7) ) . "). " ) .
                         li( i( "A container can be cheaply returned from functions either by making a clone or by giving up ownership
                                 of the container." ) . " Two special member functions, " . code( "clone()" ) . " and " . 
                             code( "release()," ) . " both return an " . code( "auto_ptr<Container>" ) . " which can be assigned
                             to another container. This effectively reduces the cost of returning a container to one heap-allocation
                             plus a call to " . code("swap()" ) . " (see " . aLocal( exampleLink(3) ) .")." ) .
                         li( i( " Certain algorithms have been implemented as member functions, because they can overwrite 
                                 pointers instead of swapping them. " ) . " This is done to avoid some nasty pitfalls with " .
                                 code( "remove()" ) . " and " . code( "unique()" ) . 
                                "(see " . aLocal( exampleLink(8) ) .", ". aLocal( refLink(1) ) 
                                  . " and " . aLocal( refLink(10) ) . "). " . " Some of the functions
                                  take predicates and the user need not apply the indirection himself. This means that
                                  functors that work on objects can be passed to these algorithms which removes the programmer
                                  from creating a functor that works on pointers."  )
                         
                         );

     $overview   .= p( "The documentation is divided into a common section and an explanation for each container. 
                        The so-called \"common interface\" sections
                        show the interface that some or all of the classes have in common and the individual parts shows the interface
                        that is only part of some of the individual classes. Before you proceed, 
                        please make sure you understand the Clonable concept and have read the usage guidelines." ) .
                    ulist( item( clonableLink() ) .
                           item( usageLink() ) .
                           item( ptrContainerLink(), " The interface that all the containers share." ) .
                           item( ptrSequenceLink(), " The interface that all sequences share." ) .
                           item( dequeLink() ) .
                           item( listLink() ) .
                           item( vectorLink() ) .
                           item( ptrAssociativeContainerLink(), " The interface that associative containers share." )  .
                           item( setLink() ) .
                           item( multisetLink() ) .
                           item( mapLink() ) . 
                           item( multimapLink() ) .
                           item( mapIteratorLink() ) .
                           item( exceptionLink() )
                           );

     $overview   .= vSpace( 1 ) . 
                    hr() . clonableRef() .
                    hr() . usageRef() .
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
                    hr() . mapIteratorRef() .
                    hr() . exceptionRef();
     
     return $overview;
}   



function examples()
{
    $header = beginSection( examplesLink() );
    $exampleList = p( "Some examples are given here and in the accompanying test
                      files:" ) .
                   olist( 
                       
                       li( aTarget( exampleLink(1) ) . example( "null pointers cannot be stored in the containers", 
"my_container.push_back( 0 );            // throws bad_ptr 
my_container.replace( an_iterator, 0 ); // throws bad_ptr
my_container.insert( an_iterator, 0 );  // throws bad_ptr                                                                 " ) ) . 
                       li( aTarget( exampleLink(2) )  ) . example( "iterators and other operations return indirected values", 
                                                                   "ptr_vector<X> pvec; std::vector<X*> vec;
*vec.begin()  = new X;   // fine, memory leak
*pvec.begin() = new X;   // compile time error
( *vec.begin() )->foo(); // call X::foo(), a bit clumsy
pvec.begin()->foo();     // no indirection needed
*vec.front()  = X();     // overwrite first element
pvec.front()  = X();     // no indirection needed" ) .
                                                                                                                                                                                                                                                                               
                       li( aTarget( exampleLink(3) )  ) . example( "copy-semantics of pointer containers", 
                                                                   "ptr_vector<T> vec1; 
...
ptr_vector<T> vec2( vec1.clone() ); // deep copy objects of 'vec1' and use them to construct 'vec2', could be very expensive
vec2 = vec1.release();              // give up ownership of pointers in 'vec1' and pass the ownership to 'vec2', rather cheap
vec2.release();                     // give up ownership; the objects will be deallocated if not assigned to another container
vec1 = vec2;                        // compile time error: 'operator=()' not defined 
ptr_vector<T> vec3( vec1 );         // compile time error: copy-constructor not defined "
                                                                    ) . 
                       li( aTarget( exampleLink(4) )  ) . example( "making a non-copyable type Clonable",
                                                                   " // a class that has no normal copy semantics
class X : boost::noncopyable { public: X* clone() const; ... };
                                                                   
// this will be found by the library by argument dependent lookup                                                                   
X* make_clone( const X& x ) 
{ return x.clone(); }
                                                                   
// we can now use the interface that requires clonability
ptr_vector<X> vec1, vec2;
...
vec2 = vec1.clone();                                 // 'clone()' requires cloning <g> 
vec2.insert( vec2.end(), vec1.begin(), vec1.end() ); // inserting always means inserting clones " ) .
                        
                       li( aTarget( exampleLink(5) )  ) . 
                       example( "objects are cloned before insertion, inserted pointers are owned by the container",
                                "class X { ... }; // assume 'X' is Clonable 
X x; // and 'X' can be stack-allocated 
ptr_list<X> list; 
list.push_back( x );             // clone 'x' and then insert the resulting pointer 
list.push_back( make_clone( x ); // do it manually
list.push_back( new X );         // always give the pointer directly to the container to avoid leaks
list.push_back( &x );            // don't do this! " ) .                        
                       li( aTarget( exampleLink(6) )  ) . 
                       example( "transferring ownership of a single element", "ptr_deque<T> deq; 
// ... fill the container somehow
std::auto_ptr<T> ptr  = deq.release_back();             // remove back element from container and give up ownership
std::auto_ptr<T> ptr2 = deq.release( deq.begin() + 2 ); // use an iterator to determine the element to release
ptr                   = deq.release_front();            // supported for 'ptr_list' and 'ptr_deque'
                                " ) .  
                       li( aTarget( exampleLink(7) )  ) . 
                       example( "transferring ownership of pointers between different pointer containers", 
                                "ptr_list<X> list; ptr_vector<X> vec;
...
// note: no cloning happens in these examples                                
list.transfer( list.begin(), vec.begin(), vec );           // make the first element of 'vec' the first element of 'list'
vec.transfer( vec.end(), list.begin(), list.end(), list ); // put all the lists element into the vector                                 
                                " ) .     
                       li( aTarget( exampleLink(8) )  ) .
                       example( "certain member functions are provided because they can be tricky to implement",
                                "ptr_vector<T> vec; T to_remove = ... ;
std::remove( vec.begin(), vec.end(), to_remove ); // ok, but only makes sense if T is copyable, can be very slow
std::remove_if( vec.ptr_begin(), vec.ptr_end(), my_indirected_predicate() ); // predicate compares objects, not pointers
                                                                             // but pointers will be overwritten = leak + disaster!
vec.remove( to_remove ); // fast + pointers are correctly de-allocated"                                                                
                                ) . 
                       li( aTarget( exampleLink(9) ) . a( "../test/incomplete_type_test.cpp", code( "incomplete_type_test.cpp" ) ) .
                           " shows how to implement the Composite pattern." ) .
                       li( a( "../test/associative_test_data.hpp", code( "associative_test_data.hpp" ) ) .
                           " shows the common interface for all associative containers." ) .
                       li( a( "../test/sequence_test_data.hpp", code( "sequence_test_data.hpp" ) ) .
                           " shows the common interface for all sequences." )
                          );

     
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
                              li( "como4.3.3" ) ) );

    return $header . $compilerList;
}



function Faq()
{
    $res  = beginSection( faqLink() );
    $faq1 = li( i("Since a pointer container is not Copy-Constructible and Assignable, I cannot 
                 put them into standard containers; what do I do?" ) .
               " Since they are Clonable, you simply put them in a pointer container." );
    $faq2 = li( i( "Calling " . code( "assign()" ) . " is very costly and I do not really need to store
                  cloned objects; I merely need to overwrite the existing ones; what do I do?;" ) .
               " Call " . code( "std::copy( first, last, c.begin() );." ) ); 
    $faq3 = li( i( "Why is there no equivalent of " . code( "boost::array<T,size>" ) . " for storing pointers?" ) .
               " For heap-allocated pointers there would be little performance benefit of having such a class." );
    $faq4 = li( i( "Why does the classes have some mutating algorithms as member functions?" ) . 
               " Some mutating algorithms are inherently unsafe and error-prone to use with pointers. These few
                 often-used algorithms are implemented so the user does not need to care about those pitfalls." ); 
    $faq5 = li( i( "Which mutating algorithms are safe to use with pointers?" ) . 
               " Any mutating algorithm that moves elements around by swapping them. An important example is " . 
               code( "std::sort()" ) . "; examples of unsafe algorithms are " . code( "std::unique()" ) . " and " .
               code( "std::remove(). That is why these algorithms are provided as member functions. " ) );
    $faq6 = li( i( "Why does " . code( "ptr_map<T>::insert()/replace() " ) . " take two arguments (the key and the pointer) 
                   instead of one " . code( "std::pair<>" ) . "? And why is the key passed by non-const reference? " ) .
                " This is the only way the function can be implemented in an exception-safe manner; since 
                  the copy-constructor of the key might throw, and since function arguments are not guaranteed to
                  be evaluated from left to right, we need to ensure that evaluating the first argument does not throw.
                  Passing the key as a reference achieves just that." );
    $faq7 = li( i( "When instantiating a ptr_container with a type " . code( "T," ) . " is " . code( " T " ) . " then
                  allowed to be incomplete at that point? " ) . 
               " Yes." );
    $faq8 = li( i( "Why are inserting member functions overloaded for both pointers and references?" ) .
               " Assuming only pointer arguments were allowed, the inexperienced programmer might forget
                 to call " . code( "make_clone()" ) . " on an object. So the code would not compile. To fix it he
                 just takes the address of the object and now he is happy because the code compiles. So to avoid
                  that from happening, we add the overloaded version. Notice that containers of smart pointers does
                  not have this problem. " );
    $faq9 = li( i( "Why do iterator-range inserts give the strong exception-safety guarantee? Is this not very inefficient?" ) .
               " It is because it is actually affordable to do so; the overhead is one heap-allocation which 
                 is relatively small compared to cloning N objects. " ); 
    $faq10 = li( aTarget( polymorphicClassProblemLink() ) . i( "What is the " . polymorphicClassProblemLink() . "?" ) . 
             " The problem refers to the relatively trouplesome way C++
               supports Object Oriented programming in connection with containers of
               pointers to polymorphic objects. In a language without garbage collection,
               you end up using either a container of smart pointers or a container that takes
               ownership of the pointers. The hard part is to find an safe, fast and
               elegant solution. " );         
    $faq11 = li( aTarget( smartPointerOverheadLink() ) .
                 i( "Are the pointer containers faster and do they have a better memory footprint than
                    a container of smart pointers? " ) .
                " The short answer is yes: they are faster and they do use less memory; in fact, they are the only way to obtain
                  the zero-overhead hallmark of C++. Smart pointers usually
                  have one word or more of memory overhead per pointer because a reference count must be maintained. And since the
                  reference count must be maintained, there is also a runtime-overhead. If your objects are big, then
                  the memory overhead is often neglible, but if you have many small objects, it is not.
                  Further reading can be found 
                  in these references: " . aLocal( refLink(12) ) . " and " . aLocal( refLink(13) ) );  

                
                  
    return $res . olist( $faq1 . $faq2 . $faq3 . $faq4 . $faq5 . $faq6 . $faq7 . $faq8 . $faq9 . $faq10 . $faq11 );
}



function literature()
{   
    $header       = beginSection( literatureLink() );
    $list = olist( 
        li( aTarget( refLink(1) ) . 'Matt Austern: ' . a( "http://www.cuj.com/documents/s=7990/cujcexp1910austern/", 
                                                          '"The Standard Librarian: Containers of Pointers"' ) 
                                  . ', C/C++ Users Journal Experts Forum.' ) .
        li( aTarget( refLink(2) ) . 'Bjarne Stroustrup, "The C++ Programming Language", ' . a( "http://www.research.att.com/~bs/3rd_safe.pdf",
                                                                                               'Appendix E: "Standard-Library Exception Safety"' ) ) .
        li( aTarget( refLink(3) ) . 'Herb Sutter, "Exceptional C++".' ) .
        li( aTarget( refLink(4) ) . 'Herb Sutter, "More Exceptional C++".' ) .
        li( aTarget( refLink(5) ) . 'Kevlin Henney: ' . a( "http://www.cuj.com/documents/s=7986/cujcexp2002henney/henney.htm", 
                                                           '"From Mechanism to Method: The Safe Stacking of Cats"' )
                                  . ', C++ Experts Forum, February 2002.' ) .
                                 
        li( aTarget( refLink(6) ) . 'Some of the few earlier attempts of smart containers I have seen are the rather interesting ' 
             . a( "http://www.ntllib.org/", "NTL" ) . ' and
             the "pointainer" from ' . a( "http://ootips.org/yonat/4dev/pointainer.h", "http://ootips.org/yonat/4dev/pointainer.h" ) 
            . '. As of this writing both libraries
             are not exceptions-safe and can leak.' ) .
        li( aTarget( refLink(7) ) . 'INTERNATIONAL STANDARD, Programming languages --- C++, ISO/IEC 14882, 1998. See section 23 in particular.' ) .
        li( aTarget( refLink(8) ) . 'C++ Standard Library Closed Issues List (Revision 27), Item 218,' . 
            a( "http://anubis.dkuug.dk/jtc1/sc22/wg21/docs/lwg-closed.html#218",
                "Algorithms do not use binary predicate objects for default comparisons." ) ) .
        li( aTarget( refLink(9) ) . 'C++ Standard Library Active Issues List (Revision 27), Item 226,' . 
            a( "http://gcc.gnu.org/onlinedocs/libstdc++/ext/lwg-active.html#226", 
               "User supplied specializations or overloads of namespace std function templates," ) ) . 
         
        li( aTarget( refLink(10) ) . 'Harald Nowak, "A remove_if for vector<T*>", C/C++ Users Journal, July 2001.' ) .
        li( aTarget( refLink(11) ) . a( "http://www.boost.org/libs/smart_ptr/smarttests.htm", "Boost smart pointer timings" ) ) .
        li( aTarget( refLink(12) ) . a( "http://www.ntllib.org/asp.html", "NTL: Array vs std::vector and boost::shared_ptr" ) )  
                     );
                

    return $header . $list;
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



function usageLink()
{
    return "Usage guidelines";
}



function ptrContainerLink()
{
    return "Common " . code( "ptr_container<>" ) . " interface"; 
}



function ptrSequenceLink()
{
    return "Common " . code( "ptr_sequence<>" ) . " interface";
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
    return "Common " . code( "ptr_associative_container<>" ) . " interface";
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
               to forward declare " . code( "make_clone()" ) . " (see the beginning of " . aLocal( exampleLink(9) ) . ")." );
        
  
    
    return $res;
}



function usageRef()
{
    $res = beginSection( usageLink() );
    $usage = p( "The recommended usage pattern of the container classes are the 
           same as the for normal standard containers." ) .
            p( code( "ptr_vector") .  ", " .  
         code("ptr_list")  .  " and " .  code("ptr_deque") .  " offer the programmer 
           different complexity tradeoffs and should be used accordingly. " . code( "ptr_vector" ) . 
           " is the type of sequence that should be used by default. " . code("ptr_list") . " should be used when there 
        are frequent insertions and deletions from the middle of the sequence " . i( "and" ) . " if the
            container is fairly large (eg. more than 100 elements). " . code("ptr_deque" ) ." is 
         the data structure of choice when most insertions and deletions take place at 
       the beginning or at the end of the sequence. " ) . 
        p( "An associative container supports  
       unique keys if it may contain at most one element for each key.  Otherwise, it 
       supports equivalent keys. " .  code( "ptr_set" ) . " and " . code( "ptr_map" ) . " support unique keys. 
           " . code( "ptr_multiset" ) . " and " . code( "ptr_multimap" ) . 
           " support equivalent keys." );
    return $res . $usage;
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
    template< typename T >
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
        typedef  <b>implementation defined</b>                       allocator_type;
        typedef  <b>implementation defined</b>                       pointer;
        typedef  <b>implementation defined</b>                       const_pointer; 
        typedef  <b>implementation defined</b>                       reverse_iterator;
        typedef  <b>implementation defined</b>                       const_reverse_iterator;
        typedef  <b>implementation defined</b>                       ptr_iterator;
        typedef  <b>implementation defined</b>                       ptr_const_iterator;
        typedef  <b>implementation defined</b>                       ptr_reverse_iterator;
        typedef  <b>implementation defined</b>                       ptr_const_reverse_iterator;        
    
    public: // construct/copy/destroy
        explicit ptr_container( const allocator_type& = allocator_type() );
        ptr_container( size_type n, const T& x, const allocator_type& = allocator_type() );
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
        void      swap( ptr_container& r );
        void      clear():
    
    public: // pointer container requirements
    
        void                            replace( iterator position, T* x );    
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
        code( "ptr_container( size_type n, const T& x, const allocator_type& = allocator_type() )" ) .
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
            effects( "Returns a mutable iterator with " . code( "value_type T*" ) ) .
            throws( "Nothing" )
            ) .
        code( "ptr_const_iterator ptr_begin() const;" ) .
        blockQuote(
            effects( "Returns a non-mutable iterator with " . code( "value_type T*" ) ) .
            throws( "Nothing" )
            ) .
        code( "ptr_iterator ptr_end();" ) .
        blockQuote(
            effects( "Returns a mutable iterator with " . code( "value_type T*" ) ) .
            throws( "Nothing" )
            ) .
        code( "ptr_const_iterator ptr_end() const;" ) .
        blockQuote(
            effects( "Returns a non-mutable iterator with " . code( "value_type T*" ) ) .
            throws( "Nothing" )
            ) .
        code( "ptr_reverse_iterator ptr_rbegin();" ) .
        blockQuote(
            effects( "Returns a mutable iterator with " . code( "value_type T*" ) ) .
            throws( "Nothing" )
            ) .
        code( "ptr_const_reverse_iterator ptr_rbegin() const;" ) .
        blockQuote(
            effects( "Returns a non-mutable iterator with " . code( "value_type T*" ) ) .
            throws( "Nothing" )
            ) .
        code( "ptr_reverse_iterator ptr_rend();" ) .
        blockQuote(
            effects( "Returns a mutable iterator with " . code( "value_type T*" ) ) .
            throws( "Nothing" )
            ) .
        code( "ptr_const_reverse_iterator ptr_rend() const;" ) .
        blockQuote(
            effects( "Returns a non-mutable iterator with " . code( "value_type T*" ) ) .
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
        code( "void replace( iterator position, T* x );" ) .
    blockQuote(
        requirements( code( "not empty()" ) . " and " . code( "x != 0" ) ) .
        effects( "Deletes the object pointed to be " . code( "position" ) . " and replaces it with " .
                 code( "x." ) ) .
        throws( code("bad_ptr_container_operation") . " if the container is empty and " 
                . code( "bad_pointer" ) . " if " . code( "x != 0." ) ) 
        ) .               
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
        template< typename InputIterator >
        assign( InputIterator first, InputIterator last );
        assign( size_type n, const T& u );
    
    public: // capacity
        void             resize( size_type sz, const T& );
    
    public: // element access
        T&        front();
        const T&  front() const;
        T&        back();
        const T&  back() const;
    
    public: // modifiers
        void      push_back( T* x );
        void      push_back( const T& x );
        void      pop_back();
        iterator  insert( iterator position, T* x );
        iterator  insert( iterator position, const T& x );
        void      insert( iterator position, size_type n, const T& x );
        template< typename InputIterator >
        void      insert( iterator position, InputIterator first, InputIterator last ); 
        iterator  erase( iterator position );
        iterator  erase( iterator first, iterator last );
    
    public: // pointer container requirements
        std::auto_ptr< T >  release_back();
        template< typename PtrContainer >
        void  transfer( iterator before, typename PtrContainer::iterator object, 
                        PtrContainer& from );
        template< typename PtrContainer > 
        void  transfer( iterator before, typename PtrContainer::iterator first, 
                        typename PtrContainer::iterator last, PtrContainer& from );
        template< typename PtrContainer >
        void  transfer( iterator before, PtrContainer& from );
    
    public: // pointer container algorithms
        void  sort();
        template< typename BinaryPredicate >
        void  sort( BinaryPredicate pred );
        void  unique();
        template< typename BinaryPredicate >
        void  unique( BinaryPredicate pred );
        void  remove( const T& x );
        template< typename Predicate >
        void  remove_if( Predicate pred );
     
    }; //  class 'ptr_sequence'

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
    code( "assign( size_type n, const T& u )" ) .
    blockQuote( 
        effects( code( "clear(); insert( begin(), n, u );" ) ) .
        postconditions( code( "size() == n" ) ) .
        exceptionSafety( "Strong guarantee" ) 
        ) .
    //////////////////////////////////////////////////////////////////////
    beginDetails( "capacity" ) .
    code( "void resize( size_type sz, const T& x );" ) .
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
    //////////////////////////////////////////////////////////////////////
    beginDetails( "element access" ) .
    code( "T& front();" ) .
    blockQuote( 
        requirements( code( "not empty()" ) ) .
        effects( code( "return *begin()" ) ) .
        throws( "Nothing" ) 
        ) .
    code( "const T& front() const;" ) .
    blockQuote( 
        requirements( code( "not empty()" ) ) .
        effects( code( "return *begin()" ) ) .
        throws( "Nothing" ) 
    ) .
    code( "T& back();" ) .
    blockQuote( 
        requirements( code( "not empty()" ) ) .
        effects( code( "return *--end()" ) ) .
        throws( "Nothing" ) 
        ) .
    code( "const T& back() const;" ) .
    blockQuote( 
        requirements( code( "not empty()" ) ) .
        effects( code( "return *--end()" ) ) .
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
    code( "void push_back( const T& x );" ) .
    blockQuote(
        effects( code( "push_back( make_clone( x ) );" ) ) .
        exceptionSafety( "Strong guarantee" ) 
        ) .
    code( "void pop_back();" ) .
    blockQuote( 
        effects( "Removes the last element in the container if it exists" ) .
        postconditions( code( "not empty()" ) .  " implies " . code("size()") . " is one less" ) .
        exceptionSafety( "Nothrow guarantee" ) 
        ) .
    code( "iterator insert( iterator position, T* x );" ) .
    blockQuote( 
        requirements( code("position") . " is a valid iterator from the container and " . 
                      code( "x" ) ." is heap-allocated and cannot be " .code( "0" ) ) .
        effects( "Inserts " . code("x") . " before " . code( "position" ) .
                 " and returns an iterator pointing to it" ) .
        throws( code("bad_pointer") . " if " . code( "x" ) . " is " . code( "0" ) ) .
        exceptionSafety( "Strong guarantee" ) 
        ) .
    code( "iterator insert( iterator position, const T& x );" ) .
    blockQuote( 
        requirements( code("position") . " is a valid iterator from the container" ) .
        effects( code( "return insert( position, make_clone( x ) );" ) ) .
        exceptionSafety( "Strong guarantee" ) 
        ) .
    code( "void insert( iterator position, size_type n, const T& x );" ) .
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
            requirements( code("[first,last)") . " is a valid range" ) .
            effects( "Removes the range of element defined by ". code( "[first,last)" ) ." and returns an
                 iterator to the following element" ) .
            throws( "Nothing" ) 
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
        ) .
pre( "template< typename PtrContainer >
void  transfer( iterator before, typename PtrContainer::iterator object, 
                PtrContainer& from );" ) .
    blockQuote( 
        requirements( code( "not from.empty()" ) ) .
        effects( "Inserts the object defined by " . code( "object" ) . "into the container 
                 and remove it from " . code( "from." ) ) .
        postconditions( code( "size()" ) . " is one more, " . code( "from.size()" ) . " is one less." ) .  
        exceptionSafety( "Strong guarantee" ) 
        ) .
pre( "template< typename PtrContainer > 
void  transfer( iterator before, typename PtrContainer::iterator first, 
                typename PtrContainer::iterator last, PtrContainer& from );" ) .
    blockQuote( 
        requirements( code( "not from.empty()" ) ) .
        effects( "Inserts the objects defined by the range " . code( "[first,last)" ) . "into the container 
                 and remove it from " . code( "from." ) ) .
        postconditions( "Let N = " . code( "std::distance(first,last);" ) . " then " . code( "size()" ) . " is N more, " .
                                     code( "from.size()" ) . " is N less." ) .  
        exceptionSafety( "Strong guarantee" ) 
        ) .
pre( "template< typename PtrContainer >
void  transfer( iterator before, PtrContainer& from ); " ) .
    blockQuote( 
        effects( "Inserts the object defined by " . code( "object" ) . "into the container 
                 and remove it from " . code( "from." ) ) .
        postconditions( code( "from.empty()." ) ) .  
        exceptionSafety( "Strong guarantee" ) 
        ) .
/////////////////////////////////////////////////////////////////////////////
beginDetails( "pointer container algorithms" ) .
        code( "void sort();" ) .
        pre( "template< typename BinaryPredicate >
void sort( BinaryPredicate pred ); " ) .
        code( "void  unique();" ) .
        pre( "template< typename BinaryPredicate >
void unique( BinaryPredicate pred );" ) .
        code( "void  remove( const T& x );" ) .
        pre( "template< typename Predicate >
void remove_if( Predicate pred );" ) ;
 
return $res . $synopsis . $details;


}



function dequeRef()
{
    $res = beginSection( dequeLink() ) . include_header( "ptr_deque.hpp" );
    $synopsis = beginSynopsis() . 
"<pre>             
namespace boost
{
    template< typename T, typename Allocator = std::allocator< T* > >
    class ptr_deque 
    {
        //
        // ptr_container requirements + ptr_sequence requirements + 
        // 
        
    public: // element access
        T&        operator[]( size_type n );
        const T&  operator[]( size_type n ) const;
        T&        at( size_type n );
        const T&  at( size_type n ) const;
    
   public: // modifiers
        void                push_front( T* x );
        void                push_front( const T& x );
        void                pop_front();
        std::auto_ptr< T >  release_front(); 
    
    }; //  class 'ptr_deque'
    
} // namespace 'boost'  
    </pre> ";

    $details =  beginDetails( "element access" ) .
        code( "T& operator[]( size_type n );" ) .
        blockQuote( 
            see( "ptr_vector::operator[]" ) 
            ) .
        code( "const T& operator[]( size_type n ) const;" ) . 
        blockQuote(
            see( "ptr_vector::operator[]" ) 
            ) .
        code( "T& at( size_type n );" ) . 
        blockQuote( 
            see( "ptr_vector::at()" ) 
            ) .
        code( "const T& at( size_type n );" ) . 
        blockQuote( 
            see( "ptr_vector::at()" ) 
            ) .
        beginDetails( "modifiers" ) .
        code( "void push_front( T* x );" ) .
        blockQuote(
            requirements( code("x") . " is heap-allocated and cannot be " . code("0") ) .
            effects( "Inserts the pointer into container and takes ownership of it" ) .
            throws( code("bad_pointer") . " if " . code( "x" ) . " is " . code( "0" ) ) .
            exceptionSafety( "Strong guarantee" ) 
            ) .
        code( "void push_front( const T& x );" ) .
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
    $res = beginSection( listLink() ) . include_header( "ptr_list.hpp" );
    $synopsis = beginSynopsis() . 
"<pre>             
namespace boost
{
template< typename T, typename Allocator = std::allocator< T* > >
class ptr_list 
{
    //
    // ptr_container requirements + ptr_sequence requirements +
    // 

public: // modifiers
    void                push_front( T* x );
    void                push_front( const T& x );    
    void                pop_front();
    std::auto_ptr< T >  release_front(); 

public: // list operations
    void  splice( iterator before, ptr_list& x );
    void  splice( iterator before, ptr_list& x, iterator i );
    void  splice( iterator before, ptr_list& x, iterator first, iterator last );
    
    void  merge( ptr_list& x ); 
    template< typename Compare > 
    void  merge( ptr_list& x, Compare comp );
    
    void  reverse();

}; //  class 'ptr_list'

} // namespace 'boost'  
</pre> ";
    
    $details = beginDetails( "modifiers" ) .
        code( "void push_front( T* x );" ) .
        blockQuote(
            see( code( "deque::push_front()" ) )
            ) .
        code( "void push_front( const T& x );" ) .
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
    
    $details .= beginDetails( "list operations" ) .
        code( "void splice( iterator before, ptr_list& x );" ) .
        blockQuote( "" ) .
        code( "void splice( iterator before, ptr_list& x, iterator i );" ) .
        blockQuote( "" ) .
        code( "void splice( iterator before, ptr_list& x, iterator first, iterator last );" ) .
        blockQuote( "" ) .
        code( "void merge( ptr_list& x );" ) .
        blockQuote( "" ) . 
        pre( "template< typename Compare > 
void merge( ptr_list& x, Compare comp );" ) .
        blockQuote( "" ) . 
        code( "void reverse();" ) ;


    return $res . $synopsis . $details; 
}



function vectorRef() 
{
    $res = beginSection( vectorLink() ) . include_header( "ptr_vector.hpp" );;
    $synopsis = beginSynopsis() . 
"<pre>             
namespace boost
{
    template< typename T, typename Allocator = std::allocator< T* > >
    class ptr_vector 
    {
        //
        // ptr_container requirements + ptr_sequence requirements + 
        // 

    public: // capacity
        size_type  capacity() const;
        void       reserve( size_type n );
    
    public: // element access
        T&        operator[]( size_type n );
        const T&  operator[]( size_type n ) const;
        T&        at( size_type n );
        const T&  at( size_type n ) const;
        
    }; //  class 'ptr_vector'
    
} // namespace 'boost'  
    </pre> ";

    $details = beginDetails( "capacity" ) .
        code( "size_type capacity() const;" ) .
        blockQuote( 
            effects( "Returns the size of the allocated buffer" ) .
            throws( "Nothing" ) 
            ) .
        beginDetails( "element access" ) .
        code( "T& operator[]( size_type n );" ) .
        blockQuote( 
            requirements( code( "n < size()" ) ) .
            effects( "Returns a reference to the n'th element" ) 
            ) .
        code( "const T& operator[]( size_type n ) const;" ) . 
        blockQuote( 
            requirements( code( "n < size()" ) ) .
            effects( "Returns a const reference to the n'th element" ) 
            ) .
        code( "T& at( size_type n );" ) . 
        blockQuote( 
            requirements( code( "n < size()" ) ) .
            effects( "Returns a reference to the n'th element" ) .
            throws( code("bad_index") . " if " . code("n >=size()" ) ) 
            ) .
        code( "const T& at( size_type n );" ) . 
        blockQuote( 
            requirements( code( "n < size()" ) ) .
            effects( "Returns a const reference to the n'th element" ) .
            throws( code("bad_index") . " if " . code("n >= size()" ) ) 
            );
    
    return $res . $synopsis . $details; 
}



function ptrAssociativeContainerRef()
{
    $res = beginSection( ptrAssociativeContainerLink() );
    $synopsis = beginSynopsis() . 
"<pre>             
namespace boost
{
    template< typename T >
    class ptr_associative_container
    {
    public: // typedefs
        typedef <b>implementation defined</b>   key_type;
        typedef <b>implementation defined</b>   key_compare;
        typedef <b>implementation defined</b>   value_compare;

    public: // observers
        key_compare    key_comp() const;
        value_compare  value_comp() const;

    public: // modifiers         
        template< typename InputIterator >
        void       insert( InputIterator first, InputIterator last );     
        void       erase( iterator position ); 
        size_type  erase( const key_type& x );
        void       erase( iterator first, iterator last );
        
    public: // pointer container requirements
        template< typename PtrContainer >
        void transfer( typename PtrContainer::iterator object, PtrContainer& from );
    
    }; //  class 'ptr_associative_container'
    
} // namespace 'boost'  
    </pre> ";
    
    $details = beginDetails( "observers" ) .
        code( "key_compare key_comp() const;" ) .
        blockQuote( "" ) .
        code( "value_compare value_comp() const;" ) .
        blockQuote( "" ) .
        /////////////////////////////////////////////////////////////////////
        beginDetails( "modifiers" ) .
        pre( "template< typename InputIterator >
void insert( InputIterator first, InputIterator last );" ) . 
        blockQuote( 
            requirements( code("[first,last)") . " is a valid range" ) .
            effects( "Inserts a cloned range " ) .
            exceptionSafety( "Basic guarantee" ) 
       ) .
        code( "void erase( iterator position );" ) . 
        blockQuote( 
            requirements( code("position") . " is a valid iterator from the container" ) .
            effects( "Removes the element defined by ". code( "position." ) ) .
            throws( "Nothing" ) 
            ) .
        code( "size_type erase( const key_type& x );" ) .
        blockQuote(
            effects( "Removes all the elements in the container with a key equivalent to " 
                     . code( "x" ) . " and return the number of erased elements." ) .
            throws( "Nothing" )  
             ) .  
        code( "void erase( iterator first, iterator last );" ) .
        blockQuote( 
            requirements( code("[first,last)") . " is a valid range" ) .
            effects( "Removes the range of elements defined by ". code( "[first,last)." ) ) .
            throws( "Nothing" ) 
            ) .

        /////////////////////////////////////////////////////////////////////
        beginDetails( "pointer container requirements" ) .
        pre( "template< typename PtrContainer >
void transfer( typename PtrContainer::iterator object, PtrContainer& from );" ) .
        blockQuote( 
            requirements( code( "not from.empty()" ) ) .
            effects( "Inserts the object defined by " . code( "object" ) . "into the container 
                     and remove it from " . code( "from." ) ) .
            postconditions( code( "size()" ) . " is one more, " . code( "from.size()" ) . " is one less." ) .  
            exceptionSafety( "Strong guarantee" ) 
            ) ;

    
    return $res . $synopsis .$details ;
}



function setRef() 
{
    $res = beginSection( setLink() ) . include_header( "ptr_set.hpp" );;
    
    $synopsis = beginSynopsis() . 
"<pre>             
    namespace boost
    {
    template< typename Key, typename Compare = ptr_less< Key >, 
              typename Allocator = std::allocator< Key* > > >
    class ptr_set
    {
        //
        // ptr_container requirements + ptr_associative_container requirements +
        //
        
    public: // modifiers         
        std::pair< iterator,bool >  insert( Key* x );                         
        std::pair< iterator,bool >  insert( const Key& x );

    public: // set algorithms
        iterator                        find( const Key& x ) const;                                            
        size_type                       count( const Key& x ) const;                                          
        iterator                        lower_bound( const Key& x ) const;                                     
        iterator                        upper_bound( const Key& x ) const;                                     
        std::pair< iterator,iterator >  equal_range( const Key& x ) const;                 

    }; //  class 'ptr_set'

} // namespace 'boost'  
</pre> ";

    $details = beginDetails( "modifiers" ) .
        code( "std::pair<iterator,bool> insert( Key* x );" ) .
        blockQuote( 
            requirements( code( "x != 0" ) ) .
            effects( "Takes ownership of " . code("x") . " and insert it if there is
                     no equivalent of it already. The " . code( "bool" ) . " part of
                     the return value indicates insertion and the " . code( "iterator" ) .
                     " points to the element with key " . code( "x." )  ) .
            throws( code("bad_pointer") . " if " . code( "x == 0" ) ) .
            exceptionSafety( "Strong guarantee" )  
             ) .                         
        code( "std::pair<iterator,bool> insert( const Key& x );" ) .
        blockQuote(
            effects( code( "return insert( make_clone( x ) );" ) ) .
            exceptionSafety( "Strong guarantee" )  
             ) .
        /////////////////////////////////////////////////////////////////////
        beginDetails( "set algorithms" ) .
        code( "iterator find( const Key& x ) const;" ) .
        blockQuote( 
            effects( "Searches for the key and returns " . code( "end()" ) . " on failure." ) .
            complexity( "Logarithmic" )
             ) .
        code( "size_type count( const Key& x ) const;" ) .  
        blockQuote(
            effects( "Counts the elements with a key equivalent to " . code( "x," ) . " that is,
                      it returns either 0 or 1." ) .
            complexity( "Logarithmic" )  
             ) .
        code( "iterator lower_bound( const Key& x ) const; " ) . 
        blockQuote(
            effects( "Returns an iterator pointing to the first element with a key not less than " . code( "x" ) ) .
            complexity( "Logarithmic" )  
             ) .
        code( "iterator upper_bound( const Key& x ) const; " ) .
        blockQuote(
            effects( "Returns an iterator pointing to the first element with a key greater than " . code( "x" ) ) .
            complexity( "Logarithmic" ) 
             ) .
        code( "std::pair<iterator,iterator> equal_range( const Key& x ) const; " ) .
        blockQuote( 
            effects( code("return std::make_pair( lower_bound( x ), upper_bound( x ) );" ) ) .
            complexity( "Logarithmic" ) 
            );
    

    return $res . $synopsis . $details; 
}



function multisetRef()
{
    $res = beginSection( multisetLink() ) . include_header( "ptr_set.hpp" );;
    
    $synopsis = beginSynopsis() . 
"<pre>             
    namespace boost
    {
    template< typename Key, typename Compare = ptr_less< Key >, 
              typename Allocator = std::allocator< Key* > > >
    class ptr_multiset
    {
        //
        // ptr_container requirements + ptr_associative_container requirements +
        //
        
    public: // modifiers         
        iterator  insert( Key* x );                         
        iterator  insert( const Key& x );

    public: // set algorithms
        iterator                        find( const Key& x ) const;                                            
        size_type                       count( const Key& x ) const;                                          
        iterator                        lower_bound( const Key& x ) const;                                     
        iterator                        upper_bound( const Key& x ) const;                                     
        std::pair< iterator,iterator >  equal_range( const Key& x ) const;                 

    }; //  class 'ptr_multiset'

} // namespace 'boost'  
</pre> ";

    $details = beginDetails( "modifiers" ) .
        code( "iterator insert( Key* x );" ) .
        blockQuote( 
            requirements( code( "x != 0" ) ) .
            effects( "Takes ownership of " . code("x") . " and insert it. 
                      The return value points to the inserted element." ) .
            throws( code("bad_pointer") . " if " . code( "x == 0" ) ) .
            exceptionSafety( "Strong guarantee" )  
             ) .                         
        code( "iterator insert( const Key& x );" ) .
        blockQuote(
            effects( code( "return insert( make_clone( x ) );" ) ) .
            exceptionSafety( "Strong guarantee" )  
             ) .
        /////////////////////////////////////////////////////////////////////
        beginDetails( "set algorithms" ) .
        code( "iterator find( const Key& x ) const;" ) .
        blockQuote( 
            effects( "Searches for the key and returns " . code( "end()" ) . " on failure." ) .
            complexity( "Logarithmic" )
             ) .
        code( "size_type count( const Key& x ) const;" ) .  
        blockQuote(
            effects( "Counts the elements with a key equivalent to " . code( "x." ) ) .
            complexity( "Logarithmic" )  
             ) .
        code( "iterator lower_bound( const Key& x ) const; " ) . 
        blockQuote(
            effects( "Returns an iterator pointing to the first element with a key not less than " . code( "x" ) ) .
            complexity( "Logarithmic" )  
             ) .
        code( "iterator upper_bound( const Key& x ) const; " ) .
        blockQuote(
            effects( "Returns an iterator pointing to the first element with a key greater than " . code( "x" ) ) .
            complexity( "Logarithmic" ) 
             ) .
        code( "std::pair<iterator,iterator> equal_range( const Key& x ) const; " ) .
        blockQuote( 
            effects( code("return std::make_pair( lower_bound( x ), upper_bound( x ) );" ) ) .
            complexity( "Logarithmic" ) 
            );
    
    return $res . $synopsis . $details;
}



function mapRef()
{
    $res = beginSection( mapLink() ) . include_header( "ptr_map.hpp" );;
    
        $synopsis = beginSynopsis() . 
"<pre>             
    namespace boost
    {
    template< typename Key, typename T, typename Compare = std::less< Key >, 
              typename Allocator = std::allocator< std::pair<const Key, T*> > >
    class ptr_map
    {
        //
        // ptr_container requirements + ptr_associative_container requirements +
        //
            
    public: // typedefs
        typedef T mapped_type;
        
    public: // modifiers         
        std::pair< iterator,bool >  insert( Key& key, T* x );                         
        std::pair< iterator,bool >  insert( Key& key, const T& x );
     
    public: // element access
        T&        operator[]( const Key& key );
        const T&  operator[]( const Key& key ) const;                

    public: // map algorithms
        iterator                                    find( const Key& x ) const;                                            
        const_iterator                              find( const Key& x );                                            
        size_type                                   count( const Key& x ) const;                                          
        iterator                                    lower_bound( const Key& x );                                     
        const_iterator                              lower_bound( const Key& x ) const;                                     
        iterator                                    upper_bound( const Key& x );                                     
        const_iterator                              upper_bound( const Key& x ) const;                                     
        std::pair< iterator,iterator >              equal_range( const Key& x );                 
        std::pair< const_iterator,const_iterator >  equal_range( const Key& x ) const;                 

    }; //  class 'ptr_map'

} // namespace 'boost'  
</pre> ";

    $details = beginDetails( "modifiers" ) .
        code( "std::pair<iterator,bool> insert( Key& key, T* x );" ) .
        blockQuote( 
            requirements( code( "x != 0" ) ) .
            effects( "Takes ownership of " . code("x") . " and insert it if there is
                     no equivalent of it already. The " . code( "bool" ) . " part of
                     the return value indicates insertion and the " . code( "iterator" ) .
                     " points to the element with key " . code( "x." )  ) .
            throws( code("bad_pointer") . " if " . code( "x == 0" ) ) .
            exceptionSafety( "Strong guarantee" )  
             ) .                         
        code( "std::pair<iterator,bool> insert( const Key& x );" ) .
        blockQuote(
            effects( code( "return insert( make_clone( x ) );" ) ) .
            exceptionSafety( "Strong guarantee" )  
             ) .
        /////////////////////////////////////////////////////////////////////
        beginDetails( "element access" ) .
        code( "T& operator[]( const Key& key );" ) .
        blockQuote(
            effects( "Returns the element with the given key if it can be found." ) .
            throws( code("bad_ptr_container_operation") . " if no element with the key
                    exists. " ) .
            complexity( "Logarithmic" )
            ) .
        code( "const T& operator[]( const Key& key ) const;" ) .                
        blockQuote( "See above" ) .
        /////////////////////////////////////////////////////////////////////
        beginDetails( "map algorithms" ) .
        code( "iterator find( const Key& x );" ) .
        blockQuote( 
            effects( "Searches for the key and returns " . code( "end()" ) . " on failure." ) .
            complexity( "Logarithmic" )
             ) .
        code( "const_iterator find( const Key& x ) const;" ) .
        blockQuote( "See above" ) .
        code( "size_type count( const Key& x ) const;" ) .  
        blockQuote(
            effects( "Counts the elements with a key equivalent to " . code( "x," ) . " that is,
                      it returns either 0 or 1." ) .
            complexity( "Logarithmic" )  
             ) .
        code( "iterator lower_bound( const Key& x ); " ) . 
        blockQuote(
            effects( "Returns an iterator pointing to the first element with a key not less than " . code( "x" ) ) .
            complexity( "Logarithmic" )  
             ) .
        code( "const_iterator lower_bound( const Key& x ) const; " ) .
        blockQuote( "See above" ) .
        code( "iterator upper_bound( const Key& x ); " ) .
        blockQuote(
            effects( "Returns an iterator pointing to the first element with a key greater than " . code( "x" ) ) .
            complexity( "Logarithmic" ) 
             ) .
        code( "const_iterator upper_bound( const Key& x ) const; " ) .
        blockQuote( "See above" ) .
        code( "std::pair<iterator,iterator> equal_range( const Key& x ); " ) .
        blockQuote( 
            effects( code("return std::make_pair( lower_bound( x ), upper_bound( x ) );" ) ) .
            complexity( "Logarithmic" ) 
            ) .
        code( "std::pair<const_iterator,const_iterator> equal_range( const Key& x ) const; " ) .
        blockQuote( "See above" );

    return $res . $synopsis . $details; 
}



function multimapRef()
{
    $res = beginSection( multimapLink() ) . include_header( "ptr_map.hpp" );;
        
    $synopsis = beginSynopsis() . 
"<pre>             
    namespace boost
    {
    template< typename Key, typename T, typename Compare = std::less< Key >, 
              typename Allocator = std::allocator< std::pair<const Key, T*> > >
    class ptr_multimap
    {
        //
        // ptr_container requirements + ptr_associative_container requirements +
        //
            
    public: // typedefs
        typedef T mapped_type;
        
    public: // modifiers         
        iterator  insert( Key& key, T* x );                         
        iterator  insert( Key& key, const T& x );
     
    public: // element access
        T&        operator[]( const Key& key );
        const T&  operator[]( const Key& key ) const;                

    public: // map algorithms
        iterator                                    find( const Key& x ) const;                                            
        const_iterator                              find( const Key& x );                                            
        size_type                                   count( const Key& x ) const;                                          
        iterator                                    lower_bound( const Key& x );                                     
        const_iterator                              lower_bound( const Key& x ) const;                                     
        iterator                                    upper_bound( const Key& x );                                     
        const_iterator                              upper_bound( const Key& x ) const;                                     
        std::pair< iterator,iterator >              equal_range( const Key& x );                 
        std::pair< const_iterator,const_iterator >  equal_range( const Key& x ) const;                 

    }; //  class 'ptr_map'

} // namespace 'boost'  
</pre> ";

    $details = beginDetails( "modifiers" ) .
        code( "iterator insert( Key& key, T* x );" ) .
        blockQuote( 
            requirements( code( "x != 0" ) ) .
            effects( "Takes ownership of " . code("x") . " and insert it.
                     The return value points to the inserted element." ) .
            throws( code("bad_pointer") . " if " . code( "x == 0" ) ) .
            exceptionSafety( "Strong guarantee" )  
             ) .                         
        code( "iterator insert( const Key& x );" ) .
        blockQuote(
            effects( code( "return insert( make_clone( x ) );" ) ) .
            exceptionSafety( "Strong guarantee" )  
             ) .
        /////////////////////////////////////////////////////////////////////
        beginDetails( "element access" ) .
        code( "T& operator[]( const Key& key );" ) .
        blockQuote(
            effects( "Returns an (not necessarily \"the\") element with the given key if it can be found." ) .
            throws( code("bad_ptr_container_operation") . " if no element with the key
                    exists. " ) .
            complexity( "Logarithmic" )
            ) .
        code( "const T& operator[]( const Key& key ) const;" ) .                
        blockQuote( "See above" ) .
        /////////////////////////////////////////////////////////////////////
        beginDetails( "map algorithms" ) .
        code( "iterator find( const Key& x );" ) .
        blockQuote( 
            effects( "Searches for the key and returns " . code( "end()" ) . " on failure." ) .
            complexity( "Logarithmic" )
             ) .
        code( "const_iterator find( const Key& x ) const;" ) .
        blockQuote( "See above" ) .
        code( "size_type count( const Key& x ) const;" ) .  
        blockQuote(
            effects( "Counts the elements with a key equivalent to " . code( "x," ) . " that is,
                      it returns either 0 or 1." ) .
            complexity( "Logarithmic" )  
             ) .
        code( "iterator lower_bound( const Key& x ); " ) . 
        blockQuote(
            effects( "Returns an iterator pointing to the first element with a key not less than " . code( "x" ) ) .
            complexity( "Logarithmic" )  
             ) .
        code( "const_iterator lower_bound( const Key& x ) const; " ) .
        blockQuote( "See above" ) .
        code( "iterator upper_bound( const Key& x ); " ) .
        blockQuote(
            effects( "Returns an iterator pointing to the first element with a key greater than " . code( "x" ) ) .
            complexity( "Logarithmic" ) 
             ) .
        code( "const_iterator upper_bound( const Key& x ) const; " ) .
        blockQuote( "See above" ) .
        code( "std::pair<iterator,iterator> equal_range( const Key& x ); " ) .
        blockQuote( 
            effects( code("return std::make_pair( lower_bound( x ), upper_bound( x ) );" ) ) .
            complexity( "Logarithmic" ) 
            ) .
        code( "std::pair<const_iterator,const_iterator> equal_range( const Key& x ) const; " ) .
        blockQuote( "See above" );

    return $res . $synopsis . $details; 
}
     



function mapIteratorRef()
{
    $res = beginSection( mapIteratorLink() );
    
    $explanation = p( "The map iterators are a bit different compared to the normal ones. The
                        reason is that it is a bit clumsy to access the key and the mapped object
                        through " . code( "i->first" ) . " and " . code( "i->second," ) 
                       . " and one tends to forget what is what. The new style can be illustrated 
                        with a small example:" ) .
        pre( "
    typedef ptr_map<string,int> map_t;
    map_t  m;
    string name = \"foo\";            // we cannot pass rvalue as key             
    m.insert( name, new int( 4 ) ); // insert pair
    m[ name ] += 5;                 // add 5 to the objects value 
    ...
    for( map_t::iterator i = m.begin(); i != m.end(); ++i )
    {
             *i += 42; // add 42 to each value
             cout << \"value=\" << *i << \", key=\" << i.key() << \"n\";
    } " ) .
        p( "So the difference from the normal map iterator is that " . code( "operator*()" ) . 
            " returns a reference to the mapped object (normally it returns a reference to a pair) and that
              the key can be accessed through the " . code( "key()" ) . " function. " ); 
                         
    return $res . $explanation;
}



function exceptionRef()
{
    $res = beginSection( exceptionLink() );
    
    $explanation = p( "There are three exceptions that are thrown by this library. The exception
                      hierarchy looks as follows:" ) .
        pre( "
namespace boost 
{            
    //             
    // library exception base class
    //
    class bad_ptr_container_operation : public std::exception
    {
    public:
        bad_ptr_container_operation( const char* what );
        
        virtual const char* what() const throw();
    };
 
    //
    // exception for signaling that a null pointer has been passes to a container     
    //                                
    class bad_pointer : public bad_ptr_container_operation
    {
    public:
        bad_pointer( const char* what );
    };

    //
    // exception for signaling out of bounds indexing
    //                  
    class bad_index : public bad_ptr_container_operation
    {
    public:
        bad_index( const char* what );
    };
} " );

    return $res . $explanation;
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


function include_header( $file )
{
    return code( "#include <boost/ptr_container/$file>" );
}

//////////////////////////////////////////////////////////////////////////////
// anchors

function polymorphicClassProblemLink()
{
    return "polymorphic class problem";
}



function smartPointerOverheadLink()
{
    return "smart pointer comparison";
}

//////////////////////////////////////////////////////////////////////////////
// reference anchors

function refLink( $number )
{
    return "[$number]";
}



function exampleLink( $number ) 
{
    return "Example $number";
}

?>
