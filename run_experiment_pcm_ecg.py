import os
NUMBER_SUBJECTS = 45
''' PCM Types:
0 = PCM 
1 = PCM D2
2 = PCM D3
3 = PCM ZT
'''
PCM_TYPE = 2

for i in range(NUMBER_SUBJECTS):
    args = ''
    for j in range(NUMBER_SUBJECTS):
        if i != j:
            args += 'ecgs/'+str(j)+' '
    os.system('./pcm_ecg.out '+str(PCM_TYPE)+' ecgs/'+str(i)+' '+args)
for i in range(NUMBER_SUBJECTS):
    args = ''
    for j in range(NUMBER_SUBJECTS):
        if i != j:
            args += str(i)+'_'+str(j)+'.psafe '
    os.system('python3 plot_psafe.py '+str(i)+'.psafe '+args+' '+str(i)+'.png')    
    
os.system('cat *.psafe > pcm.psafe')
os.system('python3 calculate_eer.py pcm.psafe')
os.system('./validate.out pcm.psafe > pcm.roc')