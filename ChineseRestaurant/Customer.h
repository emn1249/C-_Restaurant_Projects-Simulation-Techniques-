#pragma once
class Customer
{
public:
	Customer(struct DepartureHall* DP, bool willGoToBuffet);
	~Customer();
	int getID() const;

	double p1 = 0.11, p2 = 0.33, p3 = 0.33, p4 = 0.23; // Probabilities for number of people in groups 
	int numberOfCustomers;
	int calculateNumberOfPeople();

	bool bWillGoToBuffet;
	class Waiter* waiter;
	class ServiceDesk* serviceDesk;
	class Cashier* cashier;
	double WST;
	double CST;
	double BT;

private:
	static int global_id_;
	const int kID_;
};

