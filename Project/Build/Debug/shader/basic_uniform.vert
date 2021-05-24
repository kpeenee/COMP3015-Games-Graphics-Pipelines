#version 460

//in variables, this are in model coordinates
layout (location = 0) in vec3 VertexPosition; 
layout (location = 1) in vec3 VertexNormal; 
layout (location = 2) in vec2 VertexTexCoord;

//out vector needed for the geometry shader
out vec3 VPosition;
out vec3 VNormal;
out vec2 VTexCoord;

uniform float Time;
 
 uniform float Freq = 2.5;
 uniform float Velocity = 2.5;
 uniform float Amp = 0.6;
 uniform bool Animate = false;

//uniforms for matrices required in the shader
uniform mat4 ModelViewMatrix;   //model view matrix
uniform mat3 NormalMatrix;		//normal matrix
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;				//model view projection matrix



 
void main() 
{ 
    if(Animate == true)
    {
        vec4 pos = vec4(VertexPosition, 1.0);

        float u = Freq * pos.x - Velocity * Time;
        pos.y = Amp * sin(u);

        vec3 n = vec3(0.0);
        n.xy = normalize(vec2(cos(u), 1.0));

      //transfrom normal from model coordinates to view coordinates
      VNormal = NormalMatrix * n;

      //transform vertex position from model coordinates to view coordinates
      VPosition =  vec3(ModelViewMatrix * pos);


      VTexCoord = VertexTexCoord;


      //turns any vertex position into model view projection in preparations to 
      //graphics pipeline processes before fragment shader (clipping)
      gl_Position = MVP * pos; 
  }else{



  //transfrom normal from model coordinates to view coordinates
  VNormal = normalize( NormalMatrix * VertexNormal);

  //transform vertex position from model coordinates to view coordinates
  VPosition =  (ModelViewMatrix * vec4(VertexPosition,1.0)).xyz;


  VTexCoord = VertexTexCoord;


  //turns any vertex position into model view projection in preparations to 
  //graphics pipeline processes before fragment shader (clipping)
  gl_Position = MVP * vec4(VertexPosition,1.0); 
  }
} 