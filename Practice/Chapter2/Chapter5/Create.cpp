#define UNICODE

#include<iostream>
#include<Unknwn.h>

#include "Create.h"
#include "Trace.h"

typedef IUnknown* (*CREATEFUNCTIONPTR)();    //IUnknown* CreateInstance(){}
		

IUnknown* CallCreateInstance(char* name)
{
	//Load dll into process
	trace("CallCreateInstance");
	trace(name);
	wchar_t wText[40];
	mbstowcs(wText, name, strlen(name) + 1);
	LPWSTR ptr = wText;
	
	HINSTANCE hComponent = ::LoadLibrary(ptr);
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

