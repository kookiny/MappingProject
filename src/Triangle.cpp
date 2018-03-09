

#include "Triangle.hpp"

//--------------------------------------------------------------
void Triangle::setup(ofVec2f A, ofVec2f B, ofVec2f C, int indice){
    posA = A;
    posB = B;
    posC = C;
    vertices.push_back(posA);
    vertices.push_back(posB);
    vertices.push_back(posC);
    id = indice;
    
    center.set(1.0/3.0*(posA.x + posB.x + posC.x), 1.0/3.0 * (posA.y + posB.y + posC.y));
    
}

//--------------------------------------------------------------
void Triangle::drawEdit(){
    ofPushStyle();
    ofSetColor(255, 255, 0);
    ofSetLineWidth(1);
    ofNoFill();
    ofDrawTriangle(posA, posB, posC);
    ofPopStyle();
}

//--------------------------------------------------------------
void Triangle::drawEditPuntos(){
    ofPushStyle();
    for(int i = 0; i < esquinas.size(); i++){
        ofSetColor(0, 255, 0);
        ofDrawCircle(esquinas[i].x, esquinas[i].y, 1);
    }
    ofPopStyle();
}

//--------------------------------------------------------------
void Triangle::addPuntos(vector<ofVec2f> newPuntos) {
	puntos.push_back(newPuntos);
}

//--------------------------------------------------------------
void Triangle::updatePuntos(float frequency, float timeFrequency, float magnitude) {
    float time = ofGetElapsedTimef();
    float noiseTime = time * timeFrequency;
    for (int i = 0; i < radios.size(); i++) {
        ofVec2f noisePos = esquinasMovidas[i] * frequency;
        ofVec2f vel;
        vel.x = ofSignedNoise( noisePos.x, noisePos.y, noiseTime );
        vel.y = ofSignedNoise( noiseTime, noisePos.y, noisePos.x );
        ofVec2f normVel = vel.getNormalized();
        ofVec2f v1 = esquinasMovidas[i] + (vel * magnitude);
        if(ofDist(v1.x, v1.y, esquinas[i].x, esquinas[i].y) < radios[i]){
            esquinasMovidas[i] = v1;
        }
    }
    

}

//--------------------------------------------------------------
int Triangle::getNearestPoint(ofVec2f cual) {
	int nearest = 0;
	float actDist = ofDist(cual.x, cual.y, esquinas[0].x, esquinas[0].y);
	for (int i = 1; i < esquinas.size(); i++){
		float temp = ofDist(cual.x, cual.y, esquinas[i].x, esquinas[i].y);
		if (temp < actDist){
			nearest = i;
			actDist = temp;
		}
	}
	return nearest;
}

//--------------------------------------------------------------
ofVec2f Triangle::getNearestVertex(ofVec2f cual) {
    int nearest = 0;
    float distA = ofDist(cual.x, cual.y, posA.x, posA.y);
    float distB = ofDist(cual.x, cual.y, posB.x, posB.y);
    float distC = ofDist(cual.x, cual.y, posC.x, posC.y);
    
    float minDist = min(min(distA, distB), distC);
    if (minDist == distA) {
        return ofVec2f(id, 0);
    }else if (minDist == distB){
        return ofVec2f(id, 1);
    }else return ofVec2f(id, 2);
}

//--------------------------------------------------------------
void Triangle::drawEditPolygons() {
	ofPushStyle();
	ofSetColor(0, 255, 255);
	for (int i = 0; i < polygons.size(); i++){
        for (int j = 0; j < polygons[i].size()-1; j++) {
            if (j == 0) {
                ofDrawLine(esquinas[polygons[i][j]], esquinas[polygons[i][polygons[i].size()]]);
            }
            ofDrawLine(esquinas[polygons[i][j]], esquinas[polygons[i][j +1]]);
        }
	}
	ofPopStyle();
}

//--------------------------------------------------------------
void Triangle::draw() {

}

//--------------------------------------------------------------
void Triangle::addPolygon(vector<int> newPoly){
    polygons.push_back(newPoly);
}

//--------------------------------------------------------------
ofVec2f Triangle::getCenter(int cual){
    ofVec2f centerPos = ofVec2f(0, 0);
    for (int i = 0; i < polygons[cual].size(); i++) {
        centerPos += polygons[cual][i];
    }
    return centerPos;
}

//--------------------------------------------------------------
ofVec2f Triangle::getMin(){
    ofVec2f minimo;
    minimo.set(min(posA.x, min(posB.x, posC.x)), min(posA.y, min(posB.y, posC.y)));
    return minimo;
}

//--------------------------------------------------------------
ofVec2f Triangle::getMax(){
    ofVec2f maximo;
    maximo.set(max(posA.x, max(posB.x, posC.x)), max(posA.y, max(posB.y, posC.y)));
    return maximo;
}

//--------------------------------------------------------------
void Triangle::addEsquina(ofVec2f cual){
    esquinas.push_back(cual);
    esquinasMovidas.push_back(cual);
}

//--------------------------------------------------------------
void Triangle::addRadio(float cual){
    radios.push_back(cual);
}

//--------------------------------------------------------------
void Triangle::updatePos(){
    posA = vertices[0];
    posB = vertices[1];
    posC = vertices[2];
}