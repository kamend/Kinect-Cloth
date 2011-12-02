#include "testApp.h"



//--------------------------------------------------------------
void testApp::setup(){
	
	// setup
	ofSetLogLevel(OF_LOG_VERBOSE);
    
    ofBackground(0,0,0);
    ofSetFrameRate(30);
    ofEnableSmoothing();
    
    cols = 640 / CLOTH_RES;
    rows = 480 / CLOTH_RES;
    
    controller.init(cols,rows);
    controller.initMesh();
 
    
    oldMouseX = -999;
    oldMouseY = -999;

    tex.loadImage("texture-brick.png");
    
    shader.load("shader");
    
    kinect.init();
    kinect.open();
    
    directional.setDirectional();
    
      
}

//--------------------------------------------------------------
void testApp::update(){
    kinect.update();
    
  //  controller.resetParticlesPosition();
    
    // apply kinect forces
    
    for(int i=0;i<cols*rows;i++) {
        
        int x = int(i) % cols;
        int y = int(i) / cols;
        
        if(y == cols-1) continue;
              
        float d = kinect.getDistanceAt(x*CLOTH_RES, y*CLOTH_RES);
        
        if(d >0 && d < 1000) {
            
            d = ofMap(d,0,4000,0,30);
                            
            ofVec3f ff = ofVec3f(0,0.0,d);
            ff.normalize();
            ff *= 3.0;
                  
            controller.particles[i]->addForce(ff);
       }
    }
   
    controller.update();
    controller.updateMesh();
    controller.updateMeshNormals();
}

//--------------------------------------------------------------
void testApp::draw(){
    glEnable(GL_DEPTH_TEST);
    ofEnableLighting();
    directional.enable();
    ofTranslate(ofGetWidth()/2-cols*CLOTH_RES/2, 100,0);
    ofBackground(0);
    shader.begin();
    shader.setUniformTexture("tex", tex.getTextureReference(), 0);
    controller.drawMesh();
    shader.end();
    directional.disable();
    ofDisableLighting();
    
    mainOutputSyphonServer.publishScreen();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    directional.setPosition(x, y, 100);
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
   
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    ofVec3f f = ofVec3f(0,0,50);
    
    controller.particles[200]->addForce(f);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
//---
void testApp::exit() {
    kinect.close();
}