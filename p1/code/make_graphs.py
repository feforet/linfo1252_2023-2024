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


#CODE PARTIE 1
#Creating boxplot 
file = sys.argv[1]

with open(file) as fd:
    data = pd.read_csv(fd)

donnees = np.array(data)

nThreads = np.unique(donnees[:,0])
execTime = donnees[:,1]
nExec = execTime.size//nThreads.size
execTime = execTime.reshape((nThreads.size,nExec))
ymax = np.max(execTime)

mean = np.mean(execTime, 1)#moyenne 
median = np.median(execTime, 1)
stdev = np.std(execTime, 1) #écart type
bp = data.boxplot(by='nombre de coeurs', column=['mesure'], grid = True, figsize=(20, 50), fontsize= 15)
[ax_tmp.set_xlabel('Nombre de Coeurs', size=20) for ax_tmp in np.asarray(bp).reshape(-1)]
#plt.errorbar([1,2,3,4,5,6,7], mean, stdev, fmt='ko-', label='Moyenne ± écart-type')
plt.plot([1,2,3,4,5,6], stdev, 'b', label='Ecart_type')
plt.plot([1,2,3,4,5,6], mean, 'ro', label='Moyenne')


plt.ylabel("Temps d'exécution [s]", size=20)
plt.xlabel("Nombre de Coeurs", size=20)
plt.ylim(0,ymax*1.05)
plt.title("Temps en fonction des coeurs de l'algo lecteurs/Ecrivains", size =20 )
plt.grid(True)
plt.legend()
plt.savefig('Lect' + ".pdf")
plt.show()




#CODE PARTIE 2
"""

#Creating boxplot 
file1 = sys.argv[1]
file2 = sys.argv[2]

with open(file1) as fd:
    data1 = pd.read_csv(fd)

with open(file2) as fd:
    data2 = pd.read_csv(fd)

donnees1 = np.array(data1)
donnees2 = np.array(data2)

nThreads = np.unique(donnees1[:,0])
execTime1 = donnees1[:,1]
nExec = execTime1.size//nThreads.size
execTime1 = execTime1.reshape((nThreads.size,nExec))

mean1 = np.mean(execTime1, 1)#moyenne 
median1 = np.median(execTime1, 1)
stdev1 = np.std(execTime1, 1) #écart typev

nThreads = np.unique(donnees2[:,0])
execTime2 = donnees2[:,1]
nExec = execTime2.size//nThreads.size
execTime2 = execTime2.reshape((nThreads.size,nExec))

mean2 = np.mean(execTime2, 1)#moyenne 
median2 = np.median(execTime2, 1)
stdev2 = np.std(execTime2, 1) #écart typev




a1 = []
a2 = []
a3 = []
a4 = []
a5 = []
a6 = []
a7 = []

b1 = []
b2 = []
b3 = []
b4 = []
b5 = []
b6 = []
b7 = []


for i in range(0, len(data1)):
    if donnees1[i][0] == 1:
        a1.append(donnees1[i][1])
        b1.append(donnees2[i][1])
    if donnees1[i][0] == 2:
        a2.append(donnees1[i][1])
        b2.append(donnees2[i][1])
    if donnees1[i][0] == 4:
        a3.append(donnees1[i][1])
        b3.append(donnees2[i][1])
    if donnees1[i][0] == 8:
        a4.append(donnees1[i][1])
        b4.append(donnees2[i][1])
    if donnees1[i][0] == 16:
        a5.append(donnees1[i][1])
        b5.append(donnees2[i][1])
    if donnees1[i][0] == 32:
        a6.append(donnees1[i][1])
        b6.append(donnees2[i][1])
    if donnees1[i][0] == 64:
        a7.append(donnees1[i][1])
        b7.append(donnees2[i][1])



tas = pd.DataFrame({ '2': a2, '4': a3, '8': a4, '16':a5, '32': a6, '64': a7})
tts = pd.DataFrame({ '2': b2, '4': b3, '8': b4, '16':b5, '32': b6, '64': b7})     

datasets = [tas, tts]
colours = ['green', 'red']
groups = ['Mutex/sémaphore', 'Algo tts', 'std dev mutex/sem', 'std dev tts', 'Moyenne' ]


x_pos_range = np.arange(len(datasets)) / (len(datasets) - 1)
x_pos = (x_pos_range * 0.5) + 0.75
# Plot
for i, data in enumerate(datasets):
    bp = plt.boxplot(
        np.array(data), sym='', whis=[0, 100], widths=0.6 / len(datasets),
        labels=list(datasets[0]), patch_artist=True,
        positions=[x_pos[i] + j * 1 for j in range(len(data.T))]
    )

    k = i % len(colours)
    for box in bp['boxes']:
        box.set(facecolor=colours[k])
    for element in ['boxes', 'fliers', 'means', 'medians']:
        plt.setp(bp[element], color=colours[k])
    for element in ['whiskers', 'caps']:
        plt.setp(bp[element], color=colours[k])
        plt.setp(bp[element], color=colours[k])
# Titles
plt.ylabel("Temps d'exécution [s]", size=20)
plt.xlabel("Nombre de Coeurs", size=20)
plt.title("Producteurs Consommateurs", size =20 )


plt.plot([0.75,2,3,4,5,5.75], stdev1, 'b', label='Ecart_type TS')
plt.plot([0.75,1.75,2.75,3.75,4.75,5.75], mean1, 'co', label='Moyenne TS')

plt.plot([1.25,2.25,3.25,4.25,5.25,6.25], stdev2, 'y', label='Ecart_type TTS')
plt.plot([1.25,2.25,3.25,4.25,5.25,6.25], mean2, 'co', label='Moyenne TTS')
#for partie 2.2/2.3
#plt.plot([1,2,3,4,5,6], stdev2, 'y', label='Ecart_type tts')
#plt.plot([1,2,3,4,5,6], mean2, 'ko', label='Moyenne tts')



# Axis ticks and labels
plt.gca().tick_params(axis='x', which='minor', length=4)
plt.gca().tick_params(axis='x', which='major', length=0)
# Change the limits of the x-axis
plt.xlim([0.5, len(list(datasets[0])) + 0.5])
plt.ylim(0,0.5)
plt.grid(True)
g1 = plt.gca().scatter(0, 0.3, color='g')
g2 = plt.gca().scatter(0, 0.3, color='r')
g3= plt.gca().scatter(0,0.5, color='b')
g4= plt.gca().scatter(0,0.5, color='y')
g5= plt.gca().scatter(0,0.5, color='c')


plt.gca().legend([g1, g2, g3, g4, g5], groups, fontsize='small')
#plt.savefig('lect_TTS' + ".pdf")
plt.show()
"""

#df1 = data.boxplot(by='nombre de coeurs', column=['mesure'])
#plt.show()
