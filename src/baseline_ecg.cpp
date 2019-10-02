#include "TemporalIntegration.hpp"
#include "utils.hpp"
#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#define SQRT_2 1.4142135623730951
#define DESCRIPTOR_SIZE 84

// CDF parameters
double INTRA_MEAN = 0.0997, INTRA_STDDEV = 0.1180, 
       INTER_MEAN = 0.5550, INTER_STDDEV = 0.1722;

float calculateDistance(const cv::Mat &point, const cv::Mat &center) 
{
	return 1.0 - center.dot(point) / cv::norm(center) / cv::norm(point);
}

float cdfKSafe(const float & distance) 
{
	return 1.0 - (1.0 + std::erf((distance - INTRA_MEAN) / (INTRA_STDDEV * SQRT_2))) / 2;
}

float cdfKNotSafe(const float & distance) 
{
	return (1.0 + std::erf((distance - INTER_MEAN) / (INTER_STDDEV * SQRT_2))) / 2;
}

int main(int argc, char const *argv[])
{
	TemporalIntegrator continuousAuthenticator;
	ofstream outputFileGenuine, outputFileImpostor;

	long now = 0, lastObservation = 0;
	int frameCounter = 1;

	vector<string> arguments(argv + 1, argv + argc);
	// create genuine file output
	string nameGenuine = arguments[0];
	vector<string> filePathGenuineOutput = splitString(nameGenuine, '/');
	outputFileGenuine.open(
    filePathGenuineOutput[filePathGenuineOutput.size()-1]+".psafe", 
    fstream::out);
  // load all descriptors from allowed user
	vector<Mat> loginSamples;
	loadEcgSubject(nameGenuine, loginSamples);
	// login sample
	Mat loginDescriptor = loginSamples[0];
	lastObservation = frameCounter * (1000/ECGS);
	continuousAuthenticator.setLastObservation(lastObservation);
	//auth cont for allowed user
	Mat biometricSample = Mat(1, DESCRIPTOR_SIZE, CV_32F);
	int len = loginSamples.size();
	for(int i = 1; i < len; i++) 
  {
		biometricSample = loginSamples[i];
		frameCounter++;
		now = frameCounter * (1000/ECGS);
		continuousAuthenticator.setNow(now);
		float distance = calculateDistance(biometricSample, loginDescriptor);
		outputFileGenuine 
      << "1 1 " 
      << continuousAuthenticator.calculatePsafe(cdfKSafe(distance), 
                                                cdfKNotSafe(distance)) 
      << endl;
		continuousAuthenticator.setLastObservation(now);
	}
	//auth cont for intruders
	double copyPSafe = continuousAuthenticator.getPSafe(), 
         copyPNotSafe = continuousAuthenticator.getPNotSafe();
	for(uint c = 1; c < arguments.size(); c++) 
  {
		string nameImpostor = arguments[c];
		vector<string> filePathImpostorOutput = splitString(nameImpostor, '/');
		outputFileImpostor.open(
      filePathGenuineOutput[filePathGenuineOutput.size()-1]
      +"_"
      +filePathImpostorOutput[filePathImpostorOutput.size()-1]
      +".psafe", 
      fstream::out);
		vector<Mat> intruderSamples;
		loadEcgSubject(nameImpostor, intruderSamples);
		int len = intruderSamples.size();
		for(int i = 0; i < len; i++) 
    { 
			biometricSample = intruderSamples[i];
			frameCounter++;
			now = frameCounter * (1000/ECGS);
			continuousAuthenticator.setNow(now);
			float distance = calculateDistance(biometricSample, loginDescriptor);
			outputFileImpostor 
        << "1 0 " 
        << continuousAuthenticator.calculatePsafe(cdfKSafe(distance), 
                                                  cdfKNotSafe(distance)) 
        << endl;
			continuousAuthenticator.setLastObservation(now);
		}
		continuousAuthenticator.setPSafe(copyPSafe);
		continuousAuthenticator.setPNotSafe(copyPNotSafe);
		outputFileImpostor.close();
	}
	return 0;
}
