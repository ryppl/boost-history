#include <boost/algorithm/cluster/dbscan.hpp>
#include <boost/test/minimal.hpp>
#include <vector>
#include <cmath>

using namespace boost::algorithm::cluster;
using namespace std;

typedef pair<float, float> two_tuple;

static float norm2_dist(two_tuple const & p1, two_tuple const & p2)
{
  float const d = ::sqrt(
    ::pow(static_cast<double>(p2.first - p1.first), 2.) +
    ::pow(static_cast<double>(p2.second - p1.second), 2.));

  return d;
}

int test_main(int, char *[])
{
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
    tuples.begin(), tuples.end(), eps1, min_points, norm2_dist);
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
    tuples.begin(), tuples.end(), eps2, min_points, norm2_dist);

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

  return 0;
}
