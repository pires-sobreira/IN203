#   TP1 11/11/2020
##  UN HELLO WORLD PARALLÈLE

### QUESTION: 
où xx est le rang de la tâche et yy le nombre de tâches lancées par MPI.
Tester le programme avec 1 à 16 processus. Que constatez-vous ?

### RÉPONDRE: 
Quand on utilise plusieurs processus, la affichage ce n'est pas dans l'order, parce que comme c'est une code parallèle il n'y a pas la necessité faire les processus dans une ordre séquentiel.


### QUESTION: 
Faites une version où chaque processus écrit son message dans un fichier Output000xx.txt en utilisant
le fichier SkeletonMPIProgramWithFilesOutput.cpp comme canvas de base.
Expliquez en quoi faire une sortie sur fichier est finalement plus clair qu’une sortie sur terminal.

### RÉPONDRE:
 Faire une sortie sur fichier est plus claire qu'une sortie sur terminal, parce que dans le terminal la sortie c'est avec tous les autres commandes que on utilise dans le terminal, donc c'est plus dificile pour trouver ça e lui interpreter, mais si on utilise une sortie sur fichier c'est plus facile pour trouver ça e lui interpreter.



##  ENVOI BLOQUANT ET NON BLOQUANT

### QUESTION:
 Expliquez pourquoi la première version est sûre et la deuxième dangereuse à utiliser. Quelle modification
faudrait-il faire pour que la deuxième version soit sûre (dans le sens qu’elle donnera toujours le même
résultat) ?

### RÉPONDRE: 
la première version est sûre parce que il utilise la fonction bloquant MPI_Send. Dans une fonction bloquant il y a la verifications des donnés pour assurer que ces modifications n’altéreront pas les données qui devront être reçues par la tâche destinataire et il rend la main seulement après que les données soient arrivées et prêtes à être utilisées par l’utilisateur. Une fonction non bloquant c'est plus dangereux parce que il n'y a pas la verification des donnès comme dans les fonctions bloquant, mais ils sont plus rapide, parce que ils rendent la main immédiatement. Dans le deuxième code pour faire il sur, on doit change la fonction MPI_ISend pour la fonction MPI_Send.



##  CIRCULATION D'UN JETON DANS UN ANNEAU

### QUESTION: 
En vous servant du premier exercice où les processus affichent sur le terminal, mettez en œuvre l’algorithme décrit ci–dessus, puis compilez et testez sur un nombres de tâches divers. Pouvez vous expliquez
pourquoi à l’exécution, les messages sont affichés dans l’ordre des numéros des processus (mis à part le
processus 0 qui affiche en dernier) ?

### RÉPONDRE: 
Ça arrive comme ça parce que le code utilise envoir et recevoir bloquant, donc les processeurs doit attendre que le processeur precedent a envoyé le valeur avec l'increment, donc l'ordre d'affichage c'est l'ordre des processeurs.


### QUESTION: 
Mettez en œuvre, compilez et exécutez avec un nombre variable de tâches. Que constatez vous à
l’affichage ? Pourquoi ?

### RÉPONDRE: 
J'ai constaté un comportement similaire que le code d'exercice précédent, pour la même raison, parce que la valeur que le processeur enverra dépend de la valeur qui a été envoyé pour le processeur précédent.