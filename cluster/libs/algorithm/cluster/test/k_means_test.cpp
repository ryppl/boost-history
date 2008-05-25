// Boost.Algorithm.Cluster Library
// Copyright Yuanming Chen 2008. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#include <boost/test/unit_test.hpp>
#define BOOST_TEST_MODULE ClusterLib

#include <boost/algorithm/cluster/k_means.hpp>
#include <boost/array.hpp>
#include <iostream>
#include <fstream>
#include <string> 
using namespace boost::algorithm::cluster;

BOOST_AUTO_TEST_CASE( basic_k_means_test )
{
   //The example can be found at:
    //http://people.revoledu.com/kardi/tutorial/kMean/NumericalExample.htm

    //Attribute 1: weight index.
    //Attribute 2: pH value
    //"POINT" represents a data point in the multi-dimensional data base
    typedef boost::array<double, 2> POINT;
    ////////////////////////////////////////////////////////////////////////
    //Four drugs. We need to get two clusters
    std::vector<POINT> vecPoints;

    POINT medicineA;
    medicineA[0] = 1;
    medicineA[1] = 1;
    vecPoints.push_back(medicineA);

    POINT medicineB;
    medicineB[0] = 2;
    medicineB[1] = 1;
    vecPoints.push_back(medicineB);

    POINT medicineC;
    medicineC[0] = 4;
    medicineC[1] = 3;
    vecPoints.push_back(medicineC);

    POINT medicineD;
    medicineD[0] = 5;
    medicineD[1] = 4;
    vecPoints.push_back(medicineD);

    const int k = 2; //2 groups of medicine

    typedef KMeansCluster<POINT> ClusterType;
    typedef KMeansClustering<ClusterType> ClusteringType;
    size_t knDimension = POINT::size();

    ClusteringType result = k_means(vecPoints.begin(), vecPoints.end(), k, 0.00001);
    int nCentroidId = 0;
    for(ClusteringType::type::const_iterator iter = result.clusters.begin();
        iter != result.clusters.end();
        iter++, nCentroidId++)
    {
        const ClusterType& cluster = *iter;
        std::cout << "Cluster " << nCentroidId << std::endl;
        std::cout << "Centroid point (";

        for(unsigned int nAttribute = 0; nAttribute < knDimension; nAttribute++)
        {
            std::cout << cluster.centroid[nAttribute];
            if(nAttribute != knDimension - 1)
            {
                std::cout << ", ";
            }
        }
        std::cout << ")" << std::endl;
        std::cout << "Number of points: " << cluster.points.size() << std::endl;

        for(std::vector<int>::const_iterator iter = cluster.points.begin();
            iter != cluster.points.end();
            iter++)
        {
            std::cout << *iter << std::endl;
        }
        std::cout << std::endl << std::endl;
    }
    double centroid0_x = result.clusters[0].centroid[0];
    double centroid0_y = result.clusters[0].centroid[1];
    BOOST_CHECK_EQUAL( centroid0_x, 1.5 );
    BOOST_CHECK_EQUAL( centroid0_y, 1 );


    double centroid1_x = result.clusters[1].centroid[0];
    double centroid1_y = result.clusters[1].centroid[1];
    BOOST_CHECK_EQUAL( centroid1_x, 4.5 );
    BOOST_CHECK_EQUAL( centroid1_y, 3.5 );
}
