@insertpiece( SetCrossPlatformSettings )
@insertpiece( DeclUavCrossPlatform )

@insertpiece( PreBindingsHeaderCS )

#define p_steepness steepness

@insertpiece( HeaderCS )

//in uvec3 gl_NumWorkGroups;
//in uvec3 gl_WorkGroupID;
//in uvec3 gl_LocalInvocationID;
//in uvec3 gl_GlobalInvocationID;
//in uint  gl_LocalInvocationIndex;

kernel void main_metal
(
	uint3 gl_GlobalInvocationID		[[thread_position_in_grid]]

	, texture2d_array<@insertpiece(uav0_pf_type), access::write> outNormalMap [[texture(UAV_SLOT_START+0)]]

	, texture2d<float> normalMap0		[[texture(0)]]
	, texture2d<float> normalMap1		[[texture(1)]]
	, texture2d<float> normalMap2		[[texture(2)]]

	, constant float4 &steepness		[[buffer(PARAMETER_SLOT)]]
)
{
	@insertpiece( BodyCS )
}
