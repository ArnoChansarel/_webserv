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
