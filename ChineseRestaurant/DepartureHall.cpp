#include <iostream>
#include "DepartureHall.h"
#include "Customer.h"
#include "ServiceDesk.h"
#include "Waiter.h"
#include "Cashier.h"
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

using namespace std;

DepartureHall::DepartureHall()
{
	for (int i = 0; i < numOfPlaces.size(); i++)
	{
		for (int j = 0; j < numOfPlaces.at(i); j++)
		{
			service_desks.push_back(new ServiceDesk(i + 1));
		}
	}

	// Spawn waiters 
	for (int i = 0; i < k; i++)
	{
		waiters.push_back(new Waiter());
	}

	// Spawn cashiers 
	for (int i = 0; i < c; i++)
	{
		cashiers.push_back(new Cashier());
	}
}

DepartureHall::~DepartureHall()
{
	while (!customersInRestaurant.empty())
	{
		Customer *c = customersInRestaurant.front();
		customersInRestaurant.pop_back();
		std::cout << "Customer #" << c->getID() << " deleted!\n";
		delete c;
	}

	for (auto *sd : service_desks)
	{
		delete sd;
	}
}

/* Calcualte arrival time for the next customer and assign it to AT*/
double DepartureHall::getAT()
{
	double x = (rand() % ((AAT + SD) - (AAT-SD))) + (double)(AAT - SD) ;
	//return (1 / (standartDeviation * sqrt(2 * 3.14))) * pow(2.72, -1 * pow((x - averageAT), 2) / 2 * pow(standartDeviation, 2));
	double r = (1 / (0.2 * sqrt(2 * 3.14))) * pow(2.72, (-1 * pow((x - 1.8), 2) / 2 * pow(0.2, 2)));
	return x;
}

/* Calculate and assign WST for customer*/
double DepartureHall::getWST()
{
	double randomNumber = (double)std::rand() / RAND_MAX;
	double r = -1 * log(randomNumber) * avgWST;
	return r;
}

/* Calculate and assign CST for customer*/
double DepartureHall::getCST()
{
	double randomNumber = (double)std::rand() / RAND_MAX;
	double r = -1 * log(randomNumber) * avgCST;
	return r;
}

/* Checks queue. If any service desk is fits to first customer in the queue, places them to that service table.*/
void DepartureHall::checkForServiceStart()
{
	if (customersInQueue.empty())
	{
		//cout << "Customers queue is empty!" << endl;
		return;
	}

	// Check if any waiter is free
	for (auto waiter : waiters)
	{
		if (waiter->getIsFree())
		{
			canStartService = true;
			currentWaiter = waiter;
			break;
		}
	}

	// Check for free service desk which is suitable for the customer.
	for (int i = 0; i < service_desks.size(); i++)
	{
		//if (service_desks.at(i)->getIsFree() && service_desks.at(i)->numberOfPlace == customersInQueue.front()->numberOfCustomers)
		if (service_desks.at(i)->getIsFree())
		{
			canStartService = true;
			currentCustomer = customersInQueue.front();
			currentServiceDesk = service_desks.at(i);
			break;
		}
	}

	if (!canStartService)
	{
		cout << "Service couldn't started!" << endl;
	}
}

void DepartureHall::spawnCustomer()
{
	double randomNumber = (double)std::rand() / RAND_MAX; // random number between 0 and 1
	if (randomNumber < 0.5)
		queueCustomer();
	else
		queueCustomerToBuffet();
}

/** 1. Spawns new customer and adds it to restaurant queue
	2. Calculates next customer arrival time and assigns it to variable
*/
void DepartureHall::queueCustomer()
{
	customersInQueue.push_back(new Customer(this, false));
	AT = programTime + getAT();
}

/** 1. Spawns new customer and adds it to buffet queue
	2. Calculates next customer arrival time and assigns it to variable
*/
void DepartureHall::queueCustomerToBuffet()
{
	customersInBuffetQueue.push_back(new Customer(this, true));
	AT = programTime + getAT();
}

/**	1. If free assings waiter to customer
	2. Calculates waiter service time
	3. Assigns waiter service time in customer class
	4. Sets waiter busy
	5. Sets service desk busy
	6. Remove customer from queue
	7. Add that customer to customers in restaurant vector.
*/
void DepartureHall::startService()
{
	if (customersInQueue.empty())
	{
		//cout << "Couldn't started service due to no one is in the queue" << endl;
		return;
	}

	// configure customer for service start
	currentCustomer->waiter = currentWaiter;
	currentCustomer->serviceDesk = currentServiceDesk;
	currentCustomer->WST = getWST() + programTime + MT;

	// configure waiter for service start
	currentWaiter->setBusy();

	// configure service desk for service start
	currentServiceDesk->setBusy();
	currentServiceDesk->served_customer_ = currentCustomer;

	// Delete customer from queue and add to restaurant 
	customersInQueue.erase(customersInQueue.begin());
	customersInRestaurant.push_back(currentCustomer);

	cout << "---------------------------------------------------------" << endl;
	cout << "Service is started for the customer with id: " << currentCustomer->getID() << endl;
	cout << "Waiter service time for this customer: " << currentCustomer->WST << endl;
	cout << "---------------------------------------------------------" << endl;
}

/** 1. Sets waiter free
	2. Sets service desk free
	3. Calculates cashier service time for the next stage
*/
void DepartureHall::endService(Customer* customer)
{
	cout << "---------------------------------------------------------" << endl;
	cout << "Ending service for customer with id:" << customer->getID() << endl;
	cout << "---------------------------------------------------------" << endl;

	customer->waiter->setFree();
	customer->serviceDesk->setFree();

	// Send to cashier queue
	auto it = std::find(customersInRestaurant.begin(), customersInRestaurant.end(), customer);
	customersInRestaurant.erase(it);

	customersInCashierQueue.push_back(customer);

	cout << "---------------------------------------------------------" << endl;
	cout << "Sending customer to cashier queue. Customer id: " << customer->getID() << endl;
	cout << "---------------------------------------------------------" << endl;

}

/* Check and return true if any cashier is free. Otherwise return false.*/
bool DepartureHall::canSendToCashier()
{
	for (auto cashier : cashiers)
	{
		if (cashier->getIsFree())
		{
			currentCashier = cashier;
			return true;
		}
	}
	return false;
}

/** 1. Check if any cashier is free
	2. Remove customer from queue
	3. Move customer to cashier
	4. Assign cashier to customer
	5. Assign customer to cashier
*/
void DepartureHall::sendToCashier(Customer* customer)
{
	if (canSendToCashier())
	{
		// Move from queue to customer
		customersInCashierQueue.erase(customersInCashierQueue.begin());
		customersInCashier.push_back(customer);

		// Assign relation
		customer->cashier = currentCashier;
		currentCashier->customer = customer;

		// Calculate CST
		customer->CST = getCST() + programTime;

		cout << "---------------------------------------------------------" << endl;
		cout << "Sending customer to cashier. Customer id: " << customer->getID() << endl;
		cout << "Cashier service time for this customer: " << customer->CST << endl;
		cout << "---------------------------------------------------------" << endl;
	}
}

/** 1. Set cashier free
	2. Delete customer
*/
void DepartureHall::endCashierService(Customer* customer)
{
	cout << "---------------------------------------------------------" << endl;
	cout << "Ending cashier service. Customer id: " << customer->getID() << endl;
	cout << "---------------------------------------------------------" << endl;
	customer->cashier->setFree();
	customer->~Customer();
}

double DepartureHall::getBT()
{
	srand(time(NULL));
	double x = (rand() % ((BTA + BTV) - (BTA - BTV) + 1)) + (double)(BTA + BTV);
	//return (1 / (standartDeviation * sqrt(2 * 3.14))) * pow(2.72, -1 * pow((x - averageAT), 2) / 2 * pow(standartDeviation, 2));
	double r = (1 / (0.2 * sqrt(2 * 3.14))) * pow(2.72, (-1 * pow((x - 1.8), 2) / 2 * pow(0.2, 2)));
	return x;
}

bool DepartureHall::checkIfBuffetFree()
{
	if (!customersInBuffetQueue.empty() && numberOfFreeSeat >= customersInBuffetQueue.front()->numberOfCustomers)
	{
		return true;
	}
	return false;
}

void DepartureHall::startBuffetService()
{
	Customer* customer = customersInBuffetQueue.front();
	numberOfFreeSeat -= customersInBuffetQueue.front()->numberOfCustomers; // Decrease number of free seats
	// Swap customer to other vector
	customersInBuffet.push_back(customersInBuffetQueue.front());
	customersInBuffetQueue.erase(customersInBuffetQueue.begin());

	cout << "---------------------------------------------------------" << endl;
	cout << "Buffet service is started for the customer with id: " << customer->getID() << endl;
	cout << "Buffet service time for this customer: " << customer->CST << endl;
	cout << "---------------------------------------------------------" << endl;
}

void DepartureHall::endBuffetService(Customer* customer)
{
	cout << "---------------------------------------------------------" << endl;
	cout << "Buffet service is ended for the customer with id: " << customer->getID() << endl;
	cout << "---------------------------------------------------------" << endl;

	numberOfFreeSeat += customer->numberOfCustomers;
	customersInCashierQueue.push_back(customer);

	auto it = std::find(customersInBuffet.begin(), customersInBuffet.end(), customer);
	customersInBuffet.erase(it);
}
