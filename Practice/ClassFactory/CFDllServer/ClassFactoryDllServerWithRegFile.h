#pragma once
#include<windows.h>

class ISum : public IUnknown {
public:
	virtual HRESULT __stdcall SumOfTwoIntegers(int,int,int*)=0;
};

class ISubtract : public IUnknown {
public:
	virtual HRESULT __stdcall SubtractionOfTwoIntegers(int,int,int*) = 0;

};

// {DA127FF1-3051-485C-8CF6-1FE22FD7BF1E}
static const CLSID CLSID_SumSubtract = { 0xda127ff1, 0x3051, 0x485c,{ 0x8c, 0xf6, 0x1f, 0xe2, 0x2f, 0xd7, 0xbf, 0x1e } };

// {3419DD70-FC74-4E03-91E8-0A730C9A77F1}
static const IID IID_ISum = { 0x3419dd70, 0xfc74, 0x4e03,{ 0x91, 0xe8, 0xa, 0x73, 0xc, 0x9a, 0x77, 0xf1 } };

// {99D04EDD-7278-4EED-B817-8B640AE3AD14}
static const IID IID_ISubtract= { 0x99d04edd, 0x7278, 0x4eed,{ 0xb8, 0x17, 0x8b, 0x64, 0xa, 0xe3, 0xad, 0x14 } };

