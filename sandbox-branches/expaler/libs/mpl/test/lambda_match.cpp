#include "boost/mpl/lambda_match.hpp"
#include "boost/mpl/placeholders.hpp"
#include "boost/mpl/pair.hpp"

#include "boost/static_assert.hpp"

namespace mpl = boost::mpl;

int main()
{
    using namespace mpl::placeholders;

    // exact match

    BOOST_STATIC_ASSERT((
          mpl::lambda_match<
              mpl::pair<int,double>
            , mpl::pair<int,double>
            >::value
        ));

    BOOST_STATIC_ASSERT((
          !mpl::lambda_match<
              mpl::pair<int,double>
            , mpl::pair<int,int>
            >::value
        ));

    // basic pattern match

    BOOST_STATIC_ASSERT((
          mpl::lambda_match<
              mpl::pair<_,_>
            , mpl::pair<int,double>
            >::value
        ));

    BOOST_STATIC_ASSERT((
          mpl::lambda_match<
              mpl::pair<_,double>
            , mpl::pair<int,double>
            >::value
        ));

    BOOST_STATIC_ASSERT((
          !mpl::lambda_match<
              mpl::pair<_,int>
            , mpl::pair<int,double>
            >::value
        ));

    // tricky pattern match

    BOOST_STATIC_ASSERT((
          mpl::lambda_match<
              mpl::pair<_1,_2>
            , mpl::pair<int,double>
            >::value
        ));

    BOOST_STATIC_ASSERT((
          mpl::lambda_match<
              mpl::pair<_1,_1>
            , mpl::pair<int,int>
            >::value
        ));

    BOOST_STATIC_ASSERT((
          !mpl::lambda_match<
              mpl::pair<_1,_1>
            , mpl::pair<int,double>
            >::value
        ));

    return 0;
}
