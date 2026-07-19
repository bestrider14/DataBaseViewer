# DataBaseViewer

Client de base de données multiplateforme, dans l'esprit de pgAdmin, développé en C++17 / Qt Widgets.

Ce projet est un projet d'apprentissage personnel visant à se remettre à niveau en C++ et Qt avant un stage. Le but est de se connecter à une base de données (locale ou distante via un tunnel SSH, par exemple une base tournant dans un conteneur Docker), d'explorer ses tables, et d'y effectuer des opérations CRUD via une interface graphique. Un éditeur SQL libre et un constructeur de requêtes graphique sont prévus comme objectifs plus avancés.

## Fonctionnalités prévues

- Connexion à une base de données (SQLite, PostgreSQL, MySQL), en local ou via tunnel SSH.
- Exploration des tables d'une base connectée.
- Affichage, édition, ajout et suppression de données via l'interface graphique.
- Filtrage et tri des données affichées.
- Sauvegarde de profils de connexion.
- Connexions multiples simultanées (multi-onglets).
- Éditeur SQL libre.
- Constructeur de requêtes SQL graphique (stretch goal).

Le détail de la feuille de route est suivi via les [issues et milestones GitHub](https://github.com/bestrider14/DataBaseViewer/issues) du dépôt.

## Prérequis

- Qt 6 (modules Widgets et SQL)
- Un compilateur supportant le C++17
- Qt Creator (recommandé) ou `qmake` + un outil de build (`make`/`nmake`/`mingw32-make`)
- Le pilote Qt SQL correspondant au moteur de base de données ciblé (`QSQLITE`, `QPSQL`, `QMYSQL`, ...)

## Compilation

Avec Qt Creator : ouvrir `DataBaseViewer.pro` et lancer le build.

En ligne de commande :

```
qmake DataBaseViewer.pro
make        # ou mingw32-make / nmake selon la plateforme
```

## État du projet

Le projet est en cours de développement initial (voir la feuille de route ci-dessus). Aucune fonctionnalité de connexion ou d'affichage n'est encore implémentée.
