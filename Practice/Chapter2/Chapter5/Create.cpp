#include<iostream>
#include<Unknwn.h>

#include "Create.h"
#include "Trace.h"

typedef IUnknown* (*CREATEFUNCTIONPTR)();    //IUnknown* CreateInstance(){}
		

IUnknown* CallCreateInstance(char* name)
{
	//Load dll into process
	HINSTANCE hComponent = ::LoadLibrary(name);
	if (hComponent == NULL)
	{
		trace("CallCreateInstance:: \t Error: Can't Load Component.");
		return NULL;
	}

	//Get address of CreateInstance function 
	CREATEFUNCTIONPTR CreateInstance = (CREATEFUNCTIONPTR)::GetProcAddress(hComponent,"CreateInstance");

	if (CreateInstance == NULL)
	{
		trace("CallCreateInstance: \t Error: Can't find CreateInstance function");
		return NULL;
	}

	return CreateInstance();
}

