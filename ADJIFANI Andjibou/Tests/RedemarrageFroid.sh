#!/bin/bash

#Script de Redémarrage à froid


echo "---------------REDEMARRAGE A FROID DE L'APPLICATION-------";
echo "";

echo "ATTENTION !! L'application va être arrêter."
echo "La base de donnée sera remise à l'état initial, tout en
conservant votre configuration système, puis il redemarrera votre application.";
echo "";

echo -n "Êtes-vous sûr de vouloir faire cet opération ? [Y/N]: ";

read OuiNon 
		

if [ "$OuiNon" = "y" -o "$OuiNon" = "Y" ]; then

	echo "Arrêt de l'application...";

		killall gedit			#Arrêt de l'application. 
							
							#Mise à l'état initial de la base de donné
       		sleep 5
		echo "Redémarrage de l'applicatio...";	#Redémarrage de l'application au bout de 5 sec
		gedit 


elif [ "$OuiNon" = "n" -o "$OuiNon" = "N" ]; then
   	
	echo "L'operation est annuler!";
	

else 

	echo "Il faut taper Y ou N!! Pas $OuiNon. Veuillez recommencer...";
	

fi


exit 0
