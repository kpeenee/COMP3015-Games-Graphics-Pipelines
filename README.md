# COMP3015 Games Graphics Pipelines  
<h2>Technology<h2>  
- Windows 10  
- Visual studio 2019  

<h2>How To Open<h2>  
There is a folder called build. Click on Build > Debug then open the Project_Template application to view 

<h2>What is This Project<h2>  
This project shows off the new version of Craig. Craig is a goofy looking model I made in blender and this project aims to make a goofy looking scene
with Craig and his new worm buddies.

<h2>How the project works<h2>  
The project now uses some Gamma Correction to get the correct colours. This was done using the formula  
vec4( pow( colour, vec3(1.0/Gamma) ), 1.0 );  

Usually the value 2.2 is used for gamma but I felt it was too bright for my scene so I used 1.5 instead.  

This scene contains 3 lights that is set up using a struct called LightInfo. This struct takes information for the Ambient and Diffuse intensity. 
These values are used for some toon shading. For the toon shading a number for levels of shading was used. I set it to 4 and using  
const float scaleFactor = 1.0 / levels;  
The value assigned to a pixel for lighting can be clamped to either 0.0, 0.25, 0.5, 0.75. To work this out I got the direction of a light by using:  
s = normalize(vec3(Lights[light].Position) - position);  
Then find the dot product between the normal and the lights direction with:  
sDotN = max( dot(s,n), 0.0 );  
Then a final colour can be gained by doing:  
Material.Kd * floor(sDotN * levels) * scaleFactor * col; 

I have created a dissolve effect by using a noise. I created a noise texture in GIMP to be used for the effect. 
The shader can read the alpha value of a pixel, returning a value between 0 and 1, and use it to create different effects. In the fragment shader, 
if the alpha of the current noise texture is below a threshold, then the current pixel on the other textures are discarded. This 
creates some holes on the object. To create the dissolve animation, I made the threshold a uniform variable and then in scenebasic_uniform.cpp 
it sets a value for threshold based on sin(time). This will return value's each frame between -1 and 1. I could have used 
abs(Threshold) to make sure negative value's were made positive but prefered it without so the scene stayed untouched for a few seconds before 
beginning to dissolve again.  

To create the worms some vertex animation was done to the surface of some objects. This was done by manipulating vertices in the vertex shader. 
A formula was used to create the wavey animation the worms use this formula is:  
n(x, t) = (-A 2????cos( 2????(x ??? vt)), 1)  
A ??? wave???s amplitude (the height of peaks)  
?? ??? wavelength (the distance between successive peaks)  
v ??? wave velocity    
t ??? time  
This formula is used to move the y position of the vertices.  


Some Silhouette shading was used to make the scene look more like a cartoon. Silhouette shading uses a geometry shader to create black lines along 
the edges of the object by using quads alligned with the edges. A geometry shader executes once for each primitive and 
can be used to add or remove geometrey from an object. For this effect to be achieved, the geometry shader needs information on adjacent 
primitives. This is done by using openGL's adjacency modes. In this case GL_TRIANGLES_ADJACENCY is used to get 6 vertices. 
With this infomation a silhouette edge can be determined if the triangle is facing forward but the corresponding adjacent is not facing forward. 
To determine if a triangle is front facing the triangles normal is used. Then using eye coordinates, if the z coordinate of the normal vector is 
positive then it is a front facing triangle. A formula can be used to work this out:  
nz = (AxBy - BxAy) + (BxCy - CxBy) + (CxAy -AxCy)  
This formula returns the normal vector then check if z is positive or negative. 
Once these edges have been found, the additional quads will be generated to create the effect.

<h2>Code navigation<h2>  
The fragment shader (basic_uniform.frag) contains all the code for gamma correction, noise and toon shading.  
The geometry shader (basic_uniform.gs) contains the silhouette code.  
The vertex shader (basic_uniform.vert) contains all the code for animation.  
Everything needed to setup the scene is located in scenebasic_uniform.cpp. This contains the code for setting up the lights, models, noise and animation parameters.


<h2>YouTube link<h2>
https://youtu.be/fkCVczuJUps
