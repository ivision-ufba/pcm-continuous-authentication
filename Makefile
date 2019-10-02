CXX = g++

CXXFLAGS = -Wall -std=c++14 -O3 -ffast-math

LDFLAGS = `pkg-config --libs opencv`

INCLUDE = `pkg-config --cflags opencv` -I $(CURDIR)/include

all: pcm_ecg.out baseline_ecg.out validate.out

build/utils.o: src/utils.cpp include/utils.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDE)

build/TemporalIntegration.o: src/TemporalIntegration.cpp include/TemporalIntegration.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDE)

build/FuzzyClustering.o: src/FuzzyClustering.cpp include/FuzzyClustering.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDE)

build/pcm_ecg.o: src/pcm_ecg.cpp build/FuzzyClustering.o build/TemporalIntegration.o include/utils.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDE)

pcm_ecg.out: build/pcm_ecg.o build/utils.o build/FuzzyClustering.o build/TemporalIntegration.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

build/baseline_ecg.o: src/baseline_ecg.cpp build/utils.o build/TemporalIntegration.o
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDE)

baseline_ecg.out: build/baseline_ecg.o build/utils.o build/TemporalIntegration.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

validate.out: src/validate.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	@rm -f build/*.o *.out