# My first server-client game
You need to run a server, link:
https://github.com/lvlcnapps/my_first_server/blob/master/BR%20TopDown%20Server/x64/Debug/BR%20TopDown%20Server.exe?raw=true

Then run a client, link:
https://github.com/lvlcnapps/my_first_server/blob/master/BR%20TopDown/x64/Release/BR%20TopDown.exe?raw=true

In server window you can see ip and port.
Insert it after running in client app.
Then type your nickname and join game.

Not working:
1) When there is 2 and more players on 1 server, nobody can see other players in window.
2) Server app just for server, it has not graphic module.
3) Pressing 2 buttons in the same time cause bugs. That because of Tcp Networking (maybe i replace it by Udp)
4) More than 5 players in one server crash the server
5) Empty name crash the server
6) Empty map, just alone player in darkness
7) Some other bugs/lags
