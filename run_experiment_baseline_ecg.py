import os
NUMBER_SUBJECTS = 45

for i in range(NUMBER_SUBJECTS):
    args = ''
    for j in range(NUMBER_SUBJECTS):
        if i != j:
            args += 'ecgs/'+str(j)+' '
    os.system('./baseline_ecg.out ecgs/'+str(i)+' '+args)
for i in range(NUMBER_SUBJECTS):
    args = ''
    for j in range(NUMBER_SUBJECTS):
        if i != j:
            args += str(i)+'_'+str(j)+'.psafe '
    os.system('python3 plot_psafe.py '+str(i)+'.psafe '+args+' baseline_'+str(i)+'.png')  
    
os.system('cat *.psafe > baseline.psafe')
os.system('python3 calculate_eer.py baseline.psafe')
os.system('./validate.out baseline.psafe > baseline.roc')