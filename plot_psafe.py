#!/usr/bin/env python

import sys
import os
from operator import itemgetter
import matplotlib.pyplot as plt
import numpy as np

NUMBER_SUBJECTS = 45

def main(argv=[__name__]):

    if len(sys.argv) < 3:
        print("usage: <filePSafe1> ... <image>")
        sys.exit(0)
        
    psafe_users = []

    ofname = sys.argv[-1]
    f, ext = os.path.splitext(ofname)
    if not IsSupportedImageType(ext):
        print("Format \"%s\" is not supported!" % ext)
        sys.exit(0)

    for i in range(1, len(sys.argv)-1):
        sfile = open(sys.argv[i], 'r')
        psafes = []
        for line in sfile.readlines():
            psafes.append(float(line.split(" ")[2]))
        psafe_users.append(psafes)
    
    DepictPsafeCurve(psafe_users, ofname, 1)


def LoadData(fname):
	sfile = open(fname, 'r')
	sfile.readline()
	tpr = []
	fpr = []
	for line in sfile.readlines():
		fields = line.rstrip().split(' ')
		tpr.append(float(fields[0]))
		fpr.append(float(fields[1]))
	return tpr, fpr


def SetupPsafeCurvePlot(plt):
    plt.xlabel("Heartbeat", fontsize=14)
    plt.ylabel("$P_{safe}$", fontsize=14)
    # plt.title("All Psafe", fontsize=14)


def SavePsafeCurvePlot(plt, fname, randomline=True, bigger=1000):
    plt.xlim(0, bigger)
    plt.ylim(0.0, 1.00)
    plt.legend(fontsize=10, loc='best')
    plt.tight_layout()
    plt.grid(color='gray', linestyle='-', linewidth=0.5)
    plt.savefig(fname)


def AddPsafeCurve(plt, yAsis, xAsis, color):
    xAsis = xAsis[:len(yAsis)]
    plt.plot(xAsis, yAsis, color=color, linewidth=1.0)

def AddPsafeLogin(plt, yAsis, xAsis):
    xAsis = xAsis[:len(yAsis)]
    plt.plot(xAsis, yAsis, color="blue", linewidth=2)


def DepictPsafeCurve(psafe_users, fname, randomline=True):
    fig = plt.figure(figsize=(15, 4), dpi=80)
    login_axis = []
    intruder_axis = []
    frame_max = 0
    len_usr = len(psafe_users[0])
    for i in range(1, len(psafe_users)):
        if len(psafe_users[i]) > frame_max:
            frame_max = len(psafe_users[i])
    for i in range(0,len_usr):
    	login_axis.append(i)
    for i in range(len_usr, len_usr + frame_max):
        intruder_axis.append(i)

    SetupPsafeCurvePlot(plt)

    AddPsafeLogin(plt, psafe_users[0], login_axis)
    x = [len(psafe_users[0]), len(psafe_users[0])]
    y = [0.0, 1.0]
    plt.plot(x, y, linestyle='dashed', color='gray', linewidth=2)

    colormap = plt.cm.gist_ncar
    colors = [colormap(i) for i in np.linspace(0, 1, NUMBER_SUBJECTS)]
    for i in range(1, len(psafe_users)):
        AddPsafeCurve(plt, psafe_users[i], intruder_axis, colors[i])

    SavePsafeCurvePlot(plt, fname, randomline, len_usr + frame_max)


def IsSupportedImageType(ext):
    fig = plt.figure()
    return (ext[1:] in fig.canvas.get_supported_filetypes())


if __name__ == "__main__":
    sys.exit(main(sys.argv))
