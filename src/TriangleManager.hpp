#ifndef TriangleManager_hpp
#define TriangleManager_hpp

#include <stdio.h>
#include "ofMain.h"
#include "Triangle.hpp"

#endif /* TriangleManager_hpp */


class TriangleManager{
public:
    vector<Triangle> triangulos;
    
    void setup();
    void update(float frequency, float timeFrequency, float magnitude);
    void draw();
    void drawEdit();
    void drawEditPuntos();
	void drawEditLines();
    
    void add(ofVec2f, ofVec2f, ofVec2f);
    int getNumTriangles();
    
    void addPolygon(int aCual, vector<int> newPoly);
    

    
};