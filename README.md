# 📊 Script de Surveillance des Ressources

Ce projet est un outil de surveillance des ressources système sous Linux. Il inclut un script Bash (`monitor_system.sh`) pour la collecte des données et un programme en C (`usage_plot.c`) pour la visualisation graphique via **Gnuplot**.

## 🛠️ Fonctionnalités

- Surveillance en temps réel des ressources (CPU, RAM, disque)
- Génération et stockage des rapports d’utilisation
- Visualisation des données sous forme de graphiques
- Automatisation via **Crontab**
- Interface en ligne de commande avec plusieurs options

## 📌 Prérequis

Avant d'exécuter le projet, assurez-vous d'avoir :

- **Linux** (Ubuntu recommandé)
- **Bash** installé (par défaut sous Linux)
- **Gnuplot** pour la visualisation des données
- **GCC** pour compiler le programme en C
- **Crontab** pour l’exécution automatique

## 🚀 Installation

### 1️⃣ Cloner le dépôt
```bash
git clone https://github.com/Drissrad/script-surveillance-ressources.git
cd script-surveillance-ressources
### 2️⃣ Donner les permissions d'exécution
```chmod +x monitor_system.sh
###3️⃣ Compiler le programme C
```gcc usage_plot.c -o usage_plot -lm
📖 Utilisation
🔍 Surveillance des ressources
Lancer le script Bash :
``` ./monitor_system.sh
Le script prend en charge plusieurs options :
``` ./monitor_system.sh -h        # Afficher l'aide
``` ./monitor_system.sh -d DATE   # Spécifier une date de début
``` ./monitor_system.sh -e DATE   # Spécifier une date de fin
``` ./monitor_system.sh -r TYPE   # Spécifier une ressource (CPU, RAM, DISK)
Exemple :
``` ./monitor_system.sh -d 2024-04-01 -e 2024-04-10 -r CPU
📊 Visualisation des données
Exécuter le programme en C pour générer les graphiques :
``` ./usage_plot data.txt
⚙️ Automatisation avec Crontab
Vous pouvez programmer l'exécution automatique du script en l'ajoutant à Crontab :
```  crontab -e
Ajoutez la ligne suivante pour exécuter le script toutes les heures :

``` 0 * * * * /chemin/vers/monitor_system.sh
🛠️ Fonctionnalités techniques
Bash pour la collecte des données système

C (GCC) pour la gestion et l’affichage des graphiques

Gnuplot pour la visualisation

Gestion des logs et stockage des rapports

Traitement des arguments en ligne de commande avec getopts

Gestion des erreurs et logs détaillés



