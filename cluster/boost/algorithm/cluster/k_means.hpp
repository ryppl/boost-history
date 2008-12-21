/*
 * References
 * - J. MacQueen, "Some methods for classification and analysis
 *   of multivariate observations", Fifth Berkeley Symposium on
 *   Math Statistics and Probability, 281-297, 1967.
 * - I.S. Dhillon and D.S. Modha, "A data-clustering algorithm
 *   on distributed memory multiprocessors",
 *   Large-Scale Parallel Data Mining, 245-260, 1999.
 * Yuanming Chen, 2008-05-08
 */

#if ! defined(BOOST_ALGORITHM_CLUSTER_K_MEANS_HPP)
#define BOOST_ALGORITHM_CLUSTER_K_MEANS_HPP

#include <boost/algorithm/cluster/detail/k_means.hpp>

#include <cassert>
#include <cmath>
#include <float.h>
#include <list>
#include <vector>

namespace boost
{
namespace algorithm
{
namespace cluster
{

template<typename PointType>
struct KMeansCluster {
  PointType centroid;
  std::vector<int> points; //The indice of points are stored here 
};

template <typename KMeansCluster> 
struct KMeansClustering { 
  typedef std::vector< KMeansCluster > type; 
  type clusters;
};

/** 
 * @param first Iterator for the first data point.
 * @param last Iterator for the last data point.
 * @param k The number of clusters.
 * @param eps The epsilon value for the k-means algorithm.
 * @return A clustering of the input data.
 */
template <typename NTupleIter>
KMeansClustering<KMeansCluster<typename NTupleIter::value_type> >
k_means(NTupleIter first,
        NTupleIter last,
        unsigned k, 
        typename NTupleIter::difference_type const & eps)
{
  typedef typename NTupleIter::difference_type DistanceType;
  typedef typename NTupleIter::value_type PointType;
  // For the c funtion test, AttributeType will be a double.
  typedef typename PointType::value_type AttributeType;
  // knumOfPoints is the n variable in the C function.
  const DistanceType knumOfPoints = last - first;
  // knDimension is the m variable in the C function
  const std::size_t knDimension = PointType::size();

  AttributeType** ppData = new AttributeType* [knumOfPoints];
  AttributeType** centroids = new AttributeType* [k]; 
  // Pre-allocate the result array.
  for(std::size_t nCentroid = 0; nCentroid < k; nCentroid++)
  {
    centroids[nCentroid] = new AttributeType[knDimension];
  }

  int nIndex = 0;
  for(NTupleIter iter = first; iter != last; iter++, nIndex++)
  {
    PointType& pt= *iter; //A point TODO: Make this const?
    ppData[nIndex] = new AttributeType[knDimension];
    for(unsigned int nAttribute = 0; nAttribute < knDimension; nAttribute++)
    {
      ppData[nIndex][nAttribute] = pt[nAttribute];
    }
  }

  int* labels = detail::k_means(ppData,
                                (int) knumOfPoints,
                                (int) knDimension,
                                k,
                                eps,
                                centroids);

  typedef KMeansCluster<PointType> KMeansClusterType;
  KMeansClustering< KMeansClusterType > clustering;
  for(std::size_t nCentroid = 0; nCentroid < k; nCentroid++)
  {
    
    KMeansClusterType cluster;
    PointType centroid;
    for(unsigned int nAttribute = 0; nAttribute < knDimension; nAttribute++)
    {
      centroid[nAttribute] = centroids[nCentroid][nAttribute];
    }
    cluster.centroid = centroid;
    clustering.clusters.push_back(cluster);
    delete[] centroids[nCentroid];
  }

  for(int nPoint = 0; nPoint < knumOfPoints; nPoint++)
  {
    int nCentroidIndex = labels[nPoint];
    clustering.clusters[nCentroidIndex].points.push_back(nPoint);
    delete[] ppData[nPoint];
  }

  delete[] centroids;
  delete[] ppData;
  delete[] labels;

  return clustering;
} // k_means

} //End of cluster namespace
} //End of algorithm namespace
} //End of boost namespace

#endif
