#version 410

in vec2 texture_coordinates;
uniform sampler2D basic_texture;
uniform vec2 lightPower;
uniform vec4 lightColor;
out vec4 fragment_color;

void main()
{
   //vec4( 0.0, 1.0, 1.0, 1.0 );

   vec4 texel = texture2D( basic_texture, texture_coordinates.st );
   fragment_color = texel;
}