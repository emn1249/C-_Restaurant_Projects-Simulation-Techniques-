#ifndef MODEL_DEPARTURE_HALL_H_
#define MODEL_DEPARTURE_HALL_H_

#include <queue>

class Customer;
class ServiceDesk;
class Waiter;

struct DepartureHall
{
	DepartureHall();
	~DepartureHall();

	int n1 = 0, n2 = 4, n3 = 10, n4 = 4; // Number of double, triple and four-person tables respectively
	std::vector<int> numOfPlaces = { n1,n2,n3,n4 }; // Initilize number of places for each table type

	const int k = 4; // Number of waiters
	std::vector<Waiter*> waiters; // Pointer to waiters

	const int c = 4; // Number of cashiers
	std::vector<class Cashier*> cashiers; // Pointer to waiters

	const int MT = 30; // Time for a customer to place to table
	double AT = 0; // Next customer arrival time 
	double getAT(); // Calculate next customer arrival time
	int AAT = 1800;
	int SD = 200;

	double avgWST = 3600; // average waiter service time
	double getWST(); // Calculate average waiter service time

	double avgCST = 180; // average cashier service time
	double getCST(); // Calculate average cashier service time

	void spawnCustomer(); // Spawn customer to restaurant queue or buffet queue based on probability
	void queueCustomer(); // Spawn new customer to restaurant queue
	void queueCustomerToBuffet(); // Spawn customer to buffet queue

	void checkForServiceStart(); // Check if first customer in the queue fits to a table and if any waiter is free. 
	bool canStartService = false;

	void startService();
	void endService(Customer* customer);

	bool canSendToCashier(); // Function to detect if any cashier is free
	void sendToCashier(Customer* customer);
	void endCashierService(Customer* customer);

	/* Variables for Buffet */
	const float pb = 0.5; // Probability for customers to go buffet
	const int b = 14; // Number of buffet seats
	int numberOfFreeSeat = b;
	float BT; // Time spend in the buffet by the customers
	const int BTA = 3200; // Buffet time average
	const int BTV = 200; // Buffet time variace
	double getBT();
	bool checkIfBuffetFree();
	void startBuffetService();
	void endBuffetService(Customer* customer);

	int programTime = 0;

	Waiter* currentWaiter = nullptr;
	Customer* currentCustomer = nullptr;
	ServiceDesk* currentServiceDesk = nullptr;
	Cashier* currentCashier = nullptr;

	std::vector<Customer*> customersInQueue;
	std::vector<Customer*> customersInBuffetQueue;
	std::vector<Customer*> customersInRestaurant;
	std::vector<Customer*> customersInBuffet;
	std::vector<Customer*> customersInCashierQueue;
	std::vector<Customer*> customersInCashier;

	std::vector<ServiceDesk*> service_desks;
};
#endif /*MODEL_DEPARTURE_HALL_H_*/