/*=============================================================================
    Copyright (c) 2003 Joel de Guzman

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#define BOOST_FUSION_MAX_ARITY 12
#include <boost/test/minimal.hpp>
#include <boost/spirit/fusion.hpp>

using namespace boost::fusion;

struct X
{
    operator char const*() const
    {
        return "<X-object>";
    }
};

int
test_main(int, char*[])
{
    std::cout << tuple_open('[');
    std::cout << tuple_close(']');
    std::cout << tuple_delimiter(", ");

/// Testing joint_view

    {
        single_view<int> v1(3);
        single_view<X> v2;
        typedef joint_view<single_view<int>, single_view<X> > view_type;

        view_type view(v1, v2);
        std::cout << generate(view) << std::endl;

        BOOST_TEST((generate(view) == make_tuple(3, X())));
    }

    {
        typedef single_view<int> sv_type;
        sv_type sv(777);
        typedef tuple<int, char, double> tuple_type;
        tuple_type t(1, 'x', 3.3);
        typedef tuple_view<tuple_type> tv_type;
        tv_type tv(t);

        {
            typedef joint_view<sv_type, tv_type> view_type;
            view_type view(sv, tv);
            std::cout << generate(view) << std::endl;
        }

        {
            typedef joint_view<tv_type, sv_type> view_type;
            view_type view(tv, sv);
            std::cout << generate(view) << std::endl;
        }

        {
            typedef joint_view<tv_type, sv_type> jv_type;
            typedef joint_view<jv_type, jv_type> jv2_type;

            jv_type jv(tv, sv);
            jv2_type jv2(jv, jv);

            std::cout << generate(jv) << std::endl;
            std::cout << generate(jv2) << std::endl;

            BOOST_TEST(generate(jv2)
                == make_tuple(1, 'x', 3.3, 777, 1, 'x', 3.3, 777));
        }

        {
            typedef joint_view<tv_type, sv_type> jv1_type;
            typedef joint_view<sv_type, tv_type> jv2_type;
            typedef joint_view<jv1_type, jv2_type> jv3_type;

            jv1_type jv1(tv, sv);
            jv2_type jv2(sv, tv);
            jv3_type jv3(jv1, jv2);

            std::cout << generate(jv1) << std::endl;
            std::cout << generate(jv2) << std::endl;
            std::cout << generate(jv3) << std::endl;

            BOOST_TEST(generate(jv3)
                == make_tuple(1, 'x', 3.3, 777, 777, 1, 'x', 3.3));
        }

        {
            typedef slice_view<
                tuple_iterator<0, tuple_type>
              , tuple_iterator<0, tuple_type>
            > sl_type;

            sl_type slv(tv.begin(), tv.begin());
            std::cout << generate(slv) << std::endl;
            std::cout << generate(sv) << std::endl;

            typedef joint_view<sl_type, sv_type> jv1_type;
            jv1_type jv1(slv, sv);
            std::cout << generate(jv1) << std::endl;

            BOOST_TEST(generate(jv1) == make_tuple(777));
        }
    }

    return 0;
}

