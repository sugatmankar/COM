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

// {16F11C5B-56A2-4FED-BAC6-090CC274A441}
const CLSID CLSID_SumSubtract = { 0x16f11c5b, 0x56a2, 0x4fed, 0xba, 0xc6, 0x9, 0xc, 0xc2, 0x74, 0xa4, 0x41 };

// {664C40CF-7B77-43F5-9204-A51452CF6EB4}
const IID IID_ISum = { 0x664c40cf, 0x7b77, 0x43f5, 0x92, 0x4, 0xa5, 0x14, 0x52, 0xcf, 0x6e, 0xb4 };

// {D67F76D9-75BF-45B6-9201-947C88A4E699}
const IID IID_ISubtract = { 0xd67f76d9, 0x75bf, 0x45b6, 0x92, 0x1, 0x94, 0x7c, 0x88, 0xa4, 0xe6, 0x99 };
