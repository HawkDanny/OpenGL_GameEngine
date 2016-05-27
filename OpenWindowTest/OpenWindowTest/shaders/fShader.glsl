#version 430

in vec3 lightLoc;
in vec3 camLoc;
in vec3 fragLoc;
in vec3 fragNorm;
in vec2 fragUV;
uniform sampler2D myTexture;


void main()
{
	vec3 fragN = normalize(fragNorm);
	vec3 l = normalize(lightLoc - fragLoc);
	vec3 v = normalize(camLoc - fragLoc);
	vec3 h = normalize(l + v);
	vec3 n = normalize(fragN);

	float ambient = 0.5;
	float diffuse = 0.3 * max(dot(l, n), 0);
	float specular = 0.5 * pow(max(dot(h, n), 0), 16);
	float bright = ambient + diffuse + specular;

	vec4 textureColor = texture(myTexture, fragUV);
	//textureColor = vec4(0.5, 0.1, 0, 1);
	gl_FragColor = vec4(bright * textureColor.xyz, textureColor.w);
	//gl_FragColor = vec4(), 1);
}