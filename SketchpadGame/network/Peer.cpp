#include "peer.h"

#include <stdexcept>
#include <random>


/********************************************************************\
*
*                               Peer::Peer
*
\********************************************************************/

Peer::Peer(std::string id, int port) {
    this->id = id;

    if ((port > 65535) || (port < 1024)) {
        this->port = (uint16_t)DEFAULT_PORT;
    }
    else {
        this->port = (uint16_t)port;
    }
    
    std::thread t(&Peer::run_server, this);
    t.detach();    
}


/********************************************************************\
*
*                               Peer::get_num_connections
*
\********************************************************************/

int Peer::get_num_connections() {
    this->connections_lock.lock();
    int num = this->connections.size();
    this->connections_lock.unlock();

    return num;
}

/********************************************************************\
*
*                               Peer::get_num_messages
*
\********************************************************************/

int Peer::get_num_messages() {
    this->message_queue_lock.lock();
    int num = this->message_queue.size();
    this->message_queue_lock.unlock();

    return num;
}


/********************************************************************\
*
*                               Peer::get_message
*
\********************************************************************/
Message Peer::get_message() {
    Message m = { "NULL", "NULL" };
    this->message_queue_lock.lock();
    if (this->message_queue.size() > 0) {
        m = this->message_queue.front();
        this->message_queue.pop_front();
    }
    this->message_queue_lock.unlock();

    return m;
}


/********************************************************************\
*
*                               Peer::send_message
*
\********************************************************************/

void Peer::send_message_randomly(std::string message) {
    if (this->connections.size() == 0) return;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, this->connections.size() - 1);
    int randomIndex = dis(gen);
    auto iter = std::next(this->connections.begin(), randomIndex);
    PeerInfo info = this->connections[iter->first];

    if (send(info.sock, message.c_str(), message.size(), 0) == -1)
        perror("send");
}

/********************************************************************\
*
*                               Peer::send_message
*
\********************************************************************/

void Peer::send_message(std::string peer_id, std::string message) {
    PeerInfo info = this->connections[peer_id];    
    
    if (send(info.sock, message.c_str(), message.size(), 0) == -1)
        perror("send");
}

/********************************************************************\
*
*                               Peer::broadcast
*
\********************************************************************/

void Peer::broadcast(std::string message) {
    for (std::map<std::string, PeerInfo>::iterator itr = this->connections.begin();
        itr != this->connections.end();
        itr++)
    {
        std::string peer_id = itr->first;
        this->send_message(peer_id, message);
    }
}


/********************************************************************\
*
*                               Peer::create_connection
*
\********************************************************************/
bool Peer::create_connection(std::string peer_id, PeerInfo info) {
    PeerInfo new_info;
    new_info.ip = info.ip;
    new_info.port = info.port;
    
    //===============================
    // Create socket
    //===============================
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
        std::cout << "[ERROR] Can't create socket, Err #" << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    //===============================
    // Fill in a hint structure
    //===============================	
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(new_info.port);
    inet_pton(AF_INET, new_info.ip, &hint.sin_addr);

    //===============================
    // Connect to server
    //===============================	
    int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connResult == SOCKET_ERROR)
    {
        std::cout << "[ERROR] Can't connect to server, Err #" << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    // if it gets here, the connection is established
    printf("[OK] connection established \n");


    // update the sockfd field
    new_info.sock = sock;
    // add the new connection to the connection list
    this->connections_lock.lock();
    this->connections.insert(std::pair<std::string, PeerInfo>(peer_id, new_info));
    this->connections_lock.unlock();

    // create a listener thread
    std::thread receive_thread(&Peer::receive_loop, this, sock, peer_id);
    receive_thread.detach();

    return 1;
}

/********************************************************************\
*
*                               Peer::run_server
*
\********************************************************************/

void Peer::run_server() {
    //-------------------------------------
    // Initialize Winsock
    WSADATA wsaData;
    int iResult;
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("[ERROR] WSAStartup failed with error: %d\n", iResult);
        return;
    }
    printf("[OK] Socket Startup\n");

    //-------------------------------------
    // Autofill the server information    
    struct addrinfo* result = NULL;
    struct addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;
    
    iResult = getaddrinfo(NULL, std::to_string(this->port).c_str(), &hints, &result);
    if (iResult != 0) {
        printf("[ERROR] getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return;
    }
    printf("[OK] fill the server information\n");

    //-------------------------------------
    // Create a SOCKET for connecting to server
    int listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listenSocket == INVALID_SOCKET) {
        printf("[ERROR] socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return;
    }


    //-------------------------------------
    iResult = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("[ERROR] bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(listenSocket);
        WSACleanup();
        return;
    }
    freeaddrinfo(result);
    printf("[Server] start listening \n");

    //-------------------------------------
    // start listening
    iResult = listen(listenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("[ERROR] listen failed with error: %d\n", WSAGetLastError());
        closesocket(listenSocket);
        WSACleanup();
        return;
    }

    //-------------------------------------
    // infinite accept loop
    int connect_number = 1;
    while (true) {
        sockaddr_storage client_addr;
        socklen_t sin_size = sizeof(client_addr);

        // accept the new connection
        printf("[OK] waiting for client\n");
        int newfd = accept(listenSocket, (sockaddr*)&client_addr, &sin_size);
        if (newfd == -1) {
            perror("[ERROR] accept");
            continue;
        }

        // get the IP address and port from the connection
        sockaddr_in* address = (sockaddr_in*)((sockaddr*)&client_addr);
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &address->sin_addr, client_ip, sizeof(client_ip));
        uint16_t client_port = htons(address->sin_port);
        PeerInfo info = { client_ip, client_port, newfd };
        printf("[OK] Client info: \n");
        printf("Client ip: %s\n", client_ip);
        printf("Client port: %u\n", client_port);
        printf("Client socket: %d\n", newfd);

        std::string peer_id = std::to_string(connect_number);
        connect_number++;

        // add the new connection to the connection list
        this->connections_lock.lock();
        this->connections.insert(std::pair<std::string, PeerInfo>(peer_id, info));
        this->connections_lock.unlock();
        std::thread receive_thread(&Peer::receive_loop, this, newfd, peer_id);
        receive_thread.detach();
    }


}


/********************************************************************\
*
*                               Peer::receive_loop
*
\********************************************************************/

void Peer::receive_loop(int sockfd, std::string peer_id) {
    try {
        // temporarily limit message size to 100 bytes
        char buf[100];
        int count = 0;
        while (true) {
            int numbytes;
            if ((numbytes = recv(sockfd, buf, 99, 0)) <= 0) {
                break;
            }
            buf[numbytes] = '\0';
            std::string s(buf);

            Message m = { peer_id, s };

            this->message_queue_lock.lock();
            this->message_queue.push_back(m);
            this->message_queue_lock.unlock();
        }

        // connection has failed
        // close the socket and terminate the thread
        closesocket(sockfd);
    }
    catch (...) {
        std::cout << "[ERROR]" << std::endl;
    }
}