#ifndef ERRORS_H
#define ERRORS_H

#include <stdio.h>
#include <windows.h>

void printError(char *msg, DWORD errorNum)
{
    char sysMsg[256];

    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, errorNum, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),sysMsg, 256, NULL);

    printf("\nWARNING: %s failde with error %d (%s)\n", msg, errorNum, sysMsg);
}

#endif