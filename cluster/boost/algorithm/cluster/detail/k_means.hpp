#if ! defined(BOOST_ALGORITHM_CLUSTER_K_MEANS_HPP)
#define BOOST_ALGORITHM_CLUSTER_K_MEANS_HPP

namespace boost
{
namespace algorithm
{
namespace cluster
{
namespace detail
{

// The original C function
template<typename AttributeType, typename differenceType>
int *k_means(AttributeType **data,
             int n,
             int m,
             int k,
             differenceType eps,
             AttributeType **centroids)
{
  // Output cluster label for each data point.
  int *labels = (int*)calloc(n, sizeof(int));

  int h, i, j; // loop counters, of course :) 
  // size of each cluster.
  int *counts = (int*)calloc(k, sizeof(int));
  // sum of squared euclidean distance.
  AttributeType old_error, error = FLT_MAX;
  AttributeType **c = (centroids) ?
    centroids :
    (AttributeType**)calloc(k, sizeof(AttributeType*));
  // temp centroids.
  AttributeType **c1 = (AttributeType**)calloc(k, sizeof(AttributeType*));

  //assert(data && k > 0 && k <= n && m > 0 && t >= 0); // for debugging.

  // initialization
  for (h = i = 0; i < k; h += n / k, i++)
  {
    c1[i] = (AttributeType*)calloc(m, sizeof(AttributeType));
    if (!centroids)
    {
      c[i] = (AttributeType*)calloc(m, sizeof(AttributeType));
    }
    // pick k points as initial centroids.
    for (j = m; j-- > 0; c[i][j] = data[h][j]);
  }

  // main loop
  do
  {
    // save error from last step.
    old_error = error, error = 0;

    // clear old counts and temp centroids.
    for (i = 0; i < k; counts[i++] = 0)
    {
      for (j = 0; j < m; c1[i][j++] = 0);
    }

    for (h = 0; h < n; h++)
    {
      // identify the closest cluster.
      AttributeType min_distance = FLT_MAX;
      for (i = 0; i < k; i++)
      {
        AttributeType distance = 0;
        for (j = m; j-- > 0; distance += pow(data[h][j] - c[i][j], 2));
        if (distance < min_distance)
        {
          labels[h] = i;
          min_distance = distance;
        }
      }
      // update size and temp centroid of the destination cluster.
      for (j = m; j-- > 0; c1[labels[h]][j] += data[h][j]);
      counts[labels[h]]++;
      // update standard error.
      error += min_distance;
    }

    for (i = 0; i < k; i++)
    {
      // update all centroids.
      for (j = 0; j < m; j++)
      {
        c[i][j] = (counts[i]) ? c1[i][j] / counts[i] : c1[i][j];
      }
    }

  } while (fabs(error - old_error) > eps);

  // housekeeping.
  for (i = 0; i < k; ++i)
  {
    if (! centroids)
      free(c[i]);
    free(c1[i]);
  }

  if (! centroids)
    free(c);
  free(c1);

  free(counts);

  return labels;
} // k_means

} // End of namespace detail.
} // End of namespace cluster.
} // End of namespace algorithm.
} // End of namespace boost.

#endif
