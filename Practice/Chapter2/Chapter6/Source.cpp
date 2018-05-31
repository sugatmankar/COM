#define UNICODE

#include <stdlib.h>

#include<iostream>

#include<assert.h>

#include<comcat.h>

//Fun declaration : Worker functions

//List the registered component categories
BOOL ListCategories();

//Register our component category
BOOL RegisterCategory();

//UnRegister our component category
void UnRegisterCategory();

//add component
void AddComponent();

//remove component
void RemoveComponent();

//List all of the components which are members of sample category
void ListCategoryMembers();

// print error message from HRESULT
void ErrorMessage(const char* str, HRESULT hr);

//get friendly name of CLSID from the registery and display
void OutputFriendlyName(const CLSID& clsid);

//Global Data and const
ICatInformation* g_pICatInformation = NULL;
ICatRegister* g_pICatRegister = NULL;




// {81940BC9-0120-4C45-A21B-4ED25500F8F7}
static GUID CATID_SampleCategories[1] = { 0x81940bc9, 0x120, 0x4c45,{ 0xa2, 0x1b, 0x4e, 0xd2, 0x55, 0x0, 0xf8, 0xf7 } };


CATEGORYINFO g_SampleCategoryInfo = {
	{ 0x81940bc9, 0x120, 0x4c45,{ 0xa2, 0x1b, 0x4e, 0xd2, 0x55, 0x0, 0xf8, 0xf7 } }, //CATID 
	LOCALE_SYSTEM_DEFAULT, //LCID
	L"This is sample category" //OLEChar description
};

//component to add to the sample category
// {9641CB03-1C0C-48D9-87C9-02D9E148F9CF}
extern "C" const CLSID CLSID_Component1 = { 0x9641cb03, 0x1c0c, 0x48d9,{ 0x87, 0xc9, 0x2, 0xd9, 0xe1, 0x48, 0xf9, 0xcf } };

int main(int argc, const char* argv[])
{
	//Initialzie com library
	OleInitialize(NULL);
	
	//Create Standard COM cat manager
	HRESULT hr = ::CoCreateInstance(CLSID_StdComponentCategoriesMgr,
		NULL,
		CLSCTX_ALL,
		IID_ICatInformation,
		(void**)&g_pICatInformation
		);

	if (FAILED(hr))
	{
		ErrorMessage("Main:: could not create COM component category manager", hr);
		OleUninitialize();
	}

	//List category
	if (!ListCategories())
	{
		OleUninitialize();
	}

	//get cat registration interface
	hr = g_pICatInformation->QueryInterface(IID_ICatRegister,(void**)&g_pICatRegister);
	if (FAILED(hr))
	{
		ErrorMessage("Main:: Attempt to query for ICatRegister failed", hr);
		OleUninitialize();
	}

	std::cout << "______Register_________" << std::endl;

	if (!RegisterCategory())
	{
		OleUninitialize();
	}
	//List cat to show newly registered cat
	std::cout << "_________ListCategories______" << std::endl;
	ListCategories();

	std::cout << "______Add Component_________" << std::endl;
	AddComponent();

	std::cout << "ListCategoryMembers" << std::endl;
	ListCategoryMembers();





}

BOOL ListCategories()
{
	BOOL bReturn = TRUE;
	//get an enumerator for the categories

	IEnumCATEGORYINFO* pIEnumCATEGORYINFO = NULL;

	HRESULT hr = g_pICatInformation->EnumCategories(::GetUserDefaultLangID(),&pIEnumCATEGORYINFO);

	if (FAILED(hr))
	{
		ErrorMessage("ListCategories:: IEnumCATEGORYINFO::EnumCategories failed.", hr);
		OleUninitialize();
	}

	//prepare for loop
	char szDescription[128];
	CATEGORYINFO catInfo;

	while ((hr = pIEnumCATEGORYINFO->Next(1, &catInfo, NULL)) == S_OK)
	{
		//convert from wchar_t to char
		::wcstombs_s(NULL,szDescription,catInfo.szDescription,sizeof(szDescription));

		//print description
		std::cout << szDescription << "\r\n";
	}

	if (FAILED(hr))
	{
		ErrorMessage("ListCategories:: IEnumCATEGORYINFO::Next failed.", hr);
		bReturn = NULL;
	}

	//release interface
	if (pIEnumCATEGORYINFO != NULL)
		pIEnumCATEGORYINFO->Release();

	return bReturn;
}

void ListCategoryMembers()
{
	//get enum for the categories
	IEnumCLSID* pIEnumCLSID = NULL;
	HRESULT hr = g_pICatInformation->EnumClassesOfCategories(1,CATID_SampleCategories,0,NULL,&pIEnumCLSID);
	if (FAILED(hr))
	{
		ErrorMessage("ListCategoryMembers:: EnumClassesOfCategories failed.", hr);
	}

	//prepare for looop
	CLSID clsid;

	//enumerate categories
	while (TRUE)
	{
		hr = pIEnumCLSID->Next(1,&clsid,NULL);
		if (hr != S_OK)
		{
			if (FAILED(hr))
			{
				ErrorMessage("ListCategoryMembers:: pIEnumCLSID::Next failed.", hr);
			}
			break;
		}
		OutputFriendlyName(clsid);
	}

	//release Interface
	if (pIEnumCLSID != NULL)
	{
		pIEnumCLSID->Release();
	}
}

void OutputFriendlyName(const CLSID& clsid) 
{
	//convert clsid to string
	wchar_t wszCLSID[80];
	int r = ::StringFromGUID2(clsid,wszCLSID,80);
	assert(r !=0);

	//convert single byte char
	//wchar_t szCLSID[40];
	//wcstombs(szCLSID,wszCLSID,40);

	//open registery key
	HKEY key = NULL;

	long l = ::RegOpenKeyEx(HKEY_CLASSES_ROOT,L"CLSID",0,KEY_ALL_ACCESS,&key);
	assert(l == ERROR_SUCCESS);

	//get friendly name
	wchar_t szFriendly[256];
	long size = sizeof(szFriendly);
	l = ::RegQueryValue(key, wszCLSID,szFriendly,&size);
	assert(l == ERROR_SUCCESS);

	//output friendly name
	std::cout << szFriendly << std::endl;

	//cleanUp
	::RegCloseKey(key);
}

void ErrorMessage(const char* str, HRESULT hr)
{
	void* pMsgBuff;

	::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		hr,
		MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
		(LPTSTR)&pMsgBuff,
		0,
		NULL
		);
	std::cout << str << "\r\n";
	std::cout << "Error (" << _HEX << hr << "): " << (char*)pMsgBuff << std::endl;
	//Free buffer
	LocalFree(pMsgBuff);
}

//Register our component category
BOOL RegisterCategory()
{
	HRESULT hr = g_pICatRegister->RegisterCategories(1, &g_SampleCategoryInfo);
	if (FAILED(hr))
	{
		ErrorMessage("Register Category : Registering the component category failed",hr);
		return FALSE;
	}
	return TRUE;
}

//unregister
void UnRegisterCategory()
{
	HRESULT hr = g_pICatRegister->UnRegisterCategories(1,CATID_SampleCategories);
	if (FAILED(hr))
	{
		ErrorMessage("UnRegister Category : UnRegistering the component category failed", hr);
	}
}

//add component
void AddComponent() 
{
	HRESULT hr = g_pICatRegister->RegisterClassImplCategories(CLSID_Component1,1,CATID_SampleCategories);
	if (FAILED(hr))
	{
		ErrorMessage("AddComponent  : AddComponent the component adding failed", hr);
	}
}

//remove component
void RemoveComponent()
{
	HRESULT hr = g_pICatRegister->UnRegisterClassImplCategories(CLSID_Component1, 1, CATID_SampleCategories);
	if (FAILED(hr))
	{
		ErrorMessage("RemoveComponent  : RemoveComponent the component removing failed", hr);
	}
}