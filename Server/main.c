#include "Network.h"
#include "Server.h"

int __cdecl main(int argc, char** argv)
{
    printf("\nWaiting for connection...\n\n");
    SOCKET ListenSocket = CreateListenSocket();
    SOCKET ClientSocket = CreateClientSocket(ListenSocket);
    printf("\nClient connected!\n\n");

    //get processes list
    char* message;
    int message_len = 0;
    message_len = GetClientMessage(ClientSocket, &message);
    UINT* ProcessList = (DWORD*)malloc(message_len);
    int ProcessCount = message_len / sizeof(DWORD);
    memcpy(ProcessList, message, message_len);
    free(message);

    printf("\nBytes recived: %d\n", message_len);
    printf("\nProcesses number: %d\n", ProcessCount);

    for(int i = 0; i < ProcessCount; i++)
        printf("\t%d\n", ProcessList[i]);
    printf("\n\n");

    //send process ID
    message = (char*)malloc(sizeof(DWORD));
    message_len = sizeof(DWORD);

    /*int ProcessNum;
    printf("\nChose process number [1: %d]: ", ProcessCount);
    scanf_s("%d", &ProcessNum);
    printf("\nClose process (ID): %d\n", ProcessList[ProcessNum]);
    memcpy(message, &ProcessList[ProcessNum], sizeof(DWORD));*/

    //close random process
    memcpy(message, &ProcessList[ProcessCount / 2], sizeof(DWORD));
    printf("\nClose prces (ID): %d\n", ProcessList[ProcessCount / 2]);
    SendClientMessage(ClientSocket, message, message_len);
    free(ProcessList);
    free(message);

    //get result(closed/not closed)
    GetClientMessage(ClientSocket, &message);
    printf("\nResult: %s\n", message);
    free(message);

    CloseConnection(ListenSocket, ClientSocket);

    printf("\nConnection closed!\n");

    getchar();
    return 0;
}