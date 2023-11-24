import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import sys





"""
fichier original, a modifier beaucoup !
"""








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
stdev = np.std(execTime, 1) *10

fig = plt.figure()
name = "Exemple"

plt.plot(nThreads, execTime, "bo")
plt.plot(nThreads, median, "k-", label="Médiane")
plt.plot(nThreads, mean, "r-", label="Moyenne")
plt.plot(nThreads, stdev, "g-", label="Ecart-type (*10)")
plt.boxplot(execTime.transpose())

plt.xlim(0,9)
plt.xticks(nThreads)
plt.ylim(0,15)
plt.yticks(np.linspace(0,15,6))
plt.xlabel("Nombre de threads [.]")
plt.ylabel("Temps d'exécution [s]")
plt.title(f"Temps d'exécution en fonction du nombre de threads\n({nExec} exécutions par nombre de threads)")
plt.grid(True)
plt.legend()
plt.savefig(name + ".png")
#plt.savefig(name + ".pdf")
plt.show()
plt.close()