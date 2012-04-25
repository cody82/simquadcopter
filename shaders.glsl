// #SHADER <frag_perpixellight>
varying vec3 N;
varying vec3 L;
void main(void)
{
 vec3 l = normalize(L);
 vec3 n = normalize(N);
 vec3 H = normalize(l + vec3(0.0,0.0,1.0));

 // compute diffuse equation
 float NdotL = dot(n,l);
 //vec4 diffuse = gl_Color * vec4(max(0.0,NdotL));
 vec4 diffuse = vec4(max(0.0,NdotL));

 vec4 ambient = gl_LightSource[0].ambient;

 float NdotH = max(0.0, dot(n,H));
 vec4 specular = vec4(0.0);
 const float specularExp = 128.0;
 if (NdotL > 0.0)
    specular = vec4(pow(NdotH, specularExp));

 gl_FragColor.rgb = (diffuse + specular + ambient).rgb;
 gl_FragColor.a = 1.0;
}


// #SHADER <vert_perpixellight> per pixel light
varying vec3 N;
varying vec3 L;
void main(void)
{
 gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
 vec3 V = (gl_ModelViewMatrix * gl_Vertex).xyz;
 L = normalize(gl_LightSource[0].position.xyz - V.xyz);
 N = normalize(gl_NormalMatrix * gl_Normal);
 gl_FrontColor = gl_Color;
 //gl_FrontColor = vec4(1.0,0.0,0.0,1.0);
}

// #SHADER <frag_perpixellight_texture>
varying vec3 N;
varying vec4 pos;
uniform sampler2D Texture;
void main(void)
{
 vec4 L = gl_LightSource[0].position - pos;

 vec3 l = normalize(vec3(L));
 vec3 n = normalize(N);
 vec3 H = normalize(l + vec3(0.0,0.0,1.0));

 // compute diffuse equation
 float NdotL = dot(n,l);
 vec4 diffuse = vec4(max(0.0,NdotL));

 vec4 ambient = gl_LightSource[0].ambient;

 float NdotH = max(0.0, dot(n,H));
 vec4 specular = vec4(0.0);
 const float specularExp = 128.0;
 if (NdotL > 0.0)
    specular = vec4(pow(NdotH, specularExp));

 vec4 tex =texture2D(Texture, gl_TexCoord[0].xy);
 gl_FragColor.rgb=tex.rgb*(diffuse + specular + ambient).rgb;
 //gl_FragColor.rgb = (diffuse + specular).rgb;
 gl_FragColor.a = 1.0;
}


// #SHADER <vert_perpixellight_texture> per pixel light
varying vec3 N;
varying vec4 pos;
void main(void)
{
 gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

 pos = gl_ModelViewMatrix * gl_Vertex;
 //L = normalize(gl_LightSource[0].position - V);
 N = normalize(gl_NormalMatrix * gl_Normal);

 gl_FrontColor = gl_Color;
 gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
 //gl_FrontColor = vec4(1.0,0.0,0.0,1.0);
}
