#version 410

uniform mat4 perspectiveMatrix;
uniform mat4 modelToWorld;
uniform mat4 zRotationMatrix;


uniform mat4 R;
uniform mat4 T;

layout(location = 0) in vec3 mesh;
layout(location = 1) in vec3 normalpos;
layout(location = 2) in vec2 vt;

//pass on the texture coordinate to the fragment shader
out vec2 texture_coordinates;
void main()
{
	mat4 view =  R*T;
	vec4 modelSpaceVec, worldSpaceVec, cameraSpaceVec, clipSpaceVec;

	modelSpaceVec = vec4( mesh, 1.0 );
	worldSpaceVec =  modelToWorld * modelSpaceVec;
	cameraSpaceVec = view * worldSpaceVec;
	clipSpaceVec = perspectiveMatrix * cameraSpaceVec;

	gl_Position = clipSpaceVec;
	texture_coordinates = vt;
}