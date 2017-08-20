/*	cs149 assignment 3
 *
 * Author: Carl Lee, Ho Yin Cheng Horace
 *
 *
 */



#include <string>
#include<vector>
#include<sys/time.h>
#include<semaphore.h>
#include<iostream>
#include<sstream>
#include<pthread.h>
#include<iomanip>
#include<cstdlib>
#include<unistd.h>
#include<signal.h>

#define SELL_HOUR_DURATION 60;
int  TOTAL_CUSTOMERS = 300;			//generate 300 customer(threads) to do the experiment


int timesUp = 0;


int HSLEEP[2] = {1,2};		//sleepin time for each seller
int MSLEEP[3] = { 2, 3, 4 };
int LSLEEP[4] = { 4, 5, 6, 7 };
int TURNAWAY = 0;
int ARRIVE = 0;
int IMPATIENT = 0;
int BUYINGSUCCESS = 0;



std::vector<pthread_mutex_t> queueMutexs; //mutex protects queue
pthread_mutex_t seatMutex;
pthread_mutex_t printMutex;


std::vector<sem_t> filledQueues; //send the signal to the sellers if the queue has people
struct itimerval sellTimer;
time_t mystart, myend;


class MyThread;
class Customer;
class Seat;
class Seller;
class Thread;
class MyTimer;

void customerArrives(Customer* customer);
void print(std::string string);

class Customer
{
public:
	Customer();
	~Customer();
	void setSeat(Seat* seat);
	Seat* getSeat();
	std::string getName();
	int getID();
	int getWaitTime();
	void addWaitTime(int waitime);



private:

	int customerid;
	Seat* seat;
	std::string name;
	int waittime;

};



class Seat
{

public:
	Seat();
	~Seat();

	void isSold();
	bool getSold();
	void setCustomer(Customer* customer);
	Customer* getCustomer();
	void setSeller(Seller* seller);
	Seller* getSeller();
	void setPriceLevel(int pricelevel);
	int getPriceLevel();
	std::string getName();


private:
	bool sold;
	Customer* customer;
	Seller* seller;
	



};

class Seller
{
public:

	Seller();
	Seller(int pricelevel, int id, int n);
	~Seller();
	void setPriceLevel(int pricelevel);
	int getPriceLevel();
	void setID(int id);
	int getID();
	std::string getName();
	int getSizeOfQueue();
	void setTheSizeOfQueue(int N);
	bool addCustomerToQueue(Customer* customer);
	std::vector<Customer*>* getQueue();
	bool checkAvailability();
	bool serveCustomer();




private:
	int pricelevel;
	int id;
	int midjump;				//only used for mid seet
	int n;						//the size of queue
	std::string name;
	std::vector<Customer*> queue;

};




using namespace std;

std::vector<Seller*>  highsellers, mediumsellers, lowersellers;
std::vector<Seat*> seats;
std::vector<Customer*> customers;

void print(std::string string)
{
	time_t now;
	time(&now);
	double elapsed = difftime(now, mystart);
	int min = 0;
	int sec = (int)elapsed;
	if (sec >= 60)
	{
		min++;
		sec -= 60;
	}
	std::stringstream ss;
	ss << min << ':' <<  sec;
	string = ss.str() + " " + string;
	pthread_mutex_lock(&printMutex);

	std::cout << string << "\n";

	pthread_mutex_unlock(&printMutex);



}

int CUSTOMERID = 0;

Customer::Customer()
{

	this->customerid = CUSTOMERID;
	this->seat = NULL;
	this->waittime = 0;

	std::ostringstream s;
	s<< "Customer " << customerid;

	this->name = s.str();
	CUSTOMERID++;
	
}


Customer::~Customer()
{
}

std::string Customer::getName()
{
	return this->name;
}

int Customer::getID()
{
	return this->customerid;
}

int Customer::getWaitTime()
{
	return this->waittime;
}

void Customer::addWaitTime(int waitime)
{
	this->waittime = this->waittime+ waitime;
}


Seller::Seller(): pricelevel(0),id(0), n(0), midjump(0)
{}


Seller::Seller(int pricelevel,int id, int n)
{
	this->pricelevel = pricelevel;
	this->id = id;
	this->midjump = 40;
	this->n = n;

	std::ostringstream s;
	if (pricelevel ==0)
	{
		s <<"H" << id;
		this->name = s.str();
	}
	else if (pricelevel ==1)
	{
		s << "M" << id;
		this->name = s.str();
		this->midjump = 40;
	}
	else                                         //pricelevel = 3
	{
		s << "L" << id;
		this->name = s.str();
	}

}


Seller::~Seller()
{
}

void Seller::setPriceLevel(int pricelevel)
{
	this->pricelevel = pricelevel;
}

int Seller::getPriceLevel()
{
	return this->pricelevel;
}

void Seller::setID(int id)
{
	this->id = id;
}

int Seller::getID()
{
	return this->id;
}

std::string Seller::getName()
{
	return this->name;
}

int Seller::getSizeOfQueue()
{
	return queue.size();
}

void Seller::setTheSizeOfQueue(int n)
{
	this->n = n;
}

bool Seller::addCustomerToQueue(Customer* customer)
{
	if (queue.size() == n)
	{
		return false;
	}

	queue.push_back(customer);
	//cout<< this->name << "  queue   "<<queue.size() <<"  ";
	return true;
}

std::vector<Customer*>* Seller::getQueue()
{
	return &queue;
}

bool Seller::checkAvailability()			//check availability, if there is seat, then return true. Otherwise return false
{
	if(queue.empty())
	{
		return false;
	}

	Customer* serve = queue[0];
	std::string string;
	std::stringstream ss1;
	ss1 << name << " serve with " << serve->getName();
	string = ss1.str();
	print(string);


	if (this->pricelevel == 0)	//since all seats was checked
	{
		for (int i = 0; i<100; i++)
		{
			Seat* seat = seats[i];


			if (seat->getSold()== false)
			{
				seat->setCustomer(*(queue.begin()));
				seat->setSeller(this);
				seat->isSold();	//change the seat sold is true
				queue.erase(queue.begin());
				BUYINGSUCCESS++;
				std::stringstream ss2;
				ss2 << serve->getName() << " buy ticket successfully and leave "<< this->name <<" queue! :D";
				string = ss2.str();
				print(string);
				return true;
			}
			continue;
		}

		for(int j = 0 ; j<queue.size() ; j++)			//At this point, all seats was sold, then return false
		{
			std::stringstream ss3;
			ss3 << serve->getName() << " cannot buy ticket and leave "<< this->name <<" queue! :(";
			string = ss3.str();
			print(string);
			TURNAWAY++;
		}

		queue.clear();


		timesUp = 1;		//stop selling
		return false;
	}

	if (this->pricelevel == 1)				//mid price seller
	{
		if (midjump ==-1)
		{
			return false;		//If midjump is -1, it means all seats were sold out, so return null
		}


		if (midjump == 40)
		{
			for (int i = 40; i<60; i++)			//row 5 and 6
			{
				Seat* seat = seats[i];

				if (seat->getSold()== false)
				{
					seat->setCustomer(*(queue.begin()));
					seat->setSeller(this);
					seat->isSold();	//change the seat sold is true
					queue.erase(queue.begin());
					BUYINGSUCCESS++;
					std::stringstream ss2;
					ss2 << serve->getName() << " buy ticket successfully and leave "<< this->name <<" queue! :D";
					string = ss2.str();
					print(string);
					return true;
				}
				continue;
			}
			midjump = 30;

		}


		while (midjump ==30)
		{

			for (int i = 30; i<40; i++)		//row 4
			{
				Seat* seat = seats[i];

				if (seat->getSold()== false)
				{
					seat->setCustomer(*(queue.begin()));
					seat->setSeller(this);
					seat->isSold();	//change the seat sold is true
					queue.erase(queue.begin());
					BUYINGSUCCESS++;
					std::stringstream ss2;
					ss2 << serve->getName() << " buy ticket successfully and leave "<< this->name <<" queue! :D";
					string = ss2.str();
					print(string);
					return true;
				}
				continue;
			}

			midjump = 60;
		}

		while (midjump ==60)
		{

			for (int i = 60; i<70; i++)		//row7
			{
				Seat* seat = seats[i];

				if (seat->getSold()== false)
				{
					seat->setCustomer(*(queue.begin()));
					seat->setSeller(this);
					seat->isSold();	//change the seat sold is true
					queue.erase(queue.begin());
					BUYINGSUCCESS++;
					std::stringstream ss2;
					ss2 << serve->getName() << " buy ticket successfully and leave "<< this->name <<" queue! :D";
					string = ss2.str();
					print(string);
					return true;
				}
				continue;
			}
			midjump = 20;
		}


		while (midjump ==20)
		{
			for (int i = 20; i<30; i++)			//row3
			{
				Seat* seat = seats[i];

				if (seat->getSold()== false)
				{
					seat->setCustomer(*(queue.begin()));
					seat->setSeller(this);
					seat->isSold();	//change the seat sold is true
					queue.erase(queue.begin());
					BUYINGSUCCESS++;
					std::stringstream ss2;
					ss2 << serve->getName() << " buy ticket successfully and leave "<< this->name <<" queue! :D";
					string = ss2.str();
					print(string);
					return true;
				}
				continue;
			}
			midjump = 70;
		}

		while (midjump ==70)
		{
			for (int i = 60; i<70; i++)		//row8
			{
				Seat* seat = seats[i];

				if (seat->getSold()== false)
				{
					seat->setCustomer(*(queue.begin()));
					seat->setSeller(this);
					seat->isSold();	//change the seat sold is true
					queue.erase(queue.begin());
					BUYINGSUCCESS++;
					std::stringstream ss2;
					ss2 << serve->getName() << " buy ticket successfully and leave "<< this->name <<" queue! :D";
					string = ss2.str();
					print(string);
					return true;
				}
				continue;
			}
			midjump = 10;
		}

		while (midjump ==10)
		{
			for (int i = 10; i<20; i++)		//row2
			{
				Seat* seat = seats[i];

				if (seat->getSold()== false)
				{
					seat->setCustomer(*(queue.begin()));
					seat->setSeller(this);
					seat->isSold();	//change the seat sold is true
					queue.erase(queue.begin());
					BUYINGSUCCESS++;
					std::stringstream ss2;
					ss2 << serve->getName() << " buy ticket successfully and leave "<< this->name <<" queue! :D";
					string = ss2.str();
					print(string);
					return true;
				}
				continue;
			}
			midjump = 80;
		}

		while (midjump ==80)
		{

			for (int i = 80; i<90; i++)		//row9
			{
				Seat* seat = seats[i];

				if (seat->getSold()== false)
				{
					seat->setCustomer(*(queue.begin()));
					seat->setSeller(this);
					seat->isSold();	//change the seat sold is true

					queue.erase(queue.begin());
					BUYINGSUCCESS++;
					std::stringstream ss2;
					ss2 << serve->getName() << " buy ticket successfully and leave "<< this->name <<" queue! :D";
					string = ss2.str();
					print(string);
					return true;
				}
				continue;
			}

			midjump = 0;
		}

		while (midjump ==0)
		{

			for (int i = 0; i<10; i++)			//row1
			{
				Seat* seat = seats[i];

				if (seat->getSold()== false)
				{
					seat->setCustomer(*(queue.begin()));
					seat->setSeller(this);
					seat->isSold();	//change the seat sold is true

					queue.erase(queue.begin());
					BUYINGSUCCESS++;
					std::stringstream ss2;
					ss2 << serve->getName() << " buy ticket successfully and leave "<< this->name <<" queue! :D";
					string = ss2.str();
					print(string);
					return true;
				}
				continue;
			}

			midjump = 90;
		}

		while (midjump ==90)
		{

			for (int i = 90; i<100; i++)			//row1
			{
				Seat* seat = seats[i];

				if (seat->getSold()== false)
				{
					seat->setCustomer(*(queue.begin()));
					seat->setSeller(this);
					seat->isSold();	//change the seat sold is true

					queue.erase(queue.begin());
					BUYINGSUCCESS++;
					std::stringstream ss2;
					ss2 << serve->getName() << " buy ticket successfully and leave "<< this->name <<" queue! :D";
					string = ss2.str();
					print(string);
					return true;
				}
				continue;
			}

			midjump = -1;
		}

		for(int j = 0 ; j<queue.size() ; j++)			//At this point, all seats was sold, then return false
		{
			std::stringstream ss3;
			ss3 << serve->getName() << " cannot buy ticket and leave "<< this->name <<" queue! :(";
			string = ss3.str();
			print(string);
			TURNAWAY++;
		}

		queue.clear();
		timesUp = 1;

		return false;					//at this point, all seats was checked, but no more seats can sell, then return null
	}
	if (this->pricelevel == 2)
	{
		for (int i = 99; i>-1; i--)
		{
			Seat* seat = seats[i];

			if (seat->getSold()== false)
			{
				seat->setCustomer(*(queue.begin()));
				seat->setSeller(this);
				seat->isSold();	//change the seat sold is true


				queue.erase(queue.begin());
				BUYINGSUCCESS++;
				std::stringstream ss2;
				ss2 << serve->getName() << " buy ticket successfully and leave "<< this->name <<" queue! :D";
				string = ss2.str();
				print(string);
				return true;
			}
			continue;
		}
		queue.erase(queue.begin());
		TURNAWAY = TURNAWAY + queue.size();
		std::stringstream ss3;
		ss3 << serve->getName() << " cannot buy ticket and leave "<< this->name <<" queue! :D";
		string = ss3.str();
		print(string);
		TURNAWAY++;

		timesUp = 1;
		return false;
	}

	return false;


}



bool Seller::serveCustomer()
{

	if(!timesUp)
	{
		if(queue.size() == 0)
		{
			if (this->pricelevel == 0)							//wait signal from customer joinning on the queue
					{
						cout<< name << " waiting........\n";
						sem_wait(&filledQueues[0]);


					}
					else if (this->pricelevel == 1)
					{
						if (this->id == 1)
							sem_wait(&filledQueues[1]);
						if (this->id == 2)
							sem_wait(&filledQueues[2]);
						if (this->id == 3)
							sem_wait(&filledQueues[3]);

					}
					else
					{
						if (this->id == 1)
							sem_wait(&filledQueues[4]);
						if (this->id == 2)
							sem_wait(&filledQueues[5]);
						if (this->id == 3)
							sem_wait(&filledQueues[6]);
						if (this->id == 4)
							sem_wait(&filledQueues[7]);
						if (this->id == 5)
							sem_wait(&filledQueues[8]);
						if (this->id == 6)
							sem_wait(&filledQueues[9]);
					}

		}

		if (this->pricelevel == 0)							//lock the queue from a specific seller
		{
			pthread_mutex_lock(&queueMutexs[0]);
		}
		else if (this->pricelevel == 1)
		{
			if (this->id == 1)
				pthread_mutex_lock(&queueMutexs[1]);
			if (this->id == 2)
				pthread_mutex_lock(&queueMutexs[2]);
			if (this->id == 3)
				pthread_mutex_lock(&queueMutexs[3]);
		}
		else
		{
			if (this->id == 1)
				pthread_mutex_lock(&queueMutexs[4]);
			if (this->id == 2)
				pthread_mutex_lock(&queueMutexs[5]);
			if (this->id == 3)
				pthread_mutex_lock(&queueMutexs[6]);
			if (this->id == 4)
				pthread_mutex_lock(&queueMutexs[7]);
			if (this->id == 5)
				pthread_mutex_lock(&queueMutexs[8]);
			if (this->id == 6)
				pthread_mutex_lock(&queueMutexs[9]);
		}



		pthread_mutex_lock(&seatMutex);					//critical section begin

		bool success = checkAvailability();

		if (this->pricelevel == 0)							//unlock the queue from a specific seller critical section end
		{
			pthread_mutex_unlock(&queueMutexs[0]);
		}
		else if (this->pricelevel == 1)
		{
			if (this->id == 1)
				pthread_mutex_unlock(&queueMutexs[1]);
			if (this->id == 2)
				pthread_mutex_unlock(&queueMutexs[2]);
			if (this->id == 3)
				pthread_mutex_unlock(&queueMutexs[3]);
		}
		else
		{
			if (this->id == 1)
				pthread_mutex_unlock(&queueMutexs[4]);
			if (this->id == 2)
				pthread_mutex_unlock(&queueMutexs[5]);
			if (this->id == 3)
				pthread_mutex_unlock(&queueMutexs[6]);
			if (this->id == 4)
				pthread_mutex_unlock(&queueMutexs[7]);
			if (this->id == 5)
				pthread_mutex_unlock(&queueMutexs[8]);
			if (this->id == 6)
				pthread_mutex_unlock(&queueMutexs[9]);
		}

		pthread_mutex_lock(&printMutex);
		std::cout << "Seat chart:\n";
		for (int i = 0; i<100 ; i++)		//PRINT THE SEATchart
		{
			Seat* seat = seats[i];
			if (i % 10 == 0)
			{
				if (seat->getSold())
				{
					std::cout << "\n" + seat->getName() + " ";
				}
				else
				{
					std::stringstream ss4;
					ss4 << "  X  ";
					std::string string = ss4.str();
					std::cout << "\n"<<string;
				}
			}
			else
			{
				if (seat->getSold())
				{
					std::cout <<seat->getName() + " ";
				}
				else
				{
					std::stringstream ss;
					ss << "  X  ";
					std::string string = ss.str();
					std::cout << string;
				}
			}
		}
		cout<<"\n";


		pthread_mutex_unlock(&printMutex);
		pthread_mutex_unlock(&seatMutex);


		int r;
		int sleeptime;
		if (this->pricelevel ==0)							//sleep a several seconds
		{
			r = rand() % 3;
			sleeptime = HSLEEP[r];
			sleep(sleeptime);
		}
		else if (this->pricelevel ==1)
		{
			r = rand() % 4;
			sleeptime = MSLEEP[r];
			sleep(sleeptime);
		}
		else
		{
			r = rand() % 5;
			sleeptime = LSLEEP[r];
			sleep(sleeptime);
		}


		if (this->pricelevel == 0)							//lock the queue from a specific seller
		{
			pthread_mutex_lock(&queueMutexs[0]);
		}
		else if (this->pricelevel == 1)
		{
			if (this->id == 1)
				pthread_mutex_lock(&queueMutexs[1]);
			if (this->id == 2)
				pthread_mutex_lock(&queueMutexs[2]);
			if (this->id == 3)
				pthread_mutex_lock(&queueMutexs[3]);
		}
		else
		{
			if (this->id == 1)
				pthread_mutex_lock(&queueMutexs[4]);
			if (this->id == 2)
				pthread_mutex_lock(&queueMutexs[5]);
			if (this->id == 3)
				pthread_mutex_lock(&queueMutexs[6]);
			if (this->id == 4)
				pthread_mutex_lock(&queueMutexs[7]);
			if (this->id == 5)
				pthread_mutex_lock(&queueMutexs[8]);
			if (this->id == 6)
				pthread_mutex_lock(&queueMutexs[9]);
		}


		for(int i = 0; i<queue.size() ; i++)
		{
			Customer* customer = queue[i];
			customer->addWaitTime(sleeptime);
			if(customer->getWaitTime() > 10)			//customer impatient
			{
				std::stringstream ss;
				ss << customer->getName() << "have wait"<< customer->getWaitTime() <<" leave" << this->getName() << " Queue  because of impatient";
				std::string string = ss.str();
				print(string);
				TURNAWAY++;
				IMPATIENT++;
				queue.erase(queue.begin()+i);
				i--;
			}

		}


		if (this->pricelevel == 0)							//unlock the queue from a specific seller critical section end
		{
			pthread_mutex_unlock(&queueMutexs[0]);
		}
		else if (this->pricelevel == 1)
		{
			if (this->id == 1)
				pthread_mutex_unlock(&queueMutexs[1]);
			if (this->id == 2)
				pthread_mutex_unlock(&queueMutexs[2]);
			if (this->id == 3)
				pthread_mutex_unlock(&queueMutexs[3]);
		}
		else
		{
			if (this->id == 1)
				pthread_mutex_unlock(&queueMutexs[4]);
			if (this->id == 2)
				pthread_mutex_unlock(&queueMutexs[5]);
			if (this->id == 3)
				pthread_mutex_unlock(&queueMutexs[6]);
			if (this->id == 4)
				pthread_mutex_unlock(&queueMutexs[7]);
			if (this->id == 5)
				pthread_mutex_unlock(&queueMutexs[8]);
			if (this->id == 6)
				pthread_mutex_unlock(&queueMutexs[9]);
		}







		if (!success)
		{
			return false;
		}

	}

	return true;


}

Seat::Seat()
{
	this->sold = false;
	this->customer = NULL;
	this->seller = NULL;
}


Seat::~Seat()
{
}

void Seat::isSold()
{
	this->sold = true;
}

bool Seat::getSold()
{
	return this->sold;
}

void Seat::setCustomer(Customer* customer)
{
	this->customer = customer;
}

Customer* Seat::getCustomer()
{
	return this->customer;
}

void Seat::setSeller(Seller* seller)
{
	this->seller = seller;
}

Seller* Seat::getSeller()
{
	return this->seller;
}


std::string Seat::getName()
{
	std::string string;
	std::stringstream ss;
	if(seller == NULL && customer == NULL)
	{
		ss << std::setfill(' ') << std::setw(4);
	}
	else
	{
		ss << (seller->getName()).c_str()<< std::setfill('0') << std::setw(3) << customer->getID();
	}
	string = ss.str();
	return string;
}

void customerArrives(Customer* customer)
{
	ARRIVE++;
	std::string string;
	if(timesUp ==1)
	{
		string = customer->getName() + "arrive and leave because all tickets are sold out!";
		print(string);
		TURNAWAY++;
		return;
	}


	int r = rand() % 3;
	bool success;
	Seller* seller;

	if (r ==0)					//high price											 //critical section begin
	{

		pthread_mutex_lock(&queueMutexs[0]);
		success = highsellers[r]->addCustomerToQueue(customer);
		seller = highsellers[r];
		pthread_mutex_unlock(&queueMutexs[0]);
		if (success)
		{
			sem_post(&filledQueues[0]);		//wake up the seller to serve
		}
	}
	else if (r ==1)				//medium price
	{
		r = rand() % 3;							//pick random seller to queue
		pthread_mutex_lock(&queueMutexs[r+1]);
		success = mediumsellers[r]->addCustomerToQueue(customer);
		seller = mediumsellers[r];
		pthread_mutex_unlock(&queueMutexs[r+1]);
		if (success)
		{
			sem_post(&filledQueues[r+1]);		//wake up the seller to serve
		}
	}
	else						//low price
	{
		r = rand() % 6;		//pick random seller to queue
		pthread_mutex_lock(&queueMutexs[r+4]);
		success = lowersellers[r]->addCustomerToQueue(customer);
		seller = lowersellers[r];
		pthread_mutex_unlock(&queueMutexs[r+4]);
		if (success)
		{
			sem_post(&filledQueues[r+4]);		//wake up the seller to serve
		}

	}																					//critical section end


	if (success)
	{
		string = customer->getName() + " arrives "+ seller->getName()  +" and waits.";
		print(string);
	}
	else
	{
		stringstream ss;
		ss<<seller->getSizeOfQueue();
		std::string str = ss.str();

		string = customer->getName() + " arrives "+ seller->getName()  + " and leave. Queue size: " + str;
		print(string);
		TURNAWAY++;

	}
}




void* selling(void* param)
{
	Seller* seller =(Seller*) param;


	cout<<seller->getName() << " start selling tickets\n";


	do
	{
		if(seller->getSizeOfQueue() ==0)
		{

			continue;
		}
		seller->serveCustomer();


	}while(!timesUp);

	return NULL;

}

void* wandering(void* param)
{

	Customer* customer = (Customer*) param;
	int duration = SELL_HOUR_DURATION;
	sleep(rand()%duration);
	customerArrives(customer);
	return NULL;
}

void* startTimer(void* param)
{
	time(&mystart);
	sellTimer.it_value.tv_sec = SELL_HOUR_DURATION;
	setitimer(ITIMER_REAL, &sellTimer, NULL);
	return NULL;

}




void timerHandler(int signal)
{

	timesUp = 1;


}


int main()
{

	int N;
	cout << "Please enter the total number of customer per ticket seller(queue size)";
	cin >> N;
	cout << "The value you entered is " << N << ".\n";



	// Initialize the mutexes and the semaphore.

	for (int i = 0; i < 10; i++)
	{
		sem_t filledqueue;
		pthread_mutex_t queuemutex;
		pthread_mutex_init(&queuemutex, NULL);
		queueMutexs.push_back(queuemutex);
		filledQueues.push_back(filledqueue);
		sem_init(&filledqueue, 0, 0);
	}





	pthread_mutex_init(&seatMutex, NULL);
	pthread_mutex_init(&printMutex,NULL);
	
	//initilize the seats
	for(int i = 0 ; i <100 ; i++)
	{
			Seat* seat = new Seat();
			seats.push_back(seat);
	}

	srand(time(0));


	//start creating thread and do the experiment

	pthread_t timerId;
	pthread_attr_t timerAttr;
	pthread_attr_init(&timerAttr);
	pthread_create(&timerId, &timerAttr, startTimer, NULL);				//create alarm thread



	Seller* highseller = new Seller(0, 1, N);										//create seller thread
	highsellers.push_back(highseller);
	pthread_t highThreadId;
	pthread_attr_t highAttr;
	pthread_attr_init(&highAttr);
	pthread_create(&highThreadId, &highAttr, selling, highseller);


	for (int i = 0; i < 3; i++)
	{
		Seller* mediumseller = new Seller(1, i+1, N);
		mediumsellers.push_back(mediumseller);
		pthread_t mediumThreadId;
		pthread_attr_t mediumAttr;
		pthread_attr_init(&mediumAttr);
		pthread_create(&mediumThreadId, &mediumAttr, selling, mediumseller);

	}

	for (int i = 0; i < 6; i++)
	{
		Seller* lowerseller = new Seller(2, i+1, N);
		lowersellers.push_back(lowerseller);
		pthread_t lowerThreadId;
		pthread_attr_t lowerAttr;
		pthread_attr_init(&lowerAttr);
		pthread_create(&lowerThreadId, &lowerAttr,selling, lowerseller);

	}




	for (int i = 0; i <TOTAL_CUSTOMERS; i++)
	{
		Customer* customer = new Customer();
		customers.push_back(customer);
		pthread_t cThreadId;
		pthread_attr_t cAttr;
		pthread_attr_init(&cAttr);
		pthread_create(&cThreadId, &cAttr,wandering, customer);
	}





	signal(SIGALRM, timerHandler);



	sleep(70);			//to prevent the main thread kill other threads




	std::string string;
	std::vector<Customer*>* queue = highseller->getQueue();
	for(int i = 0; i<(queue->size()) ; i++)
	{
		string = "From "+ highseller->getName() + ", " +( queue->at(i))->getName()   + " and leave.";
		print(string);
		TURNAWAY++;
	}



	for(int i = 0 ; i<3 ; i++)
	{
		Seller* mediumseller = mediumsellers[i];
		std::vector<Customer*>* queue = mediumseller->getQueue();
		for(int j = 0; j<(queue->size()) ; j++)
		{
			string = "From "+ mediumseller->getName() + ", " +  (queue->at(j))->getName()   + " and leave.";
			print(string);
			TURNAWAY++;
		}

	}

	for(int i = 0 ; i<6; i++)
	{
		Seller* lowerseller = lowersellers[i];
		std::vector<Customer*>* queue = lowerseller->getQueue();
		for(int j = 0; j<(queue->size()) ; j++)
		{
			string = "From "+ lowerseller->getName() + ", " + (queue->at(j))->getName()   + " and leave.";
			print(string);
			TURNAWAY++;
		}
	}


	cout<<"\n";
	cout<<"\n";
	cout<<"Statistics\n";
	cout<< "Turn away: " << TURNAWAY << "\n";
	cout<< "Impatient: " <<IMPATIENT<< "\n";
	cout<<"Arrive: " << ARRIVE<<"\n";
	cout<<"Buying sucessfully: "<< BUYINGSUCCESS<<"\n";
	cout<< "Times Up:     "<< timesUp<< " \n ";


	return 0;


}

