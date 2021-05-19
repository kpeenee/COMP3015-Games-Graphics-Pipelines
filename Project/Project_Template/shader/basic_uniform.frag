#version 460

//in variable that receives the diffuse calculation from the vertex shader
in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

//out variable, this typical for all fragment shaders
layout (location = 0) out vec4 FragColor;


layout(binding = 0) uniform sampler2D BrickTex;
layout(binding=1) uniform sampler2D MossTex;
layout(binding = 2) uniform sampler2D CraigTex;
layout(binding = 3) uniform sampler2D DirtTex;

uniform bool texUse;


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

vec3 blinnPhongModel( int light, vec3 position, vec3 n )
{
   //Get Tex Colour
   vec4 baseColour;
   vec4 decorationColour;
   if(texUse == false)
   {
         baseColour = texture(BrickTex, TexCoord);
         decorationColour = texture(MossTex, TexCoord);
   }
   if(texUse != false)
   {
         baseColour = texture(CraigTex, TexCoord);
         decorationColour = texture(DirtTex, TexCoord);
   }
        
   
   vec3 col = mix(baseColour.rgb, decorationColour.rgb, decorationColour.a);


  //Ambient
  vec3 ambient = Lights[light].La * Material.Ka * col;

  //Diffuse
  vec3 s = normalize(vec3(Lights[light].Position) - position);  //calculate light direction
  float sDotN = max( dot(s,n), 0.0 );       //calculate dot product
  vec3 diffuse = Material.Kd * floor(sDotN * levels) * scaleFactor * col;       // Diffuse calculation

  //Specular
  vec3 spec = vec3(0.0);
  if( sDotN > 0.0 )
    {
        vec3 v = normalize(-position.xyz);
        vec3 h = normalize(v + s);
        spec = Material.Ks * pow( max( dot(h,n), 0.0 ),
                                  Material.Shininess );
    }
 return ambient + Lights[light].L * (diffuse + spec);
}

void main()
{
    //we pass LightInyensity to outr FragColor, notice the difference between vector types
    // vec3 and vec4 and how we solved the problem
    vec3 Colour = vec3(0.0);
    for( int i = 0; i < 3; i++ )
        Colour += blinnPhongModel( i, Position, normalize(Normal));
    FragColor = vec4(Colour, 1.0);
}
