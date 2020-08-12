/*
 * File main.h
 * Copyright © ENAC, 2013 (Antoine Varet).
 *
 * ENAC's URL/Lien ENAC : http://www.enac.fr/.
 * sourcesonoff's URL : http://www.recherche.enac.fr/~avaret/sourcesonoff/
 * Mail to/Adresse électronique : avaret@recherche.enac.fr et nicolas.larrieu@enac.fr
 *
 
**fr**
 
Cette œuvre est une mise en œuvre du programme sourcesonoff, c'est-à-dire un programme permettant de générer du trafic réseau réaliste à l'aide de sources ON/OFF
Ce programme informatique est une œuvre libre, soumise à une double licence libre
Etant précisé que les deux licences appliquées conjointement ou indépendamment à
 l’œuvre seront, en cas de litige, interprétées au regard de la loi française et soumis à la compétence des tribunaux français ; vous pouvez utiliser l’œuvre, la modifier, la publier et la redistribuer dès lors que vous respectez les termes de l’une au moins des deux licences suivantes :  
-  Soit la licence GNU General Public License comme publiée par la Free Software
     Foundation, dans sa version 3 ou ultérieure ;  
-  Soit la licence CeCILL comme publiée par CeCILL, dans sa version 2 ou ultérieure.

Vous trouverez plus d'informations sur ces licences aux adresses suivantes :
-  http://www.recherche.enac.fr/~avaret/sourcesonoff/gnu_gplv3.txt
     ou fichier joint dans l'archive;
-  http://www.recherche.enac.fr/~avaret/sourcesonoff/Licence_CeCILL_V2-fr.txt
     ou fichier joint dans l'archive.
 
**en**
 
This work is an implementation of the sourcesonoff program, thus a program to generate realistic network data trafic with ON/OFF sources
This library is free software under the terms of two free licences. In case of problem, the licences will be interpreted with the french law and submitted to the competence of the french courts; you can use the program, modify it, publish and redistribute it if you respect the terms of at least one of the next licenses:
-  The GNU Lesser General Public License v3 of the Free Software Fundation,
-  The CeCILL License, version 2 or later, published by CeCILL.
 
See more information about the licenses at:
-  http://www.recherche.enac.fr/~avaret/sourcesonoff/gnu_gplv3.txt or local file;
-  http://www.recherche.enac.fr/~avaret/sourcesonoff/Licence_CeCILL_V2-fr.txt or local file.
    
*/



/*
 * main.h
 *
 *  Created on: 5 oct. 2012
 *      Author: avaret
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <error.h> 
#include <errno.h> 
#include <getopt.h>
#include <sys/wait.h>

//#include "common.h"

#include "network.h"
#include "distribs.h"


#define ARGNONE 0 /* No argument required */
#define ARGREQ	1 /* An argument is expected/mandatory */
#define ARGOPT	2 /* An optional argument may be present */


/* MAX_ALLOWED_SOURCES limits the maximum number of different sources */
#define MAX_ALLOWED_SOURCES 1000



/* Function prototypes */

POneSourceOnOff initializeOneSourceOnOff(POneSourceOnOff nextOne);
bool checkConsistencyOfTDistrib(Tdistrib dist);
bool checkConsistencyOfOneSourceOnOff(TOneSourceOnOff source);
void checkConsistencyOfAllSourcesOnOff(void);
Edistrib stringtoEdistrib(const char * str);
void createNewSourcesSet(void);
void parseCmdLine(int argc, char **argv);
POneSourceOnOff freeThisAndReturnNext(POneSourceOnOff s);
void freeAll(POneSourceOnOff s);
void goChild(TOneSourceOnOff src, int index);
void startAllSources(void);

int main(int argc, char*argv[]);


#endif /* MAIN_H_ */
