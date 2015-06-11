#!/bin/bash

#Script de Mise à jour: appuis """http://doc.ubuntu-fr.org/deplacer_root"""


echo "---------------MISE A JOUR DU SYSTEME-------";
echo "";


echo "ATTENTION !! La partition inutilisée va être montée et sera mise à jour.
      la nouvelle partition sera prise en charge après redémarrage de la machine";

echo "";

echo -n "Êtes-vous sûr de vouloir faire cet opération ? [O/N]: ";

 
read OuiNon
 
##########################################################################################

#mes deux repertoires ou vont se faire ma copie de fichier
 
source="/mnt/Part_UTILISE/" 
destination="/mnt/Part_INUTILISE/"

#########################################################################################

#mes deux partitions 

Part_UTILISE="/dev/sda2"			#ma racine (/)
Part_INUTILISE="/dev/sda3"			#ma nouvelle partition 

#########################################################################################

#mes deux repertoires ou seront monter mes deux partitions

OLD_PARTITION="/mnt/Part_UTILISE"
NEW_PARTITION="/mnt/Part_INUTILISE"

##########################################################################################		

if [ "$OuiNon" = "o" -o "$OuiNon" = "O" ]; then				#Premeir If

echo "Debut de l'opération cela peut prendre quelques minutes... ";

###########################################################################################

#monte ma partition inutilisé et ma partition utilisé dans le dossier /mnt

			echo"monte l'ancienne partition /dev/sda2 dans le dossier /mnt/Part_UTILISE";
			mount $Part_INUTILISE $NEW_PARTITION   	#monte ma partition inutilisé 

			echo " monte la nouvelle partition /dev/sda3 dans le dossier /mnt/Part_INUTILISE";
			mount $Part_UTILISE $OLD_PARTITION 	#monte ma partition utilisé 
			
			echo -n "montage des partitions réussie, continuer ? [O/N]: ";

read ON	

	if [ "$ON" = "o" -o "$ON" = "O" ]; then					#Second If

		
##########################################################################################	

	#copie tous les fichiers de l'ancienne partition vers la nouvelle
			
		echo "Copie des fichiers de l'ancienne partition vers la nouvelle avec rsync, cela peut prendre quelques minutes...";
 

		rsync -axvz $source $destination

		echo -n "Copie des fichiers de l'ancienne partition vers la nouvelle réussie, continuer ? [O/N]: ";

	read ON	


		if [ "$ON" = "o" -o "$ON" = "O" ]; then					#Troisième If

###########################################################################################

		#Configuration du grub: mise à jour du menu démarrage grub

			echo "mise à jour du menu démarrage GRUB";		

 			mount /dev/sda3 /mnt
 			mount --bind /dev /mnt/dev
 			mount --bind /proc /mnt/proc
		 	mount --bind /sys /mnt/sys
 			chroot /mnt /bin/bash	
			
		#mise à jour du grub

			grub-mkconfig -o /boot/grub/grub.cfg
			grub-install /dev/sda

################################################################################################
 
 
		echo "Fin de l'opération, le systeme est mise a jour ";

###############################Redémarrage système##########################
	
		echo -n "voulez vous redémarrer la machine maintenant ? [O/N]: ";

		read OuiNon


			if [ "$OuiNon" = "o" -o "$OuiNon" = "O" ]; then	 #Quatrième If

			
			echo "*************REDEMARRAGE DE LA MACHINE***********";

			sleep(5)

			shutdown -r now 	#Redémarrage maintenant

			elif [ "$OuiNon" = "n" -o "$OuiNon" = "N" ]; then

			fi						#fin Quatrième If



###################SI l'on souhaite pas continuer après la Copie des fichiers de l'ancienne partition vers la nouvelle##################################################



			elif [ "$OuiNon" = "n" -o "$OuiNon" = "N" ]; then

			#démonte mes partition

			echo"démonte l'ancienne partition /dev/sda2 dans le dossier /mnt/Part_UTILISE";
			mount $NEW_PARTITION $Part_INUTILISE  	#démonte ma partition inutilisé 

			echo "démonte la nouvelle partition /dev/sda3 dans le dossier /mnt Part_INUTILISE";
			mount $OLD_PARTITION $Part_UTILISE  	#démonte ma partition utilisé 
			
			echo "démontage des partitions réussie";

		fi									#fin Troisième If

###################SI le montage est faite et que l'on souhaite pas continuer###################

	elif [ "$OuiNon" = "n" -o "$OuiNon" = "N" ]; then

#démonte mes partition

			echo"démonte l'ancienne partition /dev/sda2 dans le dossier /mnt/Part_UTILISE";
			mount $NEW_PARTITION $Part_INUTILISE  	#démonte ma partition inutilisé 

			echo "démonte la nouvelle partition /dev/sda3 dans le dossier /mnt/Part_INUTILISE";
			mount $OLD_PARTITION $Part_UTILISE  	#démonte ma partition utilisé 
			
			echo "démontage des partitions réussie";

	fi									#fin Second If

#######################Si on annule la mise à jour###################################


elif [ "$OuiNon" = "n" -o "$OuiNon" = "N" ]; then
   	
	echo "L'operation est annuler!";
	

else 

	echo "Il faut taper O ou N!! Pas $OuiNon. Veuillez recommencer...";
	

fi									#fin Premier If


exit 0
