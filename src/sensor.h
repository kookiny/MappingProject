#pragma once 

#include "ofMain.h"

class sensor : public ofThread {

		void threadedFunction() {
			serial.listDevices();
			vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
			serial.setup(1, 38400);
		// start

		while (isThreadRunning()) {
			if (serial.available() > 5) {
				lock();
				serial.readBytes(bytes, 5);
				unlock();
				//ofPushStyle();
				//ofSetColor(255);
				//ofDrawBitmapString("serial = " + ofToString(bytes), 900, 50);
				//ofPopStyle();
			}
		}

		// done
	}
public:
		ofSerial serial;
		unsigned char bytes[2];
};