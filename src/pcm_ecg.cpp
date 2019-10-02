#include "FuzzyClustering.hpp"
#include "TemporalIntegration.hpp"
#include "utils.hpp"
#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#define MAX_HISTORIC 10
#define NUMBER_CLUSTERS 2
#define FUZZINESS 1.5
#define EPSILON 0.0001
#define DIST_TYPE kDistCosine
#define INIT_TYPE kInitHeuristic
#define K_TYPE kPossibilisticCMeans
#define MAX_ITERATIONS 100
#define MAX_DEBUG 40

int main(int argc, char const *argv[])
{
	TemporalIntegrator continuousAuthenticator;
	ofstream outputFileGenuine, outputFileImpostor;

	long now = 0, lastObservation = 0;
	int frameCounter = 1;

	vector<string> arguments(argv + 1, argv + argc);
  // PCM variant
	int pcmType = stoi(arguments[0]);
  // create genuine file output
	string nameGenuine = arguments[1];
	vector<string> filePathGenuineOutput = splitString(nameGenuine, '/');
	outputFileGenuine.open(
    filePathGenuineOutput[filePathGenuineOutput.size()-1]+".psafe", 
    fstream::out);
	vector<Mat> loginSamples;
  // load all descriptors from allowed user
	loadEcgSubject(nameGenuine, loginSamples);
	// login sample
	Mat loginDescriptor = loginSamples[0];
	// fill the historic
	deque<Mat> historic, copyHistoric;
	Mat biometricSample = Mat(1, 84, CV_32F);
	while(historic.size() < MAX_HISTORIC - 1) 
  {
		historic.push_back(loginSamples[frameCounter].clone());
		lastObservation = frameCounter * (1000/ECGS);
	}
	continuousAuthenticator.setLastObservation(lastObservation);
	//auth cont for allowed user
	int len = loginSamples.size();
	for(int i = MAX_HISTORIC + 1; i < len; i++) 
  {
		biometricSample = loginSamples[i];
		frameCounter++;
		now = frameCounter * (1000/ECGS);
		continuousAuthenticator.setNow(now);
		historic.push_back(biometricSample.clone());
		//PCM clustering
		Mat dataset = parseDataset(historic);
		Fuzzy fuzzyClustering(dataset, loginDescriptor.clone(), NUMBER_CLUSTERS, 
                          FUZZINESS, EPSILON, DIST_TYPE, INIT_TYPE, K_TYPE, 
                          static_cast<ConstantsType>(pcmType));
		fuzzyClustering.clustering(MAX_ITERATIONS);
		outputFileGenuine 
      << "1 1 " 
      << continuousAuthenticator.calculatePsafe(
          fuzzyClustering.getLastTrueMembership(), 
          fuzzyClustering.getLastIntruderMembership()
          ) 
      << endl;
		continuousAuthenticator.setLastObservation(now);
		historic.pop_front();
	}
	//auth cont for intruders
	double copyPSafe = continuousAuthenticator.getPSafe(), 
         copyPNotSafe = continuousAuthenticator.getPNotSafe();
	copyHistoric = historic;
	for(uint c = 2; c < arguments.size(); c++) {
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
		for(int i = 0; i < len; i++) { 
			biometricSample = intruderSamples[i];
			frameCounter++;
			now = frameCounter * (1000/ECGS);
			continuousAuthenticator.setNow(now);
			historic.push_back(biometricSample.clone());
			//PCM Clustering
			Mat dataset = parseDataset(historic);
			Fuzzy fuzzyClustering(dataset, loginDescriptor.clone(), NUMBER_CLUSTERS, 
                            FUZZINESS, EPSILON, DIST_TYPE, INIT_TYPE, K_TYPE, 
                            static_cast<ConstantsType>(pcmType));
			fuzzyClustering.clustering(MAX_ITERATIONS);
			outputFileImpostor 
        << "1 0 " 
        << continuousAuthenticator.calculatePsafe(
            fuzzyClustering.getLastTrueMembership(), 
            fuzzyClustering.getLastIntruderMembership()
            ) 
        << endl;
			continuousAuthenticator.setLastObservation(now);
			historic.pop_front();
		}
		continuousAuthenticator.setPSafe(copyPSafe);
		continuousAuthenticator.setPNotSafe(copyPNotSafe);
		historic = copyHistoric;
		outputFileImpostor.close();
	}
	return 0;
}
