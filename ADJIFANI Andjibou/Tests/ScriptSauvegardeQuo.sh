#!/bin/bash
###################################
# Jour
###################################
jour=$(date +%A)
source="REPERTOIRE_A_SAUVER"
destination="REPERTOIRE_DE_SAUVEGARDE"
###################################

#monte la partition racine sur le répertoire /home/console/Part_UTILISE 
mount /dev/sda2 //home/console/Part_UTILISE 
# Sauvegarde des fichiers de REPERTOIRE_A_SAUVER
###################################
rsync -avz --exclude='/*/.local' $source ssh 192.168.58.26:$destination
###################################
#démonte la partition racine
umount /home/console/Part_UTILISE  /dev/sda2

exit 0
