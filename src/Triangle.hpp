

#ifndef Triangle_hpp
#define Triangle_hpp

#include <stdio.h>
#include "ofMain.h"

#endif /* Triangle_hpp */

class Triangle{
public:
    ofVec2f posA;
    ofVec2f posB;
    ofVec2f posC;
    vector<ofVec2f> vertices;
    int id;
    ofVec2f center;
    void setup(ofVec2f, ofVec2f, ofVec2f, int);
    void drawEdit();
    void drawEditPuntos();
	void drawEditPolygons();
    void draw();
    ofVec2f getCenter(int cual);
    ofVec2f getMin();
    ofVec2f getMax();
    
    void updatePos();

	//Puntos
	vector<vector <ofVec2f> > puntos;//{(punto1), (punto2), (punto2-punto1)/Norm(punto2-punto1), coord}
    vector<vector <ofVec2f> > puntosInicio;
	void addPuntos(vector<ofVec2f> newPuntos);
    void updatePuntos(float frequency, float timeFrequency, float magnitude);
	int getNearestPoint(ofVec2f cual);
    ofVec2f getNearestVertex(ofVec2f cual);
    vector<vector<int> > polygons;
    vector<ofVec2f> polyPos;
    vector<ofVec2f> centros;
    void addPolygon(vector<int>);
    
    vector<ofVec2f> esquinas;
    vector<ofVec2f> esquinasMovidas;
    vector<float> radios;
    void addEsquina(ofVec2f);
    void addRadio(float);
    
    float sign (ofVec2f p1, ofVec2f p2, ofVec2f p3){
        return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
    }
    
    bool isPointInTriangle (ofVec2f pt){
        
        if(pt == posA||pt == posC||pt == posB)
            return false;
        
        bool b1, b2, b3;
        
        b1 = sign(pt, posA,  posB) < 0.0f;
        b2 = sign(pt, posB,  posC) < 0.0f;
        b3 = sign(pt, posC,  posA) < 0.0f;
        
        return ((b1 == b2) && (b2 == b3));
    }
};