//
//  ofxKCloth.cpp
//  KinectCloth
//
//  Created by Kamen Dimitrov on 11/30/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "ofxKCloth.h"

//---------------------------
// ClothParicle

ClothParticle::ClothParticle(ofVec3f _pos, float _mass, float _drag = 0.96) {

    forces = ofVec3f(0,0,0);
   
    pos = _pos;
    initPos = _pos;
    mass = _mass;
    
    if(mass == 0) inverse_mass = 0;
    else if(mass < 0.001) mass = 0.001;
    
    if(mass != 0.0f) inverse_mass = 1.0 / mass;
    
    drag = _drag;

}
//---------------------------
void ClothParticle::update() {
    // back to init position force
    ofVec3f backToInitPos = pos - initPos;
    if(backToInitPos.length() > 10) {
        backToInitPos.normalize();
        vel -= backToInitPos;
    }
    
    forces *= inverse_mass;
    vel += forces;

    forces.set(0);
 
    vel.limit(5);
    
    pos += vel;
    vel *= drag;
    
    // bound
  //  if(pos.z > 100) pos.z = 100;
}
//---------------------------
void ClothParticle::addForce(ofVec3f _f) {
   forces += _f;
}
//---------------------------
// Cloth Spring

ClothSpring::ClothSpring(ClothParticle *_a,ClothParticle *_b, float _k=.2) {
    k = 0.2;
    a = _a;
    b = _b;
   
    ofVec3f l = b->pos - a->pos;
    float length = l.length();
    rest_length = length;
}
//---------------------------
void ClothSpring::update() {
    
    ofVec3f dir = b->pos - a->pos;
    float dist = dir.length();
    if (dist == 0.0) dist = 0.0001; // prevent division by zero
    float f = (rest_length - dist) * k; // linear force spring
    dir.normalize();
    
    a->addForce(dir * -f);
    b->addForce(dir * f);
    
    
}

// ClothController
//---------------------------
ClothController::ClothController() {
    particlesNum = 0;
    cols = 0;
    rows = 0;
}

//---------------------------

void ClothController::init(int _c,int _r) {
   
    cols = _c;
    rows = _r;
   
    
    // create particles
    particlesNum = rows * cols;
        
    for(int i=0;i<particlesNum;i++) {
        
        int x = int(i) % cols * CLOTH_RES;
        int y = int(i) / cols * CLOTH_RES;
        
        ofVec3f pos = ofVec3f( x,y, 0);
        
        ClothParticle* p = new ClothParticle(pos,(y == 0 || y == rows-1) ? 0 : 1,0.96);

        particles.push_back(p);
    }

    
    // create springs
    
    for(int i=0;i<rows * cols;i++) {
        
        int x = int(i) % cols;
        int y = int(i) / cols;
        
        // horizontal structural springs
        
        if(x < cols-1) {
            ClothSpring *sp = new ClothSpring(particles[i],particles[i+1]);
            springs.push_back(sp);
        }
        
        // verticles structural springs
        
        if(y < rows-1) {
            ClothSpring *sp = new ClothSpring(particles[i],particles[i+cols]);
            springs.push_back(sp);
        }
        
        //shear springs left to right
        if (x<cols-1 && y<rows-1) {
            ClothSpring *sp = new ClothSpring(particles[i],particles[i+cols+1]);
            springs.push_back(sp);
        }
        
        //shear springs right to left
        if (y>0 && x<cols-1 && y<rows) {
            ClothSpring *sp = new ClothSpring(particles[i],particles[i-cols+1]);
            springs.push_back(sp);
        }
        
        //bending springs horizontal
        if (x<cols-2) {
            ClothSpring *sp = new ClothSpring(particles[i],particles[i+2]);
            springs.push_back(sp);
        }
        
        //bending springs vertical
        if (y<rows-2) {
            ClothSpring *sp = new ClothSpring(particles[i],particles[i+2*cols]);
            springs.push_back(sp);
        }
        
        //bending springs diagonal
        if (y<rows-2 && x<cols-2) {
            ClothSpring *sp = new ClothSpring(particles[i],particles[i+2+2*cols]);
            springs.push_back(sp);
        }        
        
        
    }

    
   
}
//---------------------------
void ClothController::resetParticlesPosition() {
    for(int i=0;i<particlesNum;i++) {
        particles[i]->pos = particles[i]->initPos;
    }
}
//---------------------------
void ClothController::initMesh() {
    // build mesh from particles
    particlesMesh.setMode(OF_PRIMITIVE_TRIANGLES);
    particlesMesh.clear();
    
    for(int i=0;i<particlesNum;i++) {
        
        int x = int(i) % cols * CLOTH_RES;
        int y = int(i) / cols * CLOTH_RES;
        
        ofVec3f pos = particles[i]->pos;
        
        particlesMesh.addVertex(pos);
        
        if(x > 0 && y > 0) {
            int idx = particlesMesh.getNumVertices()-1;
            particlesMesh.addTriangle(idx-1-cols, idx-cols, idx);
            particlesMesh.addTriangle(idx-1-cols,idx,idx-1);   
            
        }
        
        particlesMesh.addColor(ofColor(255,255,255));
        particlesMesh.addTexCoord(ofVec2f(x,y));
        particlesMesh.addNormal(ofVec3f(0,0,0));
    }      
}
//---------------------------
void ClothController::updateMesh() {
    for(int i=0;i<particlesNum;i++) {
        particlesMesh.setVertex(i, particles[i]->pos);
    }            
                
}
//---------------------------
void ClothController::updateMeshNormals() {
    ofxKMesh::calcMeshNormals(particlesMesh);
}
//---------------------------
void ClothController::drawMesh() {
    particlesMesh.drawFaces();
}

//---------------------------

void ClothController::draw() {
    
    ofTranslate(ofGetWidth()/2-200, 100, 0);
      
    //springs
    ofSetColor(255, 255, 255);
    glBegin(GL_LINES);
    for (int i=0; i<springs.size(); i++) {
        glVertex3f(springs[i]->a->pos.x, springs[i]->a->pos.y, springs[i]->a->pos.z);
        glVertex3f(springs[i]->b->pos.x, springs[i]->b->pos.y, springs[i]->b->pos.z);
    }    
    glEnd();
    
   
    //particles
    ofSetColor(0, 0, 255);
    glPointSize(5);
    glBegin(GL_POINTS);
    for (int i=0; i<particles.size(); i++) {
        glVertex3f(particles[i]->pos.x, particles[i]->pos.y, particles[i]->pos.z);
    }    
    glEnd();

     
}

void ClothController::applyForces() {
    // apply forces to particles
    for(int i=0;i < particles.size();i++) {
        particles[i]->addForce(ofVec3f(0,0.1,0));
        particles[i]->addForce(ofVec3f(0,0,sin(ofGetElapsedTimef())*0.2));
    }    
}
//---------------------------
void ClothController::update() {
   
    // update springs
    for(int i=0;i < springs.size(); i++) {
        springs[i]->update();
    }
    
    // update particles
    for(int i=0;i < particles.size(); i++) {
        particles[i]->update();
    }
}
//---------------------------
void ClothController::applyForce(int rx, int cy, ofVec3f force) {
    
    int index = rx + cy * cols;
   
    particles[index]->addForce(force);
    
}













