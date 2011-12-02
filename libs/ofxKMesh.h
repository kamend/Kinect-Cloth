//
//  ofxKMesh.h
//  emptyExample
//
//  Created by Kamen Dimitrov on 11/6/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef emptyExample_ofxKMesh_h
#define emptyExample_ofxKMesh_h

#include "ofMain.h"

class ofxKMesh {
public:
    static void calcMeshNormals(ofMesh &mesh);
    static ofVec3f calcNormal(ofVec3f p1,ofVec3f p2, ofVec3f p3);
};



#endif
