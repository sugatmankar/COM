#include<Windows.h>

#include"ClassFactoryDllServerWithRegFile.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

ISum *pISum = NULL;
ISubtract *pISubtract = NULL;

//typedef HRESULT(*CREATEINSTANCE)(IUnknown *pUnknownOuter, const IID &iid, void **ppv);
typedef HRESULT(*DLLGETCLASSOBJECT)(const IID &, const IID &, void **);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow) {

	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR AppName[] = TEXT("ComClient");
	HRESULT hr;

	//COM Initialization 
	hr = CoInitialize(NULL);  // Present in ObjBase.h

	if (FAILED(hr)) {
		MessageBox(NULL, TEXT("COM Initialization Failed"), TEXT("ERROR"), MB_OK);
		exit(0);
	}

	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hInstance = hInstance;
	wndclass.lpszClassName = AppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wndclass);

	hwnd = CreateWindow(AppName, TEXT("Client of COM DLL SERVER"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	//COM uninitialization
	CoUninitialize();

	return((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {

	HRESULT hr;
	int iNum1, iNum2, iSum;
	TCHAR str[255];

	switch (iMsg)
	{
	case WM_CREATE:
	{
		//HRESULT CreateInstance(IUnknown *pUnknownOuter, const IID &iid, void **ppv)
		/*HINSTANCE hComp = LoadLibrary("CFDllServer.dll");
		if (hComp==NULL) {
			MessageBox(hwnd, TEXT("Failed to Load DLL"), TEXT("ERROR"), MB_OK);
			DestroyWindow(hwnd);
			break;
		}*/
		DLLGETCLASSOBJECT inst = (DLLGETCLASSOBJECT)::GetProcAddress(LoadLibrary("CFDllServer.dll"), "DllGetClassObject");

		hr = inst(CLSID_SumSubtract, IID_ISum, (void**)&pISum); //combaseapi.h
		if (FAILED(hr)) {
			MessageBox(hwnd, TEXT("Failed to obtain ISum interface"), TEXT("ERROR"), MB_OK);
			DestroyWindow(hwnd);
			break;
		}
		iNum1 = 55;
		iNum2 = 45;

		pISum->SumOfTwoIntegers(iNum1, iNum2, &iSum);

		wsprintf(str, TEXT("Sum of %d and %d is %d"), iNum1, iNum2, iSum);
		MessageBox(hwnd, str, TEXT("Result"), MB_OK);

		hr = pISum->QueryInterface(IID_ISubtract, (void**)&pISubtract);
		if (FAILED(hr)) {
			MessageBox(hwnd, TEXT("Failed to obtain ISubtract interface"), TEXT("ERROR"), MB_OK);
			DestroyWindow(hwnd);
		}
		pISum->Release();
		pISum = NULL;

		iNum1 = 555;
		iNum2 = 444;

		pISubtract->SubtractionOfTwoIntegers(iNum1, iNum2, &iSum);

		wsprintf(str, TEXT("Subtraction of %d and %d is %d"), iNum1, iNum2, iSum);
		MessageBox(hwnd, str, TEXT("Result"), MB_OK);
		pISubtract->Release();
		pISubtract = NULL;

		DestroyWindow(hwnd);
	}
	break;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	break;


	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}