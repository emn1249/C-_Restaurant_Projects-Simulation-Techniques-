#include "pch.h"
#include "ServiceDesk.h"
#include "Customer.h"

ServiceDesk::ServiceDesk(int nop)
{
	numberOfPlace = nop;
}


ServiceDesk::~ServiceDesk()
{
}

bool ServiceDesk::getIsFree()
{
	return isFree;
}

void ServiceDesk::setFree()
{
	isFree = true;
}

void ServiceDesk::setBusy()
{
	isFree = false;
}