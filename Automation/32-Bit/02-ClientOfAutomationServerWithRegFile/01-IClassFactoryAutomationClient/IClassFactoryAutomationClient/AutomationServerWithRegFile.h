class IMyMath :public IDispatch
{
public:
	virtual HRESULT __stdcall SumOfTwoIntegers(int, int, int *) = 0;// pure virtual
	virtual HRESULT __stdcall SubtractionOfTwoIntegers(int, int, int *) = 0;// pure virtual
};
// CLSID of MyMath Component : {71169462-E879-4B9B-ACFF-FB718082D292}
const CLSID CLSID_MyMath = { 0x71169462, 0xe879, 0x4b9b, 0xac, 0xff, 0xfb, 0x71, 0x80, 0x82, 0xd2, 0x92 };
// IID of ISum Interface : {2778214B-DE94-4BD8-B40B-DCF950D2FACC}
const IID IID_IMyMath = { 0x2778214b, 0xde94, 0x4bd8, 0xb4, 0xb, 0xdc, 0xf9, 0x50, 0xd2, 0xfa, 0xcc };
