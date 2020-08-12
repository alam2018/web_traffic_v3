/*
 * tx_rx.h
 *
 *  Created on: Aug 21, 2019
 *      Author: user
 */


#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <error.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include "common.h"
#include "pthread.h"


#define WITHOUT_PACKET_IMPLIMENTATION				//Used when broken packet size is calculated at every second. Assignment from prof. bauschert
#undef WITHOUT_PACKET_IMPLIMENTATION

#define TOTAL_USER 							100
//#define TOTAL_NUMBER_OF_PACKET  			1000
#define TOTAL_NUMBER_OF_PACKET  			200
#define SEC_TO_NANO_SECONDS  				1000000000
#define SEC_TO_MICRP_SEC					1000000
#define NANO_TO_MICRO_SEC					0.001
#define SIMULATION_RUN_TIME_MINUTE			140

#ifndef WITHOUT_PACKET_IMPLIMENTATION
#define	MAX_PKT_CALL_NO_PRO_SESSION			30
#define PKT_SESSION_TIME					35          //Calculated in minutes. Ref TR 101 112 V3.2.0 (1998-04)
#endif

POneSourceOnOff completeSetOfSourcesToUse [TOTAL_USER];

typedef struct UserData
{
	double packet_size[TOTAL_USER][TOTAL_NUMBER_OF_PACKET];
	double packet_time[TOTAL_USER][TOTAL_NUMBER_OF_PACKET];

	struct timespec send_start_time;

}User;

User userdata;

struct timespec receiver_start_time;

pthread_mutex_t lock, clock_init;

//void receiver ();
void *receiver (void *arg);
void *start_transmission(void *arg);
void nanosleep_manually_compensated(myInteger expected_duration);
myInteger clock_now(void);
void initialize_stat_report ();

void normal_generator ();
