#version 330

out vec4 color;

in vec2 v_texCoords;

uniform sampler2D s_screenTexture;
uniform sampler2D s_logoOverlayTexture;
uniform vec4 u_logoOverlayTint;
uniform vec2 u_framebufferDimensions;
uniform vec2 u_logoOverlayTextureDimensions;

void main()
{
	color = texture(s_screenTexture, v_texCoords);
	vec2 textureRatio = u_framebufferDimensions / u_logoOverlayTextureDimensions;
	vec2 logoOverlayTexCoords = textureRatio * v_texCoords;

	logoOverlayTexCoords *= 5.0;
	logoOverlayTexCoords.y += 0.2; //Move down
	logoOverlayTexCoords.x += 0.1; //Move left

	float maskX = step(logoOverlayTexCoords.x, 1.0) * step(0.0, logoOverlayTexCoords.x);
	float maskY = step(logoOverlayTexCoords.y, 1.0) * step(0.0, logoOverlayTexCoords.y);
	vec4 logoOverlay = texture(s_logoOverlayTexture, logoOverlayTexCoords * vec2(1.0, -1.0));

	color = vec4(mix(color, u_logoOverlayTint, logoOverlay.r * maskX * maskY).rgb, 0.5);

}