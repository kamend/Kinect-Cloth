//
//  ofxKMesh.cpp
//  emptyExample
//
//  Created by Kamen Dimitrov on 11/6/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "ofxKMesh.h"

void ofxKMesh::calcMeshNormals(ofMesh &mesh) {
    
    // if vertices are 0 return
    if(!mesh.getNumIndices() > 0) {
        return;
    }
    
    // zero all normals

    // got through all triangles and calculate the average normal
    
    int numTriangles = mesh.getNumIndices() / 3;
     
    int j=0;
    
    
    for(int i=0; i < numTriangles; i++) {
        
        int p1index = mesh.getIndex(i*3);
        int p2index = mesh.getIndex(i*3+1);
        int p3index = mesh.getIndex(i*3+2);
        
        ofVec3f p1 = mesh.getVertex(p1index);
        ofVec3f p2 = mesh.getVertex(p2index);
        ofVec3f p3 = mesh.getVertex(p3index);
        
        // calc normal for p1, p2,p3
        ofVec3f n;
        
        n = calcNormal(p1,p2,p3);
        
        // average normal for p1,p2,p3
        
        ofVec3f n1 = mesh.getNormal(p1index);
        n1 += n;
        n1 /= 2;
        
        mesh.setNormal(p1index,n1);
         
        
        ofVec3f n2 = mesh.getNormal(p2index);
        n2 += n;
        n2 /= 2;
        mesh.setNormal(p2index,n2);
        
            
        ofVec3f n3 = mesh.getNormal(p3index);
        n3 += n;
        n3 /= 2;
        mesh.setNormal(p3index,n3);
        
        j++;
        
    }
    
    
    
}


ofVec3f ofxKMesh::calcNormal(ofVec3f p1,ofVec3f p2, ofVec3f p3) {
    ofVec3f v1 = p2 - p1;
    ofVec3f v2 = p3 - p1;
    
    ofVec3f n = v1.cross(v2);
    return n;
    
}

