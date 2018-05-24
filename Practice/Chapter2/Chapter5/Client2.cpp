// To compile, use: cl /EHsc Client2.cpp Create.cpp Trace.cpp GUIDs.cpp UUID.lib 
#include<iostream>
#include<objbase.h>

#include"IFace.h"
#include"Create.h"
#include"Trace.h"

int main(int argc, const char* argv[])
{
	openLogFile("Client2Log.txt");

	HRESULT hr;

	//Get Name of the component to Use.
	char name[40];

	std::cout << "Enter the filename of a component to use [Component?.dll]";

	std::cin >> name;

	std::cout << std::endl;

	trace("Client2: Get an IUnknown Pointer");

	IUnknown* pIunknown = CallCreateInstance(name);

	if (pIunknown == NULL)
	{
		trace("Client2: CallCreateInstance Failed.");
	}
	else {
		trace("Client2: CallCreateInstance Succeeded.");
	}

	trace("Client2: Get Interface IX");

	IX* pIX;

	hr = pIunknown->QueryInterface(IID_IX, (void**)&pIX);

	if (SUCCEEDED(hr))
	{
		trace("Client2: Succeeded getting IX.");
		pIX->Fx();
		pIX->Release();

	}
	else
	{
		trace("Client2: Failed to get IX.");
	}

	trace("Client2: Get Interface IY");

	IY* pIY;

	hr = pIunknown->QueryInterface(IID_IY, (void**)&pIY);

	if (SUCCEEDED(hr))
	{
		trace("Client2: Succeeded getting IY.");
		pIY->Fy();
		pIY->Release();

	}
	else
	{
		trace("Client2: Failed to get IY.");
	}

	trace("Client2: Release IUnknown");

	pIunknown->Release();

	CloseLogFile();

	return 0;
}