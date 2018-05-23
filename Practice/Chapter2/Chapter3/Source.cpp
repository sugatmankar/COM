#include<iostream>
#include<objbase.h>

void trace(const char* msg) { std::cout << msg << std::endl; }

interface IX : public IUnknown
{
	virtual void __stdcall Fx() = 0;
};

interface IY : public IUnknown
{
	virtual void __stdcall Fy() = 0;

};

interface IZ : public IUnknown
{
	virtual void __stdcall Fz() = 0;
};

// fwd ref for GUIDs
extern const IID IID_IX;
extern const IID IID_IY;
extern const IID IID_IZ;

//Component
class CA : public IX, public IY
{
	//methods of IUnknown
	virtual HRESULT __stdcall QueryInterface(const IID& iid, void **ppv);
	virtual ULONG __stdcall AddRef()
	{
		return 0;
	}
	virtual ULONG __stdcall Release()
	{
		return 0;
	}

	virtual void __stdcall Fx()
	{
		trace("CA:: Fx");
	}

	virtual void __stdcall Fy()
	{
		trace("CA:: Fy");
	}

};

HRESULT __stdcall CA::QueryInterface(const IID& iid, void **ppv)
{
	if (iid == IID_IUnknown)
	{
		trace("CA::QueryInterface: Return ptr to IUnknown");
		*ppv = static_cast<IX*>(this);
	}
	else if (iid == IID_IX)
	{
		trace("CA::QueryInterface: Return ptr to IX");
		*ppv = static_cast<IX*>(this);
	}
	else if (iid == IID_IY)
	{
		trace("CA::QueryInterface: Return ptr to IY");
		*ppv = static_cast<IY*>(this);
	}
	else
	{
		trace("CA::QueryInterface No interface supported");
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	reinterpret_cast<IUnknown*>(*ppv)->AddRef();

	return S_OK;
}

//Creation func
IUnknown* CreateInstance()
{
	IUnknown* pI = static_cast<IX*>(new CA);
	pI->AddRef();
	return pI;
}

// {2165357F-2988-4954-A599-905F2FDC5D12}
static const IID IID_IX =
{ 0x2165357f, 0x2988, 0x4954,{ 0xa5, 0x99, 0x90, 0x5f, 0x2f, 0xdc, 0x5d, 0x12 } };

// {67569502-CE06-4C27-957A-FF2172744F63}
static const IID IID_IY =
{ 0x67569502, 0xce06, 0x4c27,{ 0x95, 0x7a, 0xff, 0x21, 0x72, 0x74, 0x4f, 0x63 } };

// {CCF634B9-CD1F-4FBA-B14D-9377F50174C2}
static const IID IID_IZ =
{ 0xccf634b9, 0xcd1f, 0x4fba,{ 0xb1, 0x4d, 0x93, 0x77, 0xf5, 0x1, 0x74, 0xc2 } };


int main(int argc, const char* argv[])
{
	HRESULT hr;

	trace("Client: Create Instance");

	IUnknown* pIUnknown = CreateInstance();

	trace("Client: Get Interface IX");
	IX* pIX = NULL;
	hr = pIUnknown->QueryInterface(IID_IX, (void**)&pIX);

	if (SUCCEEDED(hr))
	{
		trace("Client: succeeded getting IX");
		pIX->Fx();
	}

	trace("Client: Get Interface IY");
	IY* pIY = NULL;
	hr = pIUnknown->QueryInterface(IID_IY, (void**)&pIY);
	if (SUCCEEDED(hr))
	{
		trace("Client: Succeeded getting IY");
		pIY->Fy();
	}

	trace("Client: Ask for unsupported interface");
	IZ* pIZ = NULL;

	hr = pIUnknown->QueryInterface(IID_IZ, (void**)&pIZ);
	if (SUCCEEDED(hr))
	{
		trace("Client: Succeeded getting IZ");
		pIZ->Fz();
	}
	else
	{
		trace("Client: Couldn't Get IZ");
	}

	trace("Client: Get IY from IX");

	IY* pIYFromIX = NULL;
	hr = pIX->QueryInterface(IID_IY, (void**)&pIYFromIX);
	if (SUCCEEDED(hr))
	{
		trace("Client: Succeeded to get IY from IX");
		pIYFromIX->Fy();
	}

	trace("Client: IUnknwown from IY");
	IUnknown *pIUnknownFromIY = NULL;
	hr = pIY->QueryInterface(IID_IUnknown, (void**)&pIUnknownFromIY);
	if (SUCCEEDED(hr))
	{
		trace("Client: Are IUnknown pointers are equals? ");
		if (pIUnknownFromIY == pIUnknown)
			trace("Client: Both IUnknown pointers are equal...");
		else
			trace("Client: Both IUnknown pointers are NOT equal...");
	}

	delete(pIUnknown);
	return 0;

}