// Command to Compile
// cl.exe /LD /EHsc Component3.cpp Trace.cpp GUIDS.cpp UUID.lib Component3.def
#include<iostream>
#include<objbase.h>

#include "IFace.h"
#include "Trace.h"



class CA : public IX, public IY,public IZ
{
	//IUnknown methods
	virtual HRESULT __stdcall QueryInterface(const IID& iid, void **ppv);
	virtual ULONG __stdcall AddRef();
	virtual ULONG __stdcall Release();

	virtual void __stdcall Fx()
	{
		trace("CA::Fx");
	}

	virtual void __stdcall Fy()
	{
		trace("CA::Fy");
	}

	virtual void __stdcall Fz()
	{
		trace("CA::Fz");
	}

private:
	long m_cRef;

public:
	CA() : m_cRef(0)
	{
		trace("Component3:: ");
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
	else if (iid == IID_IY)
	{
		trace("CA::QueryInterface Return ptr to IY");
		*ppv = static_cast<IY*>(this);
	}
	else if (iid == IID_IZ)
	{
		trace("CA::QueryInterface Return ptr to IZ");
		*ppv = static_cast<IZ*>(this);
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
		openLogFile("Component3.txt");
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
