// Command to Compile
// cl.exe /LD Component2.cpp GUIDS.cpp UUID.lib Component2.def


#include<iostream>
#include<objbase.h>

#include "IFace.h"
#include "Trace.h"

class CA : public IX,public IY
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

private:
	long m_cRef;

public:
	CA() : m_cRef(0)
	{
		trace("Component2:: ");
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
	fprintf_s(gpFile, "CA::AddRef Refrence Count is %d", m_cRef + 1);
	return InterlockedIncrement(&m_cRef);
}

ULONG __stdcall CA::Release()
{
	fprintf_s(gpFile, "CA::Release Refrence Count is %d", m_cRef - 1);
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