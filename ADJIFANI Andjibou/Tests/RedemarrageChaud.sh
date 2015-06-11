#!/bin/bash

#Script de Redémarrage à chaud


echo "---------------REDEMARRAGE A CHAUD DE L'APPLICATION-------";
echo "";

echo "ATTENTION !! L'application sera arrêter puis redémarrer toute en 
conservant votre configuration";
echo "";

echo -n "Êtes-vous sûr de vouloir faire cet opération ? [O/N]: ";

read OuiNon 
	


if [ "$OuiNon" = "o" -o "$OuiNon" = "O" ]; then

	echo "Arrêt de l'application...";
	
		cd /home/operateur/build-Locker_Control-Desktop-Debug/
	
	echo " ";	

		killall -TERM ./Locker_Control		#fermeture de l'application et au bout de 5 sec, il la relance
       		sleep 5

		echo "Redémarrage de l'applicatio...";

		cd /home/test/Locker_Control
		export DISPLAY=:0.0
		./Locker_Control&  			#Démarrage de l'application


elif [ "$OuiNon" = "n" -o "$OuiNon" = "N" ]; then
   	
	echo "L'operation est annuler!";
	

else 

	echo "Il faut taper O ou N!! Pas $OuiNon. Veuillez recommencer...";
	

fi


exit 0

