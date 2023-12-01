import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import sys

file = sys.argv[1]

with open(file) as fd:
    donnees = pd.read_csv(fd)

donnees = np.array(donnees)

nThreads = np.unique(donnees[:,0])
execTime = donnees[:,1]
nExec = execTime.size//nThreads.size
execTime = execTime.reshape((nThreads.size,nExec))
mean = np.mean(execTime, 1)
median = np.median(execTime, 1)
stdev = np.std(execTime, 1)

fig = plt.figure()
name = file[:-4]

"""
plt.plot(nThreads, execTime, "bo")
plt.plot(nThreads, mean, "r-", label="Moyenne")
plt.plot(nThreads, stdev, "g-", label="Ecart-type")
plt.fill_between(nThreads, mean-stdev, mean+stdev, label="Ecart-type")
plt.xlim(0,np.max(nThreads)*1.1)
plt.xticks(nThreads)
ymax = np.max(execTime)
plt.ylim(0,ymax*1.1)
plt.yticks(np.linspace(0,ymax,6))
plt.xlabel("Nombre de threads [.]")
plt.ylabel("Temps d'exécution [s]")
plt.title(f"Temps d'exécution en fonction du nombre de threads\n({nExec} exécutions par nombre de threads)")
plt.grid(True)
plt.legend()
plt.savefig(name + ".png")
#plt.savefig(name + ".pdf")
plt.show()
plt.close()
"""

#new graphs
plt.plot(nThreads, execTime, "ro", label='Mesures')
plt.errorbar(nThreads, mean, stdev, fmt='ko-', label='Moyenne ± écart-type')
plt.plot(nThreads, stdev, "g-", label="Ecart-type")
#plt.fill_between(nThreads, mean-stdev, mean+stdev, label="Ecart-type")
plt.xlim(0,np.max(nThreads)*1.05)
plt.xticks(nThreads)
ymax = np.max(execTime)
plt.ylim(0,ymax*1.05)
#plt.yticks(np.linspace(0,ymax,6))
plt.yticks(np.concatenate((mean, [np.max(stdev)])))
plt.xlabel("Nombre de threads [.]")
plt.ylabel("Temps d'exécution [s]")
plt.title(f"Temps d'exécution en fonction du nombre de threads\n({nExec} exécutions par nombre de threads)")
plt.grid(True)
plt.legend()
plt.savefig(name + ".png")
plt.savefig(name + ".pdf")
#plt.show()
plt.close()
