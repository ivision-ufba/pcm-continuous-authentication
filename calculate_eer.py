import numpy as np
import sys
from sklearn import metrics

if len(sys.argv) < 2:
    print("usage: <filePSafe>")
    sys.exit(0)

sfile = open(sys.argv[1], 'r')
y_true = []
y_score = []

for line in sfile.readlines():
    handle = line.split(" ")
    if handle[0] == handle[1]:
        y_true.append(0)
    else:
        y_true.append(1)
    y_score.append(float(handle[2]))


fpr, tpr, __ = metrics.roc_curve(y_true, y_score,)
fnr = 1 - tpr
eer = fpr[np.nanargmin(np.absolute((fnr - fpr)))]
print('ERR: {}'.format(1-eer))