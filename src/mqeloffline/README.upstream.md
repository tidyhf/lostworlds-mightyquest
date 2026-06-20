# The Mighty Quest For Epic Loot - Server

MQEL is/was a free to play game from Ubisoft that was released in 2015 and shut down in 2016. It's a hack-and-slash crossed with dungeon-defense where you build up your own castle with traps and monsters; then invade other players castles in search for epic loot.

On the technical side the game-core is writtten in C++ with the graphical part being Javascript through libCEF. It communicates with the gameserver through JSON-based REST endpoints. As such, this repo is mainly to show how one can implement such 'strange' servers in the C++ system provided by Localnetworking; and ofcourse to revive a game that a lot of people enjoyed.

Although the game is nolonger available through the Steam-store, nor on the official site, you can still download it by visiting `steam://install/239220` in your browser or terminal to download it from Steam. The rest of the dependencies, this repo included, can be downloaded via the [AYRIA](https://ayria.se) client.

### Setup for developers
```
git clone https://github.com/Convery/Platformwrapper_cpp.git
mklink Gamedir/steam_api.dll Platformwrapper/Bin/steam_api[d].dll

git clone https://github.com/AyriaPublic/Bootstrapmodule_cpp.git
mklink Gamedir/Localbootstrap.dll Bootstrapmodule_cpp/Bin/Nativebootstrap32[d].dll

git clone https://github.com/Hedgehogscience/Localnetworking_cpp.git
mklink Gamedir/Plugins/Developerplugin.dll Localnetworking_cpp/Bin/Localnetworking[d].ayria32

git clone https://github.com/Hedgehogscience/MQELOffline_cpp.git
mklink Gamedir/Plugins/Developermodule.dll MQELOffline_cpp/Bin/MQELOffline[d].LN32

cd Gamedir
./MightyQuest.exe -server_url https://Gameserver -environmentName mqel-live -branchName mqel -steamid 76561201696194782 -steamticket "" -token ""
```
