#include <fstream>
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <algorithm>
#include <functional>

using namespace std;

std::pair<std::vector<double>, std::vector<double>> read_comparison(const std::string & path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error: unable to open file " << path << std::endl;
        exit(1);
    }
    
    std::pair<std::vector<double>, std::vector<double>> ret;
    int id1, id2;
    double sim;
    while (file >> id1 >> id2 >> sim) {
        if (id1 == id2)
            ret.first.push_back(sim);
        else
            ret.second.push_back(sim);
    }
    return ret;
}

std::pair<std::vector<double>, std::vector<double>> compute_scores(std::vector<double> & genuine, std::vector<double> & impostor) {
    std::vector<double> far, trr;
    
    std::sort(genuine.begin(), genuine.end());
    std::sort(impostor.begin(), impostor.end());
    
    const int gen_size = genuine.size();
    const int imp_size = impostor.size();
    int gen = -1;
    for (int imp = 0; imp < imp_size; imp++) {
        while (gen + 1 < gen_size && genuine[gen + 1] <= impostor[imp])
            gen++;
        far.push_back((double) (imp + 1) / imp_size);
        trr.push_back((double) (gen + 1) / gen_size);
    }
    // int n = far.size();
    // for(int i = 1; i < n; i++) {
    //     if(far[i-1] <= 1 - trr[i-1] && far[i] >= 1 - trr[i])
    //         std::cout << far[i] << " " << impostor[i] << std::endl; 
    // } 
    return make_pair(far, trr);
}

int main(int argc, char ** argv) {
    if (argc <= 1) {
        std::cerr << "Error: pass path to comparison output file as parameter" << std::endl;
        return 1;
    }
    
    std::vector<double> genuine_scores, impostor_scores;
    std::tie(genuine_scores, impostor_scores) = read_comparison(std::string(argv[1]));
    
    std::vector<double> far, trr;
    std::tie(far, trr) = compute_scores(genuine_scores, impostor_scores);
    for (int i = 0, len = far.size(); i < len; i++)
        std::cout << far[i] << " " << trr[i] << std::endl;
}
