#version 330 core

layout (location = 0) out vec4 AlbedoOut;
layout (location = 1) out vec4 NormalsOut;
layout (location = 2) out vec4 EmissiveOut;
layout (location = 3) out vec4 MatPropsOut;

layout (location = 4) out vec4 ObjectIDOut;

layout (location = 5) out vec4 VelocityOut;


in VS_OUT
{
	vec3 FragPositionWorldSpace;
	vec2 TexCoords;
	mat3 TBN;
	mat3 TS_TBN;
	vec3 ViewPositionTangentSpace;
	vec3 FragPositionTangentSpace;
	vec4 ObjectID;
	vec4 PreviousFragPositionClipSpace;
	vec4 CurrentFragPositionClipSpace;
} fs_in;

// Global Uniforms
uniform float uWorldTime = 1.0f;
uniform vec3 uViewPositionWorldSpace;
uniform mat4 uPreviousViewProjection;
uniform mat4 uViewProjection;

// Variable Declarations
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughMap;
uniform float emissiveIntensity;

uniform sampler2D emissiveMap;

vec3 emissiveMult;
uniform sampler2D aoMap;

// Fragment Main
void main()
{
	// Base Color
vec4 albedoMap_sampler = texture2D( albedoMap, fs_in.TexCoords );
	AlbedoOut = vec4(albedoMap_sampler.rgb, 1.0);

	// Normal
	vec4 normalMap_sampler = texture2D( normalMap, fs_in.TexCoords );
	vec3 normal = normalize( normalMap_sampler.rgb * 2.0 - 1.0 );
	normal = normalize( fs_in.TBN * normal );
	NormalsOut = vec4( normal, 1.0 );

	// Material Properties
	vec4 metallicMap_sampler = texture2D( metallicMap, fs_in.TexCoords );
	vec4 roughMap_sampler = texture2D( roughMap, fs_in.TexCoords );
	vec4 aoMap_sampler = texture2D( aoMap, fs_in.TexCoords );
	MatPropsOut = vec4( clamp( metallicMap_sampler.rgb.x, 0.0, 1.0 ), clamp( roughMap_sampler.rgb.x, 0.0, 1.0 ), clamp( aoMap_sampler.rgb.x, 0.0, 1.0 ), 1.0);

	// Emissive
	
vec4 emissiveMap_sampler = texture2D( emissiveMap, fs_in.TexCoords );
emissiveMult = emissiveIntensity * emissiveMap_sampler.rgb;
	EmissiveOut = vec4(emissiveMult, 1.0);

	ObjectIDOut = fs_in.ObjectID;
	vec2 _a = ( fs_in.CurrentFragPositionClipSpace.xy / fs_in.CurrentFragPositionClipSpace.w ) * 0.5 + 0.5;
	vec2 _b = ( fs_in.PreviousFragPositionClipSpace.xy / fs_in.PreviousFragPositionClipSpace.w ) * 0.5 + 0.5;
	vec2 _vel = vec2( _a - _b );
	VelocityOut = vec4( _vel, 0.0, 1.0 );
}