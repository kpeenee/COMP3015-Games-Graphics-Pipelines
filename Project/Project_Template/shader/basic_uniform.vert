#version 460

//in variables, this are in model coordinates
layout (location = 0) in vec3 VertexPosition; 
layout (location = 1) in vec3 VertexNormal; 
layout (location = 2) in vec2 VertexTexCoord;

//out vector needed for the geometry shader
out vec3 VPosition;
out vec3 VNormal;
out vec2 VTexCoord;
 

//uniforms for matrices required in the shader
uniform mat4 ModelViewMatrix;   //model view matrix
uniform mat3 NormalMatrix;		//normal matrix
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;				//model view projection matrix



 
void main() 
{ 

  //transfrom normal from model coordinates to view coordinates
  VNormal = normalize( NormalMatrix * VertexNormal);

  //transform vertex position from model coordinates to view coordinates
  VPosition =  (ModelViewMatrix * vec4(VertexPosition,1.0)).xyz;


  VTexCoord = VertexTexCoord;


  //turns any vertex position into model view projection in preparations to 
  //graphics pipeline processes before fragment shader (clipping)
  gl_Position = MVP * vec4(VertexPosition,1.0); 
} 