#pragma once

#include "ofMain.h"
#include "TriangleManager.hpp"
#include "ofxGui.h"
#include "ofxJSON.h"
//#include "sensor.h"
#include "ofxOsc.h"

#define PORT 7007
#define NUM_MSG_STRINGS 20

//#include "ofxSyphon.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    
    vector<vector<vector<ofVec2f> > > polyPos;
    vector<vector<ofVec2f> > centros;
    
    
    //GUI
    ofImage             foto;
    void                setupGUI();
    ofxPanel            gui;
    bool                mHideGUI;
    ofParameter<float>	frequency;
    ofParameter<float>	timeFrequency;
    ofParameter<float>	magnitude;
	ofParameter<float>	timeFrequencyMax;
	ofParameter<float>	magnitudeMax;
	ofParameter<int>	ShaderMin;
	ofParameter<float> escalar;
	ofParameter<float> mover;
    ofxToggle           enableNewTriangle;
    ofxToggle           enableFoto;
    ofxToggle			enableNewPuntos;
    ofxToggle			enableNewPoly;
    ofxToggle			enablePolygons;
    ofxToggle			enableMask;
    ofxToggle			editTriangle;
    ofxToggle			editPunto;

	bool blancosUp;
	bool blancosDown;
	void updateBlancos();
	int blancosAlpha;
	int storedTime;
    
    bool dragged;
    void restore();
    
    
    TriangleManager triMan;
    ofVec2f tempA;
    ofVec2f tempB;
    ofVec2f tempC;
    int		tempTriangle;
    int		tempPunto;
    
    vector<int> newPolygon;
    
    ofImage triangulo;
    
    //JSON
    ofxJSONElement mJSON;
    void saveJSON();
    void loadJSON();
    
    //Shader
    ofShader shader;
    ofPlanePrimitive plane;
    ofImage img;
    void setupShader();
    void updateShader();
    void drawShader();
    ofVec2f shaderMin;
    ofVec2f shaderMax;
    
    
    //LAVA
    ofImage sqImg;
    ofTexture circleTexture;
    
    vector <ofPoint> NormCirclePts;
    vector <ofPoint> NormCircleCoords;
    
    //-----------SYPHON--------
   // ofxSyphonServer         ser;
    
    //MASK
    void drawMask();
	/*
    //sensor
    sensor threadedSensor;
    float serialData;
    float serialDataTemp = 22;
	*/
	//OSC
	ofxOscSender sender;
	ofxOscReceiver	receiver;

	int up, down, left, right;
};
