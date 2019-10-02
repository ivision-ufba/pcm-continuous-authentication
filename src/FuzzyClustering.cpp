#include "FuzzyClustering.hpp"
#include <iostream>

float 
Fuzzy::calc_dist (const cv::Mat &point, 
                  const cv::Mat &center, 
                  const DistType dist_type) 
{
	float f_dist = 0.f;
	int dimension = point.cols;
	switch (dist_type) 
  {
		case kDistL1: 
    {
			// L1, Manhattan
			for (int d = 0; d < dimension; d++) {
				f_dist += fabs (point.at<float> (0,d) - center.at<float> (0,d));
			}
		}
		break;
		case kDistL2: 
    {
			// L2, Euclid
			for (int d = 0; d < dimension; d++) {
				float t = point.at<float> (0,d) - center.at<float> (0,d);
				f_dist += t * t;
			}
		}
		break;
		case kDistCosine: 
    {
			//cosine distance 
			f_dist = 1.0 - 
					(center.dot(point) / cv::norm(center) / cv::norm(point));
		}
		break;
		default:
			std::cout << "Error while calculating distance for clustering" << std::endl;
		break;
	}
	return f_dist;
}

float 
Fuzzy::getLastTrueMembership() 
{
	return membership_.at<float> (number_points_ - 1, 0);
}

float 
Fuzzy::getLastIntruderMembership() 
{
	return membership_.at<float> (number_points_ - 1, 1);
}

void 
Fuzzy::debugAll() 
{
	cv::Mat lastPoint = rows_.row (number_points_ - 1);

	std::cout << "	last point distance to centroids: ";

	for (unsigned int j = 0; j < number_clusters_; j++) 
  {
		cv::Mat center = centroids_.row (j);
		std::cout << std::fixed << std::setprecision(5) 
              << calc_dist (lastPoint, center, dist_type_) 
              << " ";
	}

	std::cout << std::endl;

	std::cout << "	last point memberships: ";

	for (unsigned int j = 0; j < number_clusters_; j++) 
		std::cout << std::fixed << std::setprecision(5) 
              << membership_.at<float> (number_points_ - 1, j) << " ";
	
  std::cout << std::endl;
	
  std::cout << "	last point distance to points: ";

	for(unsigned int j = 0; j < number_points_ - 1; j++) 
  {
		cv::Mat point = rows_.row (j);
		std::cout << std::fixed << std::setprecision(5) 
              << calc_dist (lastPoint, point, dist_type_) << " ";
	}

	std::cout << std::endl;

	std::cout << "	constants: " << std::fixed << std::setprecision(5) 
            << possibilistic_constant_[0] << " " << possibilistic_constant_[1] 
            << std::endl;

	std::cout << std::endl;
}

void 
Fuzzy::debugDistance() 
{
	cv::Mat lastPoint = rows_.row (number_points_ - 1);
	std::cout << "last point dists: ";
	for (unsigned int j = 0; j < number_clusters_; j++) 
  {
		cv::Mat center = centroids_.row (j);
		std::cout << calc_dist (lastPoint, center, dist_type_) << " ";
	}
	std::cout << std::endl;
}

void 
Fuzzy::debugMembership() 
{
	std::cout << "last point memberships: ";
	for (unsigned int j = 0; j < number_clusters_; j++) 
		std::cout << "membership: " 
              <<  membership_.at<float> (number_points_ - 1, j) << " ";
	
  std::cout << std::endl;
}

void 
Fuzzy::initRandom () 
{
	srand ((unsigned int) time (NULL));
	float normalization_factor;
	for (unsigned int j = 0 ; j < number_points_; j++) 
  {
		normalization_factor = 0.0;
		for (unsigned int i = 0; i < number_clusters_; i++)	
			normalization_factor += 
		membership_.at<float> (j, i) = (rand () / (RAND_MAX + 0.0));
		// Normalization
		for (unsigned int i = 0; i < number_clusters_; i++)
			membership_.at<float> (j, i) /= normalization_factor;
	}
	computeCentroids();
}

void 
Fuzzy::initHeuristic () 
{
	//find the object farest from the login medoid
	cv::Mat center = rows_.row(0);
	float maxDist = calc_dist(center, loginDescriptor_, dist_type_);
	int maxIndex = 0;
	for (unsigned int j = 1; j < number_points_; j++) 
  {
		center = rows_.row(j);
		float dist = calc_dist(center, loginDescriptor_, dist_type_);
		if(dist < maxDist) 
    {
			maxDist = dist;
			maxIndex = j;
		}
	}
	center = rows_.row(maxIndex);
	for (int f = 0; f < dimension_; f++)
		centroids_.at<float> (1, f) = center.at<float> (0, f);
	for (int f = 0; f < dimension_; f++)
		centroids_.at<float> (0, f) = loginDescriptor_.at<float> (0, f);
	if(k_type_ == kPossibilisticCMeans) 
  {
		cv::Mat center1 = centroids_.row (0);
		cv::Mat center2 = centroids_.row (1);
		float distance = calc_dist(center1, center2, dist_type_);
		float new_constant = 0;
		switch (const_type_)
		{
			case kPCM:
				new_constant = 2.0 - distance;
				break;
			case kPCMD2:
				new_constant = 1.0 - distance;
				break;
			case kPCMD3:
				new_constant = 0.666 - distance;
				break;
			case kPCMZT:
				cv::Mat actual = rows_.row(number_points_ - 1);
				float d_zt = calc_dist(center1, actual, dist_type_);
				new_constant = 2.0 - distance - d_zt;
				break;
		}
		if(new_constant <= 0)
			new_constant = 0.001;
		possibilistic_constant_[0] = new_constant;
		possibilistic_constant_[1] = distance;
	}
	updateMembership ();
	computeCentroids();
}

void 
Fuzzy::initEverything () 
{
	switch (init_type_) 
  {
		case kInitRandom:
			initRandom ();
		break;
		case kInitHeuristic:
			initHeuristic ();
		break;
	}
}

void 
Fuzzy::computeCentroids () 
{
	cv::Mat u_ji_m  = cv::Mat::zeros (number_points_, number_clusters_, CV_32FC1);
	//　Initialization
	for (unsigned int j = 1; j < number_clusters_; j++)
		for (int f = 0; f < dimension_; f++) 
			centroids_.at<float> (j, f) = 0.0;
	// weight ** fuzziness
	for (unsigned int j = 1; j < number_clusters_; j++)
		for (unsigned int i = 0 ; i < number_points_; i++) 
			u_ji_m.at<float> (i, j) = pow ( membership_.at<float> (i, j), fuzziness_);
	// Update centroids
	for (unsigned int j = 1; j < number_clusters_; j++)
		for (unsigned int i = 0 ; i < number_points_; i++)	
			for (int f = 0; f < dimension_; f++)
				centroids_.at<float> (j, f) += u_ji_m.at<float> (i, j) * rows_.at<float> (i, f);
	// Normalization
	float normalization;
	for (unsigned int j = 1; j < number_clusters_; j++)
  {
		normalization = 0.0;
		for (unsigned int i = 0 ; i < number_points_; i++) 
			normalization += u_ji_m.at<float> (i, j);
		for (int f = 0; f < dimension_; f++)
			centroids_.at<float> (j, f) /= normalization;
	}
  if(k_type_ == kPossibilisticCMeans) 
  {
		cv::Mat center1 = centroids_.row (0);
		cv::Mat center2 = centroids_.row (1);
		float distance = calc_dist(center1, center2, dist_type_);
		float new_constant = 0;
		switch (const_type_)
		{
			case kPCM:
				new_constant = 2.0 - distance;
				break;
			case kPCMD2:
				new_constant = 1.0 - distance;
				break;
			case kPCMD3:
				new_constant = 0.666 - distance;
				break;
			case kPCMZT:
				cv::Mat actual = rows_.row(number_points_ - 1);
				float d_zt = calc_dist(center1, actual, dist_type_);
				new_constant = 2.0 - distance - d_zt;
				break;
		}
		if(new_constant <= 0)
			new_constant = 0.001;
		possibilistic_constant_[0] = new_constant;
		possibilistic_constant_[1] = distance;
  }
}

bool 
Fuzzy::updateMembership () 
{
	switch(k_type_) 
  {
		case kPossibilisticCMeans: 
			for (unsigned int i = 0 ; i < number_points_; i++) 
      {
				cv::Mat point = rows_.row (i);
				for (unsigned int j = 0; j < number_clusters_; j++) 
        {
					cv::Mat center = centroids_.row (j);
					float factor = 
            calc_dist(point, center, dist_type_) / possibilistic_constant_[j];
					new_membership_.at<float> (i, j) = 
            1 / (1 + pow(factor, 1 / (fuzziness_ - 1)));
				}
			}
			if (!can_stop() )
      {
				membership_ = new_membership_.clone ();
				return false; 
			}
			return true;
		break;
		case kFuzzyCMeans:
			cv::Mat matrix_norm_one_xi_minus_cj = 
        cv::Mat::zeros (number_clusters_, number_points_, CV_32FC1);
			// Initialization
			for (unsigned int i = 0 ; i < number_points_; i++)
				for (unsigned int j = 0; j < number_clusters_; j++) 
					matrix_norm_one_xi_minus_cj.at<float> (j, i) = 0.0;

			for (unsigned int i = 0 ; i < number_points_; i++) 
      {
				// Calculate distances from each cluster.
				cv::Mat point = rows_.row (i);
				for (unsigned int j = 0; j < number_clusters_; j++) 
        {
					cv::Mat center = centroids_.row (j);
					matrix_norm_one_xi_minus_cj.at<float> (j, i) =
            calc_dist (point, center, dist_type_);
				}
			}

			float coeff;
			for (unsigned int i = 0 ; i < number_points_; i++) 
      {
				for (unsigned int j = 0; j < number_clusters_; j++)
         {
					coeff = 0.0;
					for (unsigned int k = 0; k < number_clusters_; k++) 
          {
						coeff += 
              pow ( (matrix_norm_one_xi_minus_cj.at<float> (j, i) /
                matrix_norm_one_xi_minus_cj.at<float> (k, i)) , 
              2.0 / (fuzziness_ - 1.0) );
					}
					new_membership_.at<float> (i, j) = 1.0 / coeff;
				}
			}
			if (!can_stop() )
      {
				membership_ = new_membership_.clone ();
				return false; 
			}
			return true;
		break;
	}
}
