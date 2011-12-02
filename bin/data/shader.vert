varying vec3 normal;
varying vec3 lightPos;

void main() {
    gl_TexCoord[0] = gl_MultiTexCoord0;
    
    
    lightPos = gl_LightSource[0].position.xyz;
    
    normal = gl_Normal * gl_NormalMatrix;
    
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}