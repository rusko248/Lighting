// DEFERRED RENDERING
// -----------------------------------
//  renders a point light source
// -----------------------------------


//from vert shader
varying vec4 pos; //location of fragment in view space

//G-Buffer data
uniform sampler2D normals;
uniform sampler2D depths;
uniform sampler2D colors;

//Util vars
uniform int screenWidth;
uniform int screenHeight;

uniform float near;
uniform float far;

//Lighting vars
uniform float radius;
varying vec4 lpos; //light position in view space
uniform vec3 diffuseLightColor;

//Makes depth values linear (i.e. inverse of the perpective projection)
float DepthToLinearZ(float dVal)
{
	return (far*near) / (far-(dVal*(far-near)));
}

void main()
{
   //normalize coord

	float sw = float (screenWidth);
	float sh = float (screenHeight);

	vec2 coord = (gl_FragCoord).xy;

	coord.x = coord.x / sw;
	coord.y = coord.y / sh;

	//Data lookups
	vec4 n = (texture2D(normals, coord)*2.0)-1.0;
   	vec3 p = normalize(pos.xyz)*DepthToLinearZ(texture2D(depths, coord).r); //position in view space of geometry
  	 vec4 c = texture2D(colors, coord);
	
   //Lighting Calcs (view space)
   vec3 ltop = lpos.xyz-p;

   float diffuseModifier = max(dot(n.xyz,normalize(ltop)), 0.0);

   float noZTestFix = step(0.0, radius - length(ltop)); //0.0 if dist > radius, 1.0 otherwise

   float attenuation = 1.0 / ((((length(ltop)/(1.0 -((length(ltop)/radius)*(length(ltop)/radius))))/radius)+1.0)*(((length(ltop)/(1.0-((length(ltop)/radius)*(length(ltop)/radius))))/radius)+1.0));
   //float attenuation = 1;

   vec4 diffuse = c * diffuseModifier * vec4(diffuseLightColor, 1.0) * attenuation * noZTestFix;
	
   //Set the color
   gl_FragColor = diffuse;// + (c*vec4(0.2, 0.2, 0.2, 1.0));
}

//attenuation from : http://imdoingitwrong.wordpress.com/2011/02/10/improved-light-attenuation/