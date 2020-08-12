/*
 * transmission.c
 *
 *  Created on: Aug 21, 2019
 *      Author: user
 */


#include "tx_rx.h"
#include <unistd.h>
#include "stdbool.h"



double bytes = 0;
long long int noPkt = 0;
//int current_time = 0;


struct timespec time_calculation (struct timespec start_time, struct timespec end_time)
{
//	 long double timeElapsed = ( long double)(end_time.tv_sec - start_time.tv_sec)*SEC_TO_NANO_SECONDS +
//			( long double)(end_time.tv_nsec - start_time.tv_nsec);

	struct timespec elapsed_time;

/*	int i, total_second = 0;
	for (i = 0; i< (SIMULATION_RUN_TIME_MINUTE *5000); i++)
	{
		if (timeElapsed > SEC_TO_NANO_SECONDS)
		{
			total_second++;
			timeElapsed = timeElapsed - SEC_TO_NANO_SECONDS;
		} else
		{
			elapsed_time.tv_sec = total_second;
			elapsed_time.tv_nsec = timeElapsed;
			break;
		}
	}*/

	if (end_time.tv_sec > start_time.tv_sec)
	{
		elapsed_time.tv_sec = end_time.tv_sec - start_time.tv_sec;

		if (end_time.tv_nsec >= start_time.tv_nsec)
		{
			elapsed_time.tv_nsec = end_time.tv_nsec - start_time.tv_nsec;
		} else
		{
			elapsed_time.tv_sec = elapsed_time.tv_sec - 1;
			elapsed_time.tv_nsec = SEC_TO_NANO_SECONDS - (start_time.tv_nsec - end_time.tv_nsec);
		}
	} else
	{
		elapsed_time.tv_sec = 0;
		elapsed_time.tv_nsec = end_time.tv_nsec - start_time.tv_nsec;
	}

	return elapsed_time;

}


#ifdef WITHOUT_PACKET_IMPLIMENTATION

void *start_transmission(void *arg)
{
    int usrID = *((int *)arg);

    clock_t time;
    double LINE_SPEED_PER_NANOSECOND =	(2048.0 * 1000000)/1000000000.0;

    time = clock();
    double time_taken, leftover_time, volume_traffic_tx;
    struct timespec current_time, temp_time;
    bool wait = false;

    int simulation_time = 0;

//    while (time_taken < (SIMULATION_RUN_TIME_MINUTE * 60))
    while (simulation_time < (SIMULATION_RUN_TIME_MINUTE * 60))
    {
    	simulation_time++;
        int i;
        for (i = 0; i<TOTAL_NUMBER_OF_PACKET; i++)
        {
        	if (wait == true)
        	{
//    			nanosleep_manually_compensated (leftover_time);
        		leftover_time = leftover_time * NANO_TO_MICRO_SEC;
        		usleep(leftover_time);
    			wait = false;

                pthread_mutex_lock(&lock);
                bytes += userdata.packet_size[usrID][i];
                pthread_mutex_unlock(&lock);
                noPkt++;

        		usleep(userdata.packet_time[usrID][i]);
        		continue;
        	}
//            clock_gettime(CLOCK_MONOTONIC, &current_time);

        	leftover_time = 0;
    		struct timespec now;
    		clock_gettime(CLOCK_MONOTONIC, &now);
    		temp_time = time_calculation(receiver_start_time, now);

    		leftover_time = SEC_TO_NANO_SECONDS - temp_time.tv_nsec;
    		volume_traffic_tx = LINE_SPEED_PER_NANOSECOND * leftover_time;


    		if (userdata.packet_size[usrID][i] > volume_traffic_tx)
    		{
                pthread_mutex_lock(&lock);
    			bytes += volume_traffic_tx;
                pthread_mutex_unlock(&lock);
    			userdata.packet_size[usrID][i] = userdata.packet_size[usrID][i] - volume_traffic_tx;
    			i = i-1;
    			wait = true;
    			continue;
    		} else
    		{
                pthread_mutex_lock(&lock);
                bytes += userdata.packet_size[usrID][i];
                pthread_mutex_unlock(&lock);
                noPkt++;
    		}
/*            pthread_mutex_lock(&lock);
            bytes += userdata.packet_size[usrID][i];
            pthread_mutex_unlock(&lock);
            noPkt++;*/
//            nanosleep_manually_compensated (userdata.packet_time[usrID][i]);
    		usleep(userdata.packet_time[usrID][i]);
    //        usleep (packet_time[usrID][i]);
        }
//		int session_wait = (0.00001 * SEC_TO_MICRP_SEC) + irand();
//		nanosleep_manually_compensated (session_wait);
//        time = clock () - time;
//        time_taken = ((double)time)/CLOCKS_PER_SEC;
    }
}

#else
int nPkt_call [TOTAL_USER][SIMULATION_RUN_TIME_MINUTE / PKT_SESSION_TIME];
int nPkt_pro_call [TOTAL_USER][SIMULATION_RUN_TIME_MINUTE / PKT_SESSION_TIME][MAX_PKT_CALL_NO_PRO_SESSION];
int noTime_betwn_call [TOTAL_USER][SIMULATION_RUN_TIME_MINUTE / PKT_SESSION_TIME][MAX_PKT_CALL_NO_PRO_SESSION];

int bound_min_max_new(double x, int min, int max);
double gaussrand();

double gaussrand2();

int bound_min_max_new(double x, int min, int max)
{
//	double temp_rand = (max*0.1)*gaussrand();
//	double rand_val = x + temp_rand;
	int y = (int) (x);
	if(y<min)
		return min;
	else if (y>max)
		return max;
	else
		return y;
}


double gaussrand()
{
	static double V1, V2, S;
	static int phase = 0;
	double X;

	srand((unsigned int) time(NULL));

	if(phase == 0) {
		do {
			double U1 = (double)rand() / RAND_MAX;
			double U2 = (double)rand() / RAND_MAX;

			V1 = 2 * U1 - 1;
			V2 = 2 * U2 - 1;
			S = V1 * V1 + V2 * V2;
			} while(S >= 1 || S == 0);

		X = V1 * sqrt(-2 * log(S) / S);
	} else
		X = V2 * sqrt(-2 * log(S) / S);

	phase = 1 - phase;

	return X;
}

#define PI 3.141592654
double gaussrand2()
{
	srand((unsigned int) time(NULL));
	static double U, V;
	static int phase = 0;
	double Z;

	if(phase == 0) {
		U = (rand() + 1.) / (RAND_MAX + 2.);
		V = rand() / (RAND_MAX + 1.);
		Z = sqrt(-2 * log(U)) * sin(2 * PI * V);
	} else
		Z = sqrt(-2 * log(U)) * cos(2 * PI * V);

	phase = 1 - phase;

	return Z;
}

void normal_generator ()
{
/*	FILE *distrib_params;
	distrib_params = fopen ("distribution_parameters.csv","w+");
	setbuf(distrib_params, NULL);
	if (distrib_params == NULL)
	{
		printf ("File not created okay, errno = %d\n", errno);
	}

	fprintf (distrib_params,"Session; Number of Packet Calls; No of packets in a call; Pkt Call time; \n");*/

	static int pkt_call_avg = 5;
	static int pkt_call_min = 1;
	static int pkt_call_max = 30;

	static int pkt_no_pro_call_avg = 25;
	static int pkt_no_pro_call_min = 1;
	static int pkt_no_pro_call_max = MAX_PKT_CALL_NO_PRO_SESSION;

	static int time_betwn_call_avg = 412;
	static int time_betwn_call_min = 60;
	static int time_betwn_call_max = 450;


	int i, j, test1=0,test2=0,test3 = 0, k;
	int total_session = SIMULATION_RUN_TIME_MINUTE / PKT_SESSION_TIME;

	for (k=0; k<TOTAL_USER; k++)
	{
		for (i=0; i<total_session; i++)
		{
			double temp_rand = gaussrand();
			nPkt_call[k][i] = 	bound_min_max_new(pkt_call_avg + temp_rand * (pkt_call_max * 0.1), pkt_call_min, pkt_call_max);
			sleep(1);

			test1 = test1 + nPkt_call[k][i];

			for (j=0; j<nPkt_call[k][i]; j++)
			{
				double temp_rand2 = gaussrand2();
				nPkt_pro_call[k][i][j] = bound_min_max_new(pkt_no_pro_call_avg + temp_rand2 * (pkt_no_pro_call_max * 0.1), pkt_no_pro_call_min, pkt_no_pro_call_max);

				if (j != (nPkt_call[k][i] -1))
				{
					noTime_betwn_call[k][i][j] = bound_min_max_new(time_betwn_call_avg + temp_rand2 * (time_betwn_call_max * 0.1), time_betwn_call_min, time_betwn_call_max);
					test3 = test3 + noTime_betwn_call[k][i][j];
				}

				test2 = test2 + nPkt_pro_call[k][i][j];

			}

		}
	}

}
void *start_transmission(void *arg)
{
    int usrID = *((int *)arg);

    clock_t time;
    double LINE_SPEED_PER_NANOSECOND =	(2048.0 * 1000000)/1000000000.0;

    time = clock();
    double time_taken, leftover_time, volume_traffic_tx;
    struct timespec current_time, temp_time;
    bool wait = false;

    int simulation_time = 0;

//    while (time_taken < (SIMULATION_RUN_TIME_MINUTE * 60))
    while (true)
    {
    	simulation_time++;
        int packet, packetCall, session;
    	int total_session = SIMULATION_RUN_TIME_MINUTE / PKT_SESSION_TIME;

    	for (session=0; session<total_session; session++)
    	{
        	for (packetCall=0; packetCall<nPkt_call[usrID][session]; packetCall++)
        	{
                for (packet = 0; packet<nPkt_pro_call[usrID][session][packetCall]; packet++)
                {
/*                	if (wait == true)
                	{
        //    			nanosleep_manually_compensated (leftover_time);
                		leftover_time = leftover_time * NANO_TO_MICRO_SEC;
                		usleep(leftover_time);
            			wait = false;

                        pthread_mutex_lock(&lock);
                        bytes += userdata.packet_size[usrID][packet];
                        pthread_mutex_unlock(&lock);
                        noPkt++;

                        nanosleep_manually_compensated(userdata.packet_time[usrID][packet]);
                		continue;
                	}*/
        //            clock_gettime(CLOCK_MONOTONIC, &current_time);

                	leftover_time = 0;
            		struct timespec now;
            		clock_gettime(CLOCK_MONOTONIC, &now);
            		temp_time = time_calculation(receiver_start_time, now);

            		leftover_time = SEC_TO_NANO_SECONDS - temp_time.tv_nsec;
            		volume_traffic_tx = LINE_SPEED_PER_NANOSECOND * leftover_time;


/*            		if (userdata.packet_size[usrID][packet] > volume_traffic_tx)
            		{
                        pthread_mutex_lock(&lock);
            			bytes += volume_traffic_tx;
                        pthread_mutex_unlock(&lock);
            			userdata.packet_size[usrID][packet] = userdata.packet_size[usrID][packet] - volume_traffic_tx;
            			packet = packet-1;
            			wait = true;
            			continue;
            		} else*/
            		{
                        pthread_mutex_lock(&lock);
                        bytes += userdata.packet_size[usrID][packet];
                        noPkt++;
                        pthread_mutex_unlock(&lock);

            		}
            		nanosleep_manually_compensated(userdata.packet_time[usrID][packet]);
            //        usleep (packet_time[usrID][i]);
                }

    			if (packet != (nPkt_call[usrID][packet] -1))
    			{
    				nanosleep_manually_compensated (noTime_betwn_call[usrID][session][packetCall]);
    			}
        	}

    	}

    }
}

#endif


void *receiver (void *arg)
{
//	long long init_time = clock_now();
	FILE *traffic_write;
	traffic_write = fopen ("traffic_simulation.csv","w+");
	setbuf(traffic_write, NULL);
	if (traffic_write == NULL)
	{
		printf ("File not created okay, errno = %d\n", errno);
	}

	fprintf (traffic_write,"Time Scale; Number of Packets; Total data; \n");
	long int time_index = 0;

    clock_t time;
    time = clock();
    double time_taken, temp_byte;

	pthread_mutex_lock(&clock_init);
    clock_gettime(CLOCK_MONOTONIC, &receiver_start_time);
	pthread_mutex_unlock(&clock_init);

	struct timespec current_time, now;

//	while (time_taken < (SIMULATION_RUN_TIME_MINUTE * 60))
	while (time_index < (SIMULATION_RUN_TIME_MINUTE * 60))
	{
		sleep (1);

		time_index++;

		pthread_mutex_lock(&lock);
		temp_byte = bytes;
		bytes = 0;
		pthread_mutex_unlock(&lock);

		clock_gettime(CLOCK_MONOTONIC, &now);
		current_time = time_calculation(receiver_start_time, now);

//		fprintf (traffic_write,"%ld; %lld; %f; \n", time_index, noPkt, temp_byte);
		fprintf (traffic_write,"%ld; %lld; %f; \n", current_time.tv_sec, noPkt, temp_byte);
		noPkt = 0;

		printf ("%f\n", temp_byte);

        time = clock () - time;
        time_taken = ((double)time)/CLOCKS_PER_SEC;

/*		pthread_mutex_lock(&clock_init);
	    current_time = (int) time_index;
		pthread_mutex_unlock(&clock_init);*/
	}
	  exit(0);
}
