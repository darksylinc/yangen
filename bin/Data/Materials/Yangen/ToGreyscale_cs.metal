@insertpiece( SetCrossPlatformSettings )
@insertpiece( DeclUavCrossPlatform )

@insertpiece( PreBindingsHeaderCS )

#define p_diffuseMapResolution	resolutionAndIndex.xy
#define p_diffuseMapIndex		resolutionAndIndex.z

@insertpiece( HeaderCS )

//in uvec3 gl_NumWorkGroups;
//in uvec3 gl_WorkGroupID;
//in uvec3 gl_LocalInvocationID;
//in uvec3 gl_GlobalInvocationID;
//in uint  gl_LocalInvocationIndex;

kernel void main_metal
(
	uint3 gl_GlobalInvocationID		[[thread_position_in_grid]]

	, texture2d_array<@insertpiece(uav0_pf_type), access::write> outGreyscaleMap [[texture(UAV_SLOT_START+0)]]

	, texture2d<float> colourMapTex			[[texture(0)]]

	, constant uint4 &resolutionAndIndex	[[buffer(PARAMETER_SLOT)]]
)
{
	@insertpiece( BodyCS )
}
