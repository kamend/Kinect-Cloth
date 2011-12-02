uniform sampler2DRect tex;
varying vec3 normal;
varying vec3 lightPos;

void main() {
    
    vec3 texColor = texture2DRect(tex,gl_TexCoord[0].st).rgb;
    
    
    vec3 nlpos = normalize(lightPos);
    vec3 nN = normalize(normal);
    float lt = max(dot(nlpos,nN),0.0);
            
    texColor *= lt;
      
    gl_FragColor = vec4(texColor,1.0);
}