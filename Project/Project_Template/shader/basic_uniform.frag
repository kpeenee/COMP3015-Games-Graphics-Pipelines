#version 460

//in variable that receives the diffuse calculation from the vertex shader
uniform vec4 LineColor;

in vec3 GPosition;
in vec3 GNormal;
in vec2 GTexCoord;

flat in int GIsEdge;



//out variable, this typical for all fragment shaders
layout (location = 0) out vec4 FragColor;


layout(binding = 0) uniform sampler2D BrickTex;
layout(binding = 1) uniform sampler2D MossTex;
layout(binding = 2) uniform sampler2D CraigTex;
layout(binding = 3) uniform sampler2D DirtTex;
layout(binding = 4) uniform sampler2D NoiseTex;

uniform bool texUse;
uniform float threshold;


//light information struct
uniform struct LightInfo 
{
  vec4 Position; // Light position in eye coords.
  vec3 La;       // Ambient intensity
  vec3 L;        // Diffuse + Specular intensity
} Lights[3];

//material information struct
uniform struct MaterialInfo 
{
  vec3 Kd;       // Diffuse reflectivity
  vec3 Ks;      // Specular reflectivity
  vec3 Ka;      // Ambient reflectivity
  float Shininess;
} Material;

const int levels = 4;
const float scaleFactor = 1.0 / levels;

vec3 toonShading( int light, vec3 position, vec3 n )
{
   //Get Tex Colour
   vec4 baseColour;
   vec4 decorationColour;
   if(texUse == false)
   {
         baseColour = texture(BrickTex, GTexCoord);
         decorationColour = texture(MossTex, GTexCoord);
   }
   if(texUse != false)
   {
         baseColour = texture(CraigTex, GTexCoord);
         decorationColour = texture(DirtTex, GTexCoord);
   }
        
   
   vec3 col = mix(baseColour.rgb, decorationColour.rgb, decorationColour.a);


  //Ambient
  vec3 ambient = Lights[light].La * Material.Ka * col;

  //Diffuse
  vec3 s = normalize(vec3(Lights[light].Position) - position);  //calculate light direction
  float sDotN = max( dot(s,n), 0.0 );       //calculate dot product
  vec3 diffuse = Material.Kd * floor(sDotN * levels) * scaleFactor * col;       // Diffuse calculation


 return ambient + Lights[light].L * diffuse ;
}



void main()
{
    
    float Gamma = 1.5;
    //we pass LightInyensity to outr FragColor, notice the difference between vector types
    // vec3 and vec4 and how we solved the problem
    vec3 Colour = vec3(0.0);

    vec4 noise = texture(NoiseTex,GTexCoord);

    if(noise.a < threshold)
        discard;
    

    if(GIsEdge == 1){
        FragColor = LineColor;
    } else{
    for( int i = 0; i < 3; i++ )
        Colour += toonShading( i, GPosition, normalize(GNormal));
        FragColor = FragColor = vec4( pow( Colour, vec3(1.0/Gamma) ), 1.0 );
         }
    
}
