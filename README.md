# WEBSERV

:construction_worker_man: README en travaux ! :construction_worker_woman:

Construction d'un serveur web pour comprendre les rouages du protocol HTTP. C++98 only.

## Comprendre les notions
### Comprendre le protocole HTTP

Il faut tout d'abord comprendre le chemin parcouru lorsque l'on suit un lien.
Sur un terminal, pour avoir accès a un fichier, on va specifier un chemin ou path.
Ex : cat path/to/my/file.txt
C'est a peu pres la meme chose sur un browser, sauf qu'on ne va pas chercher l'acces a un fichier en local mais bien sur une autre machine. On va donc préciser une adresse associée a ce path.

Admettons que vous cherchez un fichier chez quelqu'un qui habite dans une autre ville. Vous connaissez son adresse et le nom du fichier recherché, rien de plus :

https://www.monsupersite.com/admin/index.html

www.monsupersite.com  est l'adresse de la personne
/admin/               la piece dans laquelle le fichier est entreposé
index.html            le nom du fichier

Et bien sur, vous pouvez vous rendre devant chez cette personne, mais interdiction d'y rentrer ! C'est là le role du serveur. Il verifiera que vous ayez bien droit d'accès au fichier demandé, ira le chercher pour vous et rendra son contenu sous forme de reponse.

Le browser quant à lui servira de coursier. C'est lui, en construisant une requete, qui ira communiquer avec le serveur. Vous n'avez rien a faire d'autre que de les laisser travailler !

Le but du projet est donc de prendre la place du serveur. Voyons maintenant comment se construisent leur communication.

<img src="docs/Main.png" width="75%">

### Composition d'une requete

La requete, c'est la demande d'acces au fichier que fait le browser ou **client**. Quand le serveur la reçoit, il commence par la parser avant de la traiter. Voyons comment elle se decompose :

```bash
[METHOD] [REQUEST URI] [HTTP VERSION]
[HEADER 1]: [value1]
[HEADER 2]: [value2]

[body, if any]

```

La premiere ligne est appelée **Request Line**. Elle contient 3 elements d'information :

**HTTP METHOD**
C'est l'action que le client veut opérer sur l'URI demandé. Il en existe un certain nombre, mais pour notre projet
nous en utiliserons 3 :
- `GET` Recupérer le contenu d'une ressource
- `POST` Envoyer des données au serveur, contenues dans le body de la requete.
- `DELETE` Supprimer une ressource

**REQUEST URI**
C'est l'adresse de la ressource ciblée par la requete. Ce peut être un chemin relatif (/index.html) ou absolu (https://www.monsupersite.com/admin/index.html).

A noter que '/' est considéré comme un chemin, celui de la racine (root) du projet.

**HTTP Version**
C'est simplement la version du protocole HTTP utilisé.

### Composition d'une reponse

A chaque requete correspond une réponse, dont les structures se ressemblent fortement :

```bash
[HTTP VERSION] [STATUS CODE] [REASON PHRASE]
[HEADER 1]: [value1]
[HEADER 2]: [value2]

[body, if any]

```
*Le body va bien entendu contenir les data demandées, dans le cas d'une requete GET. N'oubliez pas de le faire suivre d'une ligne vide pour que la reponse soit a la norme et comprise par le client.*

Ici la première ligne se nomme la **Status Line**, et contient aussi 3 éléments d'information important.

**HTTP Version**
La version du protocole HTTP qui doit correspondre à celle de la requete.

**STATUS CODE**
C'est un code à 3 chiffres qui donne une information sur la façon dont la requete a été traitée par le serveur.
Ils sont classés en 5 groupes, en fonction du premier chiffre du code :

- 1XX : Pas utilisé en HTTP/1.0
- 2XX : Succès
- 3XX : Redirection
- 4XX : Erreur coté client
- 5XX : Erreur coté serveur

**REASON PHRASE**
C'est une phrase en anglais qui correspond au Status Code de la Reponse, et apporte des précisions sur la nature de l'information renvoyée.

Voici un récapitulatif des status codes que nous utilisons dans ce projet :
| Status Code | Reason Phrase |
|---|---|
|`200`| Ok |
|`201`| Created |
|`400`| Bad Request |
|`403`| Forbidden |
|`404`| Not Found |
|`405`| Method Not Allowed |
|`500`| Internal Server Error |
|`504`| Gateway Timeout |

Vous retrouverez la liste complète des status code [ici](https://en.wikipedia.org/wiki/List_of_HTTP_status_codes)


## Structure du Serveur
On sait maintenant que le projet va donc consister à gérer des requetes, potentiellement de plusieurs clients à la fois, des les traiter, puis construire et renvoyer les reponses correspondantes. 


:construction_worker_man: Au delà, la Readme est en chantier ! :construction_worker_man:
## Gestion des connexions

La fonction TcpListener::runTcpListener() du serveur gère les connexions TCP en utilisant la fonction select pour surveiller les descripteurs de fichiers prêts à être lus ou écrits. Elle initialise des ensembles de descripteurs de fichiers (fd_set), les met à jour dans une boucle continue, et utilise select pour détecter les descripteurs actifs. Lorsqu'un descripteur est prêt à être lu, il détermine s'il s'agit d'un nouveau serveur et gère les nouvelles connexions, ou lit et traite les requêtes existantes. Pour les descripteurs prêts à être écrits, la fonction envoie les réponses appropriées. Cette approche permet de gérer efficacement plusieurs connexions réseau de manière non bloquante.

### Gestion des Upload/Download

:construction_worker_man:

### Creation de la Reponse

:construction_worker_man:



### Documentation utilisée

- [Une doc comprehensible et digeste par l'etre humain sur le protocole HTTP](https://www.jmarshall.com/easy/http/#structure)
- [Une autre doc tout aussi intéressante](https://www.garshol.priv.no/download/text/http-tut.html#creq)


===================================================================================================================
### Fonctionnalités

Notre version de Bash ne cherchera pas à reproduire la totalité des features du shell; les opérateurs **&&** et **||**, les wildcards ou symboles spéciaux à interpréter ne seront pas pris en compte.
Néanmoins, voici une liste des fonctionnalités que nous allons pouvoir gérer :

- Afficher un prompt en attente d'une nouvelle commande. Aucune erreur ne devrait quitter notre programme. 
- Trouver et executer les commandes transmises par l'utilisateur.
- Implémenter les pipes `|`.
- Gérer les redirections telles que :
  - `<` Input.
  - `<<` Heredoc.
  - `>` Output en mode simple.
  - `>>` Output en mode *append*.
- Gérer les quotes simples et doubles. On renverra une erreur si une quote n'est pas fermée.
- Gérer les variables d'environnement ainsi que `$?`.
- `ctrl-C`, `ctrl-D`, et `ctrl-\` doivent réagir exactement comme dans Bash.
- Implémenter les built-ins suivants:
  - `echo` avec l'option `-n`.
  - `cd` suivi d'un chemin relatif ou absolu.
  - `pwd`
  - `export`
  - `unset`
  - `env`
  - `exit`
- Avoir un historique fonctionnel.
- Avoir une solide gestion des erreurs potentiellement transmises par l'utilisateur. Donc pas de crash/segfault/leak d'aucun genre.

### Achitecture générale

Notre programme se base sur une loop principale qui tourne indéfiniment et réalise 3 grandes actions.

1 - Tout d'abord, avec la fonction **readline()** un prompt est affiché pour que l'utilisateur puisse entrer sa ligne de commande.
*A noter que readline() renvoie une string qui a été allouée dynamiquement.*

2 - Il faut ensuite pouvoir délivrer à l'executeur des blocs d'instructions dans un format unique que nous définissons lors du parsing.
En suivant le fonctionnement interne de Bash, deux parties se dégagent : Le **lexer** et le **parser**.

3 - Enfin, on doit envoyer nos blocs d'instructions à l'**executor**. C'est une loop de processus enfants qui executent chacun un bloc d'instruction.
Quand cette fonction se termine, on revient à notre loop principale et le prompt est à nouveau renvoyé en attendant la prochaine commande utilisateur.

<img src="docs/Main.png" width="75%">

> ***srcs/minishell.c***

## Parsing
### LEXER
Le rôle du lexer est tout simplement de séparer chaque "mot" les uns des autres. Un mot est défini comme une suite de caractères autres que séparateurs (espaces, tabulations...). Cependant, une string définie par des doubles ou simples guillements compte comme un seul mot, guillemets inclus. C'est donc à ce moment qu'on vérifiera que chaque quote est bien fermée.
Avant cela, fera un premier check de la place des pipes **"|"** et redirections **"<" ">" "<<" ">>"**, que plusieurs ne se suivent pas ou ne commencent/termine pas notre ligne de commande. Il faut penser à tous les cas de figure possibles.

On va donc récupérer chaque mots et les ajouter dans une liste chaînée.

<img src="docs/lexer_struct.png" width="60%">


Chaque élément de notre liste chaînée comporte donc un pointeur vers une string que nous avons alloué dynamiquement avec **malloc()**. La difficulté aura été ici de trouver la taille de chaque malloc, spécialement dans le cas de nos quotes.

### PARSER

Une fois notre liste chaînée délivrée par le lexer, il va falloir la convertir en une seconde liste chaînée propre au parser, où chaque élément contiendra une instruction à envoyer à l'executeur par après.
Pour faire simple, nous séparons notre liste lexer à chaque pipe **|** et classons chaque élément à l'interieur d'un groupe selon 3 catégories :
- La commande suivie de ses arguments. Sous forme de double pointeur char, la commande sera toujours  l'index 0.
- L'option builtin (à *NULL* par défaut) est un pointeur sur fonction. En plus de remplir le **char, si un de nos built-in est reconnu cette option permettra d'envoyer les arguments à l'une de nos fonctions.
- L'option redirection (à *NULL* par défaut). Cette string est composée du token de redirection, un espace et le nom de fichier spécifié.

<img src="docs/lexer_to_parser.png" width="100%">

> ***Illustration trouvée sur le github de [Maia de Graaf](https://github.com/maiadegraaf)***


<img src="docs/parser_struct.png" width="60%">

> ***On trouve aussi un pointeur sur la liste chaînée contenant les variables d'environnement***



Après avoir malloc chaque nouvel élément de notre liste, nous entrons dans une boucle de tri jusqu'à rencontrer un pipe **|** ou la fin de notre première liste.
Ainsi, tant qu'un token de redirection n'est pas rencontré, nous récuperons chaque mot et le stockons dans le **char. Nous ne regardons pas si les commandes ou arguments sont valides, ce sera à la partie d'execution de le faire.

<img src="docs/get_cmd_elem.png" width="70%">

> ***srcs/parser/parser.c***



Quand aux redirections, plusieurs problèmes se sont posés ici. En effet, et pour correspondre au comportement de bash, si l'utilisateur a entré plusieurs redirections en entrée comme en sortie, il faut:
- tester chaque input et renvoyer une erreur si un fichier n'existe pas ou si l'utilisateur n'a pas les droits
- tester chaque output, les créer si inexistants.

On aura donc une fonction qui testera d'ouvrir le file descriptor actuel si on cherche à lui attribuer une nouvelle string.
Ainsi, l'executor se retrouvera soit avec la dernière redirection du bloc, soit avec la dernière qui a posé une erreur. Ce sera à l'executor de réouvrir la redirection qu'on lui a envoyé, et de renvoyer une erreur s'il y en a une. En attendant, on aura testé chaque FD et crée les fichiers dans le cas des outputs:

<img src="docs/try_open_rd.png" width="60%">

> ***srcs/parser/redirections.c***


### EXPANDER
A la fin de notre Parsing, nous envoyons les éléments de notre liste vers notre Expander. Le rôle de l'expander est de remplacer chaque variable d'environnement par sa valeur.
Par exemple la commande suivante devrait afficher votre nom d'utilisateur.
```bash
$> echo $USER
```
Cependant nous avons fait le choix d'octroyer à cette fonction la mission de supprimer les quotes, puisque ces deux actions sont étroitement liées. En effet, selon si les quotes sont doubles ou simples, les variables d'environnement ne seront pas interprétées.
On oubliera pas non plus la capacité de récupérer le dernier code d'erreur renvoyé par notre boucle :
```bash
$> echo $?
```

Aussi, si le pointeur builtin est different de NULL, alors nous n'enverrons pas nos commandes à l'expander. En effet dans certains cas certaines fonctionnalités comme **export** auront besoin des quotes. L'appel à l'expander se fera donc dans les built-ins mêmes.





















## Executor

Pour une définition des processus Unix, des commandes **fork()**, **execve()** et **dup2()** ainsi que toute la documentation relative, voir mon projet [pipex](https://github.com/ArnoChansarel/pipex). Tout l'executor est majoritairement basé sur ce projet.

Première particularité cependant, on commence par regarder le nombre de commandes entrées par l'utilisateur (càd nobre de pipes). Car en effet dans Bash les commandes builtins ne sont pas executées dans un processus forké si la ligne de commande ne comporte pas de pipes. POur arriver au même résultat il faut donc faire un premier check dès qu'on entre dans notre executor.

On lance notre boucle qui va créer les processus child via la fonction **fork()**, puis laisser le processus parent les attendre via **father_waits()**. Le jeu va être de bien gérer les copies de file descriptor via **dup2()**, car beaucoup de paramètres rentrent en compte. Si pipe il y a, si redirections il y a, etc... 

<img src="docs/fork_n_wait.png" width="70%">

> ***srcs/executor/executor.c***

C'est donc dans notre fonction **child()** qu'on ouvrira la dernière redirection récupérée dans le parser et qu'on renverra une erreur si besoin. Ensuite chaque fonction notée **n_process()** a pour but les copies de file descriptor. Enfin, la fonction **execute_process()** se déroule comme suit : 
- Retrouver notre executable de commande via la variable d'environement "PATH"
- La vérifier avec la fonction **access()**
- Transformer notre liste chaînée de variable d'environement en un tableau de string pour l'envoyer à **execve()**
- Appeler **execve()** qui terminera le processus une fois la commande executée

<img src="docs/child.png" width="70%">

> ***srcs/executor/executor.c***

## Built-ins

| Command | Description |
|---|---|
|`cd`| Change de répertoire courant, et update les variable `PWD` et `OLDPWD`. Si aucun argument, change pour `HOME`|
|`echo`| Affiche une ligne de texte. Si un argument `-n` est spécifié, pas de retour à la ligne.|
|`env` | Affiche la liste de nos variables d'environnement. |
|`exit`| Ferme le programme. Accepte un argument numérique qui sera la valeur de retour. |
|`export`| Exporte une nouvelle variable d'environement. Si aucun argument, doit afficher la liste des variables exportées classées par ordre alphabetique.|
|`pwd`| Affiche le répertoire courant sous forme de chemin absolu. |
|`unset`| Supprime la variable d'environement spécifiée. |

## Problèmes rencontrés
:construction_worker_man: :construction_worker_woman:
Projet éternellement perfectible

## Documentation utilisée

### Github
- [Maia de Graaf](https://github.com/maiadegraaf/minishell)
- [Alejandro Pérez](https://github.com/madebypixel02/minishell)
- [Swoorup Joshi](https://github.com/Swoorup/mysh)

### Docs
- [Une doc comprehensible et digeste par l'etre humain sur le protocole HTTP](https://www.jmarshall.com/easy/http/#structure)
- [Une autre doc](https://www.garshol.priv.no/download/text/http-tut.html#creq)

### Videos
- [fd, dup()/dup2() system call tutorial](https://www.youtube.com/watch?v=EqndHT606Tw)
- [Fork() system call tutorial](https://www.youtube.com/watch?v=xVSPv-9x3gk)