## Client\Server for closing process on the remote host 

1. Client App
    1. Gets processes list
    2. Sends processes list to Server
    3. Gets process id from Server and try close this process
    4. Sends message to Server with result(success or failure) to Server
2. Server App
    1. Gets processes list from Client
    2. Chosed one random process and sends id to Client
    3. Gets result(success of failure) from Client

