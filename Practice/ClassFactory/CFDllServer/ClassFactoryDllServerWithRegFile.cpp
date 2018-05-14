#include<Windows.h>
#include"ClassFactoryDllServerWithRegFile.h"

class CSumSubtract : public ISum, ISubtract {
private:
	long m_cRef;

public:
	//ctor
	CSumSubtract(void);
	//dtor
	~CSumSubtract(void);

	//IUnknown Specific methods declarations
	HRESULT __stdcall QueryInterface(const IID &, void**);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	//CSUM specific methods
	HRESULT __stdcall SumOfTwoIntegers(int, int, int*);

	//CSubtract specific methods
	HRESULT __stdcall SubtractionOfTwoIntegers(int, int, int*);
};

class CSumSubtractClassFactory : public IClassFactory {
private:
	long m_cRef;
public:
	CSumSubtractClassFactory(void);

	~CSumSubtractClassFactory(void);

	//IUnknown Specific methods declarations
	HRESULT __stdcall QueryInterface(const IID &, void**);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);


	//class Factory Specific Methods
	HRESULT __stdcall CreateInstance(IUnknown *, const IID &, void **);
	HRESULT __stdcall LockServer(BOOL);
};

// global variable declaration
long glNumberOfActiveComponents = 0;
// number of locks on this dll
long glNumberOfServerLocks = 0;

//DLL Main
BOOL WINAPI DllMain(HINSTANCE hDll, DWORD dwReason, LPVOID reserved) {
	switch (dwReason) {
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return(TRUE);
}

CSumSubtract::CSumSubtract(void)
{
	m_cRef = 1; //hardcoded initialization to anticipate possible failure of QueryInterface
	InterlockedIncrement(&glNumberOfActiveComponents);
}

CSumSubtract::~CSumSubtract(void)
{
	InterlockedDecrement(&glNumberOfActiveComponents);
}

HRESULT CSumSubtract::QueryInterface(const IID &iid, void **ppv)
{
	if (iid == IID_IUnknown)
		*ppv = static_cast<ISum *>(this);
	else if (iid == IID_ISum)
		*ppv = static_cast<ISum *>(this);
	else if (iid == IID_ISubtract)
		*ppv = static_cast<ISubtract *>(this);
	else {
		*ppv = NULL;
		return(E_NOINTERFACE);
	}
	reinterpret_cast<IUnknown *>(*ppv)->AddRef();
	return(S_OK);
}

ULONG CSumSubtract::AddRef(void)
{
	InterlockedIncrement(&m_cRef);
	return 0;
}

ULONG CSumSubtract::Release(void)
{
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0) {
		delete(this);
		return(0);
	}
	return(m_cRef);
}

HRESULT CSumSubtract::SumOfTwoIntegers(int num1, int num2, int *pSum)
{
	*pSum = num1 + num2;
	return(S_OK);
}

HRESULT CSumSubtract::SubtractionOfTwoIntegers(int num1, int num2, int *pSubtract)
{
	*pSubtract = num1 - num2;
	return(S_OK);
}


CSumSubtractClassFactory::CSumSubtractClassFactory(void)
{
	m_cRef = 1;
}

CSumSubtractClassFactory::~CSumSubtractClassFactory(void)
{
}

HRESULT CSumSubtractClassFactory::QueryInterface(const IID &iid, void **ppv)
{
	if (iid == IID_IUnknown)
		*ppv = static_cast<IClassFactory *>(this);
	else if (iid == IID_IClassFactory)
		*ppv = static_cast<IClassFactory *>(this);
	else {
		*ppv = NULL;
		return(E_NOINTERFACE);
	}
	reinterpret_cast<IUnknown *>(*ppv)->AddRef();
	return(S_OK);
}

ULONG CSumSubtractClassFactory::AddRef(void)
{
	InterlockedIncrement(&m_cRef);
	return 0;
}

ULONG CSumSubtractClassFactory::Release(void)
{
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0) {
		delete(this);
		return(0);
	}
	return(m_cRef);
}

HRESULT CSumSubtractClassFactory::CreateInstance(IUnknown *pUnknownOuter, const IID &iid, void **ppv)
{
	CSumSubtract *pCSumSubtract = NULL;
	HRESULT hr;

	if (pUnknownOuter != NULL) {
		return(CLASS_E_NOAGGREGATION);
	}

	pCSumSubtract = new CSumSubtract;

	if (pCSumSubtract == NULL)
		return(E_OUTOFMEMORY);

	hr = pCSumSubtract->QueryInterface(iid, ppv);
	pCSumSubtract->Release();

	return(hr);
}

HRESULT CSumSubtractClassFactory::LockServer(BOOL fLock)
{
	if (fLock) {
		InterlockedIncrement(&glNumberOfServerLocks);
	}
	else {
		InterlockedDecrement(&glNumberOfServerLocks);
	}

	return(S_OK);
}

//Implementation of EXPORTED functions

HRESULT __stdcall DllGetClassObject(const IID & clsid, const IID& iid, void **ppv)
{
	CSumSubtractClassFactory *pCSumSubtractClassFactory = NULL;
	HRESULT hr;

	if (clsid != CLSID_SumSubtract)
		return(CLASS_E_CLASSNOTAVAILABLE);

	pCSumSubtractClassFactory = new CSumSubtractClassFactory;

	if (pCSumSubtractClassFactory == NULL)
		return(E_OUTOFMEMORY);

	hr = pCSumSubtractClassFactory->QueryInterface(iid, ppv);
	pCSumSubtractClassFactory->Release();

	return(hr);
}

HRESULT __stdcall DllCanUnloadNow(void) {
	if (glNumberOfActiveComponents == 0 && glNumberOfServerLocks == 0)
		return(S_OK);
	else
		return(S_FALSE);
}