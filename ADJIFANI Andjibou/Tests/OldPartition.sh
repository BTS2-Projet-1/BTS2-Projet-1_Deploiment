#!/bin/bash

#Script de Mise à jour: Revenir à l'ancienne partition






#démonte mes partition

			echo"démonte l'ancienne partition /dev/sda2 dans le dossier /mnt/Part_UTILISE";
			umount $NEW_PARTITION $Part_INUTILISE  	#démonte ma partition inutilisé 

			echo "démonte la nouvelle partition /dev/sda3 dans le dossier /mnt Part_INUTILISE";
			umount $OLD_PARTITION $Part_UTILISE  	#démonte ma partition utilisé 
			


		#Configuration du grub: mise à jour du menu démarrage grub

			echo "mise à jour du menu démarrage GRUB";		

 			mount /dev/sda2 /mnt
 			mount --bind /dev /mnt/dev
 			mount --bind /proc /mnt/proc
		 	mount --bind /sys /mnt/sys
 			chroot /mnt /bin/bash	
			
		#mise à jour du grub

			grub-mkconfig -o /boot/grub/grub.cfg
			grub-install /dev/sda

			Compt1=$(($Compt+1))

 			echo "$Compt1" > /home/test/CompteurDeLaMiseJour/Compteur.txt		#Écrire 1 dans le fichier
 
			echo "Fin de l'opération, le systeme est mise a jour ";

			echo -n "voulez vous redémarrer la machine maintenant ? [O/N]: ";

			read OuiNon


			if [ "$OuiNon" = "o" -o "$OuiNon" = "O" ]; then	 		#if redemarrer la machine

			
				echo "*************REDEMARRAGE DE LA MACHINE***********";

				sleep 5
	
				shutdown -r now 					#Redémarrage maintenant

				elif [ "$OuiNon" = "n" -o "$OuiNon" = "N" ]; then
			
				echo "";

			fi
