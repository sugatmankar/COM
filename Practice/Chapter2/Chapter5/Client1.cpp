// To compile, use: cl Client1.cpp Create.cpp GUIDs.cpp UUID.lib

#include<iostream>
#include<objbase.h>

#include"IFace.h"
#include"Create.h"
#include"Trace.h"

int main(int argc, const char* argv[])
{
	openLogFile("Client1Log.txt");
	
	HRESULT hr;

	//Get Name of the component to Use.
	char name[40];

	std::cout << "Enter the filename of a component to use [Component?.dll]";

	std::cin >> name;

	std::cout << std::endl;

	trace("Client1: Get an IUnknown Pointer");

	IUnknown* pIunknown = CallCreateInstance(name);

	if (pIunknown == NULL)
	{
		trace("Client1: CallCreateInstance Failed.");
	}
	else {
		trace("Client1: CallCreateInstance Succeeded.");
	}

	trace("Client1: Get Interface IX");

	IX* pIX;

	hr = pIunknown->QueryInterface(IID_IX, (void**)&pIX);

	if (SUCCEEDED(hr))
	{
		trace("Client1: Succeeded getting IX.");
		pIX->Fx();
		pIX->Release();

	}
	else
	{
		trace("Client1: Failed to get IX.");
	}

	trace("Client1: Release IUnknown");

	pIunknown->Release();

	CloseLogFile();

	return 0;
}