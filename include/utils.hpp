#ifndef UTILS 
#define UTILS

#include <opencv2/core/core.hpp>
#include <bits/stdc++.h>

using namespace std;
using namespace cv;

// How many ECGs per second
#define ECGS 1

/**
 * @brief Convert a deque of Mat in a Mat
 * 
 * @param dequeMats 
 * @return Mat 
 */
Mat parseDataset(const deque<Mat> & dequeMats);
/**
 * @brief C++ version of python split string
 * 
 * @param s 
 * @param delim 
 * @return vector<string> 
 */
vector<string> splitString(const string &s, char delim);
/**
 * @brief Load all descritors as a vector of Mats
 * 
 * @param pathFileSubject 
 * @param samples vector of descriptors
 */
void loadEcgSubject(const string & pathFileSubject, vector<Mat> & samples);

#endif