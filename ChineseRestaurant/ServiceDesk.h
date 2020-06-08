#pragma once
class ServiceDesk
{
public:
	ServiceDesk(int nop);
	~ServiceDesk();	
	int numberOfPlace;
	bool getIsFree();
	void setFree();
	void setBusy();

	class Customer *served_customer_;
private:
	bool isFree = true;
};

