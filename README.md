# 📊 Script de Surveillance des Ressources

Ce projet est un outil de surveillance des ressources système sous Linux.  
Il inclut un script Bash (`monitor_system.sh`) pour la collecte des données et un programme en C (`usage_plot.c`) pour la visualisation graphique via **Gnuplot**.

---

## 🛠️ Fonctionnalités

- 🔁 Surveillance en temps réel des ressources (CPU, RAM, disque)
- 🧾 Génération et stockage des rapports d’utilisation
- 📈 Visualisation des données sous forme de graphiques
- ⏱️ Automatisation via **Crontab**
- 💻 Interface en ligne de commande avec plusieurs options

---

## 📌 Prérequis

Assurez-vous d’avoir installé :

- 🐧 Linux (Ubuntu recommandé)
- 🖥️ Bash (par défaut sous Linux)
- 📉 Gnuplot pour la visualisation
- 🧱 GCC pour compiler le programme C
- 🕒 Crontab pour l’exécution automatique

---

## 🚀 Installation & Utilisation

```bash
# 1️⃣ Cloner le dépôt
git clone https://github.com/Drissrad/script-surveillance-ressources.git
cd script-surveillance-ressources

# 2️⃣ Donner les permissions d'exécution
chmod +x monitor_system.sh

# 3️⃣ Compiler le programme C
gcc usage_plot.c -o usage_plot -lm

# 🔍 Lancer la surveillance
./monitor_system.sh

# ✅ Options disponibles :
./monitor_system.sh -h           # Afficher l'aide
./monitor_system.sh -d DATE      # Spécifier une date de début
./monitor_system.sh -e DATE      # Spécifier une date de fin
./monitor_system.sh -r TYPE      # Spécifier la ressource (CPU, RAM, DISK)

# 🧪 Exemple
./monitor_system.sh -d 2024-04-01 -e 2024-04-10 -r CPU

# 📊 Visualiser les données
./usage_plot data.txt

# ⚙️ Automatiser avec Crontab (exécution chaque heure)
crontab -e
# Ajouter la ligne suivante :
0 * * * * /chemin/vers/monitor_system.sh
