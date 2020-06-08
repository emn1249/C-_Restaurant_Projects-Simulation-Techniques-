#include "pch.h"
#include "Waiter.h"


Waiter::Waiter()
{
}


Waiter::~Waiter()
{
}

bool Waiter::getIsFree()
{
	return isFree;
}

void Waiter::setBusy()
{
	isFree = false;
}

void Waiter::setFree()
{
	isFree = true;
}