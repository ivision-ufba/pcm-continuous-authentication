#include "utils.hpp"

Mat 
parseDataset(const deque<Mat> & dequeMats) 
{
	Mat dataset;
	for (uint i = 0; i < dequeMats.size(); ++i) 
		dataset.push_back(dequeMats[i]);
	return dataset;
}

vector<string> 
splitString(const string &s, char delim) 
{
    stringstream ss(s);
    string item;
    vector<string> tokens;
    while (getline(ss, item, delim)) 
        tokens.push_back(item);
    
    return tokens;
}

void 
loadEcgSubject(const string & pathFileSubject, vector<Mat> & samples) 
{
	ifstream fileSubject(pathFileSubject);
	float value;
	Mat v = Mat(1, 84, CV_32F);
	int counter = 0;
	while(fileSubject >> value) 
  {
		if(counter < 84) 
    {
			v.at<float>(0,counter) = value;
      counter++;
		}
		else 
    {
			samples.push_back(v);
			v = Mat(1, 84, CV_32F);
			counter = 0;
			v.at<float>(0,counter) = value;
      counter++;
		}
	}
}