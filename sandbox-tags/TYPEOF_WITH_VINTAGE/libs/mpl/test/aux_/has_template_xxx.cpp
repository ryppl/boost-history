// (C) Copyright Jonathan Turkanis 2003.
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies. This
// software is provided "as is" without express or implied warranty, and
// with no claim as to its suitability for any purpose.

#if defined(__BORLANDC__) && (__BORLANDC__ >= 0x600)
#define BOOST_STRICT_CONFIG
#endif  

#include <string>
#include <boost/mpl/aux_/has_template_xxx.hpp>
#include <boost/static_assert.hpp>

BOOST_HAS_TEMPLATE_XXX_TRAIT_NAMED_DEF(has_template_xxx, xxx, 5)
#define CHECK_HAS_TEMPLATE_XXX(type)                       \
    BOOST_STATIC_ASSERT(has_template_xxx< type >::value);  \
    /**/
#define CHECK_NO_TEMPLATE_XXX(type)                         \
    BOOST_STATIC_ASSERT(!has_template_xxx< type >::value);  \
    /**/

struct has_xxx_1 {
    template<typename T1>
    struct xxx { typedef int type; };
};

struct has_xxx_2 {
    template<typename T1, typename T2>
    struct xxx { typedef int type; };
};

struct has_xxx_3 {
    template<typename T1, typename T2, typename T3>
    struct xxx { typedef int type; };
};

struct has_xxx_4 {
    template<typename T1, typename T2, typename T3, typename T4>
    struct xxx { typedef int type; };
};

struct has_xxx_5 {
    template<typename T1, typename T2, typename T3, typename T4, typename T5>
    struct xxx { typedef int type; };
};

struct has_xxx_with_defaults1 {
    template<typename T1 = int>
    struct xxx { typedef int type; };
};

struct has_xxx_with_defaults2 {
    template<typename T1, typename T2 = int>
    struct xxx { typedef int type; };
};

struct has_xxx_with_defaults3 {
    template<typename T1, typename T2 = int, typename T3 = int>
    struct xxx { typedef int type; };
};

struct has_xxx_with_defaults4 {
    template<typename T1, typename T2, typename T3 = int, typename T4 = int>
    struct xxx { typedef int type; };
};

struct has_xxx_with_defaults5 {
    template< typename T1, typename T2, typename T3,
              typename T4 = int, typename T5 = int >
    struct xxx { typedef int type; };
};

struct no_xxx_1 { };
struct no_xxx_2 { void xxx(); };
struct no_xxx_3 { int xxx; };
struct no_xxx_4 { static int xxx(); };
struct no_xxx_5 { typedef int xxx; };
struct no_xxx_6 { struct xxx; };
struct no_xxx_7 { typedef int& xxx; };
struct no_xxx_8 { typedef int* xxx; };
struct no_xxx_9 { typedef int xxx[10]; };
struct no_xxx_10 { typedef void (*xxx)(); };
struct no_xxx_11 { typedef void (xxx)(); };

int main()
{
    CHECK_HAS_TEMPLATE_XXX(has_xxx_1);
    CHECK_HAS_TEMPLATE_XXX(has_xxx_2);
    CHECK_HAS_TEMPLATE_XXX(has_xxx_3);
    CHECK_HAS_TEMPLATE_XXX(has_xxx_4);
    CHECK_HAS_TEMPLATE_XXX(has_xxx_5);
    CHECK_HAS_TEMPLATE_XXX(has_xxx_with_defaults1);
    CHECK_HAS_TEMPLATE_XXX(has_xxx_with_defaults2);
    CHECK_HAS_TEMPLATE_XXX(has_xxx_with_defaults3);
    CHECK_HAS_TEMPLATE_XXX(has_xxx_with_defaults4);
    CHECK_HAS_TEMPLATE_XXX(has_xxx_with_defaults5);

    CHECK_NO_TEMPLATE_XXX(no_xxx_1);
    CHECK_NO_TEMPLATE_XXX(no_xxx_2);
    CHECK_NO_TEMPLATE_XXX(no_xxx_3);
    CHECK_NO_TEMPLATE_XXX(no_xxx_4);
    CHECK_NO_TEMPLATE_XXX(no_xxx_5);
    CHECK_NO_TEMPLATE_XXX(no_xxx_6);
    CHECK_NO_TEMPLATE_XXX(no_xxx_7);
    CHECK_NO_TEMPLATE_XXX(no_xxx_8);
    CHECK_NO_TEMPLATE_XXX(no_xxx_9);
    CHECK_NO_TEMPLATE_XXX(no_xxx_10);
    CHECK_NO_TEMPLATE_XXX(no_xxx_11);

    CHECK_NO_TEMPLATE_XXX(std::string);
    CHECK_NO_TEMPLATE_XXX(int);
    CHECK_NO_TEMPLATE_XXX(int*);
    CHECK_NO_TEMPLATE_XXX(int&);
    CHECK_NO_TEMPLATE_XXX(int[1]);
    CHECK_NO_TEMPLATE_XXX(int(*)[1]);
    CHECK_NO_TEMPLATE_XXX(int(&)[1]);
    CHECK_NO_TEMPLATE_XXX(void(*)());
    CHECK_NO_TEMPLATE_XXX(void(&)());
    CHECK_NO_TEMPLATE_XXX(void());

    return 0;
}
