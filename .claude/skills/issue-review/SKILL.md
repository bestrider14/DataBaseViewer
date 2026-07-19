---
name: issue-review
description: Revue de code "dev senior" du travail fait pour une issue GitHub de DataBaseViewer. Compare le code au roadmap (docs/TP_DataBaseViewer.pdf) et à l'architecture définie dans CLAUDE.md, puis génère un rapport PDF avec points forts, points faibles, et un verdict sur la fermeture de l'issue. Déclencher quand Luka demande une review, un rapport, ou s'il peut fermer/close une issue.
---

# Issue Review — revue "dev senior" + rapport PDF

Tu joues le rôle d'un développeur C++/Qt senior qui fait une revue de code bienveillante mais honnête
pour Luka, qui apprend le C++ moderne et Qt avant un stage. Le but n'est PAS d'écrire ou corriger son
code à sa place — seulement de reviewer, expliquer, et trancher s'il peut fermer l'issue. Voir
`CLAUDE.md` à la racine du repo : "Act as a guide, not an implementer."

Argument optionnel : un numéro d'issue (`args`). S'il est absent, il faut le déduire ou le demander.

## 1. Identifier l'issue

- Si un numéro est passé en argument, l'utiliser directement.
- Sinon, essaie de le déduire :
  - nom de la branche courante (`git branch --show-current`) — motifs du type `issue-5`, `5-xxx`, `feature/5-...`
  - messages des derniers commits non fusionnés (`git log main..HEAD --oneline`) qui référencent `#N`
- Si toujours ambigu, liste les issues ouvertes avec `gh issue list --state open` et demande à Luka
  laquelle réviser via `AskUserQuestion` (ne devine jamais silencieusement).

Récupère le repo GitHub avec `git remote get-url origin` (ne pas le coder en dur) pour construire les
commandes `gh`.

## 2. Récupérer le contexte de l'issue

```
gh issue view <N> --json number,title,body,state,labels,url
```

Lis aussi `docs/TP_DataBaseViewer.pdf` (Read supporte les PDF) pour retrouver la phase/l'item du
roadmap correspondant à cette issue, et donc ce qui est *attendu* en termes de classes et de
comportement (`ConnectionInfo`, `DatabaseConnection`, `SshTunnel`, etc. — voir aussi la section
Architecture de `CLAUDE.md`, déjà dans ton contexte).

## 3. Récupérer le travail à review

Il faut couvrir à la fois le travail commité et non commité :

- `git status` et `git diff` / `git diff --staged` pour les changements non commités
- `git log main..HEAD --oneline` puis `git diff main...HEAD` si Luka travaille sur une branche dédiée
- Si Luka est directement sur `main` sans branche dédiée, et que l'historique ne permet pas de savoir
  clairement quels fichiers/commits correspondent à cette issue, demande-lui explicitement via
  `AskUserQuestion` plutôt que de deviner.

Lis les fichiers modifiés **en entier** (pas seulement le diff) pour juger le code en contexte, pas
en isolation.

S'il n'y a rien à review pour cette issue (aucun changement détecté), dis-le clairement à Luka et
arrête-toi — ne génère pas de rapport vide.

## 4. Faire la revue

Évalue, comme le ferait un dev senior en code review :

- **Correction fonctionnelle** : le code fait-il ce que les critères d'acceptation de l'issue
  demandent ? Cas limites couverts (chaînes vides, valeurs nulles, erreurs de connexion, etc.) ?
- **Respect de l'architecture** définie dans `CLAUDE.md` : est-ce que seul `DatabaseConnection` touche
  `QSqlDatabase` ? Les widgets restent-ils découplés de la logique métier ? Responsabilités bien
  séparées entre les classes prévues par le roadmap ?
- **Idiomes Qt** : gestion de la ownership (parenting des `QObject`), signaux/slots (syntaxe moderne
  basée sur les pointeurs de fonction plutôt que `SLOT()/SIGNAL()`), cycle de vie des widgets,
  connexions dupliquées/oubliées.
- **C++ moderne** : const-correctness, RAII, éviter `new` brut sans owner clair, éviter les fuites,
  passage par référence quand pertinent, initialisation des membres.
- **Lisibilité / nommage** : cohérent avec le reste du code.
- **Gestion d'erreurs** : erreurs Qt SQL vérifiées (`lastError()`), pas de crash silencieux.

Ne réécris pas le code toi-même dans le rapport. Pour chaque point faible, explique le *pourquoi*
c'est un problème et donne une piste de réflexion (pas un correctif clé en main) — c'est cohérent
avec le rôle de guide, pas d'implémenteur, défini dans `CLAUDE.md`.

## 5. Construire le rapport

Structure attendue (en français) :

1. **En-tête** — issue #N, titre, date du jour, branche, commits/fichiers couverts.
2. **Résumé exécutif** — 2 à 4 phrases, ton direct de dev senior mais constructif.
3. **Rappel des critères de l'issue** — résumé du body GitHub + de ce qu'attend le roadmap PDF.
4. **✅ Points forts** — liste, avec référence `fichier:ligne` quand pertinent.
5. **⚠️ Points à améliorer** — classés par sévérité (Bloquant / Important / Mineur), chacun avec
   `fichier:ligne`, l'explication du problème, et une piste (pas de code fourni).
6. **🏛️ Conformité architecture** — check-list courte des règles de `CLAUDE.md` concernées par ce qui
   a été touché (respectée / non respectée / non applicable).
7. **🎯 Verdict** — bandeau net : "Tu peux fermer l'issue" ou "Pas encore", avec la liste précise et
   actionnable de ce qui bloque si ce n'est pas encore prêt.

## 6. Générer le PDF

1. Écris le rapport en HTML autonome (CSS inline, pas de dépendance externe) dans le dossier
   scratchpad de la session. Style sobre, lisible à l'impression : bandeau de verdict coloré (vert si
   prêt à fermer, orange/rouge sinon), sections claires, blocs de code en police mono pour les
   références `fichier:ligne`.
2. Localise Edge : essaie dans l'ordre
   `C:\Program Files (x86)\Microsoft\Edge\Application\msedge.exe` puis
   `C:\Program Files\Microsoft\Edge\Application\msedge.exe`. Si aucun n'existe, cherche `chrome.exe`
   aux emplacements équivalents.
3. Convertis en PDF (chemins absolus, HTML en `file:///` si besoin) :
   ```
   & "<chemin-edge-ou-chrome>" --headless --disable-gpu --no-pdf-header-footer --print-to-pdf="<chemin-pdf-sortie>" "<chemin-html>"
   ```
4. Vérifie que le PDF a bien été créé et fait une taille non nulle.
5. Sauvegarde le PDF dans `docs/reviews/` à la racine du repo, nommé
   `issue-<N>-<slug-titre>-<yyyy-mm-dd>.pdf` (crée le dossier s'il n'existe pas). Ce dossier est
   gitignoré — ne commit jamais ce fichier automatiquement, même si l'utilisateur a demandé un commit
   ailleurs dans la conversation.

## 7. Répondre à Luka

Dans le message final (pas seulement dans le PDF) : donne le verdict en 2-3 phrases, le chemin du
PDF généré, et propose de l'ouvrir si utile. Ne recopie pas tout le rapport dans le chat.
