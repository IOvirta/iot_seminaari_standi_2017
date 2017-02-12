# IOT-standi Serverisovellus
##requirements

OpenCv
```
http://docs.opencv.org/2.4/doc/tutorials/introduction/linux_install/linux_install.html
```

OpenSSL
```
sudo apt-get install libssl-dev
```

ASIO
```
http://think-async.com/Asio/Download
-TAI-
sudo apt-get install libasio-dev
```

##build
```
cd bin
cmake ..
make
```

iotservice.cpp - pääkoodi

tcpnetworking.cpp/h - tcp liikenne, asetusten haku ja tietojen vienti
android-sovellukseen

videohandler.cpp/h - ohjaa kameraa, liikkeentunnistusta ja striimaamista(?)

Koodissa kommentoinnit pääsääntöisesti header tiedostoissa.
