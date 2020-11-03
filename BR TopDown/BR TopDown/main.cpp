#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <iostream>

#define pi 3.1415

using namespace std;

int rotation(int mouse_x, int mouse_y, float pl_x, float pl_y) {
    //cout << atan2((mouse_y - pl_y), (mouse_x - pl_x)) / pi * 180;
    return atan2((mouse_y - pl_y), (mouse_x - pl_x)) / pi * 180;
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

int main()
{
    float pl_x = 0, pl_y = 0;       //player data
    int hp = 100, gun = 0, id;      //player data
    clock_t flagTime = clock();
    //textures
    sf::Texture playerT;
    if (!playerT.loadFromFile("player.png")) {}
    playerT.setSmooth(true);

    //sprites
    sf::Sprite playerSp;
    playerSp.setTexture(playerT);
    playerSp.setOrigin(sf::Vector2f(13.f, 13.f));
    vector<sf::Sprite> allPlayers;
    vector<vector<float> > pl_data;
    allPlayers.push_back(playerSp);

    sf::IpAddress ip;
    int port, playerCount;
    string player_name;
    cout << "Insert ip and port:\n";
    cin >> ip >> port;
    cout << "Type your player name:\n";
    cin >> player_name;
    //sf::IpAddress ip = "192.168.1.239";
    sf::IpAddress my_ip = sf::IpAddress::getLocalAddress();
    sf::TcpSocket socket;
    socket.setBlocking(false);
    char connectionType;
    char buffer2[2000];
    std::size_t received;
    std::string text = "Connected_to:_Client ";

    socket.connect(ip, port);
        sf::Packet pack;
        pack << text << player_name;
        socket.send(pack);

        sf::RenderWindow window(sf::VideoMode(600, 600), "Battle Royale 0.1");
        while (window.isOpen())
        {
            clock_t nowTime = clock();
            if (nowTime - flagTime > 10) {
                int fps = 1000 / (nowTime - flagTime);
                flagTime = nowTime;

                sf::Event event;
                while (window.pollEvent(event))
                {
                    if (event.type == sf::Event::Closed)
                        window.close();
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
                    string mes = "struct_test ";
                    sf::Packet packet;
                    packet << mes;
                    socket.send(packet);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                    string moveMes = "move_ls ";
                    sf::Packet packet;
                    packet << moveMes;
                    socket.send(packet);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                    string moveMes = "move_rs ";
                    sf::Packet packet;
                    packet << moveMes;
                    socket.send(packet);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                    string moveMes = "move_us ";
                    sf::Packet packet;
                    packet << moveMes;
                    socket.send(packet);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                    string moveMes = "move_ds ";
                    sf::Packet packet;
                    packet << moveMes;
                    socket.send(packet);
                }
                window.clear();


                sf::Packet rec;
                socket.receive(rec); // receiving data and applying it
                rec >> buffer;
                if (eqv("dis ")) {
                    int reason;
                    rec >> reason;
                    socket.disconnect();
                    cout << "Disconnected from server. Why:\n";
                    if (reason == 0) {
                        cout << "Player with nickname " + player_name + " already in game";
                        return 0;
                    }
                    if (reason == 1) {
                        cout << "You were banned from server";
                        return 0;
                    }
                    return 0;
                }
                if (eqv("conn ")) {
                    rec >> id >> pl_x >> pl_y;
                    playerCount = id;
                    playerSp.setPosition(pl_x, pl_y);
                }
                if (eqv("new_pos ")) {
                    int who;
                    rec >> who >> pl_x >> pl_y;
                    playerSp.setPosition(pl_x, pl_y);
                }
                sf::Vector2i localPos = sf::Mouse::getPosition(window);
                playerSp.setRotation(rotation(localPos.x, localPos.y, pl_x, pl_y));
                window.draw(playerSp);
                window.display();
            }
        }
    return 0;
}