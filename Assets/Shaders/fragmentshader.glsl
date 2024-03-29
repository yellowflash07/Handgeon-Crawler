// Fragment shader
#version 420

in vec4 colour;
in vec4 vertexWorldPos;			// vertex in "world space"
in vec4 vertexWorldNormal;	
in vec2 texCoord;
in vec4 boneId;
in vec4 boneWeight;
out vec4 outputColour;		// To the frame buffer (aka screen)

//uniform vec3 directionalLightColour;
// rgb are the rgb of the light colour
//uniform vec4 directionalLight_Direction_power;
// xyz is the normalized direction, w = power (between 0 and 1)

// If true, then passes the colour without calculating lighting
uniform bool bDoNotLight;		// Really a float (0.0 or not zero)
uniform bool hasTexture;
uniform bool hasMask;
uniform bool bIsSkyBox;
uniform bool hasVertexColor;
uniform vec4 color;
uniform vec4 eyeLocation;

uniform bool bUseDebugColour;	// if this is true, then use debugColourRGBA for the colour
uniform vec4 debugColourRGBA;	
uniform float transparency;
uniform sampler2D texture_00;
uniform sampler2D texture_01;
uniform sampler2D texture_02;
uniform sampler2D texture_03;
uniform sampler2D maskTexture;

uniform bool hasRenderTexture;
uniform sampler2D renderTexture;

uniform samplerCube skyBoxCubeMap;
uniform vec2 UV_Offset;
uniform vec4 textureMixRatio_0_3;

struct sLight
{
	vec4 position;			
	vec4 diffuse;	// Colour of the light (used for diffuse)
	vec4 specular;	// rgb = highlight colour, w = power
	vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
	vec4 direction;	// Spot, directional lights
	vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
	                // 0 = pointlight
					// 1 = spot light
					// 2 = directional light
	vec4 param2;	// x = 0 for off, 1 for on
};

const int POINT_LIGHT_TYPE = 0;
const int SPOT_LIGHT_TYPE = 1;
const int DIRECTIONAL_LIGHT_TYPE = 2;

const int NUMBEROFLIGHTS = 50;
uniform sLight theLights[NUMBEROFLIGHTS];  	// 70 uniforms
//... is really:
//uniform vec4 theLights[0].position;
//uniform vec4 theLights[1].position;
//uniform vec4 theLights[2].position;
// etc...

uniform bool useBones;
vec4 calculateLightContrib( vec3 vertexMaterialColour, vec3 vertexNormal, 
                            vec3 vertexWorldPos, vec4 vertexSpecular );
float rand(vec2 co);

void main()
{
//	gl_FragColor = vec4(color, 1.0);

	if (useBones)
	{
		vec4 finalColor = vec4(0.0, 0.0, 0.0, 1.0);
		finalColor += boneWeight[0] *  vec4(1.0, 0.0, 0.0, 1.0); // Red for bone 1
		finalColor += boneWeight[1] *  vec4(0.0, 1.0, 0.0, 1.0); // Green for bone 2
		finalColor += boneWeight[2] *  vec4(0.0, 0.0, 1.0, 1.0); // Blue for bone 3
		finalColor += boneWeight[3] *  vec4(1.0, 1.0, 0.0, 1.0); // Yellow for bone 4
		//vec4 weightColor = vec4(boneWeight.x, boneWeight.y, boneWeight.z, 1.0f);
		outputColour = finalColor;
		return;
	}

	if ( bIsSkyBox )
	{
		vec4 skyBoxSampleColour = texture( skyBoxCubeMap, vertexWorldNormal.xyz ).rgba;
		outputColour.rgb = skyBoxSampleColour.rgb;
		outputColour.a = 1.0f;
		return;
	}
	
	if( hasRenderTexture )
	{
		vec4 renderTextureColor =  texture( renderTexture, texCoord.st ).rgba;
		outputColour.rgb = renderTextureColor.rgb;
		outputColour.a = 1.0f;
		//chromatic aberration
//		vec2 color_Offset = vec2(0.01f, 0.01f);
//		vec2 texCoordRed = texCoord.st + color_Offset;
//		vec2 texCoordBlue = texCoord.st - color_Offset;
//		vec4 red = texture( renderTexture, texCoordRed ).rgba;
//		vec4 blue = texture( renderTexture, texCoordBlue ).rgba;
//		outputColour.r = red.r;
//		outputColour.g = renderTextureColor.g;
//		outputColour.b = blue.b;

		//Grain
//		float grain = rand(texCoord.st);
//		outputColour.rgb = mix(renderTextureColor.rgb, vec3(grain), 0.5f);
//		outputColour.a = 1.0f;

		//bloom
		 
		return;
	}


	vec4 vertexRGBA = colour;
	if(hasVertexColor)
	{
		//vertexRGBA = colour;
	}		

	if(hasTexture)
	{

		vec4 texColour = texture( texture_00, texCoord.st + UV_Offset.xy).rgba * textureMixRatio_0_3.x 	
						+ texture( texture_01, texCoord.st ).rgba * textureMixRatio_0_3.y
						+ texture( texture_02, texCoord.st ).rgba * textureMixRatio_0_3.z
						+ texture( texture_03, texCoord.st ).rgba * textureMixRatio_0_3.w;
		vertexRGBA = texColour;
	}

	if(hasMask)
	{
		float maskValue = texture( maskTexture, texCoord.st ).r;
		if(maskValue < 0.1f)
		{
			discard;
		}
	}

	if ( bUseDebugColour )
	{	
		vertexRGBA = debugColourRGBA;
	}
	
	if ( bDoNotLight )
	{
		outputColour = vertexRGBA;
		return;
	}
	
	// *************************************
	// Hard code the specular (for now)
	vec4 vertexSpecular = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	// xyzw or rgba or stuw
	// RGB is the specular highglight colour (usually white or the colour of the light)
	// 4th value is the specular POWER (STARTS at 1, and goes to 1000000s)
	
	vec4 vertexColourLit = calculateLightContrib( vertexRGBA.rgb, vertexWorldNormal.xyz, 
	                                              vertexWorldPos.xyz, vertexSpecular );
	// *************************************
			
	outputColour.rgb = vertexColourLit.rgb;
	
	// Real gamma correction is a curve, but we'll Rock-n-Roll it here
	outputColour.rgb *= 1.35f;
	
	outputColour.a = transparency;
}


vec4 calculateLightContrib( vec3 vertexMaterialColour, vec3 vertexNormal, 
                            vec3 vertexWorldPos, vec4 vertexSpecular )
{
	vec3 norm = normalize(vertexNormal);
	
	vec4 finalObjectColour = vec4( 0.0f, 0.0f, 0.0f, 1.0f );
	
	for ( int index = 0; index < NUMBEROFLIGHTS; index++ )
	{	
		// ********************************************************
		// is light "on"
		if ( theLights[index].param2.x == 0.0f )
		{	// it's off
			continue;
		}
		
		// Cast to an int (note with c'tor)
		int intLightType = int(theLights[index].param1.x);
		
		// We will do the directional light here... 
		// (BEFORE the attenuation, since sunlight has no attenuation, really)
		if ( intLightType == DIRECTIONAL_LIGHT_TYPE )		// = 2
		{
			// This is supposed to simulate sunlight. 
			// SO: 
			// -- There's ONLY direction, no position
			// -- Almost always, there's only 1 of these in a scene
			// Cheapest light to calculate. 

			vec3 lightContrib = theLights[index].diffuse.rgb;
			
			// Get the dot product of the light and normalize
			float dotProduct = dot( -theLights[index].direction.xyz,  
									   normalize(norm.xyz) );	// -1 to 1

			dotProduct = max( 0.0f, dotProduct );		// 0 to 1
		
			lightContrib *= dotProduct;		
			
			finalObjectColour.rgb += (vertexMaterialColour.rgb * theLights[index].diffuse.rgb * lightContrib); 
									 //+ (materialSpecular.rgb * lightSpecularContrib.rgb);
			// NOTE: There isn't any attenuation, like with sunlight.
			// (This is part of the reason directional lights are fast to calculate)


			return finalObjectColour;		
		}
		
		// Assume it's a point light 
		// intLightType = 0
		
		// Contribution for this light
		vec3 vLightToVertex = theLights[index].position.xyz - vertexWorldPos.xyz;
		float distanceToLight = length(vLightToVertex);	
		vec3 lightVector = normalize(vLightToVertex);
		float dotProduct = dot(lightVector, vertexNormal.xyz);	 
		
		dotProduct = max( 0.0f, dotProduct );	
		
		vec3 lightDiffuseContrib = dotProduct * theLights[index].diffuse.rgb;
			

		// Specular 
		vec3 lightSpecularContrib = vec3(0.0f);
			
		vec3 reflectVector = reflect( -lightVector, normalize(norm.xyz) );

		// Get eye or view vector
		// The location of the vertex in the world to your eye
		vec3 eyeVector = normalize(eyeLocation.xyz - vertexWorldPos.xyz);

		// To simplify, we are NOT using the light specular value, just the object�s.
		float objectSpecularPower = vertexSpecular.w; 
		
//		lightSpecularContrib = pow( max(0.0f, dot( eyeVector, reflectVector) ), objectSpecularPower )
//			                   * vertexSpecular.rgb;	//* theLights[lightIndex].Specular.rgb
		lightSpecularContrib = pow( max(0.0f, dot( eyeVector, reflectVector) ), objectSpecularPower )
			                   * theLights[index].specular.rgb;
							   
		// Attenuation
		float attenuation = 1.0f / 
				( theLights[index].atten.x + 										
				  theLights[index].atten.y * distanceToLight +						
				  theLights[index].atten.z * distanceToLight*distanceToLight );  	
				  
		// total light contribution is Diffuse + Specular
		lightDiffuseContrib *= attenuation;
		lightSpecularContrib *= attenuation;
		
		
		// But is it a spot light
		if ( intLightType == SPOT_LIGHT_TYPE )		// = 1
		{	
		

			// Yes, it's a spotlight
			// Calcualate light vector (light to vertex, in world)
			vec3 vertexToLight = vertexWorldPos.xyz - theLights[index].position.xyz;

			vertexToLight = normalize(vertexToLight);

			float currentLightRayAngle
					= dot( vertexToLight.xyz, theLights[index].direction.xyz );
					
			currentLightRayAngle = max(0.0f, currentLightRayAngle);

			//vec4 param1;	
			// x = lightType, y = inner angle, z = outer angle, w = TBD

			// Is this inside the cone? 
			float outerConeAngleCos = cos(radians(theLights[index].param1.z));
			float innerConeAngleCos = cos(radians(theLights[index].param1.y));
							
			// Is it completely outside of the spot?
			if ( currentLightRayAngle < outerConeAngleCos )
			{
				// Nope. so it's in the dark
				lightDiffuseContrib = vec3(0.0f, 0.0f, 0.0f);
				lightSpecularContrib = vec3(0.0f, 0.0f, 0.0f);
			}
			else if ( currentLightRayAngle < innerConeAngleCos )
			{
				// Angle is between the inner and outer cone
				// (this is called the penumbra of the spot light, by the way)
				// 
				// This blends the brightness from full brightness, near the inner cone
				//	to black, near the outter cone
				float penumbraRatio = (currentLightRayAngle - outerConeAngleCos) / 
									  (innerConeAngleCos - outerConeAngleCos);
									  
				lightDiffuseContrib *= penumbraRatio;
				lightSpecularContrib *= penumbraRatio;
			}
						
		}// if ( intLightType == 1 )
		
		
					
		finalObjectColour.rgb += (vertexMaterialColour.rgb * lightDiffuseContrib.rgb)
								  + (vertexSpecular.rgb  * lightSpecularContrib.rgb );

	}
	
	finalObjectColour.a = 1.0f;
	
	return finalObjectColour;
}

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}