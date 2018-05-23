#ifndef TRACE
#include<iostream>

FILE *gpFile;
void trace(const char* msg)
{
	fprintf_s(gpFile, msg);
	fprintf_s(gpFile, "\n\n");
	std::cout << msg << std::endl;

}

void openLogFile(const char* fileName)
{
	fopen_s(&gpFile, fileName, "w");
}

void CloseLogFile()
{
	fclose(gpFile);
}
#endif // !TRACE



