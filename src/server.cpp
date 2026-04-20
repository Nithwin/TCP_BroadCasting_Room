#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include <cstring>
#include <mutex>

using namespace std;
vector<int> clients;
mutex roster_lock;

void receiveMessage(int sender_way){
    char buffer[1024];
    while(true){
        memset(buffer, 1024);
        int byte_received = recv(sender_way, buffer, 1024, 0);
        if(byte_received <= 0){
            cout << "Disconnected...\n";
            return;
        }
        roster_lock.lock();
        for(int client_way:clients){
            if(sender_way != client_way){
                send(client_way, buffer, byte_received, 0);
            }
        }
        roster_lock.unlock();
        cout << "\nBroadCasted the message to room\n";
    }
}

int main(){
    int my_way = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);

    bind(my_way, (struct sockaddr*) &address, sizeof(address));

    listen(my_way, 10);
    cout << "\n....Waiting for User...\n";

    while(true){
        int caller_way = accept(my_way, nullptr, nullptr);
        cout << "\nNew user joined the room!\n Total Users: " << clients.size() << "\n";
        roster_lock.lock();
        clients.push_back(caller_way);
        roster_lock.unlock();

        thread client_thread(receiveMessage, caller_way);
        client_thread.detach();
    }
    close(my_way);
    return 0;
}
