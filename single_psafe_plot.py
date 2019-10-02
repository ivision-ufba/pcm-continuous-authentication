import os
NUMBER_SUBJECTS = 45

i = 15
args = ''
for j in range(NUMBER_SUBJECTS):
    if i != j:
        args += 'results/psafes/pcm_d3_ecg/'+str(i)+'_'+str(j)+'.psafe '
os.system('python3 plot_psafe.py results/psafes/pcm_d3_ecg/'+str(i)+'.psafe '+args+' '+str(i)+'.png')   