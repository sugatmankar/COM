// Command to Compile
// cl.exe /LD /EHsc Component1.cpp Trace.cpp GUIDS.cpp UUID.lib /DEF Component1.def 
#define UNICODE

#include<iostream>
#include<objbase.h>

#include "IFace.h"
#include "Trace.h"


class CA : public IX
{
	//IUnknown methods
	virtual HRESULT __stdcall QueryInterface(const IID& iid, void **ppv);
	virtual ULONG __stdcall AddRef();
	virtual ULONG __stdcall Release();

	virtual void __stdcall Fx()
	{
		trace("CA::Fx");
	}

private:
	long m_cRef;

public:
	CA() : m_cRef(0)
	{
		trace("Component1:: ");
	}

	~CA()
	{
		trace("CA: ~CA: Destroy self.");
	}
};


HRESULT __stdcall CA::QueryInterface(const IID& iid, void **ppv)
{
	if (iid == IID_IUnknown)
	{
		trace("CA::QueryInterface Return ptr to IUnknown");
		*ppv = static_cast<IX*>(this);
	}
	else if (iid == IID_IX)
	{
		trace("CA::QueryInterface Return ptr to IX");
		*ppv = static_cast<IX*>(this);
	}
	else
	{
		trace("CA::QueryInterface Interface Requested not supported");
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return S_OK;
}

ULONG __stdcall CA::AddRef()
{
	fprintf_s(getFilePtr(), "CA::AddRef Refrence Count is %d\n\n", m_cRef + 1);
	return InterlockedIncrement(&m_cRef);
}

ULONG __stdcall CA::Release()
{
	fprintf_s(getFilePtr(), "CA::Release Refrence Count is %d\n\n", m_cRef - 1);
	if (InterlockedDecrement(&m_cRef) == 0)
	{
		delete(this);
		return 0;
	}
	return m_cRef;
}

//creation function 

extern "C" IUnknown* CreateInstance()
{
	trace("Component1: Create Instance");
	IUnknown* pIUnknown = static_cast<IX*>(new CA);
	pIUnknown->AddRef();
	return pIUnknown;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		openLogFile("Component1.txt");
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		CloseLogFile();
		break;
	}
	return TRUE;
}
