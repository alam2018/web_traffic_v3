/*
 * File main.c
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
 * main.c
 *
 *  Created on: 5 oct. 2012
 *      Author: avaret
 */

#include "main.h"

#include "usage.h"
#include "long_usage.h"
//#include <pthread.h>

#include "tx_rx.h"


//POneSourceOnOff completeSetOfSourcesToUse = NULL;


/* Verbosity level: 0=normal, >0=speaker */
int verbose = 0;

/* Dry_Run mode enables the program to show the distribution and the random values,
 * it does no network operation (send nothing and receive nothing) */
int dry_run = 0;

POneSourceOnOff initializeOneSourceOnOff(POneSourceOnOff nextOne)
{
	POneSourceOnOff newsource = malloc(sizeof(TOneSourceOnOff));
	memset(newsource, 0, sizeof(TOneSourceOnOff));

	if (nextOne)
		newsource->number_of_source = nextOne->number_of_source + 1;
	else
		newsource->number_of_source = 1;

	newsource->receiver 		= true;
	newsource->ipv4 		= true;

	newsource->Don.type		= pareto;//uniform;
//	newsource->Don.max 		= 1500; /* in bytes per timeunit */
	newsource->Don.max 		= 1500; /* in bytes per timeunit */
	newsource->Don.min 		= 50; /* in bytes per timeunit */
	newsource->Don.lambda 	= 1.0;
	newsource->Don.k 		= 1.0;
	newsource->Don.avg 		= (double) newsource->Don.max * 0.5;
	newsource->Don.sigma 	= (double) newsource->Don.max * 0.1;
	newsource->Don.alpha 	= 1.1;
	newsource->Don.xm 		= 0.081;//4;

	newsource->Doff.type		= normal;
//	newsource->Doff.max 		= 100*MS_IN_NS; /* in ns */
//	newsource->Doff.max 		= (myInteger) (0.1 * MS_IN_NS); /* in ns */
//	newsource->Doff.max 		= (myInteger) ((1.95+1.95*0.5) * MS_IN_NS); /* in ns */
//	newsource->Doff.max 		= (myInteger) (2 * 0.00195 * SEC_TO_MICRP_SEC); /* in ns */
	newsource->Doff.max 		= (myInteger) ( 2*0.00195 * SEC_TO_MICRP_SEC); /* in ns */
	newsource->Doff.min 		= (myInteger) (0.00001 * SEC_TO_MICRP_SEC); /* in ns */
	newsource->Doff.lambda 		= 0.28;
	newsource->Doff.k 			= 0.74;
//	newsource->Doff.avg 		= (double) 1950000.0;
	newsource->Doff.avg 		= (double) newsource->Doff.max * 0.5;
//	newsource->Doff.avg 		= (double) (0.00195 * SEC_TO_MICRP_SEC);
	newsource->Doff.sigma 		= (double) newsource->Doff.max * 0.1;
	newsource->Doff.alpha 		= 0.36;
	newsource->Doff.xm 			= 1.0;

	newsource->udp_delay_precision 	= DEFAULT_DELAY_PRECISION;
	newsource->internal_buffer_size = DEFAULT_INTERNAL_BUFFER_SIZE;
	newsource->port_number 		= DEFAULT_PORT_NUMBER;

	newsource->turns 		= TOTAL_NUMBER_OF_PACKET;

	newsource->next 		= (void *) nextOne;
	return newsource;
}


/* Return a distribution from a string */
Edistrib stringtoEdistrib(const char * str)
{
	if (str == NULL )
		return erroneous;

	switch (str[0]) {
	case 'c':
	case 'C':
		return constant;
	case 'u':
	case 'U':
		return uniform;
	case 'e':
	case 'E':
		return exponential;
	case 'g': /* Gaussian = Normal */
	case 'G':
	case 'n':
	case 'N':
		return normal;
	case 'w':
	case 'W':
		return weibull;
	case 'p':
	case 'P': {
		if (strlen(str) < 2)
			return erroneous;

		switch (str[1]) {
		case 'o':
		case 'O':
			return poisson;
		case 'a':
		case 'A':
			return pareto;
		}

		return erroneous;
	}
	default:
		return erroneous;
	}
}


void goChild(TOneSourceOnOff src, int index)
{
	dry_run = 1;
	Pdistribdata dataOn[TOTAL_USER], dataOff[TOTAL_USER];

	if(src.rand_seed!=0)
		srand(src.rand_seed);
	else
		srand((unsigned int) rand() +src.number_of_source);


//	if (dry_run || !src.receiver) {
	if (dry_run) {

		dataOn[index]  = getDistribution(src.turns, src.Don);
		dataOff[index] = getDistribution(src.turns, src.Doff);
	} else {
		dataOn[index]  = NULL;
		dataOff[index] = NULL;
	}

	if (dry_run) {
		printf("Source #%d: ", src.number_of_source);
		if(src.receiver)
			printf("receiver ");
		else
			printf("transmitter (to <%s>) ", src.destination);

		if(src.tcp)
			printf("TCP (VBR) ");
		else
			printf("UDP (CBR) ");

		printDistributions(src.Don, dataOn[index], 1, verbose > 0, index);
		printDistributions(src.Doff, dataOff[index], 0, verbose > 0, index);
	} else {
		nanosleep_manually_compensated(src.delay_before_start);

		runNetworkWithDistributions(src, dataOn[index], dataOff[index]);
	}

//	free_distribdata(dataOn[index]);
//	free_distribdata(dataOff[index]);
}

void thread_launcher ()
{
    pthread_t tid[TOTAL_USER], tid_rec;
/*    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("Mutex initialization failed.\n");
        return 1;
    }*/
    int i;
    if( pthread_create(&tid_rec, NULL, receiver, NULL) != 0 )
    {
        printf("Failed to create thread\n");
        exit(EXIT_FAILURE);
    }

    for (i=0; i<TOTAL_USER; i++)
    {
        if( pthread_create(&tid[i], NULL, start_transmission, &i) != 0 )
        {
            printf("Failed to create thread\n");
            exit(EXIT_FAILURE);
        } else
        	usleep (10000);//usleep (1600);
//        	sleep (1);
    }



//	receiver ();
    for (i=0; i<TOTAL_USER; i++)
    {
    	(void) pthread_join (tid[i], NULL);
    }
}


void startAllSources()
{
	signal(SIGCHLD, SIG_IGN );
	POneSourceOnOff src[TOTAL_USER];
	int i;
	for (i=0; i< TOTAL_USER; i++)
	{
		src[i] = initializeOneSourceOnOff(completeSetOfSourcesToUse[i]);

		goChild(*src[i], i);
	}
//	createNewSourcesSet(i);

//	POneSourceOnOff src = completeSetOfSourcesToUse;
	TOneSourceOnOff current_src;

/*	int fork_result;


	current_src = *src[i];
	goChild(current_src, );*/

	thread_launcher();

/*	while (src != NULL ) {
		if (src->defined_by_user) {
			fork_result = fork();

			if (fork_result > 0) {
				 parent process: nothing to do
			} else if (fork_result == 0) {
				 child process
				current_src = *src;
				freeAll(src);
				goChild(current_src);
				 child ends here
				exit(0);
			} else {
				 error during the fork
				exit(3);  We quit, and all children will be killed too
			}
		}

		src = freeThisAndReturnNext(src);
	}*/

	/* All children started, the parent wait until all children are closed */
	wait(NULL);
}

int main(int argc, char*argv[])
{
	setbuf(stdout, NULL);
	srand((unsigned int) time(NULL));

	initialize_stat_report ();
#ifndef WITHOUT_PACKET_IMPLIMENTATION
    normal_generator ();
#endif

	printf("\nSources ON/OFF generator\n");

	startAllSources();
	return EXIT_SUCCESS;
}
