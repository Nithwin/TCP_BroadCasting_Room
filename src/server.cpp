#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include <cstring>
#include <mutex>
#include <unordered_map>

using namespace std;
unordered_map<int,string> clients;
mutex roster_lock;

void broadcastMessage(const string& message, int excluded_socket = -1)
{
    lock_guard<mutex> lock(roster_lock);
    for (const auto& [client_way, user] : clients)
    {
        if (client_way != excluded_socket)
        {
            send(client_way, message.c_str(), message.length(), 0);
        }
    }
}

void removeClient(int sender_way)
{
    lock_guard<mutex> lock(roster_lock);
    clients.erase(sender_way);
    close(sender_way);
}

void receiveMessage(int sender_way){
    char buffer[1024];

    int byte_received = recv(sender_way, buffer, 1024, 0);
    if(byte_received <= 0){
        removeClient(sender_way);
        cout << "Disconnected...\n";
        return;
    }
    string username(buffer, byte_received);
    {
        lock_guard<mutex> lock(roster_lock);
        clients[sender_way] = username;
    }

    cout << username << " has joined the room\n";
    cout << "Total Count - " << clients.size() << "\n";
    broadcastMessage("[server]: " + username + " connected", sender_way);

    while(true){
        memset(buffer, 0, 1024);
        int byte_received = recv(sender_way, buffer, 1024, 0);
        if(byte_received <= 0){
            removeClient(sender_way);
            cout << "Disconnected...\n";
            return;
        }
        string message(buffer, byte_received);
        string formatted_message = "[" + username + "]: " + message;
        broadcastMessage(formatted_message, sender_way);
        cout << formatted_message << "\n";
        cout << "BroadCasted the message to room\n";
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
        cout << "New connection accepted\n";

        thread client_thread(receiveMessage, caller_way);
        client_thread.detach();
    }
    close(my_way);
    return 0;
}
