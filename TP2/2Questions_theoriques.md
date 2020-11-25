# TP2 de PIRES SOBREIRA Gabriel

`pandoc -s --toc tp3.md --css=./github-pandoc.css -o tp3.html`


## Question du cour N°1
*Reprendre l’exercice sur l’interblocage donné dans le cours et décrivez deux scénarios :*
1. Un premier scénario où il n’y a pas d’interblocage ;
2. Un deuxième scénario où il y a interblocage.

*Quelle est à votre avis la probabilité d’avoir un interblocage ?*
>il y a une grand possibilité d'interblocage et le programmeur doit payer beaucoup d'attention pour éviter ces cas. En plus, le risque c'est plus grand pour les donnés de grand taille, parce que ils ne peuvent pas être stockés dans le buffer.

## Question du cours n°2
*Alice a parallélisé en partie un code sur machine à mémoire distribuée. Pour un jeu de données spécifiques, elle remarque que la partie qu’elle exécute en parallèle représente en temps de traitement 90% du temps d’exécution du programme en séquentiel. En utilisant la loi d’Amdhal, pouvez-vous prédire l’accélération maximale que pourra obtenir Alice avec son code (en considérant n ≫ 1) ?*

>D'accord avec la formule d'Amdahl, on observe que avec une infinité de nœuds de calcul( n >> 1), le limite pour diminuit le temp maximale d'execution c'est 1/f. Donc, la acceleration maximale c'est **S(n)=10**.

 *À votre avis, pour ce jeu de donné spécifique, quel nombre de nœuds de calcul semble-t-il raisonnable de prendre pour ne pas trop gaspiller de ressources CPU ?* 

>D'accord l'equation d'efficacite de programme:
>***E(n)=100 x S(n) / n***.
>On doit trouver **n** pour maximizer le programme (augmente l'efficace et l'acceleretion). Donc, pour ça **n = 9**

*En effectuant son calcul sur son calculateur, Alice s’aperçoit qu’elle obtient une accélération maximale de quatre en augmentant le nombre de nœuds de calcul pour son jeu spécifique de n données. En doublant la quantité de données à traiter, et en supposant la complexité de l’algorithme parallèle linéaire, quelle accélération maximale peut espérer Alice en utilisant la loi de Gustafson ?*

>D'accord avec la loi de Gustafson, nous avons conclu que la nouvelle accélération S(n) doit être: **S(n) = 2 * S_pre(n) - s**. Où, *S_pre* est l'accélération précédent et *s* est la proportion avec le temps d'execution du code exécuté en séquentiel, pour le temps d'execution du code ecécuté en parallèle. Donc, on peut considere *s* << 1. Pour ça on peut simplifier l'équation: **S(n) = 2 * S_pre(n)**. Et on peut remplacer *S_pre(n)* pour 4. Donc, on a que: **S(n) = 2 * 4 = 8**

## Mandelbrot 

*Expliquer votre stratégie pour faire une partition équitable des lignes de l'image entre chaque processus*

>Pour faire la partition équitable des lignes de l'image entre chaque processus, j'ai fait le processus 0 comme le precessus maître et les autres sont le processus "esclave", donc, les lignes sont envoyé à chaque processus "esclave". chaque ligne c'est envoyé à une processus, et quand le processus c'est fini pour lui traiter, c'est envoyé autre ligne à il. Après le maître recevoir tous les lignes et il assemble l'image.

           | Taille image : 800 x 600 | 
-----------+---------------------------

séquentiel | 15.61

1          | 68.0961          
2          | 34.6578             
3          | 27.2937             
4          | 23.3431             
8          | 27.3288             


*Discuter sur ce qu'on observe, la logique qui s'y cache.*

>On observe que le temp pour traiter le code reduce significativement jusqu'à avec 4 processus, augmente nouvement quand on utilisé 8 processus. De plus, le programme séquentiel est plus rapide que le parallèle.

*Expliquer votre stratégie pour faire une partition dynamique des lignes de l'image entre chaque processus*

>Pour faire une partition dynamique des lignes, il est possible de rendre chaque processeur responsable d'un pixel au lieu d'une ligne, ce qui rend le processus beaucoup plus rapide et plus dynamique. Les roles pour faire ça c'est très similaire que le exercice precedent, avec les processus "esclave" responsable pour calculer chaque pixel, et le processus maître pour assemble l'image.  

           | Taille image : 800 x 600 | 
-----------+---------------------------

séquentiel | 15.61       

1          | ne pas exécute             
2          | 18.4623             
3          | 9.06241             
4          | 6.12944             
8          | 2.83532             



## Produit matrice-vecteur



*Expliquer la façon dont vous avez calculé la dimension locale sur chaque processus, en particulier quand le nombre de processus ne divise pas la dimension de la matrice.*

>Pour calculé la dimension locale sur chaque processus, vous avez divisé la dimension de la matrice pour le nombre de processus utilisé. Car le nombre de processus ne divise pas la dimension de la matrice, on doit designé un processus pour faire le calcul avec une dimension different de los autres.   