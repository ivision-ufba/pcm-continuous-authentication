# Continuous Authentication using Possibilistic C-Means

### Requeriments
* OpenCV 3.3.0+
* matplotlib
* sklearn
* numpy
* R-base (optional)

## Reproducing ECG experiments

Just `python3 run_experiment_pcm_ecg.py` for PCM and `python3
run_experiment_baseline_ecg.py`

To change the variant of PCM, just change `PCM_TYPE` on
`run_experiment_baseline_ecg.py`

## Hot to adapt to other biometrics
1. Create a new function that returns an `std::vector<Mat>` of descriptors for
   each subject. In other words, you need to replace the `loadEcgSubject`
   function in `utils.cpp` to your function.
2. Change the value of `DESCRIPTOR_SIZE` in `baseline_ecg.cpp` and `pcm_ecg.cpp`
   for the size of your descriptors.
3. Change `NUMBER_SUBJECTS` in`run_experiment_pcm_ecg.py` and
   `run_experiment_baseline_ecg.py` to your number of subjects

If you want to use `plot_roc_ecg.R`, you need to adjust the file's path inside
it. In general, the run experiment script will generate a `method.psafe` file
with all psafe values. The `validate.out` will compile this file in a ROC curve
values `.roc`, so, in the end, you can plot the roc curves using this `.roc`
files.
