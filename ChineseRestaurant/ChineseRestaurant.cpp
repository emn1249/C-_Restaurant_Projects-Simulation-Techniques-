// Emin.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <thread>
#include <chrono>
#include "DepartureHall.h"
#include "ServiceDesk.h"
#include "Customer.h"

using namespace std;
int main()
{
	DepartureHall DP = DepartureHall(); // Create Departure Hall

	while (1)
	{
		// Schedule time delay
		this_thread::sleep_for(chrono::milliseconds(1));

		// Check if any customer should be spawned. Spawns to buffet queue or restaurant queue based on probability.
		if (DP.AT == DP.programTime)
			DP.spawnCustomer();

		// Check queue and place customer if free service desk exists 
		DP.checkForServiceStart();
		if (DP.canStartService)
		{
			DP.startService();
			DP.canStartService = false;
		}

		// Move customers from buffet queue to seat if it's available
		if (DP.checkIfBuffetFree())
		{
			DP.startBuffetService();
		}

		// Check if waiter service time is over for any customer
		for (auto customer : DP.customersInRestaurant)
		{
			if ((int)customer->WST == DP.programTime)
			{
				DP.endService(customer);
			}
		}

		// Check if buffet time is over for any customer
		for (auto customer : DP.customersInBuffet)
		{
			if ((int)customer->BT == DP.programTime)
			{
				DP.endBuffetService(customer);
			}
		}

		// Check if anyone in cashier queue can go to cashier
		for (auto customer : DP.customersInCashierQueue)
		{
			DP.sendToCashier(customer);
		}

		// Check if cashier time is over for any customer
		for (auto customer : DP.customersInCashier)
		{
			if ((int)customer->CST == DP.programTime)
			{
				DP.endCashierService(customer);
			}
		}

		// Update Time and display it
		DP.programTime++;
		if (DP.programTime % 200 == 0)
		{
			cout << "Time: " << DP.programTime << endl;
		}
	}

	system("Pause");
}
