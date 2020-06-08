#pragma once
class Cashier
{
public:
	Cashier();
	~Cashier();
	bool getIsFree();
	void setBusy();
	void setFree();
	class Customer* customer = nullptr;

private:
	bool isFree = true;

};

