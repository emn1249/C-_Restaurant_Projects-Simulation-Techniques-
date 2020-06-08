#include <iostream>
#include <thread>
#include <chrono>
#include "Customer.h"
#include "ServiceDesk.h"
#include "DepartureHall.h"

using namespace std;

int Customer::global_id_ = 0;

Customer::Customer(DepartureHall* DP, bool willGoToBuffet) :kID_(global_id_++), bWillGoToBuffet(willGoToBuffet)
{
	// Calculate number of people for this customer
	numberOfCustomers = calculateNumberOfPeople();

	// Print info about customer
	cout << "---------------------------------------------------------" << endl;
	cout << "New customer created. ID number: " << kID_ << endl;
	cout << "Number of people for this customer: " << numberOfCustomers << endl;
	cout << "---------------------------------------------------------" << endl;
}


Customer::~Customer()
{

}

int Customer::getID() const
{
	return kID_;
}

int Customer::calculateNumberOfPeople()
{
	
	double probability = (double)std::rand() / RAND_MAX;

	if (probability <= p1)
		return 1;
	else if (probability <= p1 + p2)
		return 2;
	else if (probability <= p1 + p2 + p3)
		return 3;
	else if (probability <= 1)
		return 4;

	return 0;
}
