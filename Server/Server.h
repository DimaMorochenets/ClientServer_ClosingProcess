#ifndef SERVER_H
#define SERVER_H

#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>

UINT CreateMessage(DWORD value, char** message);

UINT CreateMessage(DWORD value, char** message)
{
    UINT message_len = snprintf(NULL, 0, "%d", value);
    char* buf = (char*)malloc(message_len + 1);
    snprintf(buf, message_len + 1, "%d", value);

    *message = buf;
    return message_len + 1;
}

#endif