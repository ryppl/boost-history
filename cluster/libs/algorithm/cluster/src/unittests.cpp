#include <boost/algorithm/cluster/dbscan.hpp>
#include <boost/algorithm/cluster/dist_func.hpp>

#include <boost/test/minimal.hpp>
#include <vector>
#include <cmath>

void test_dbscan();
void test_dist_fun();

int test_main(int, char *[])
{
    test_dbscan();
    
    test_dist_fun();

    return 0;
}

void test_dbscan() 
{
    using namespace boost::algorithm::cluster;
    using namespace std;

    typedef pair<float, float> two_tuple;
    typedef vector<two_tuple> tuple_data;
    typedef tuple_data::iterator tuple_data_iter;
    typedef vector<tuple_data_iter> cluster;
    typedef cluster_data<cluster> cluster_data;

    tuple_data tuples(10);
    BOOST_CHECK(10 == tuples.size());

    for (size_t n = 0; n < 5; ++n)
        tuples[n].second = 1.f;

    cluster_data clustering;
    BOOST_CHECK(clustering.empty());

    float eps1 = 0.2f;
    size_t min_points = 3;
    clustering = dbscan<cluster>(
        tuples.begin(), tuples.end(), eps1, min_points,
        euclid_dist<two_tuple, two_tuple>);
#if 0
    cerr << "clusters=" << clustering.size() << "\n";
    size_t c = 1;

    for (cluster_data::iterator citer = clustering.begin();
        citer != clustering.end();
        ++citer)
    {
        cerr << "CLUSTER: " << c << "\n";
        for (cluster::iterator it = citer->begin();
            it != citer->end();
            ++it)
        {
            cerr << " (" << (*it)->first << ", " << (*it)->second << ")\n";
        }
        ++c;
    }
#endif
    BOOST_CHECK(2 == clustering.size());

    float eps2 = 1.01f;
    clustering = dbscan<cluster>(
        tuples.begin(), tuples.end(), eps2, min_points,
        euclid_dist<two_tuple, two_tuple>);

#if 0
    cerr << "clusters=" << clustering.size() << "\n";
    c = 0;

    for (cluster_data::iterator citer = clustering.begin();
        citer != clustering.end();
        ++citer)
    {
        cerr << "CLUSTER: " << c << "\n";
        for (cluster::iterator it = citer->begin();
            it != citer->end();
            ++it)
        {
            cerr << " (" << (*it)->first << ", " << (*it)->second << ")\n";
        }
        ++c;
    }
#endif

    BOOST_CHECK(1 == clustering.size());
}


void test_dist_fun() 
{
    using namespace std;
    using namespace boost;
    using namespace boost::algorithm::cluster;

    // Euclidian distance function tests
    {
        BOOST_CHECK(0 == euclid_dist(make_pair(0., 0.), make_pair(0., 0.)));
        BOOST_CHECK(0 == euclid_dist(make_tuple(0., 0.), make_tuple(0., 0.)));
        BOOST_CHECK(0 == euclid_dist(make_pair(0., 0.), make_tuple(0., 0.)));
        BOOST_CHECK(0 == euclid_dist(make_tuple(0., 0.), make_pair(0., 0.)));
        BOOST_CHECK(0 == euclid_dist(make_tuple(0., 0., 0.), make_tuple(0., 0., 0.)));
        BOOST_CHECK(0 == euclid_dist
            (
                make_tuple(0, 0, 0, 0, 0, 0, 0, 0, 0, 0), 
                make_tuple(0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
            ));

        BOOST_CHECK(0 == euclid_dist(make_pair(23, 42), make_pair(23, 42)));
        
        BOOST_CHECK(5 == euclid_dist(make_pair(0, 0), make_pair(3, 4)));
        BOOST_CHECK(5 == euclid_dist(make_pair(3, 4), make_pair(0, 0)));

        BOOST_CHECK(5 == euclid_dist(make_tuple(0, 0), make_tuple(3, 4)));
        BOOST_CHECK(5 == euclid_dist(make_tuple(3, 4), make_tuple(0, 0)));
    }

    // Manhattan distance function tests
    {
        BOOST_CHECK(0 == manhattan_dist(make_pair(0., 0.), make_pair(0., 0.)));
        BOOST_CHECK(0 == manhattan_dist(make_tuple(0., 0.), make_tuple(0., 0.)));
        BOOST_CHECK(0 == manhattan_dist(make_pair(0., 0.), make_tuple(0., 0.)));
        BOOST_CHECK(0 == manhattan_dist(make_tuple(0., 0.), make_pair(0., 0.)));
        BOOST_CHECK(0 == manhattan_dist(make_tuple(0., 0., 0.), make_tuple(0., 0., 0.)));
        BOOST_CHECK(0 == manhattan_dist
            (
                make_tuple(0, 0, 0, 0, 0, 0, 0, 0, 0, 0), 
                make_tuple(0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
            ));

        BOOST_CHECK(0 == manhattan_dist(make_pair(23, 42), make_pair(23, 42)));
        
        BOOST_CHECK(7 == manhattan_dist(make_pair(0, 0), make_pair(3, 4)));
        BOOST_CHECK(7 == manhattan_dist(make_pair(3, 4), make_pair(0, 0)));

        BOOST_CHECK(7 == manhattan_dist(make_tuple(0, 0), make_tuple(3, 4)));
        BOOST_CHECK(7 == manhattan_dist(make_tuple(3, 4), make_tuple(0, 0)));
    }
}
