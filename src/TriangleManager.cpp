
#include "TriangleManager.hpp"

//--------------------------------------------------------------
void TriangleManager::setup(){

    
}

//--------------------------------------------------------------
void TriangleManager::add(ofVec2f A, ofVec2f B, ofVec2f C){
    Triangle temp;
    temp.setup(A, B, C, triangulos.size());
    triangulos.push_back(temp);

}

//--------------------------------------------------------------
void TriangleManager::drawEdit(){
    for (int i = 0; i < triangulos.size(); i++) {
        triangulos[i].drawEdit();
    }
}

//--------------------------------------------------------------
int TriangleManager::getNumTriangles(){
    return triangulos.size();
}

//--------------------------------------------------------------
void TriangleManager::drawEditPuntos(){
    for (int i = 0; i < triangulos.size(); i++) {
        triangulos[i].drawEditPuntos();
    }
}

//--------------------------------------------------------------
void TriangleManager::update(float frequency, float timeFrequency, float magnitude){
    for (int i = 0; i < triangulos.size(); i++) {
        triangulos[i].updatePuntos(frequency, timeFrequency, magnitude);
    }
}

//--------------------------------------------------------------
void TriangleManager::drawEditLines(){
	for (int i = 0; i < triangulos.size(); i++) {
		triangulos[i].drawEditPolygons();
	}
	
}

//--------------------------------------------------------------
void TriangleManager::draw(){

}

//--------------------------------------------------------------
void TriangleManager::addPolygon(int aCual, vector<int> newPoly){
    triangulos[aCual].addPolygon(newPoly);
}