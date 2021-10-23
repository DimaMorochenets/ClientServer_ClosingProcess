#ifndef CLIENT_H
#define CLIENT_H

#include "Errors.h"
#include <stdlib.h>
#include <stdio.h>
#include <TlHelp32.h>

int GetProcessList(DWORD* ProcessList, UINT MaxProcNum);
BOOL CloseProcess(DWORD ProcessID);

int GetProcessList(DWORD* ProcessList, UINT MaxProcNum)
{
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(hProcessSnap == INVALID_HANDLE_VALUE){
        printError("hProcessSnap", GetLastError());
        return 0;
    }

    if(!Process32First(hProcessSnap, &pe32)){
        printError("nProcess32First", GetLastError());
        return 0;
    }

    DWORD* curProcess = ProcessList;
    do{
        if((UINT)(curProcess - ProcessList) >= MaxProcNum){
            CloseHandle(hProcessSnap);
            return (UINT)(curProcess - ProcessList);
        }
        *curProcess = pe32.th32ProcessID;
        curProcess++;
    }while(Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
    return (UINT)(curProcess - ProcessList);
}
BOOL CloseProcess(DWORD ProcessID)
{
    HANDLE hHandle;
    hHandle = OpenProcess(PROCESS_TERMINATE, FALSE, ProcessID);

    if(hHandle == NULL){
        printf("Process not found or procces cannot be closed!");
        return FALSE;
    }
    if(!TerminateProcess(hHandle, 0)){
        CloseHandle(hHandle);
        return FALSE;
    }

    CloseHandle(hHandle);
    return TRUE;
}

#endif