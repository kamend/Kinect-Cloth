//
//  ofxKCloth.h
//  KinectCloth
//
//  Created by Kamen Dimitrov on 11/30/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef KinectCloth_ofxKCloth_h
#define KinectCloth_ofxKCloth_h

#include "ofMain.h"
#include "ofxKMesh.h"


#define CLOTH_RES 10

class ClothParticle {
public:
    ofVec3f pos;
    ofVec3f initPos;
    ofVec3f vel;
    ofVec3f forces;
    
    float mass;
    float inverse_mass;
    float drag;

    
    ClothParticle(ofVec3f _pos, float _mass, float _drag);
    
    void update();
    void addForce(ofVec3f _f);
    
};


class ClothSpring {
public:
    ClothParticle *a, *b;
    float k;
    float rest_length;

    
    ClothSpring(ClothParticle *_a,ClothParticle *_b, float _k);
    void update();
    
};

class ClothController {
public:
    int rows,cols;
    int particlesNum;
    ofMesh particlesMesh;
    vector <ClothSpring*> springs;
    vector <ClothParticle*> particles;
  
    ClothController();
    
    void init(int _r, int _c);
    void resetParticlesPosition();
    void applyForces();
    void update();
    void draw();
    void applyForce(int rx, int cy, ofVec3f force);
    // mesh functions
    void initMesh();
    void updateMeshNormals();
    void updateMesh();
    void drawMesh();

};


#endif
