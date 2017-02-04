# IOT-standi Serverisovellus
----------------------------------------------------------
# build
cd bin
cmake ..
make
----------------------------------------------------------
./iovirta_iot
----------------------------------------------------------
iotservice.cpp - pääkoodi

tcpnetworking.cpp/h - tcp liikenne, asetusten haku ja tietojen vienti
android-sovellukseen

videohandler.cpp/h - ohjaa kameraa, liikkeentunnistusta ja striimaamista(?)

Koodissa kommentoinnit pääsääntöisesti header tiedostoissa.
