// To compile, use: cl Client3.cpp Create.cpp GUIDs.cpp UUID.lib

#include<iostream>
#include<objbase.h>

#include"IFace.h"
#include"Create.h"
#include"Trace.h"

int main(int argc, const char* argv[])
{
	openLogFile("Client3Log.txt");

	HRESULT hr;

	//Get Name of the component to Use.
	char name[40];

	std::cout << "Enter the filename of a component to use [Component?.dll]";

	std::cin >> name;

	std::cout << std::endl;

	trace("Client3: Get an IUnknown Pointer");

	IUnknown* pIunknown = CallCreateInstance(name);

	if (pIunknown == NULL)
	{
		trace("Client3: CallCreateInstance Failed.");
	}
	else {
		trace("Client3: CallCreateInstance Succeeded.");
	}

	trace("Client3: Get Interface IX");

	IX* pIX;

	hr = pIunknown->QueryInterface(IID_IX, (void**)&pIX);

	if (SUCCEEDED(hr))
	{
		trace("Client3: Succeeded getting IX.");
		pIX->Fx();
		pIX->Release();

	}
	else
	{
		trace("Client3: Failed to get IX.");
	}

	trace("Client3: Get Interface IY");

	IY* pIY;

	hr = pIunknown->QueryInterface(IID_IY, (void**)&pIY);

	if (SUCCEEDED(hr))
	{
		trace("Client3: Succeeded getting IY.");
		pIY->Fy();
		pIY->Release();

	}
	else
	{
		trace("Client3: Failed to get IY.");
	}


	trace("Client3: Get Interface IZ");

	IZ* pIZ;

	hr = pIunknown->QueryInterface(IID_IZ, (void**)&pIZ);

	if (SUCCEEDED(hr))
	{
		trace("Client3: Succeeded getting IZ.");
		pIZ->Fz();
		pIZ->Release();

	}
	else
	{
		trace("Client3: Failed to get IZ.");
	}

	trace("Client3: Release IUnknown");

	pIunknown->Release();

	CloseLogFile();

	return 0;
}