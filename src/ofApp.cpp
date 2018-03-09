#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(0);
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofEnableAlphaBlending();
    ofEnableAntiAliasing();
    
    tempA.set(-1, -1);
    tempB.set(-1, -1);
    tempC.set(-1, -1);
    tempPunto = -1;
    tempTriangle = -1;
    triMan.setup();
    loadJSON();
    
    foto.loadImage("lava.jpg");
    foto.resize(ofGetWidth(), ofGetHeight());
    setupGUI();
    
    setupShader();
    triangulo.load("t3.png");
    
    //LAVA
    sqImg.loadImage("lava.jpg");
    sqImg.resize(ofGetWidth(), ofGetHeight());
    circleTexture.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);
    circleTexture.loadData(sqImg.getPixels(), ofGetWidth(), ofGetHeight(), GL_RGB);
    
    //--------------SYPHON------------------
   // ser.setName("TECTONICA");
    
    //SENSOR
   // threadedSensor.startThread(true);

	//Blanco
	blancosDown = false;
	blancosUp = false;
	blancosAlpha = 0;
	storedTime = 0;

	//osc
	sender.setup("", 2345);

	cout << "listening for osc messages on port " << PORT << "\n";
	receiver.setup(PORT);


	up = down = right = left = 0;
}

//--------------------------------------------------------------
void ofApp::setupGUI(){
    gui.setup("GUI");
    gui.add(enableFoto.setup("Foto", false));
    gui.add(enableNewTriangle.setup("New Triangle", false));
    gui.add(enableNewPuntos.setup("New Punto", false));
    gui.add(enableNewPoly.setup("New Polygon", false));
    gui.add(enablePolygons.setup("Polygons", true));
    gui.add(enableMask.setup("Mask", true));
    gui.add(editTriangle.setup("Edit Triangle", false));
    gui.add(editPunto.setup("Edit Punto", false));
    gui.add( frequency.set("Frequency",  0.15f,  0.01f,  2.0f) );
    gui.add( timeFrequency.set("Time Frequency",  0.3f,  0.001f,  3.0f) );
    gui.add( magnitude.set("Magnitude",  0.35f,  0.0f,  5.0f) );
	gui.add(timeFrequencyMax.set("Time Frequency Max", 0.0f, 0.001f, 3.0f));
	gui.add(magnitudeMax.set("Magnitude Max", 0.0f, 0.0f, 5.0f));
	gui.add(ShaderMin.set("shader", 123, 0, 255));
	gui.add(escalar.set("escalar", 1.0, 0.0, 1.0));
	gui.add(mover.set("moiver", 0, 0, 200));
    mHideGUI = true;
}

//--------------------------------------------------------------
void ofApp::setupShader(){
    shaderMin = triMan.triangulos[0].getMin();
    shaderMax = triMan.triangulos[0].getMax();
    for (int i = 1; i < triMan.getNumTriangles(); i++) {
        ofVec2f tempMin = triMan.triangulos[i].getMin();
        ofVec2f tempMax = triMan.triangulos[i].getMax();
        shaderMin.set(min(shaderMin.x, tempMin.x), min(shaderMin.y, tempMin.y));
        shaderMax.set(max(shaderMax.x, tempMax.x), max(shaderMax.y, tempMax.y));
    }
    //Shader
#ifdef TARGET_OPENGLES
    shader.load("shadersES2/shader");
#else
    if(ofIsGLProgrammableRenderer()){
        shader.load("shadersGL3/shader");
    }else{
        shader.load("shadersGL2/shader");
    }
#endif
    
    //img.allocate(80, 60, OF_IMAGE_COLOR);
	img.allocate(80, 60, OF_IMAGE_GRAYSCALE);
    plane.set(shaderMax.x, 1200, 80, 60);
    plane.mapTexCoordsFromTexture(img.getTextureReference());
}

//--------------------------------------------------------------
void ofApp::update(){
	//for OSC Reciever
	while (receiver.hasWaitingMessages())
	{
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);
		cout << m.getAddress() << endl;
		float amp, sig;
		// check for mouse moved message
		if (m.getAddress() == "/amp")
		{
			// both the arguments are int32's
			amp = m.getArgAsFloat(0);
			cout << "AMP: " << amp << endl;
			float mapAmp = ofMap(amp, 0, 0.1, 0, 1);
			magnitude.set(mapAmp);

		}
		else if (m.getAddress() == "/sig")
		{
			// both the arguments are int32's
			sig = m.getArgAsFloat(0);
			cout << "SIG: " << sig << endl;
			float mapFreq = ofMap(sig, 0, 0.1, 0, 1);
			frequency.set(mapFreq);

		}
		// check for mouse button message
	
		else
		{
			// unrecognized message: display on the bottom of the screen
			string msg_string;
			msg_string = m.getAddress();
			msg_string += ": ";
			for (int i = 0; i<m.getNumArgs(); i++)
			{
				// get the argument type
				msg_string += m.getArgTypeName(i);
				msg_string += ":";
				// display the argument - make sure we get the right type
				if (m.getArgType(i) == OFXOSC_TYPE_INT32)
					msg_string += ofToString(m.getArgAsInt32(i));
				else if (m.getArgType(i) == OFXOSC_TYPE_FLOAT)
					msg_string += ofToString(m.getArgAsFloat(i));
				else if (m.getArgType(i) == OFXOSC_TYPE_STRING)
					msg_string += m.getArgAsString(i);
				else
					msg_string += "unknown";
			}
		}

	}


    triMan.update(frequency, timeFrequency, magnitude);
//    for (int i = 0; i < triMan.getNumTriangles(); i++) {
//        vector<vector<ofVec2f> > tempTri;
//        vector<ofVec2f> centerTemp;
//        for (int j = 0; j < triMan.triangulos[i].polygons.size(); j++) {
//            vector<ofVec2f> tempPoly;
//            for (int k = 0; k < triMan.triangulos[i].polygons[j].size(); k++) {
//                tempPoly.push_back(triMan.triangulos[i].puntos[triMan.triangulos[i].polygons[j][k]][2]);
//            }
//            tempTri.push_back(tempPoly);
//            tempPoly.clear();
//            centerTemp.push_back(triMan.triangulos[i].getCenter(j));
//        }
//        polyPos.push_back(tempTri);
//        centros.push_back(centerTemp);
//    }
    updateShader();
   // threadedSensor.lock();
    //serialDataTemp = std::atoi(reinterpret_cast<char(&)[sizeof(threadedSensor.bytes)]>(threadedSensor.bytes));
   // threadedSensor.unlock();
	//magnitude = ofMap(serialDataTemp, 0, 65000, 0.0f, magnitudeMax);
	//frequency = ofMap(serialDataTemp, 0, 20000, 0.01f, 1.0f);
	//timeFrequency = ofMap(serialDataTemp, 0, 20000, 0.001f, timeFrequencyMax);
   // ofLogNotice("ofApp::setup") << "serial temp" + ofToString(serialDataTemp);
    restore();
	updateBlancos();
	ofxOscMessage msg;
	msg.setAddress("oscVol");
	msg.addInt32Arg(blancosAlpha);
	sender.sendMessage(msg);
}

//---------------------------------------
void ofApp::updateBlancos() {
	if (blancosAlpha < 255 && blancosUp){
		blancosAlpha++;
		ofxOscMessage msg;
		msg.setAddress("oscVol");
		msg.addInt32Arg(blancosAlpha);
		sender.sendMessage(msg);
		if (blancosAlpha >= 255)
		{
			blancosUp = false;
			storedTime = ofGetElapsedTimeMillis();
		}
	}
	else if (blancosAlpha > 0 && blancosDown){
		blancosAlpha--;
		ofxOscMessage msg;
		msg.setAddress("oscVol");
		msg.addInt32Arg(blancosAlpha);
		sender.sendMessage(msg);
		if (blancosAlpha == 0)
		{
			blancosDown = false;
			storedTime = ofGetElapsedTimeMillis();
		}
	}
	if (blancosAlpha == 0 && !blancosUp && !blancosDown && ofGetElapsedTimeMillis() - storedTime > 180000){
		blancosUp = true;
	}else if (blancosAlpha >= 255 && !blancosUp && !blancosDown && ofGetElapsedTimeMillis() - storedTime > 60000) {
		blancosDown = true;
	}
}

//---------------------------------------
void ofApp::restore(){
    //centro
    triMan.triangulos[7].vertices[2]=triMan.triangulos[6].vertices[1];
    triMan.triangulos[7].updatePos();
    triMan.triangulos[3].vertices[0]=triMan.triangulos[2].vertices[2];
    triMan.triangulos[3].updatePos();
    triMan.triangulos[1].vertices[2]=triMan.triangulos[0].vertices[0];
    triMan.triangulos[1].updatePos();
    triMan.triangulos[2].vertices[0]=triMan.triangulos[0].vertices[0];
    triMan.triangulos[2].updatePos();
    triMan.triangulos[7].vertices[1]=triMan.triangulos[0].vertices[0];
    triMan.triangulos[7].updatePos();
    triMan.triangulos[6].vertices[2]=triMan.triangulos[0].vertices[1];
    triMan.triangulos[6].updatePos();
    triMan.triangulos[3].vertices[2]=triMan.triangulos[1].vertices[1];
    triMan.triangulos[3].updatePos();
    triMan.triangulos[1].vertices[0]=triMan.triangulos[0].vertices[2];
    triMan.triangulos[1].updatePos();
    //BL
    triMan.triangulos[5].vertices[0]=triMan.triangulos[23].vertices[1];
    triMan.triangulos[5].updatePos();
    triMan.triangulos[5].vertices[1]=triMan.triangulos[23].vertices[0];
    triMan.triangulos[5].updatePos();
    triMan.triangulos[4].vertices[1]=triMan.triangulos[23].vertices[0];
    triMan.triangulos[4].updatePos();
    triMan.triangulos[4].vertices[2]=triMan.triangulos[23].vertices[2];
    triMan.triangulos[4].updatePos();
    //BR
    triMan.triangulos[9].vertices[1]=triMan.triangulos[10].vertices[1];
    triMan.triangulos[9].updatePos();
    triMan.triangulos[9].vertices[0]=triMan.triangulos[10].vertices[0];
    triMan.triangulos[9].updatePos();
    triMan.triangulos[8].vertices[1]=triMan.triangulos[10].vertices[0];
    triMan.triangulos[8].updatePos();
    triMan.triangulos[8].vertices[2]=triMan.triangulos[10].vertices[2];
    triMan.triangulos[8].updatePos();
    //UL
    triMan.triangulos[20].vertices[2]=triMan.triangulos[26].vertices[2];
    triMan.triangulos[20].updatePos();
    triMan.triangulos[21].vertices[0]=triMan.triangulos[26].vertices[2];
    triMan.triangulos[21].updatePos();
    triMan.triangulos[22].vertices[0]=triMan.triangulos[26].vertices[2];
    triMan.triangulos[22].updatePos();
    triMan.triangulos[25].vertices[0]=triMan.triangulos[26].vertices[2];
    triMan.triangulos[25].updatePos();
    triMan.triangulos[21].vertices[2]=triMan.triangulos[20].vertices[0];
    triMan.triangulos[21].updatePos();
    triMan.triangulos[25].vertices[1]=triMan.triangulos[26].vertices[0];
    triMan.triangulos[25].updatePos();
    //UR
    triMan.triangulos[14].vertices[1]=triMan.triangulos[27].vertices[2];
    triMan.triangulos[14].updatePos();
    triMan.triangulos[15].vertices[0]=triMan.triangulos[27].vertices[2];
    triMan.triangulos[15].updatePos();
    triMan.triangulos[16].vertices[0]=triMan.triangulos[27].vertices[2];
    triMan.triangulos[16].updatePos();
    triMan.triangulos[24].vertices[1]=triMan.triangulos[27].vertices[2];
    triMan.triangulos[24].updatePos();
    triMan.triangulos[14].vertices[2]=triMan.triangulos[27].vertices[0];
    triMan.triangulos[14].updatePos();
    triMan.triangulos[24].vertices[0]=triMan.triangulos[27].vertices[1];
    triMan.triangulos[24].updatePos();
    //CL
    triMan.triangulos[19].vertices[0]=triMan.triangulos[18].vertices[2];
    triMan.triangulos[19].updatePos();
    triMan.triangulos[17].vertices[0]=triMan.triangulos[18].vertices[2];
    triMan.triangulos[17].updatePos();
    triMan.triangulos[19].vertices[2]=triMan.triangulos[18].vertices[1];
    triMan.triangulos[17].updatePos();
    
    //CR
    triMan.triangulos[11].vertices[1]=triMan.triangulos[12].vertices[0];
    triMan.triangulos[11].updatePos();
    triMan.triangulos[13].vertices[0]=triMan.triangulos[12].vertices[0];
    triMan.triangulos[13].updatePos();
    triMan.triangulos[13].vertices[2]=triMan.triangulos[12].vertices[1];
    triMan.triangulos[13].updatePos();
    triMan.triangulos[11].vertices[2]=triMan.triangulos[12].vertices[2];
    triMan.triangulos[11].updatePos();
    
}

//--------------------------------------------------------------
void ofApp::updateShader(){
    float noiseScale = 1.0;//ofMap(mouseX, 0, ofGetWidth(), 0, 0.1);
    float noiseVel = ofGetElapsedTimef();
    
    unsigned char * pixels = img.getPixels();
    //int w = img.getWidth() * 3;
	int w = img.getWidth() ;
    int h = img.getHeight();
    /*for(int y=0; y<h; y++) {
        for(int x=0; x<w; x++) {
            int i = y * w + x;
            float noiseVelue = ofNoise(x * noiseScale, y * noiseScale, noiseVel);
            if (i%3 == 0) {
                pixels[i] = 180 + 51 * noiseVelue;
            }else if (i%3 == 1){
                pixels[i] = 220 * (1 - noiseVelue);
            }else if (i%3 == 2)
                pixels[i] = 0;
        }
    }*/
	for (int y = 0; y<h; y++) {
		for (int x = 0; x<w; x++) {
			int i = y * w + x;
			float noiseVelue = ofNoise(x * noiseScale, y * noiseScale, noiseVel);
			pixels[i] = ofMap(255 * noiseVelue, 0, 255, ShaderMin, 255);
		}
	}
    img.update();
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofPushMatrix();
	ofScale(0.9, 1);
	ofTranslate(103 + right - left, -6 - up + down);
    if (!editTriangle) {
        drawShader();
        ofPopStyle();
    }
    
    
    
    //EDIT
    if (enableFoto) {
        circleTexture.draw(0, 0, ofGetWidth(), ofGetHeight());
        //foto.draw(0, 0, ofGetWidth(), ofGetHeight());
        
    }
    
    if (enableNewTriangle) {
        triMan.drawEdit();
        ofPushStyle();
        ofSetColor(255, 255, 0);
        ofDrawCircle(ofGetMouseX(), ofGetMouseY(), 3);
        ofSetLineWidth(3);
        if (tempA != ofVec2f(-1, -1) && tempB == ofVec2f(-1, -1)){
            ofDrawLine(tempA.x, tempA.y, ofGetMouseX(), ofGetMouseY());
        }else if (tempA != ofVec2f(-1, -1) && tempB != ofVec2f(-1, -1) && tempC == ofVec2f(-1, -1)){
            ofDrawLine(tempA.x, tempA.y, ofGetMouseX(), ofGetMouseY());
            ofDrawLine(tempB.x, tempB.y, ofGetMouseX(), ofGetMouseY());
            ofDrawLine(tempB.x, tempB.y, tempA.x, tempA.y);
        }
        ofPopStyle();
    }
    
    if (!enableNewTriangle && enableNewPuntos) {
        triangulo.draw(triMan.triangulos[3].getMin(), triMan.triangulos[3].getMax().x - triMan.triangulos[3].getMin().x, triMan.triangulos[3].getMax().y - triMan.triangulos[3].getMin().y);
        triMan.drawEdit();
        triMan.drawEditPuntos();
        ofPushStyle();
        ofSetColor(255, 0, 0);
        ofDrawCircle(ofGetMouseX(), ofGetMouseY(), 2);
        if (tempA != ofVec2f(-1, -1) && tempB == ofVec2f(-1, -1)) {
            ofNoFill();
            ofDrawCircle(tempA, ofDist(tempA.x, tempA.y, ofGetMouseX(), ofGetMouseY()));
        }
        ofPopStyle();
    }
    
    if (!enableNewTriangle && !enableNewPuntos && enableNewPoly) {
        triangulo.draw(triMan.triangulos[3].getMin(), triMan.triangulos[3].getMax().x - triMan.triangulos[3].getMin().x, triMan.triangulos[3].getMax().y - triMan.triangulos[3].getMin().y);
        triMan.drawEdit();
        triMan.drawEditPuntos();
        ofPushStyle();
        ofSetColor(0, 0, 255);
        if (newPolygon.size() && tempTriangle != -1) {
            for (int j = 0; j < newPolygon.size() -1; j++) {
                if (j == 0) {
                    ofDrawLine(triMan.triangulos[tempTriangle].esquinas[newPolygon[j]], triMan.triangulos[tempTriangle].esquinas[newPolygon[newPolygon.size()-1]]);
                }
                ofDrawLine(triMan.triangulos[tempTriangle].esquinas[newPolygon[j]], triMan.triangulos[tempTriangle].esquinas[newPolygon[j+1]]);
            }
        }
        ofPopStyle();
    }
    
    if (editTriangle) {
        ofPushStyle();
        for (int i = 0; i < triMan.getNumTriangles(); i++) {
            ofSetColor(0, 100, 100);
            ofDrawTriangle(triMan.triangulos[i].posA, triMan.triangulos[i].posB, triMan.triangulos[i].posC);
            ofSetColor(255, 255, 0);
            ofDrawBitmapString(ofToString(i), triMan.triangulos[i].center.x, triMan.triangulos[i].center.y);
        }
        if (tempTriangle != -1) {
            ofSetColor(255, 255, 255);
            ofDrawTriangle(triMan.triangulos[tempTriangle].posA, triMan.triangulos[tempTriangle].posB, triMan.triangulos[tempTriangle].posC);
            if (tempA != ofVec2f(-1, -1)) {
                ofSetColor(0, 255, 255);
                ofDrawCircle(triMan.triangulos[tempTriangle].vertices[tempA.y].x, triMan.triangulos[tempTriangle].vertices[tempA.y].y, 1);
            }
        }
        
        ofPopStyle();
    }

	if (editPunto) {
		triMan.drawEdit();
		triMan.drawEditPuntos();
		if (tempA != ofVec2f(-1, -1)) {
			ofPushStyle();
			ofSetColor(255);
			ofDrawCircle(triMan.triangulos[tempTriangle].esquinas[tempA.x].x, triMan.triangulos[tempTriangle].esquinas[tempA.x].y, 1);
			ofNoFill();
			ofDrawCircle(triMan.triangulos[tempTriangle].esquinas[tempA.x].x, triMan.triangulos[tempTriangle].esquinas[tempA.x].y, triMan.triangulos[tempTriangle].radios[tempA.y]);
			ofPopStyle();
		}
	}
    
    
    //LAVA
    if (enablePolygons) {
        ofPushStyle();
        ofSetColor(255, 255, 255);
        for (int g = 0; g < triMan.getNumTriangles(); g++) {
            for (int h = 0; h < triMan.triangulos[g].polygons.size(); h++) {
                circleTexture.bind();
                glBegin(GL_POLYGON);
                for(int i = 0; i < triMan.triangulos[g].polygons[h].size(); i++){
                    glTexCoord2f(triMan.triangulos[g].esquinas[triMan.triangulos[g].polygons[h][i]].x, triMan.triangulos[g].esquinas[triMan.triangulos[g].polygons[h][i]].y);
                    glVertex2f( triMan.triangulos[g].esquinasMovidas[triMan.triangulos[g].polygons[h][i]].x, triMan.triangulos[g].esquinasMovidas[triMan.triangulos[g].polygons[h][i]].y + 5);
                }
                glEnd();
                circleTexture.unbind();
            }
        }
        ofPopStyle();
    }
	
	if (blancosAlpha > 0){
		ofPushStyle();
		ofSetColor(255, 255, 255, blancosAlpha);
		for (int i = 0; i < triMan.getNumTriangles(); i++){
			ofDrawTriangle(triMan.triangulos[i].posA, triMan.triangulos[i].posB, triMan.triangulos[i].posC);
		}
		ofPopStyle();
	}
    
    //Mask
    if (enableMask) {
        drawMask();
    }
    
    //-------------SYPHON---------------
    //ser.publishScreen();
	ofPopMatrix();
	ofPushStyle();
	ofSetColor(0);
	ofDrawCircle(1735, 1200, 200);
	ofPopStyle();
    //GUI
    if (!mHideGUI) {
        gui.draw();
    }
    
    //Anotaciones
   /* ofPushStyle();
    ofSetColor(255);
    ofDrawBitmapString("time" + ofToString(ofGetElapsedTimeMillis()), 850, 20 );
    ofDrawBitmapString("up" + ofToString(blancosUp), 950, 20 );
    ofDrawBitmapString("down" + ofToString(blancosDown), 950, 30 );
    ofDrawBitmapString("blancosAlpha" + ofToString(blancosAlpha), 950, 40 );
    ofDrawBitmapString("storedTime" + ofToString(storedTime), 950, 50 );
    ofPopStyle();*/
}


//--------------------------------------------------------------
void ofApp::drawMask(){
    ofPushStyle();
    ofSetColor(0);
    ofBeginShape();
    ofVertex(0, 0);
    ofVertex(1920, 0);
    ofVertex(1920, 1080);
    ofVertex(0, 1920);
    for (int i = 4; i < triMan.getNumTriangles(); i++) {
		if (i != 7 && i != 6)
		{
			ofNextContour();
			ofVertex(triMan.triangulos[i].posA.x, triMan.triangulos[i].posA.y);
			ofVertex(triMan.triangulos[i].posB.x, triMan.triangulos[i].posB.y);
			ofVertex(triMan.triangulos[i].posC.x, triMan.triangulos[i].posC.y);
		}
        
    }
	ofNextContour();
	ofVertex(triMan.triangulos[0].posA.x, triMan.triangulos[0].posA.y);
	ofVertex(triMan.triangulos[2].posB.x, triMan.triangulos[2].posB.y);
	ofVertex(triMan.triangulos[2].posC.x, triMan.triangulos[2].posC.y);
	ofVertex(triMan.triangulos[3].posB.x, triMan.triangulos[3].posB.y);
	ofVertex(triMan.triangulos[1].posB.x, triMan.triangulos[1].posB.y);
	ofVertex(triMan.triangulos[0].posC.x, triMan.triangulos[0].posC.y);
	ofVertex(triMan.triangulos[0].posB.x, triMan.triangulos[0].posB.y);
	ofVertex(triMan.triangulos[6].posA.x, triMan.triangulos[6].posA.y);
	ofVertex(triMan.triangulos[6].posB.x, triMan.triangulos[6].posB.y);
	ofVertex(triMan.triangulos[7].posA.x, triMan.triangulos[7].posA.y);
    ofEndShape();
    
    ofPopStyle();
    
}

//--------------------------------------------------------------
void ofApp::drawShader(){
    ofPushStyle();
    img.getTextureReference().bind();
    
    shader.begin();
    
    ofPushMatrix();
    
    // translate plane into center screen.
    float tx = ofGetWidth() / 2;
    float ty = ofGetHeight() / 2;
    ofTranslate(tx, ty);
    
    // the mouse/touch Y position changes the rotation of the plane.
    //float percentY = mouseY / (float)ofGetHeight();
    //float rotation = ofMap(percentY, 0, 1, -60, 60, true) + 60;
    //ofRotate(rotation, 1, 0, 0);
    
    plane.draw();
    
    ofPopMatrix();
    
    shader.end();
    ofPopStyle();
    //ofSetColor(ofColor::red);
    //img.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'g') {
        mHideGUI = ! mHideGUI;
    }else if (key == 't') {
        saveJSON();
	}
	else if (key == OF_KEY_UP) {
		up++;
	}
	else if (key == OF_KEY_DOWN) {
		down++;
	}
	else if (key == OF_KEY_RIGHT) {
		right++;
	}
	else if (key == OF_KEY_LEFT) {
		left++;
	}
    
    if (!enableNewTriangle && !enableNewPuntos && enableNewPoly){
        if (key == ' ' && newPolygon.size()){
            triMan.triangulos[tempTriangle].addPolygon(newPolygon);
            tempTriangle = -1;
            newPolygon.clear();
        }
    }
    if (key == ' ') {
        if (editTriangle && tempTriangle != -1) {
            tempTriangle = -1;
            tempA.set(-1, -1);
        }
    }
    if (editTriangle && tempTriangle != -1 && tempA != ofVec2f(-1, -1)) {
        if (key == 'w') {
            triMan.triangulos[tempTriangle].vertices[tempA.y].y --;
        }else if (key == 's') {
            triMan.triangulos[tempTriangle].vertices[tempA.y].y ++;
        }else if (key == 'd') {
            triMan.triangulos[tempTriangle].vertices[tempA.y].x ++;
        }else if (key == 'a') {
            triMan.triangulos[tempTriangle].vertices[tempA.y].x --;
        }
        triMan.triangulos[tempTriangle].updatePos();
    }
	else if (editPunto && tempTriangle != -1 && tempA != ofVec2f(-1, -1)) {
		if (key == 'w') {
			triMan.triangulos[tempTriangle].esquinas[tempA.x].y--;
			triMan.triangulos[tempTriangle].esquinasMovidas[tempA.x].y--;
		}
		else if (key == 's') {
			triMan.triangulos[tempTriangle].esquinas[tempA.x].y++;
			triMan.triangulos[tempTriangle].esquinasMovidas[tempA.x].y++;
		}
		else if (key == 'd') {
			triMan.triangulos[tempTriangle].esquinas[tempA.x].x++;
			triMan.triangulos[tempTriangle].esquinasMovidas[tempA.x].x++;
		}
		else if (key == 'a') {
			triMan.triangulos[tempTriangle].esquinas[tempA.x].x--;
			triMan.triangulos[tempTriangle].esquinasMovidas[tempA.x].x--;
		}
	}

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    if (editTriangle) {
        if (tempTriangle != -1 && tempA != ofVec2f(-1, -1)) {
            triMan.triangulos[tempTriangle].vertices[tempA.y].set(x, y);
            triMan.triangulos[tempTriangle].updatePos();
            dragged = true;
        }
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    if (enableNewTriangle) {
        if (tempA == ofVec2f(-1, -1)) {
            tempA.set(x, y);
        }else if (tempA != ofVec2f(-1, -1) && tempB == ofVec2f(-1, -1)){
            tempB.set(x, y);
        }else if (tempA != ofVec2f(-1, -1) && tempB != ofVec2f(-1, -1) && tempC == ofVec2f(-1, -1)){
            tempC.set(x, y);
            triMan.add(tempA, tempB, tempC);
            tempA.set(-1, -1);
            tempB.set(-1, -1);
            tempC.set(-1, -1);
        }
    }
    
    if (!enableNewTriangle && enableNewPuntos) {
        
        if (tempA == ofVec2f(-1, -1)) {
            for (int i = 0; i < triMan.getNumTriangles(); i++){
                if (triMan.triangulos[i].isPointInTriangle(ofVec2f(x, y))){
                    triMan.triangulos[i].addEsquina(ofVec2f(x, y));
                    tempA.set(x, y);
                    tempTriangle = i;
                    break;
                }
            }
        }else if (tempA != ofVec2f(0, 0) && tempB == ofVec2f(-1, -1)){
            if (triMan.triangulos[tempTriangle].isPointInTriangle(ofVec2f(x, y))) {
                tempB.set(x, y);
                triMan.triangulos[tempTriangle].addRadio(ofDist(tempA.x, tempA.y, tempB.x, tempB.y));
                tempA.set(-1, -1);
                tempB.set(-1, -1);
                tempTriangle = -1;
            }
        }
    }
    
    if (!enableNewTriangle && !enableNewPuntos && enableNewPoly) {
        if (newPolygon.size() == 0) {
            for (int i = 0; i < triMan.getNumTriangles(); i++){
                if (triMan.triangulos[i].isPointInTriangle(ofVec2f(x, y))){
                    tempTriangle = i;
                    break;
                }
            }
        }
        newPolygon.push_back(triMan.triangulos[tempTriangle].getNearestPoint(ofVec2f(x, y)));
    }
    
    if (editTriangle) {
        if (tempTriangle != -1) {
            tempA = triMan.triangulos[tempTriangle].getNearestVertex(ofVec2f(x, y));
        }else{
            tempTriangle = 0;
            for (int i = 1; i < triMan.getNumTriangles(); i++) {
                if (triMan.triangulos[i].isPointInTriangle(ofVec2f(x, y))) {
                    tempTriangle = i;
                    break;
                }
            }
        }
    }
    
	if (editPunto) {
		if (tempTriangle == -1) {
			tempTriangle = 0;
			for (int i = 1; i < triMan.getNumTriangles(); i++) {
				if (triMan.triangulos[i].isPointInTriangle(ofVec2f(x, y))) {
					tempTriangle = i;
				}
			}
		}
		int temp = triMan.triangulos[tempTriangle].getNearestPoint(ofVec2f(x, y));
		tempA.set(temp, triMan.triangulos[tempTriangle].radios[temp]);
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    if (editTriangle && dragged) {
        tempA.set(-1, -1);
        dragged = false;
    }
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::saveJSON(){
    
    mJSON.clear();
    if (triMan.triangulos.size()) {
        Json::Value points(Json::arrayValue);
        for (int j = 0; j < triMan.triangulos.size(); j++){
            Json::Value pos(Json::objectValue);
            pos["Id"] = triMan.triangulos[j].id;
            pos["Ax"] = triMan.triangulos[j].posA.x;
            pos["Ay"] = triMan.triangulos[j].posA.y;
            pos["Bx"] = triMan.triangulos[j].posB.x;
            pos["By"] = triMan.triangulos[j].posB.y;
            pos["Cx"] = triMan.triangulos[j].posC.x;
            pos["Cy"] = triMan.triangulos[j].posC.y;
            points.append(pos);
        }
        mJSON["Triangles"]  = points;
    }
    
    
    if (!mJSON.save("mesh.json", true)){
        ofLogNotice("ofApp::setup") << "mesh.json written unsuccessfully.";
    }
    else{
        ofLogNotice("ofApp::setup") << "mesh.json written successfully.";
    }
    //-------
    
    
    mJSON.clear();
    if (triMan.triangulos.size()) {
        Json::Value points(Json::arrayValue);
        for (int j = 0; j < triMan.triangulos.size(); j++){
            Json::Value pos(Json::objectValue);
            pos["Id"] = j;
            pos["length"] = (int)triMan.triangulos[j].esquinas.size();
            for (int i = 0; i < triMan.triangulos[j].esquinas.size(); i++) {
                
                pos["Ax" + ofToString(i)] = triMan.triangulos[j].esquinas[i].x;
                pos["Ay" + ofToString(i)] = triMan.triangulos[j].esquinas[i].y;
                if (triMan.triangulos[j].esquinas.size() == triMan.triangulos[j].radios.size()) {
                    pos["R" + ofToString(i)] = triMan.triangulos[j].radios[i];
                }
            }
            points.append(pos);
        }
        mJSON["Puntos"]  = points;
    }
    
    
    if (!mJSON.save("puntos.json", true)){
        ofLogNotice("ofApp::setup") << "puntos.json written unsuccessfully.";
    }
    else{
        ofLogNotice("ofApp::setup") << "puntos.json written successfully.";
    }
    //-------
    
    
    mJSON.clear();
    if (triMan.triangulos.size()) {
        Json::Value points(Json::arrayValue);
        for (int j = 0; j < triMan.triangulos.size(); j++){
            Json::Value pos(Json::objectValue);
            pos["Id"] = j;
            pos["length"] = (int)triMan.triangulos[j].polygons.size();
            for (int i = 0; i < triMan.triangulos[j].polygons.size(); i++) {
                pos["polyLength" + ofToString(i)] = (int)triMan.triangulos[j].polygons[i].size();
                for (int k = 0; k < triMan.triangulos[j].polygons[i].size(); k++) {
                    pos[ofToString(i) + "index" + ofToString(k)] = triMan.triangulos[j].polygons[i][k];
                }
            }
            points.append(pos);
        }
        mJSON["Polygons"]  = points;
    }
    if (!mJSON.save("polygons.json", true)){
        ofLogNotice("ofApp::setup") << "polygons.json written unsuccessfully.";
    }
    else{
        ofLogNotice("ofApp::setup") << "polygons.json written successfully.";
    }
    //-------
    

    
   }

//--------------------------------------------------------------
void ofApp::loadJSON(){
    std::string file;
    file = "mesh.json";
    bool parsingSuccessful = mJSON.open(file);
    if(parsingSuccessful){
        ofLogNotice("ofApp::setup") << "successfully read: "<<file<<std::endl;
    }else{
        ofLogError("ofApp::setup")  << "Failed to parse JSON" << std::endl;
    }
    
    for (const Json::Value & points : mJSON["Triangles"])
    {
        float Ax   = points["Ax"].asFloat();
        float Ay   = points["Ay"].asFloat();
        float Bx   = points["Bx"].asFloat();
        float By   = points["By"].asFloat();
        float Cx   = points["Cx"].asFloat();
        float Cy   = points["Cy"].asFloat();
        
        
        triMan.add(ofVec2f(Ax, Ay), ofVec2f(Bx, By), ofVec2f(Cx, Cy));
    }
    
    file = "puntos.json";
    parsingSuccessful = mJSON.open(file);
    if(parsingSuccessful){
        ofLogNotice("ofApp::setup") << "successfully read: "<<file<<std::endl;
    }else{
        ofLogError("ofApp::setup")  << "Failed to parse JSON" << std::endl;
    }
    
    for (const Json::Value & points : mJSON["Puntos"])
    {
        int Id     = points["Id"].asInt();
        int len    = points["length"].asInt();
        for(int i = 0; i < len; i ++){
            ofVec2f temp;
            temp.set(ofVec2f(points["Ax" + ofToString(i)].asFloat(), points["Ay" + ofToString(i)].asFloat()));
            triMan.triangulos[Id].addEsquina(temp);
            triMan.triangulos[Id].addRadio(points["R" + ofToString(i)].asFloat());
        }
    }
    
    
    file = "polygons.json";
    parsingSuccessful = mJSON.open(file);
    if(parsingSuccessful){
        ofLogNotice("ofApp::setup") << "successfully read: "<<file<<std::endl;
    }else{
        ofLogError("ofApp::setup")  << "Failed to parse JSON" << std::endl;
    }
    
    for (const Json::Value & points : mJSON["Polygons"])
    {
        int Id     = points["Id"].asInt();
        int len    = points["length"].asInt();
        for(int i = 0; i < len; i ++){
            int polyLen = points["polyLength" + ofToString(i)].asInt();
            newPolygon.clear();
            for (int j = 0; j < polyLen; j++) {
                newPolygon.push_back(points[ofToString(i) + "index" + ofToString(j)].asInt());
            }
            triMan.triangulos[Id].polygons.push_back(newPolygon);
            newPolygon.clear();
        }
    }
    
    for (int i = 0; i < triMan.getNumTriangles(); i++) {
        triMan.triangulos[i].puntosInicio = triMan.triangulos[i].puntos;
    }
    
    

}

