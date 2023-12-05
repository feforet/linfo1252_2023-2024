import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import sys
"""
file = sys.argv[1]

with open(file) as fd:
    donnees = pd.read_csv(fd)

donnees = np.array(donnees)

nThreads = np.unique(donnees[:,0])
execTime = donnees[:,1]
nExec = execTime.size//nThreads.size
execTime = execTime.reshape((nThreads.size,nExec))

fig = plt.figure()
name = file[:-4]
#print(donnees)

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
#plt.savefig(name + ".png")
#plt.savefig(name + ".pdf")
#plt.show()
plt.close()

"""
#Creating boxplot 
file = sys.argv[1]

with open(file) as fd:
    data = pd.read_csv(fd)

donnees = np.array(data)

nThreads = np.unique(donnees[:,0])
execTime = donnees[:,1]
nExec = execTime.size//nThreads.size
execTime = execTime.reshape((nThreads.size,nExec))

mean = np.mean(execTime, 1)#moyenne 
median = np.median(execTime, 1)
stdev = np.std(execTime, 1) #écart type
bp = data.boxplot(by='nombre de coeurs', column=['mesure'], grid = True, figsize=(20, 50), fontsize= 15)
[ax_tmp.set_xlabel('Nombre de Coeurs', size=20) for ax_tmp in np.asarray(bp).reshape(-1)]
#plt.errorbar([1,2,3,4,5,6], mean, stdev, fmt='ko-', label='Moyenne ± écart-type')
plt.plot([1,2,3,4,5,6], stdev, 'b', label='Ecart_type')
plt.plot([1,2,3,4,5,6], mean, 'ro', label='Moyenne')


plt.ylabel("Temps d'exécution [s]", size=20)
plt.xlabel("Nombre de Coeurs", size=20)
plt.title("Temps en fonction des coeurs pour le problème des philosophes", size =20 )
plt.grid(True)
plt.legend()
plt.show()

