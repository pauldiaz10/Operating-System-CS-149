#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define NUM_TICKETS 100 // total number of tickets
#define TIME 60 // 1 hour = 1 sec / min

int randNumber(int min, int max) {
	int delta = max - min + 1;
	int random_number;
	random_number = rand() % delta + min;

	return random_number;
}

//create 10 x 10 seats
//false = not occupied
char *seats[10][10] = {
		{ "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  " },
		{ "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  " },
		{ "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  " },
		{ "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  " },
		{ "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  " },
		{ "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  " },
		{ "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  " },
		{ "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  " },
		{ "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  " },
		{ "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  ", "  -  " },
};

static int numTickets = NUM_TICKETS;
static int numOfCustomers = 15;

static pthread_mutex_t lock;
//TODO: Array of seats
//TODO: Customer Queue
//TODO: Actual reading and writing of seats to the array
//TODO: Outputs and labeling of customers
//TODO: Command line parameter for customers where N = 5, 10, 15


//Assigns seats starting from the front, pref [0][0]
//Sleeps 1 or 2 seconds to simulate transaction
void *highTicketSeller(void *ptr) {

	int id = ((int*)ptr);
	int customer[numOfCustomers];
	int customerNum = 1;

	for (int i = 0; i < numOfCustomers; i++) {
		customer[i] = randNumber(0, 59);

	}
	for (int i = 0; i < TIME; i++) {
		sleep(1);
		int increment = 0;
		int row;
		int column;

		for (int j = 0; j < numOfCustomers; j++) {
			int seated = 0;
			if(customer[j] <= i)
			printf("00:%d %s \n", i, "Customer arrived in seller queue H");
			if (customer[j] <= i && numTickets > 0) {
				customer[j] = 100;
				pthread_mutex_lock(&lock);
				for(int i = 0; i < 10; i++){
				 if(seated == 1){
				 break;
				 }
				 for(int j = 0; j < 10; j++){
					 row = i;
					 column = j;

				 if(strcmp(seats[i][j],"  -  ") == 0){
				 if(customerNum == 1){
					 seats[i][j] = "H001";
				 }
				 else if(customerNum == 2){
					 seats[i][j] = "H002";

				 }
				 else if(customerNum == 3){
					 seats[i][j] = "H003";

				 }
				 else if(customerNum == 4){
					 seats[i][j] = "H004";

				 }
				 else if(customerNum == 5){
					 seats[i][j] = "H005";

				 }
				 else if(customerNum == 6){
					 seats[i][j] = "H006";

				 				 }
				 else if(customerNum == 7){
					 seats[i][j] = "H007";

				 				 }
				 else if(customerNum == 8){
					 seats[i][j] = "H008";

				 				 }
				 else if(customerNum == 9){
					 seats[i][j] = "H009";

				 				 }
				 else if(customerNum == 10){
					 seats[i][j] = "H0010";

				 				 }
				 else if(customerNum == 11){
					 seats[i][j] = "H0011";

				 				 }
				 else if(customerNum == 12){
					 seats[i][j] = "H0012";

				 				 }
				 else if(customerNum == 13){
					 seats[i][j] = "H0013";

				 				 }
				 else if(customerNum == 14){
					 seats[i][j] = "H0014";

				 				 }
				 else if(customerNum == 15){
					 seats[i][j] = "H0015";

				 }
							seated = 1;
							numTickets--;
							customerNum++;
							break;
						}
					}
				}

				pthread_mutex_unlock(&lock);
				int sleep_time = randNumber(1, 2);
				sleep(sleep_time);
				increment += sleep_time;
				int temp = i + sleep_time;
				printf("00:%d %s %s\n", temp, seats[row][column],"completed transaction.");
				for (int i = 0; i < 10; i++) {
							for (int j = 0; j < 10; j++) {
								printf("%s ", seats[i][j]);
							}
							printf("\n");
						}
				//printf("00:%d %s %s", i + increment, seats[i][j],"completed transaction.");
				 //printf("%s","Hi");

			}
			else if(numTickets <= 0){
				printf("00:%d H seller - %s\n", i, "Sorry, all sold out!");
			}
		}
		i += increment;
	}
	pthread_exit(0);
}
//Assigns seats starting from the middle, priority 4/5/3/6/2/7/1/8/0/9
//Sleeps 2, 3, or 4 seconds to simulate transaction
void *mediumTicketSeller(void *ptr) {

	int id = ((int*)ptr);
	int customer[numOfCustomers];
	int customerNum = 1;
	char customerBeingHelped[] = "M101";

	for (int i = 0; i < numOfCustomers; i++) {
		customer[i] = randNumber(0, 59);

	}

	for (int i = 0; i < TIME; i++) {
		sleep(1);
		int increment = 0;
		for (int j = 0; j < numOfCustomers; j++) {
			if(customer[j] <= i)
				printf("00:%d %s%d \n", i,"Customer arrived in seller queue M", id);
			if (customer[j] <= i  && numTickets > 0) {
				customer[j] = 100;
				pthread_mutex_lock(&lock);

				int row = 4;
				int c = 0;
				while (0 == 0) {
					for (int column = 0; column < 10; column++) {
						c = column;
						if (strcmp(seats[row][column], "  -  ") == 0) {
							 numTickets--;
							if (customerNum == 1 && id == 1) {
								seats[row][column] = "M101";

							} else if (customerNum == 2 && id == 1) {
								seats[row][column] = "M102";

							} else if (customerNum == 3 && id == 1) {
								seats[row][column] = "M103";

							} else if (customerNum == 4 && id == 1) {
								seats[row][column] = "M104";

							} else if (customerNum == 5 && id == 1) {
								seats[row][column] = "M105";

							} else if (customerNum == 6 && id == 1) {
								seats[row][column] = "M106";

							} else if (customerNum == 7 && id == 1) {
								seats[row][column] = "M107";

							} else if (customerNum == 8 && id == 1) {
								seats[row][column] = "M108";

							} else if (customerNum == 9 && id == 1) {
								seats[row][column] = "M109";

							} else if (customerNum == 10 && id == 1) {
								seats[row][column] = "M1010";

							} else if (customerNum == 11 && id == 1) {
								seats[row][column] = "M1011";

							} else if (customerNum == 12 && id == 1) {
								seats[row][column] = "M1012";

							} else if (customerNum == 13 && id == 1) {
								seats[row][column] = "M1013";

							} else if (customerNum == 14 && id == 1) {
								seats[row][column] = "M1014";

							} else if (customerNum == 15 && id == 1) {
								seats[row][column] = "M1015";

							}
							else if (customerNum == 1 && id == 2) {
								seats[row][column] = "M201";
							} else if (customerNum == 2 && id == 2) {
								seats[row][column] = "M202";

							} else if (customerNum == 3 && id == 2) {
								seats[row][column] = "M203";

							} else if (customerNum == 4 && id == 2) {
								seats[row][column] = "M204";

							} else if (customerNum == 5 && id == 2) {
								seats[row][column] = "M205";

							} else if (customerNum == 6 && id == 2) {
								seats[row][column] = "M206";

							} else if (customerNum == 7 && id == 2) {
								seats[row][column] = "M207";

							} else if (customerNum == 8 && id == 2) {
								seats[row][column] = "M208";

							} else if (customerNum == 9 && id == 2) {
								seats[row][column] = "M209";

							} else if (customerNum == 10 && id == 2) {
								seats[row][column] = "M2010";

							} else if (customerNum == 11 && id == 2) {
								seats[row][column] = "M2011";

							} else if (customerNum == 12 && id == 2) {
								seats[row][column] = "M2012";

							} else if (customerNum == 13 && id == 2) {
								seats[row][column] = "M2013";

							} else if (customerNum == 14 && id == 2) {
								seats[row][column] = "M2014";

							} else if (customerNum == 15 && id == 2) {
								seats[row][column] = "M2015";

							}
							else if (customerNum == 1 && id == 3) {
								seats[row][column] = "M301";
							} else if (customerNum == 2 && id == 3) {
								seats[row][column] = "M302";

							} else if (customerNum == 3 && id == 3) {
								seats[row][column] = "M303";

							} else if (customerNum == 4 && id == 3) {
								seats[row][column] = "M304";

							} else if (customerNum == 5 && id == 3) {
								seats[row][column] = "M305";

							} else if (customerNum == 6 && id == 3) {
								seats[row][column] = "M306";

							} else if (customerNum == 7 && id == 3) {
								seats[row][column] = "M307";

							} else if (customerNum == 8 && id == 3) {
								seats[row][column] = "M308";

							} else if (customerNum == 9 && id == 3) {
								seats[row][column] = "M309";

							} else if (customerNum == 10 && id == 3) {
								seats[row][column] = "M3010";

							} else if (customerNum == 11 && id == 3) {
								seats[row][column] = "M3011";

							} else if (customerNum == 12 && id == 3) {
								seats[row][column] = "M3012";

							} else if (customerNum == 13 && id == 3) {
								seats[row][column] = "M3013";

							} else if (customerNum == 14 && id == 3) {
								seats[row][column] = "M3014";

							} else if (customerNum == 15 && id == 3) {
								seats[row][column] = "M3015";

							}
							customerNum++;
							break;
						} else if (column == 9 && row == 4) {
							column = 0;
							row = 5;
						} else if (column == 9 && row == 5) {
							column = 0;
							row = 3;
						} else if (column == 9 && row == 3) {
							column = 0;
							row = 6;
						} else if (column == 9 && row == 6) {
							column = 0;
							row = 2;
						} else if (column == 9 && row == 2) {
							column = 0;
							row = 7;
						} else if (column == 9 && row == 7) {
							column = 0;
							row = 1;
						} else if (column == 9 && row == 1) {
							column = 0;
							row = 8;
						} else if (column == 9 && row == 8) {
							column = 0;
							row = 0;
						} else if (column == 9 && row == 0) {
							column = 0;
							row = 9;
						} else if (column == 9 && row == 9) {
							break;
						}

					}

					break;

				}
				pthread_mutex_unlock(&lock);
				int sleep_time = randNumber(2, 4);
				sleep(sleep_time);
				increment += sleep_time;
				int temp = i + sleep_time;
				printf("00:%d %s %s\n", temp, seats[row][c],"completed transaction.");
				for (int i = 0; i < 10; i++) {
							for (int j = 0; j < 10; j++) {
								printf("%s ", seats[i][j]);
							}
							printf("\n");
						}

		}

			else if(numTickets <= 0){
			printf("00:%d M%d seller - %s\n", i, id, "Sorry, all sold out!");
			}

		}
		i += increment;

	}
	pthread_exit(0);
}
//Assigns seats starting from the back
//Sleeps 4, 5, 6, or 7 seconds to simulate transaction
	void *lowTicketSeller(void *ptr) {
		int id = ((int*)ptr);
		int customerNum = 1;
		int customer[numOfCustomers];
		int increment;
		for (int i = 0; i < numOfCustomers; i++) {
			customer[i] = randNumber(0, 59);
		}

		for (int i = 0; i < TIME; i++) {
			sleep(1);

			for (int j = 0; j < numOfCustomers; j++) {
				int seated = 0;
				increment = 0;
				int row;
				int column;
				if(customer[j] <= i)
					printf("00:%d %s%d \n", i,"Customer arrived in seller queue L", id);
				if (customer[j] <= i  && numTickets > 0) {
					customer[j] = 100;
					pthread_mutex_lock(&lock);
					for(int i = 9; i >= 0; i--){
					 for(int j = 0; j < 10; j++){
						 row = i;
						 column = j;
					 if(strcmp(seats[i][j],"  -  ")== 0){
							numTickets--;
							if (customerNum == 1 && id == 1) {
								seats[i][j] = "L101";
							} else if (customerNum == 2 && id == 1) {
								seats[i][j] = "L102";

							} else if (customerNum == 3 && id == 1) {
								seats[i][j] = "L103";

							} else if (customerNum == 4 && id == 1) {
								seats[i][j] = "L104";

							} else if (customerNum == 5 && id == 1) {
								seats[i][j] = "L105";

							} else if (customerNum == 6 && id == 1) {
								seats[i][j] = "L106";

							} else if (customerNum == 7 && id == 1) {
								seats[i][j] = "L107";

							} else if (customerNum == 8 && id == 1) {
								seats[i][j] = "L108";

							} else if (customerNum == 9 && id == 1) {
								seats[i][j] = "L109";

							} else if (customerNum == 10 && id == 1) {
								seats[i][j] = "L1010";

							} else if (customerNum == 11 && id == 1) {
								seats[i][j] = "L1011";

							} else if (customerNum == 12 && id == 1) {
								seats[i][j] = "L1012";

							} else if (customerNum == 13 && id == 1) {
								seats[i][j] = "L1013";

							} else if (customerNum == 14 && id == 1) {
								seats[i][j] = "L1014";

							} else if (customerNum == 15 && id == 1) {
								seats[i][j] = "L1015";

							} else if (customerNum == 1 && id == 2) {
								seats[i][j] = "L201";
							} else if (customerNum == 2 && id == 2) {
								seats[i][j] = "L202";

							} else if (customerNum == 3 && id == 2) {
								seats[i][j] = "L203";

							} else if (customerNum == 4 && id == 2) {
								seats[i][j] = "L204";

							} else if (customerNum == 5 && id == 2) {
								seats[i][j] = "L205";

							} else if (customerNum == 6 && id == 2) {
								seats[i][j] = "L206";

							} else if (customerNum == 7 && id == 2) {
								seats[i][j] = "L207";

							} else if (customerNum == 8 && id == 2) {
								seats[i][j] = "L208";

							} else if (customerNum == 9 && id == 2) {
								seats[i][j] = "L209";

							} else if (customerNum == 10 && id == 2) {
								seats[i][j] = "L2010";

							} else if (customerNum == 11 && id == 2) {
								seats[i][j] = "L2011";

							} else if (customerNum == 12 && id == 2) {
								seats[i][j] = "L2012";

							} else if (customerNum == 13 && id == 2) {
								seats[i][j] = "L2013";

							} else if (customerNum == 14 && id == 2) {
								seats[i][j] = "L2014";

							} else if (customerNum == 15 && id == 2) {
								seats[i][j] = "L2015";

							} else if (customerNum == 1 && id == 3) {
								seats[i][j] = "L301";
							} else if (customerNum == 2 && id == 3) {
								seats[i][j] = "L302";

							} else if (customerNum == 3 && id == 3) {
								seats[i][j] = "L303";

							} else if (customerNum == 4 && id == 3) {
								seats[i][j] = "L304";

							} else if (customerNum == 5 && id == 3) {
								seats[i][j] = "L305";

							} else if (customerNum == 6 && id == 3) {
								seats[i][j] = "L306";

							} else if (customerNum == 7 && id == 3) {
								seats[i][j] = "L307";

							} else if (customerNum == 8 && id == 3) {
								seats[i][j] = "L308";

							} else if (customerNum == 9 && id == 3) {
								seats[i][j] = "L309";

							} else if (customerNum == 10 && id == 3) {
								seats[i][j] = "L3010";

							} else if (customerNum == 11 && id == 3) {
								seats[i][j] = "L3011";

							} else if (customerNum == 12 && id == 3) {
								seats[i][j] = "L3012";

							} else if (customerNum == 13 && id == 3) {
								seats[i][j] = "L3013";

							} else if (customerNum == 14 && id == 3) {
								seats[i][j] = "L3014";

							} else if (customerNum == 15 && id == 3) {
								seats[i][j] = "L3015";

							} else if (customerNum == 1 && id == 4) {
								seats[i][j] = "L401";
							} else if (customerNum == 2 && id == 4) {
								seats[i][j] = "L402";

							} else if (customerNum == 3 && id == 4) {
								seats[i][j] = "L403";

							} else if (customerNum == 4 && id == 4) {
								seats[i][j] = "L404";

							} else if (customerNum == 5 && id == 4) {
								seats[i][j] = "L405";

							} else if (customerNum == 6 && id == 4) {
								seats[i][j] = "L406";

							} else if (customerNum == 7 && id == 4) {
								seats[i][j] = "L407";

							} else if (customerNum == 8 && id == 4) {
								seats[i][j] = "L408";

							} else if (customerNum == 9 && id == 4) {
								seats[i][j] = "L409";
							} else if (customerNum == 10 && id ==4) {
								seats[i][j] = "L4010";

							} else if (customerNum == 11 && id == 4) {
								seats[i][j] = "L4011";

							} else if (customerNum == 12 && id == 4) {
								seats[i][j] = "L4012";

							} else if (customerNum == 13 && id == 4) {
								seats[i][j] = "L4013";

							} else if (customerNum == 14 && id == 4) {
								seats[i][j] = "L4014";

							} else if (customerNum == 15 && id == 4) {
								seats[i][j] = "L4015";

							} else if (customerNum == 1 && id == 5) {
								seats[i][j] = "L501";
							} else if (customerNum == 2 && id == 5) {
								seats[i][j] = "L502";

							} else if (customerNum == 3 && id == 5) {
								seats[i][j] = "L503";

							} else if (customerNum == 4 && id == 5) {
								seats[i][j] = "L504";

							} else if (customerNum == 5 && id == 5) {
								seats[i][j] = "L505";

							} else if (customerNum == 6 && id == 5) {
								seats[i][j] = "L506";

							} else if (customerNum == 7 && id == 5) {
								seats[i][j] = "L507";

							} else if (customerNum == 8 && id == 5) {
								seats[i][j] = "L508";

							} else if (customerNum == 9 && id == 5) {
								seats[i][j] = "L509";

							} else if (customerNum == 10 && id == 5) {
								seats[i][j] = "L5010";

							} else if (customerNum == 11 && id == 5) {
								seats[i][j] = "L5011";

							} else if (customerNum == 12 && id == 5) {
								seats[i][j] = "L5012";

							} else if (customerNum == 13 && id == 5) {
								seats[i][j] = "L5013";

							} else if (customerNum == 14 && id == 5) {
								seats[i][j] = "L5014";

							} else if (customerNum == 15 && id == 5) {
								seats[i][j] = "L5015";

							} else if (customerNum == 1 && id == 6) {
								seats[i][j] = "L601";
							} else if (customerNum == 2 && id == 6) {
								seats[i][j] = "L602";

							} else if (customerNum == 3 && id == 6) {
								seats[i][j] = "L603";

							} else if (customerNum == 4 && id == 6) {
								seats[i][j] = "L604";

							} else if (customerNum == 5 && id == 6) {
								seats[i][j] = "L605";

							} else if (customerNum == 6 && id == 6) {
								seats[i][j] = "L606";

							} else if (customerNum == 7 && id == 6) {
								seats[i][j] = "L607";

							} else if (customerNum == 8 && id == 6) {
								seats[i][j] = "L608";

							} else if (customerNum == 9 && id == 6) {
								seats[i][j] = "L609";

							} else if (customerNum == 10 && id == 6) {
								seats[i][j] = "L6010";

							} else if (customerNum == 11 && id == 6) {
								seats[i][j] = "L6011";

							} else if (customerNum == 12 && id == 6) {
								seats[i][j] = "L6012";

							} else if (customerNum == 13 && id == 6) {
								seats[i][j] = "L6013";

							} else if (customerNum == 14 && id == 6) {
								seats[i][j] = "L6014";

							} else if (customerNum == 15 && id == 6) {
								seats[i][j] = "L6015";

							}
							customerNum++;

					 //seats[i][j] = "L";
					 seated = 1;
					 break;
					 }
					 }
					 if(seated == 1){
					 	break;
					 }
					 }

					pthread_mutex_unlock(&lock);
					int sleep_time = randNumber(4, 7);
					sleep(sleep_time);
					increment += sleep_time;
					int temp = i + sleep_time;
					printf("00:%d %s %s\n", temp, seats[row][column],"completed transaction.");
					for (int i = 0; i < 10; i++) {
								for (int j = 0; j < 10; j++) {
									printf("%s ", seats[i][j]);
								}
								printf("\n");
							}
				}
				else if(numTickets <= 0){
					printf("00:%d L%d seller - %s\n", i, id, "Sorry, all sold out!");
				}


			}

			i += increment;
		}

		pthread_exit(0);
	}


	int main(void) {

		printf("Enter number of customers: ");
		scanf("%d", &numOfCustomers);
		//srand(time(NULL));
		srand(getpid());
		//Initialize mutex lock
		pthread_mutex_init(&lock, 0);

		//Create the worker threads
		pthread_t H, M1, M2, M3, L1, L2, L3, L4, L5, L6;
		pthread_create(&H, NULL, highTicketSeller, (void *) 0);

		pthread_create(&M1, NULL, mediumTicketSeller, (void *) 1);
		pthread_create(&M2, NULL, mediumTicketSeller, (void *) 2);
		pthread_create(&M3, NULL, mediumTicketSeller, (void *) 3);

		pthread_create(&L1, NULL, lowTicketSeller, (void *) 1);
		pthread_create(&L2, NULL, lowTicketSeller, (void *) 2);
		pthread_create(&L3, NULL, lowTicketSeller, (void *) 3);
		pthread_create(&L4, NULL, lowTicketSeller, (void *) 4);
		pthread_create(&L5, NULL, lowTicketSeller, (void *) 5);
		pthread_create(&L6, NULL, lowTicketSeller, (void *) 6);

		//Resume main thread after all threads have terminated
		pthread_join(H, 0);
		pthread_join(M1, 0);
		pthread_join(M2, 0);
		pthread_join(M3, 0);
		pthread_join(L1, 0);
		pthread_join(L2, 0);
		pthread_join(L3, 0);
		pthread_join(L4, 0);
		pthread_join(L5, 0);
		pthread_join(L6, 0);

		//Cleanup
		pthread_mutex_destroy(&lock);

		printf("\n");
		printf("%s\n", "Final seating chart");
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				printf("%s ", seats[i][j]);
			}
			printf("\n");
		}

		return 0;
	}


