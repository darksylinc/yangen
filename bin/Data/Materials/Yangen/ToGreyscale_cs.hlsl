@insertpiece( SetCrossPlatformSettings )
@insertpiece( DeclUavCrossPlatform )

@insertpiece( PreBindingsHeaderCS )

RWTexture2DArray<@insertpiece(uav0_pf_type)> outGreyscaleMap : register(u0);

Texture2DArray colourMapTex : register(t0);

uniform uint4 resolutionAndIndex;

#define p_diffuseMapResolution	resolutionAndIndex.xy
#define p_diffuseMapIndex		resolutionAndIndex.z

@insertpiece( HeaderCS )

//in uvec3 gl_NumWorkGroups;
//in uvec3 gl_WorkGroupID;
//in uvec3 gl_LocalInvocationID;
//in uvec3 gl_GlobalInvocationID;
//in uint  gl_LocalInvocationIndex;

[numthreads(@value( threads_per_group_x ), @value( threads_per_group_y ), @value( threads_per_group_z ))]
void main
(
	uint3 gl_GlobalInvocationID		: SV_DispatchThreadId
)
{
	@insertpiece( BodyCS )
}
