/**
 * @file FuzzyClustering.hpp
 * @author https://github.com/shingt/FuzzyCMeans
 * @brief Modified version of PCM
 * @version 0.1
 * @date 2019-10-01
 * 
 * @copyright Copyright (c) 2014 Shinichi Goto All rights reserved.
 * 
 */
#ifndef FUZZY_CLUSTERING
#define FUZZY_CLUSTERING
#include <iostream>
#include <opencv2/core/core.hpp>
#include <bits/stdc++.h>

#include <stdlib.h>

// Weigh initialization type
typedef enum
{
  kInitRandom,
  kInitHeuristic,
} InitType;
// Distance type
typedef enum
{
  kDistL1,
  kDistL2,
  kDistCosine
} DistType;
// Fuzzy algorithm type
typedef enum
{
  kFuzzyCMeans,
  kPossibilisticCMeans
} ClusteringType;
// PCM Variant
typedef enum
{
  kPCM = 0,
  kPCMD2 = 1,
  kPCMD3 = 2,
  kPCMZT = 3
} ConstantsType;

class Fuzzy
{
public:
  Fuzzy(
      const cv::Mat &rows,
      const cv::Mat &loginDescriptor,
      const int number_clusters,
      const float fuzziness,
      const float epsilon,
      const DistType dist_type,
      const InitType init_type,
      const ClusteringType k_type,
      const ConstantsType const_type)
      :
      rows_(rows),
      loginDescriptor_(loginDescriptor),
      number_clusters_(number_clusters),
      fuzziness_(fuzziness),
      epsilon_(epsilon),
      dist_type_(dist_type),
      init_type_(init_type),
      k_type_(k_type),
      const_type_(const_type),
      number_points_(rows.rows),
      dimension_(rows.cols)
  {
    centroids_ = cv::Mat::zeros(number_clusters_, dimension_, CV_32FC1);
    membership_ = cv::Mat::zeros(number_points_, number_clusters_, CV_32FC1);
    new_membership_ = cv::Mat::zeros(number_points_, number_clusters_, CV_32FC1);
    for (unsigned int j = 0; j < number_clusters_; j++)
      possibilistic_constant_.push_back(0.0);
    initEverything();
  };

  void initEverything();
  void initRandom();
  void initHeuristic();

  void computeCentroids();
  bool updateMembership();

  void debugDistance();
  void debugMembership();
  void debugAll();
  float getLastTrueMembership();
  float getLastIntruderMembership();

  float calc_dist(
      const cv::Mat &point,
      const cv::Mat &center,
      const DistType dist_type);

  inline const bool can_stop()
  {
    float t = cv::norm(membership_ - new_membership_);
    return t < epsilon_;
  }

  inline void clustering(const unsigned int num_iteration = 10000)
  {
    unsigned int iteration = 0;

    while (!updateMembership() && iteration++ < num_iteration)
    {
      computeCentroids();
    }
  }

  inline void onlineClustering(const cv::Mat &rows, 
                               const unsigned int num_iteration = 10000)
  {
    rows_ = rows.clone();
    clustering(num_iteration);
  }

  inline const cv::Mat get_centroids_() { return centroids_; }
  inline const cv::Mat get_membership_() { return membership_; }
  inline const cv::Mat get_new_membership_() { return new_membership_; }
  inline const cv::Mat get_loginDescriptor_() { return loginDescriptor_; }

private:
  cv::Mat rows_;
  cv::Mat loginDescriptor_;

  unsigned int number_clusters_;
  float fuzziness_;
  float epsilon_;

  DistType dist_type_;
  InitType init_type_;
  ClusteringType k_type_;
  ConstantsType const_type_;

  cv::Mat centroids_;
  cv::Mat membership_;
  cv::Mat new_membership_;

  unsigned int number_points_;
  int dimension_;

  std::vector<float> possibilistic_constant_;
};
#endif
