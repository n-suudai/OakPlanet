
#include "FileSystemTest.hpp"


#include "Oak/Platform/FileSystem.hpp"
#include <cstdio>



int FileSystemTestMain()
{
    Oak::FileSystem file;

    file.Open("FileSystemTest.txt");

    const char writeText[] = "Hello FileSystem!!\n";

    file.Write(writeText, sizeof(writeText));

    file.Close();


    file.Open("FileSystemTest.txt");

    char readText[64] = "";

    Oak::SizeT readSize = file.Read(readText, 63);

    readText[readSize] = '\0';

    file.Close();

    OutputDebugStringA(readText);

    return 0;
}

