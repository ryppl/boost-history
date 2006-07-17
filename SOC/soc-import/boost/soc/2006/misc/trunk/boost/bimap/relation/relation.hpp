// Boost.Bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bimap for library home page.

/// \file relation/relation.hpp
/// \brief Includes the relation class

#ifndef BOOST_BIMAP_RELATION_RELATION_HPP
#define BOOST_BIMAP_RELATION_RELATION_HPP

#include <boost/mpl/if.hpp>
#include <boost/bimap/detail/mpl/bool_result.hpp>

// Boost.Bimap
#include <boost/bimap/relation/mutant_relation.hpp>
#include <boost/bimap/relation/standard_relation.hpp>
#include <boost/bimap/tags/support/value_type_of.hpp>

namespace boost {
namespace bimap {

/// \brief Defines the relation class, an extension of the std::pair class, and utilities to work with it.
/**

The relation class is an extension of the std framework proposed to relate
two types. In the actual standard std::pair is the way to tie to types
together. This two types are related but in a directed way. The first type
is related to the second one but the inverse relationship does not hold.
This is the reason why the names of the two members: \c first and \c second
is a consistent naming convension. However, there are a lot of cases where
do not exist a prefered type. There is no way to have the two types at
the same level with std::pair. In that cases a relation will fit better.
With a nice left/right naming convention and support to view the relation
as a signature compatible std::pair with first/second, is a very lightweight
class that as an additional feature supports user defined tags to access
the values so the code generated is autodocummented.

\ingroup relation_group

                                                                          **/

namespace relation {

namespace detail {

/// \brief Metafunction to test if the compiler supports the mutant idiom.
/**

See also select_relation, mutant_relation, standard_relation.
                                                                    **/

template< class TA, class TB >
struct is_mutant_idiom_supported_for
{
    // I have to build the two views and see if they are the correct size

    static const int sAB = (  sizeof( typename tags::support::value_type_of<TA>::type ) +
                              sizeof( typename tags::support::value_type_of<TB>::type )   );

    static const bool mutant_is_supported =
        ( sizeof( structured_pair<TA,TB,normal_layout> ) == sAB ) &&
        ( sizeof( structured_pair<TA,TB,mirror_layout> ) == sAB );

    BOOST_BIMAP_MPL_BOOL_RESULT(true); //TODO replace true with mutant_is_supported

};

} // namespace detail

/// \brief Abstraction of a related pair of values, that extends the std::pair class.
/**

A \c select_relation<X,Y>::type has to be viewed in the following way: x is related
with y and y is related with x.

\verbatim
            __________             __________
           |          |           |          |
           |          |           |          |
           |    X     | <-------> |    Y     |
           |          |           |          |
           |__________|           |__________|

              left                  right

\endverbatim

By no means a relation imposes a ordering between the data.
This is the main difference with the concept of a pair in
the std library. If used with tags you can even exchange the
left and right parameter in the template definition and all
the code will continue working.

If not tags are used, the first/second members are replaced
by the more symetric approach: member_at::left and
member_at::right.

\note Even when user tags are specified the
      member_at idiom still works.

With the actual standard, the optimal implementation is not ANSI compliant.
However most compilers support it so this metafunction check if the compiler
will do the right thing with the layout of each class and if the mutant idiom
is supported. If this is the case mutant_relation will be used. If it is not
supported then standard_relation, a suboptimasl approach based in references
will be used. Using this metafunction is compliante with the standard and
optimal for each compiler.

See also mutant_relation, standard_relation, member_at, get(), value_type_of,
pair_by(), pair_type_by.

\ingroup relation_group
                                                           **/

template< class TA, class TB >
struct select_relation
{
    typedef typename mpl::if_< detail::is_mutant_idiom_supported_for<TA,TB> ,
                       // {
                              mutant_relation<TA,TB>,
                       // }
                       // else
                       // {
                              standard_relation<TA,TB>
                       // }

    >::type type;
};


/// \brief Functions, Metafunctions and Utilies to work with the relation class.
/**

All the entities defined in this namespace use the member_at idiom and user tags
to work with the relation class. It is a good pratice to use this utilities instead
of the direct access to members of the class. For example:

\code

typedef relation<int,std::string> rel;
rel r;
...

// This way of access to the value of a member
int number = get<member_at::left>(r);

// Is prefered over the more direct approach
int number = r.left;

\endcode

There is no runtime penalty in using the get() function and what is more
important have an easier migration path if the user decides to use tags and
want to autodocument the code using them. Remember that this is no necessary
since the member_at idiom is supported even when there are user defined tags
specified. The following is the same example using user tags.

\code

struct id {};
struct name {};

typedef relation< tagged<int,id>, tagged<std::string,name> > rel;
rel r;
...

// This is still valid...
int number = get<member_at::left>(r);

// But now we can use a better way to access the relation information
int worker_id = get<id>(r);

get<name>(r) = "John Smith";

\endcode

\ingroup relation_group

                                                                               **/
namespace support {}

} // namespace relation
} // namespace bimap
} // namespace boost


#endif // BOOST_BIMAP_RELATION_RELATION_HPP

