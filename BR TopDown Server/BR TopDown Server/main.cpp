#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int search_for_player(vector<string> pl, string name) {
    for (string i : pl) {
        if (i == name) {
            return 1;
        }
    }
    return 0;
}
char buffer[2000];
bool eqv(string comm) {
    int u = 0, n = 0;
    for (char i : buffer) {
        if (i != ' ') {
            n++;
        }
        else {
            break;
        }
    }
    //cout << n << endl;
    for (char i : buffer) {
        if (u == n - 1) {
            return true;
        }
        if (i != comm[u]) {
            //cout << i << ' ' << comm[u] << endl;
            return false;
        }
        if (u < comm.length() - 1) {
            //cout << i << ' ' << comm[u] << endl;
            u++;
        }
        else {
        }
    }
    return true;
}

struct PlayerData {
    float x, y;
    int hp, gun;
};

int main()
{
    int playerCount = 0;
    clock_t flagTime = clock();
    float playersSpeed = 2;
    vector<string> player_names;
    vector<vector<float> > pl_data;
    sf::IpAddress ip = sf::IpAddress::getLocalAddress();
    int port = 2000;
    sf::TcpSocket socket;
    sf::SocketSelector selector;
    selector.add(socket);
    
    char buffer2[2000];
    std::size_t received;
    std::size_t received2;
    std::cout << ip << ' ' << port << ' ' << endl;
    sf::TcpListener listener;
    listener.setBlocking(true);
    listener.listen(port);
    selector.add(listener);
    listener.accept(socket);
    vector<sf::TcpSocket*> clients;
    while (true) {
        clock_t nowTime = clock();
        if (nowTime - flagTime > 10) {
            int fps = 1000 / (nowTime - flagTime);
            flagTime = nowTime;
            if (selector.wait()) {
                if (selector.isReady(listener)) {
                    sf::TcpSocket* client = new sf::TcpSocket;
                    if (listener.accept(*client) == sf::Socket::Done) {
                        clients.push_back(client);
                        selector.add(*client);
                    }
                    else {
                        delete client;
                    }
                }
                else {
                    int currPl = 0;
                    for (std::vector<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it) {
                        sf::TcpSocket& client = **it;
                        client.setBlocking(false);
                        if (selector.isReady(client)) {
                            sf::Packet packet;
                            if (client.receive(packet) != sf::Socket::Done) {}
                            else {
                                packet >> buffer;
                                string comm = "Connected_to:_Client ";
                                if (eqv(comm)) {
                                    std::cout << "connected\n";
                                    packet >> buffer2;
                                    if (search_for_player(player_names, buffer2) == 0) {
                                        float pl_x = 1.0 * rand() / 55, pl_y = 1.0 * rand() / 55;
                                        vector<float> lol;
                                        lol.push_back(pl_x); lol.push_back(pl_y);
                                        lol.push_back(100); lol.push_back(0);
                                        pl_data.push_back(lol);

                                        player_names.push_back(buffer2);
                                        sf::Packet snd;
                                        string tt = "conn ";
                                        playerCount++;

                                        snd << tt << currPl << pl_x << pl_y;
                                        client.send(snd);
                                    }
                                    else {
                                        sf::Packet snd;
                                        string tt = "dis ";
                                        int a = 0;
                                        snd << tt << a;
                                        client.send(snd);
                                    }
                                }
                                if (eqv("move_ls ")) {
                                    sf::Packet snd;
                                    string command = "new_pos ";

                                    snd << command << currPl << pl_data[currPl][0] - playersSpeed << pl_data[currPl][1];
                                    pl_data[currPl][0] -= playersSpeed;
                                    client.send(snd);
                                }
                                if (eqv("move_rs ")) {
                                    sf::Packet snd;
                                    string command = "new_pos ";
                                    snd << command << currPl << pl_data[currPl][0] + playersSpeed << pl_data[currPl][1];
                                    pl_data[currPl][0] += playersSpeed;
                                    client.send(snd);
                                }
                                if (eqv("move_us ")) {
                                    sf::Packet snd;
                                    string command = "new_pos ";
                                    snd << command << currPl << pl_data[currPl][0] << pl_data[currPl][1] - playersSpeed;
                                    pl_data[currPl][1] -= playersSpeed;
                                    client.send(snd);
                                }
                                if (eqv("move_ds ")) {
                                    sf::Packet snd;
                                    string command = "new_pos ";
                                    snd << command << currPl << pl_data[currPl][0] << pl_data[currPl][1] + playersSpeed;
                                    pl_data[currPl][1] += playersSpeed;
                                    client.send(snd);
                                }
                                if (eqv("struct_test ")) {
                                    for (int play_ers = 0; play_ers < playerCount; play_ers++) {
                                        cout << play_ers << ' ' << pl_data[play_ers][0] << ' ' << pl_data[play_ers][1] << endl;
                                    }
                                }
                            }
                        }
                        currPl++;
                    }
                }
            }
        }
    }
    return 0;
}