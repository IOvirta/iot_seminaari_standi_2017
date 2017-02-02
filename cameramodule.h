#ifndef _cameramodule
#define _cameramodule

#include <string>

class CameraModule{
	
public:
	//Käsketään aloittamaan videon lähettäminen annettuun osoitteeseen.
	void setTransmitVideoOn(string ip_address);
	
	//Käsketään lopettamaan videon lähettäminen.
	void setTransmitVideoOff();
	
	
};

#endif
