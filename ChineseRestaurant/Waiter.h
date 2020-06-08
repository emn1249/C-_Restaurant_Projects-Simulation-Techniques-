#pragma once
class Waiter
{
public:
	Waiter();
	~Waiter();
	bool getIsFree();
	void setBusy();
	void setFree();
private:
	bool isFree = true;
};

