# Competitive_prog

Ce dépôt contient des implémentations de diverses primitives de concurrence et de structures de données, principalement en C et Java, organisées en laboratoires (`lab3`, `lab5`). Ces implémentations sont conçues pour la programmation compétitive et l'apprentissage des concepts de concurrence.

## Structure du dépôt

Le dépôt est organisé en répertoires de laboratoires, chacun contenant des exercices ou des projets spécifiques liés à la concurrence.

### `lab3`

Ce répertoire contient des implémentations en C de :

*   **Queues bloquantes (Blocking Queues)** : Implémentations utilisant des sémaphores (`sem_blocking_queue.c`) et des variables de condition (`cond_blocking_queue.c`).
*   **Buffers bornés (Bounded Buffers)** : Une implémentation de base d'un buffer borné (`bounded_buffer.c`).
*   **Utilitaires** : Fichiers d'aide (`utils.c`, `utils.h`) et un programme principal pour tester les queues bloquantes (`main_blocking_queue.c`).
*   **Fichiers de test** : Plusieurs fichiers `test-XX.txt` pour valider les implémentations.

### `lab5`

Ce répertoire contient des implémentations en C et Java, axées sur des concepts plus avancés de concurrence, notamment :

*   **Thread Pool** : Implémentation d'un pool de threads (`thread_pool.c`, `thread_pool.h`).
*   **Executor** : Un système d'exécution de tâches (`executor.c`, `executor.h`, `ExecutorMain.java`, `ExecutorRunnable.java`).
*   **Future** : Implémentation du concept de Future pour la gestion des résultats asynchrones (`future.c`, `future.h`).
*   **Scénarios et Tâches** : Fichiers pour définir des scénarios et des tâches (`scenario.c`, `scenario.h`, `task.c`, `task.h`).
*   **Queues bloquantes et Buffers bornés** : Des implémentations similaires à `lab3` mais potentiellement adaptées ou étendues pour `lab5`.
*   **Fichiers de test** : Plusieurs fichiers `test-XX.txt` pour valider les implémentations.

## Compilation et Exécution

Chaque répertoire de laboratoire contient un `Makefile` pour faciliter la compilation des programmes C. Pour les programmes Java, une compilation et exécution standard via `javac` et `java` est attendue.

### Exemple pour `lab3` (C)

Pour compiler :

```bash
cd Competitive_prog/lab3
make
```

Pour exécuter (par exemple, le programme principal de la queue bloquante) :

```bash
./main_blocking_queue
```

### Exemple pour `lab5` (Java)

Pour compiler :

```bash
cd Competitive_prog/lab5
javac *.java
```

Pour exécuter (par exemple, le programme principal de l'executor) :

```bash
java ExecutorMain
```

## Contribution

Les contributions sont les bienvenues. Veuillez suivre les pratiques de codage établies et soumettre des pull requests pour toute amélioration ou correction de bug.

## Licence

Ce projet est sous licence MIT. Voir le fichier `LICENSE` pour plus de détails (si présent).

