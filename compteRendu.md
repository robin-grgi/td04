# TD 4 Processus et Thread

## 1 Premiers threads, PID et Thread ID

### Exercice 1

Lors de l'exécution du programme sans appel à pthread join, le message "bye bye from thread" n'est pas affiché La valeur de retour n'est pas 1 car le processus appelant tente d'accéder à la zone mémoire retour avant que celle-ci ait été initialisée. On en déduit que la durée de vie du thread est au maximum identique à la durée de vie du processus appelant.

Lors de l'exécution du programme avec un appel à exit(0) comme ci dessous :
```c
void *fn_thread(void *arg) {
  printf("Hello from the thread\n");
  exit(0);
```
Le processus appelant s'arrête en même temps que le thread.

En plaçant le exit(0) avant l'appel à pthread_join le thread n'a même pas le temps d'afficher "Hello from thread" qu'il est stoppé par l'arrêt du processus appelant.

on en déduit qu'exit met fin au processus appelant et que pthread_exit met fin uniquement au thread.

### Exercice 2


Lorsque n1 = 1 et n2 = 2 on constate que le thread associé à n1 a le temps d'exécuter deux printf pendant que le thread associé à n2 fasse un seul printf.

```
$ ./threads.exe 1 2
Program start PID : 5414
PID : 5414, Thread ID : 242226944
PID : 5414, Thread ID : 233834240
PID : 5414, Thread ID : 242226944
PID : 5414, Thread ID : 242226944
PID : 5414, Thread ID : 233834240
PID : 5414, Thread ID : 242226944
PID : 5414, Thread ID : 242226944
PID : 5414, Thread ID : 233834240
PID : 5414, Thread ID : 233834240
PID : 5414, Thread ID : 233834240
Status for n1 : 0
Status for n2 : 0
```

lorsque n1 = 1 et n2 = 6 alors on remarque que tous les printf du thread associé à n1 ont le temps de s'exécuter avant les printf du thread associé à n2.

```
$ ./threads.exe 1 6
Program start PID : 5759
PID : 5759, Thread ID : 1862194944
PID : 5759, Thread ID : 1862194944
PID : 5759, Thread ID : 1862194944
PID : 5759, Thread ID : 1862194944
PID : 5759, Thread ID : 1862194944
PID : 5759, Thread ID : 1853802240
PID : 5759, Thread ID : 1853802240
PID : 5759, Thread ID : 1853802240
PID : 5759, Thread ID : 1853802240
PID : 5759, Thread ID : 1853802240
Status for n1 : 0
Status for n2 : 0
```

### Exercice 3

On constate qu'effectivement les thread id de posix et du noyau linux sont différents.

```
$ ./threads.exe 1 1
Program start PID : 6667
PID : 6667, Posix Thread ID : -1839167744, Linux Thread ID: 6668
PID : 6667, Posix Thread ID : -1847560448, Linux Thread ID: 6669
PID : 6667, Posix Thread ID : -1839167744, Linux Thread ID: 6668
PID : 6667, Posix Thread ID : -1847560448, Linux Thread ID: 6669
PID : 6667, Posix Thread ID : -1839167744, Linux Thread ID: 6668
PID : 6667, Posix Thread ID : -1847560448, Linux Thread ID: 6669
PID : 6667, Posix Thread ID : -1839167744, Linux Thread ID: 6668
PID : 6667, Posix Thread ID : -1847560448, Linux Thread ID: 6669
PID : 6667, Posix Thread ID : -1839167744, Linux Thread ID: 6668
PID : 6667, Posix Thread ID : -1847560448, Linux Thread ID: 6669
Status for n1 : 0
Status for n2 : 0
```

## 2 Différences Processus et Threads

### Exercice 4

Il n'est pas possible d'exécuter plus de 10000 processus (environ) sous peine de voir l'erreur suivante :

```
$ ./multiple_fork.exe 
fork: Resource temporarily unavailable
Abandon (core dumped)
```
### Exercice 5

On remarque que le temps de création des threads est inférieur au temps de création des processus.

```
$ ./multiple_fork.exe 
Time taken for creating 4000 processus (nanoseconds): 152353317
Time taken for creating 4000 processus (milliseconds): 152

$ ./multiple_threads.exe 
Time taken for creating 4000 threads (nanoseconds): 82502161
Time taken for creating 4000 threads (milliseconds): 82
```

## 3 Observation des Processus et Threads

### Exercice 6

Les processus et threads sont bien visibles :

![multiple_fork.png](multiple_fork.png)


![multiple_threads.png](multiple_threads.png)


## Programmes (presque) justes

### Exercice 7

exécution avec sleep(1) :

```
$ ./juste_presque.exe 
Dans la thread #2
Dans la thread #3
Dans la thread #4
Dans la thread #4
Dans la thread #4
```

exécution sans sleep(1):
```
$ ./juste_presque.exe 
Dans la thread #0
Dans la thread #0
Dans la thread #0
Dans la thread #0
Dans la thread #0
```

Il ne me semble pas raisonnable que le rogramme soit dépendant d'une instruction d'attente pour des raisons d'efficacité.

Pour corriger cette erreur il suffit de remplacer les lignes

```c
long n = *(long*) arg;
```
par
```c
long n = (long)arg;
```
et

```c
pthread_create(&threads[i], NULL, func, (void *)&i);
```
par
```c
pthread_create(&threads[i], NULL, func, (void *)i);
```

cf [juste.c](juste.c)

### Exercice 8

On constate qu'effectivement, le programme ne fonctionne pas toujours correctement;
```
Réinitialisation du jeu avec 19 allumettes.
Joueur 1 prend 3 allumettes, reste 16 allumettes
Joueur 0 prend 1 allumettes, reste 15 allumettes
Joueur 1 prend 2 allumettes, reste 13 allumettes
Joueur 0 prend 1 allumettes, reste 12 allumettes
Joueur 1 prend 1 allumettes, reste 11 allumettes
Joueur 0 prend 1 allumettes, reste 10 allumettes
Joueur 1 prend 3 allumettes, reste 7 allumettes
Joueur 0 prend 1 allumettes, reste 6 allumettes
Joueur 1 prend 2 allumettes, reste 4 allumettes
Joueur 0 prend 1 allumettes, reste 3 allumettes
Joueur 1 prend 1 allumettes, reste 2 allumettes
Joueur 0 prend 2 allumettes, reste 0 allumettes
Joueur 1 prend 1 allumettes, reste 18 allumettes
Réinitialisation du jeu avec 19 allumettes.
```

En supprimant l'appel à sleep(1) le nombre d'allumettes devient parfois négatif :
```
Réinitialisation du jeu avec 19 allumettes.
Joueur 1 prend 0 allumettes, reste -1 allumettes
Joueur 1 prend 1 allumettes, reste 13 allumettes
Joueur 1 prend 1 allumettes, reste 12 allumettes
Joueur 1 prend 2 allumettes, reste 10 allumettes
Joueur 1 prend 2 allumettes, reste 8 allumettes
Joueur 1 prend 3 allumettes, reste 5 allumettes
Joueur 1 prend 2 allumettes, reste 3 allumettes
Joueur 1 prend 3 allumettes, reste 0 allumettes
Joueur 1 prend 1 allumettes, reste 18 allumettes
Joueur 1 prend 1 allumettes, reste 17 allumettes
Joueur 1 prend 3 allumettes, reste 14 allumettes
Joueur 1 prend 1 allumettes, reste 13 allumettes
Joueur 1 prend 3 allumettes, reste 10 allumettes
Joueur 1 prend 3 allumettes, reste 7 allumettes
Joueur 1 prend 2 allumettes, reste 5 allumettes
Joueur 1 prend 2 allumettes, reste 3 allumettes
Joueur 1 prend 2 allumettes, reste 1 allumettes
Joueur 1 prend 1 allumettes, reste 0 allumettes
Joueur 1 prend 1 allumettes, reste -1 allumettes
Réinitialisation du jeu avec 19 allumettes.
```

Effectivement, le CPU est utilisé à 100%. 

![fullCPU.png](fullCPU.png)

Je pense que cette consommation anormale des ressources de calcul provient du fait que l'on utilise une boucle infinie qui initialise en permanence de nouveaux threads qui prennent 1 seconde à terminer leur exécution ainsi le processeur crée plus rapidement de nouveaux threads qu'il termine les anciens threads ce qui le surcharge.

## 5 Processus et Threads sous Win32

Horrible. Je n'ai rien compris et j'ai l'impression que cette partie du TD n'est absolument pas réalisable en autonomie.