## Gabriel PIRES SOBREIRA

## Simulation d'une colonisation d'une galaxie - Projet IN203
# RAPPORT FINAL 

##  PARAMÈTRES:


### lscpu:


    
    |--------------------|---------------------------------|
    |Architecture:       |x86_64                           |
    |CPU op-mode(s):     |32-bit, 64-bit                   |
    |Byte Order:         |Little Endian                    |
    |Address sizes:      |36 bits physical, 48 bits virtual|
    |CPU(s):             |8                                |
    |On-line CPU(s) list:|0-7                              |
    |Thread(s) per core: |2                                |
    |Core(s) per socket: |4                                |
    |Socket(s):          |1                                |
    |--------------------|---------------------------------|

### Parametres.txt:

Pour ajuster l'image à la taille de l'écran d'ordinateur la hauteur et la largeur de l'image formée ont été modifiées, dans le fichier « parametres.txt »

    1366       # Nombre de pixels en largeur (détermine le pas de temps)
    768       # Nombre de pixels en hauteur
    1.E-5     # Chance que sur une planète apparaise une civilisation techno en deltaT années
    0.28      # Chance qu'une civilisation technologique disparaisse en deltaT ans
    0.35      # % de chance qu'une civilisation porte ses efforts pour aller coloniser d'autre planètes
    4.E-5     # Chance qu'une civilisation rende sa planète inhabitable en deltaT années

### Calcul du temp:

Pour meilleur analyser le temps passé c'était modifié la façon de lui calculer dans le fichier « colonisation.cpp ». Maintenant c'est fait le calcul du moyen du temps pour faire les calculs, du temps d'affichage et du temps total.

-----------

## Code Séquentiel:

Pour meilleure faire la comparaison du temp, la duration étais limité à 1 millon d'années:

    Temps passe :    1000725 années  |  CPU(ms) : calcul moyen 17.704  affichage moyen 6.391  Total moyen 24.095

------
## Parallélisation en MÉMOIRE PARTAGÉE

### 1) Parallélisation de boucle en mémoire partagée

#### Analyse de la Concurrence:

- **Lecture :** Il y a une problème de concurrence d'accés si seulement la function « mise_a_jour » est parallèle. Cela se produit parce que les threads dans la function essaient d'accéder à la fonction random, qui est dehors de la région parallèle, tous au même temps. Pour eviter ça il fallait que la fonction random doit être dans la region parallèle et créer un generateur de variables aléatoires pour chaque thread.  

- **Écriture :** Si deux threads assayent de changer une planète pour __Habitée__, il n'y a pas de problème parce que les deux changeront pour le même valeur. Par rapport quand une planète est déjà habitée les autres threads n'essayent pas de changer la planéte. Donc, n'y a pas des problèmes avec le concours d'écriture.

Un pixel peut faire des chamins suivants:

    Habitable <----> Habité ----> Inhabitable


Les temps pour la Parallélisation statique a été analysés, montrant les résultats présentés ci-dessous:

    1 thread :  Temps passe :    1000262 années  |  CPU(ms) : calcul moyen 11.453  affichage moyen 4.185  Total moyen 15.638

    2 threads:  Temps passe :    1000262 années  |  CPU(ms) : calcul moyen 5.754  affichage moyen 3.917  Total moyen 9.671

    3 threads:  Temps passe :    1000262 années  |  CPU(ms) : calcul moyen 3.318  affichage moyen 3.356  Total 6.674
    
    4 threads:  Temps passe :    1000262 années  |  CPU(ms) : calcul moyen 2.289  affichage moyen 2.764  Total moyen 5.053

    8 threads:  Temps passe :    1000262 années  |  CPU(ms) : calcul moyen 22.455  affichage moyen 3.407  Total moyen 25.863

    16 threads: Temps passe :    1000262 années  |  CPU(ms) : calcul moyen 2.652  affichage moyen 2.970  Total moyen 5.622

    32 threads: Temps passe :    1000262 années  |  CPU(ms) : calcul moyen 2.700  affichage moyen 2.890  Total moyen 5.591

Calcul des accélérations (Temps total moyen du code sequenciel divise par le temps total de chaque thread) avec parallèles (omp for) en comparant avec le code initiel en sequenciel.


  THREADS  | accélération     
:---------:|:--------:
1          | 1.5408  
2          | 2.4915  
3          | 3.6103 
4          | 4.7685  
8          | 0.9316  
16         | 4.2858  
32         | 4.3096  



## 2) Recouvrement calcul / affichage en mémoire partagée

On a crée la region parallèle pour effectuer la division entre affichage et calcul dans la fonction « main » et on a utilisé le thread 0 pour être responsable de l'affichage et d'autres feront les calculs. On va diviser, dans la function « mise_a_jour », à chaque thread de calcul une partie de la matrice des planètes. Exactement comme dans le code precedent, il aura data race, mais ce n'est pas un problème, parce que ça c'est le même comportement que dans une fonction séquentielle. 

Avec 

    #pragma omp sections nowait


omp for:

    1:  Temps passe :    1000262 années  |  CPU(ms) : calcul moyen 11.209  affichage moyen 3.685  Total moyen 14.894
    2:  Temps passe :    3056625 années  |  CPU(ms) : calcul 4.573  affichage 1.372  Total 5.9451
    3:  Temps passe :    3049200 années  |  CPU(ms) : calcul 3.793  affichage 1.670  Total 5.4638
    4:  Temps passe :    3039300 années  |  CPU(ms) : calcul 4.320  affichage 2.001  Total 6.32239
    8:  Temps passe :    1000262 années  |  CPU(ms) : calcul moyen 33.093  affichage moyen 72.771  Total moyen 105.864
    16: Temps passe :    3059100 années  |  CPU(ms) : calcul 4.435  affichage 1.603  Total 6.039
    32: Temps passe :    3116025 années  |  CPU(ms) : calcul 4.549  affichage 1.658  Total 6.2089

  THREADS  | accélération     
:---------:|:--------:
1          | 1.6178  
2          | 4.0529  
3          | 4.4099 
4          | 3.8111  
8          | 0.2276  
16         | 3.9899  
32         | 3.8807  
## Parallélisation en MÉMOIRE DISTRIBUÉ

### **MPI_1:	Recouvrement calcul / affichage**

Avec le même pricipe utilise dans la question précédent, on a fait la répartition des tâches entre les processus. Le processus 0 étais responsable pour faire l'affichage de l'image dans l'écran; le processus 1 étais responsable pour faire les calculs de la nouvelle matrice. Comme dans cette partie on utilise memoire distribué c'est necessaire faire l'echange de messages, donc dans le code le processus envoi une message bloquante de la nouvelle matrice au processus 0, et aprés ça le processus 0 vais comencer la procédure d'affichage et le processus 1 commencera un nouveau calcul de matrice. 

    Temps passe :    1000725 années  |  CPU(ms) : calcul moyen 6.774  affichage moyen 3.823  Total moyen 10.597

    Speedup Total: 2.2738


### **MPI_2:	Parallélisation en sous-grilles**

Pour faire la division du calcul en plusieurs processus, on va faire division de la matrice des planètes en sous-grilles pour chaque processus. Les lignes seront réparties entre les processus.

Aprés la repartition entre les processus. chaqu'un aura faire les calculs d'une matrice planétaire de même taille. La taille est égale au nombre de lignes divisé par le nombre de processeurs et jusqu'à deux autres lignes « fantômes ». Il y a jusqu'à deux lignes « fantômes » parce que si le processus est resposable pour le première ou le dernier matrice planetaire, il n'y a qu'une seule zone frontalière avec des zones d'autres processus, donc il aura seulement une ligne « fantômes », mais sinon il aura deux zones frontalière avec des zones d'autres processus. Las lignes « fantômes » sont utiliser pour stocker l'information de la dernier et / ou la premiere ligne des zones adjacentes, pour possibiliter faire le calcul de expansion correctement pour tous les lignes. Aprés les calculs on utilise la fonction MPI_GATHER pour envoier toutes les données au processus 0 qui est le responsable de l'affichage.

|     NUM_PROCESS   |     Total moyen(ms)    |     Speedup Total    |
|:-----------------:|:----------------:|:--------------------:|
|     séquentiel    |     24.095       |     -                |
|     2             |     7.986        |     3.0172           |
|     3             |     4.305        |     5.5970           |
|     4             |     3.809        |     6.3258           |
|     8             |     5.654        |     4.2616           |
|     16            |     6.402        |     3.7637           |
|     32            |     8.334        |     2.8912           |

## Parallélisation en mémoire partagée et distribuée

Dans cette dernière étape du projet, nous effectuerons la jonction des deux méthodes de parallélisation. Nous partagerons les tâches d'affichage et de calcul avec parallélisation en mémoire distribuée. Ainsi, le processus 0 sera responsable de l'affichage et tous les autres se partageront le travail de calcul. La division de calcul suivra la même logique que la question précédente avec des cellules fantômes. Le seul changement sera la création d'une région parallèle à OpenMP dans la fonction mise_a_jour. Ainsi, nous allons créer un code hybride entre openMP et MPI.

Pour la dernière étape du projet, on va faire la jontion du MPI et du OpenMP. Alors, on partagera les tâches d'affichage et de calcul avec MPI. Ainsi, la division de tâches sera: le processus 0 est responsable pour faire l'affichage, les autres seront responsable de faire les calculs. La logique utilisé pour faire la division de calcul c'est la même utilise que l'utilise dans la question précédente. La differece c'est que on va crée une région parallèle avec OpenMP dans la fonction « mise_a_jour ».

| PROCESS X THREAD | Total (ms) | Speedup |
|:----------------:|:----------:|:-------:|
|   séquentiel     | 24.095     | ---     |
| 2x2              | 4.431      |         |
| 2x4              | 3.182      | 7.5723  |
| 2x8              | 3.436      |         |
| 2x16             | 3.336      |         |
| 4x2              | 5.086      |         |
| 4x4              | 63.610     |         |
| 4x8              | 154.176    |         |
| 4x16             | 5.258      |         |
| 8x2              | 39.367     |         |
| 8x4              | 154.744    |         |
| 8x8              | 234.864    |         |
| 8x16             | 5.096      |         |
| 16x2             | 97.147     |         |
| 16x4             | 253.718    |         |
| 16x8             | 535.122    |         |
| 16x16            | 5.348      |         |