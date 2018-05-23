
#include<iostream>

#include<objbase.h>

void trace(const char* msg) { std::cout << msg << std::endl; }


interface IX
{
	virtual void __stdcall Fx1() = 0;
	virtual void __stdcall Fx2() = 0;
};

interface IY
{
	virtual void __stdcall Fy1() = 0;
	virtual void __stdcall Fy2() = 0;
};

class CA : public IX, public IY
{
	//implement IX
	virtual void __stdcall Fx1() {
		trace("CA::FX1");
	}

	virtual void __stdcall Fx2() {
		trace("CA::FX2");
	}

	virtual void __stdcall Fy1() {
		trace("CA::Fy1");
	}

	virtual void __stdcall Fy2() {
		trace("CA::Fy2");
	}
};

int main(int argc, const char* argv[]) 
{
	trace("Client: Create an instance of the component");
	
	CA* pA = new CA;

	//Get IX pointer
	IX* pIX = pA;

	trace("Client:: Use IX");
	pIX->Fx1();
	pIX->Fx2();

	trace("Client:: Use IY");
	IY* pIY = pA;
	pIY->Fy1();
	pIY->Fy2();

	trace("Client:: Delete component");
	delete(pA);

	return 0;
}