@insertpiece( SetCrossPlatformSettings )
@insertpiece( DeclUavCrossPlatform )

@insertpiece( PreBindingsHeaderCS )

@insertpiece( HeaderCS )

//in uvec3 gl_NumWorkGroups;
//in uvec3 gl_WorkGroupID;
//in uvec3 gl_LocalInvocationID;
//in uvec3 gl_GlobalInvocationID;
//in uint  gl_LocalInvocationIndex;

kernel void main_metal
(
	uint3 gl_GlobalInvocationID		[[thread_position_in_grid]]

	, texture2d_array<@insertpiece(uav0_pf_type), access::write> outRoughnessMap [[texture(UAV_SLOT_START+0)]]

	, texture2d<float> blurTex0				[[texture(0)]]
	, texture2d<float> blurTex1				[[texture(1)]]

	, constant DiffuseToRoughnessParams &p	[[buffer(CONST_SLOT_START+0)]]
)
{
	@insertpiece( BodyCS )
}
