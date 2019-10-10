# Continuous Authentication using Possibilistic C-Means
Implementation of the paper https://ieeexplore.ieee.org/document/8491508 using
ECG biometric feature.

### Requeriments
* OpenCV 3.3.0+
* matplotlib
* sklearn
* numpy
* R-base (optional)

## Contents
```yml
-- ecgs: Database with 45 records. Each record contains the descriptors of each 
heartbeat. The size of the descriptors is 84;

-- include: 
  -- FuzzyClustering.hpp: Fuzzy class declaration which contains the PCM;
  -- TemporalIntegration.hpp: TemporalIntegrator class declaration which handles
     with the temporal integration stuff;
  -- utils.hpp: auxiliary functions declaration;

--src:
  -- baseline_ecg.cpp: runs CDF continuous authentication for one genuine access
     followed by one or more impostor access. It saves the results in different
     files. The genuine file is "genuineFileName.psafe" and each impostor is 
     "genuineFileName_impostorFileName.psafe". Each row in the files is on the
     format "X Y P", where X = 1, Y = 1 for genuine access, Y = 0 for impostor
     access, and P is the Psafe value;
  -- FuzzyClustering.cpp: Fuzzy class implementation
  -- pcm_ecg.cpp: runs PCM continuous authentication for one genuine access
     followed by one or more impostor access. It saves the results in different
     files. The genuine file is "genuineFileName.psafe" and each impostor is 
     "genuineFileName_impostorFileName.psafe". Each row in the files is on the
     format "X Y P", where X = 1, Y = 1 for genuine access, Y = 0 for impostor
     access, and P is the Psafe value;
  -- TemporalIntegration.cpp: TemporalIntegrator class implementation;
  -- utils.cpp: auxiliary functions implementation;
  -- validate.cpp: reads a file in the format of the output from pcm_ecg or 
     baseline_ecg, and outputs the FAR and 1 - FRR values. The output can be 
     used for generate ROC curves or calculate EER;  

Makefile: compilation macros;

plot_psafe.py: Plot psafe values. The first file passed as argument is the 
genuine access and the others the impostors. It reads the output from the 
baseline_ecg and pcm_ecg;

plot_roc_ecg.R: Plot the ROC curve. It uses the outputs from validate. The file
names are inside the code (e.g. "results/roc/pcm_ecg.roc"), but you can change;

run_experiment_baseline_ecg.py: runs baseline_ecg for all subjects in ecgs/, 
generates the psafe plots and calculates the EER;

run_experiment_pcm_ecg.py: runs pcm_ecg for all subjects in ecgs/, generates the
psafe plots and calculates the EER;

single_psafe_plot.py: hard-coded plot for psafe values from one subject. The i
variable defines which subject will be plotted. But requires the psafe values in
the folder "results/psafes/pcm_d3_ecg/". You can change.

```
## Running ECG experiments
If you have the minimum requirements:
```bash
mkdir build
make all
python3 run_experiment_pcm_ecg.py
python3 run_experiment_baseline_ecg.py
```

To change the variant of PCM, just change `PCM_TYPE` on
`run_experiment_baseline_ecg.py`

## How to adapt to other biometric features

#### Loading the data
1. Create a new function that returns an `std::vector<Mat>` of descriptors for
   each subject. In other words, you need to replace the `loadEcgSubject`
   function in `utils.cpp` to your function.
2. Change the value of `DESCRIPTOR_SIZE` in `baseline_ecg.cpp` and `pcm_ecg.cpp`
   for the size of your descriptors.
3. Change `NUMBER_SUBJECTS` in`run_experiment_pcm_ecg.py` and
   `run_experiment_baseline_ecg.py` to your number of subjects

#### Using timestamp instead of fix rate
If you want to use the timestamps from your samples, you need to load it for
each sample. So, just set the values of `now` and `lastObservation` for the
respective timestamps.

#### Plotting
If you want to use `plot_roc_ecg.R`, you need to adjust the file's path inside
it. In general, the `run_experiment` script will generate a `methodName.psafe`
file with all psafe values. The `validate.out` will compile this file in a ROC
curve values `.roc`, so, in the end, you can plot the roc curves using this
`.roc` files.

#### Custom parameters:
That is the list of parameters that can be customized or recalculated in a 
different way:

Fuzzy:
```yml
possibilistic_constant_[0]: eta genuine value
possibilistic_constant_[1]: eta impostor value
fuzziness_: fuzziness value
number_clusters_: number of clusters
epsilon_: epsilon value
DistType: the distance used can be L1, L2 or Cosine distance
ClusteringType: can be PCM or FCM clustering
InitType: the type of initialization, can be random of using a heuristic
```

TemporalIntegrator:
```yml
K_DROP: K constant that defines the decay rate in absence of observations (in ms)
```

utils:
```yml
ECGS: how many ECGs per second. If you want use other biometric feature, just
rename and set a value of samples per second.
```

baseline_ecg:
```yml
INTRA_MEAN: mean of the distances of the genuine samples
INTER_MEAN: mean of the distances of the impostor samples
INTRA_STDDEV: standard deviance of the distances of the genuine samples
INTER_STDDEV: standard deviance of the distances of the genuine samples
```

pcm_ecg:
```yml
MAX_HISTORIC: the size of the samples historic
NUMBER_CLUSTERS: number of clusters passed to Fuzzy object
FUZZINESS: fuzziness value passed to Fuzzy object
EPSILON: epsilon value passed to Fuzzy object
DIST_TYPE: the distance used by the Fuzzy object
INIT_TYPE: the type of initialization passed to Fuzzy object
K_TYPE: clustering type of the Fuzzy object
MAX_ITERATIONS: the maximum number of iterations of a historic cluster
```