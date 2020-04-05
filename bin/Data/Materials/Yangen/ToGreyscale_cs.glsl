@insertpiece( SetCrossPlatformSettings )
@insertpiece( DeclUavCrossPlatform )

@insertpiece( PreBindingsHeaderCS )

layout (@insertpiece(uav0_pf_type)) uniform restrict writeonly image2DArray outGreyscaleMap;

uniform sampler2DArray colourMapTex;

uniform uint4 resolutionAndIndex;

#define p_diffuseMapResolution	resolutionAndIndex.xy
#define p_diffuseMapIndex		resolutionAndIndex.z

layout( local_size_x = @value( threads_per_group_x ),
		local_size_y = @value( threads_per_group_y ),
		local_size_z = @value( threads_per_group_z ) ) in;

@insertpiece( HeaderCS )

//in uvec3 gl_NumWorkGroups;
//in uvec3 gl_WorkGroupID;
//in uvec3 gl_LocalInvocationID;
//in uvec3 gl_GlobalInvocationID;
//in uint  gl_LocalInvocationIndex;

void main()
{
	@insertpiece( BodyCS )
}
