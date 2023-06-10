#pragma once
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <thread>
#include <deque>
#include <mutex>
#include <string>
#include <iostream>
#include <vector>
#include <map>

#define DEFAULT_PORT 12829

struct PeerInfo {
    const char* ip;
    uint16_t port;
    int sock;
};

//body protocol
// F:5:5:0.9:0.7:0.2   --> fire for voxel position x=5, y=5
// C:5:5:0.5:0.4:0.6   --> notify the colour of the voxel hitted,

struct Message {
    std::string sender_name;
    std::string body;
};

class Peer {
public:
    // constructor
    Peer(std::string id, int port);

    int get_num_connections();
    int get_num_messages();
    Message get_message();    

    bool create_connection(std::string peer_id, PeerInfo info);

    void send_message(std::string peer_id, std::string message);
    void send_message_randomly(std::string message);
    void broadcast(std::string message);
private:
    std::string id;
    int port;

    std::map<std::string, PeerInfo> connections;
    std::mutex connections_lock;
    
    std::deque<Message> message_queue;
    std::mutex message_queue_lock;

    void run_server();
    void receive_loop(int sockfd, std::string peer_id);
};
