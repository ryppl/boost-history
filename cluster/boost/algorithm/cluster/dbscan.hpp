//  (C) Copyright Jonathan Franklin 2008.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if ! defined(BOOST_ALGORITHM_CLUSTER_DBSCAN_HPP)
#define BOOST_ALGORITHM_CLUSTER_DBSCAN_HPP

#include <boost/algorithm/cluster/cluster_data.hpp>
#include <boost/algorithm/cluster/concept.hpp>
#include <boost/algorithm/cluster/detail/naive_query.hpp>
#include <boost/utility/result_of.hpp>
#include <vector>

namespace boost
{
namespace algorithm
{
namespace cluster
{

namespace detail
{
// TODO: Where should we put these?
int const UNCLASSIFIED = -1;
int const NOISE = 0;

// TODO: Replace this so we don't have to store the cluster info for each tuple?
template<typename NTupleIterT>
struct node
{
  node(NTupleIterT const & t) : tuple(t), cluster(UNCLASSIFIED) {}

  NTupleIterT tuple;
  int cluster;
};

} // End of namespace detail.

/*! DBSCAN density-based clustering algorithm.
 * TODO: Document this function.
 * \param[in] begin
 * \param[in] end
 * \param[in] eps
 * \param[in] min_points
 * \param[in] d
 * \return The cluster data (partitioning of the tuples).
 */
template<typename ClusterT, typename NTupleIterT,
         typename DistanceT, typename DistFunT>
cluster_data<ClusterT>
dbscan(NTupleIterT const & begin,
       NTupleIterT const & end, 
       DistanceT const & eps,
       std::size_t min_points,
       DistFunT const & d)
{
  // Concept check.
  function_requires<
    DistanceComparableConcept<typename NTupleIterT::value_type, DistFunT> >();
    //DistanceComparableConcept<int, DistFunT> >();
  // TODO: Need to devise a concept check for the return type of DistFunT,
  // such that it is compatible with the difference_type for 2 NTuples.

  // TODO: Rework the algorithm to NOT make this extra collection?
  typedef detail::node<NTupleIterT> node;
  typedef std::vector<node> ntuple_nodes;
  ntuple_nodes tuples;

  // Initialize algorithm.
  //std::size_t num_elems = 0;
  for(NTupleIterT it = begin; it != end; ++it)
  {
    //++num_elems;
    tuples.push_back(node(it));
  }

  typedef cluster_data<ClusterT> cluster_data;
  cluster_data p;

  // TODO: We should try to make cluster_num go away.
  int cluster_num = 0;
  for(typename ntuple_nodes::iterator it = tuples.begin();
      it != tuples.end(); ++it)
  {
    // Skip this tuple if its already been classified as a cluster or noise.
    if (it->cluster != detail::UNCLASSIFIED)
      continue;

    // Expand cluster.

    std::vector<typename ntuple_nodes::iterator> seeds;
    detail::naive_query(it, tuples.begin(), tuples.end(), eps, d, seeds);
    // If the neighborhood of this tuple is too small, then mark it as noise.
    if (seeds.size() < min_points)
    {
      it->cluster = detail::NOISE;
      continue;
    }

    // Start the next cluster.
    ++cluster_num;
    p.push_back(ClusterT()); // TODO: This is goofy.
    ClusterT & cur_cluster = p.back();

    // Mark entire neighborhood as part of the current cluster.
    it->cluster = cluster_num;
    cur_cluster.push_back(it->tuple);
    for (std::size_t n = 0; n < seeds.size(); ++n)
    {
      seeds[n]->cluster = cluster_num;
      cur_cluster.push_back(seeds[n]->tuple);
    }

    // Keep adding seeds and processing them until we find all points that
    // are Density Reachable.
    while (! seeds.empty())
    {
      typename ntuple_nodes::iterator cur = seeds.back();
      seeds.pop_back();

      std::vector<typename ntuple_nodes::iterator> results;
      detail::naive_query(cur, tuples.begin(), tuples.end(), eps, d, results);

      if (results.size() >= min_points)
      {
        for (std::size_t n = 0; n < results.size(); ++n)
        {
          if (results[n]->cluster < 1) // Not assigned to cluster yet.
          {
            if (detail::UNCLASSIFIED == results[n]->cluster)
              seeds.push_back(results[n]);
            results[n]->cluster = cluster_num;
            cur_cluster.push_back(results[n]->tuple);
          }
        }
      }
    }

  } // Outer loop for all tuples.

  return p;
}

} // End of namespace cluster

// TODO: Should we be exporting this?
using namespace cluster;

} // End of namespace algorithm

} // End of namespace boost

#endif
