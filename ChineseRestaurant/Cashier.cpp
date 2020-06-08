#include "pch.h"
#include "Cashier.h"


Cashier::Cashier()
{
}


Cashier::~Cashier()
{
}

bool Cashier::getIsFree()
{
	return isFree;
}

void Cashier::setBusy()
{
	isFree = false;
}

void Cashier::setFree()
{
	isFree = true;
}