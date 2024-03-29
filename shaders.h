#pragma once 

#include <osg/GL>

//
// vertex color shader
//
#if OSG_GLES3_FEATURES

const char* ColorShaderVert =
"#version 300 es\n"
"#pragma import_defines( LIGHTING )\n"
"precision mediump float;\n"
"in vec4 osg_Vertex;\n"
"in vec4 osg_Color;\n"
"out vec4 vertColor;\n"
"uniform mat4 osg_ModelViewProjectionMatrix;\n"
"\n"
"#if defined(LIGHTING)\n"
"in vec3 osg_Normal;\n"
"uniform mat4 osg_ModelViewMatrix;\n"
"uniform mat3 osg_NormalMatrix;\n"
"const vec3 lightPos = vec3(0.0, 0.0, 10.0);\n"
"const vec4 lightAmbient = vec4(0.1, 0.1, 0.1, 1.0);\n"
"const vec4 lightDiffuse = vec4(0.4, 0.4, 0.4, 1.0);\n"
"const vec4 lightSpecular = vec4(0.8, 0.8, 0.8, 1.0);\n"
"void DirectionalLight(in vec3 normal,\n"
"                      in vec3 ecPos,\n"
"                      inout vec4 ambient,\n"
"                      inout vec4 diffuse,\n"
"                      inout vec4 specular)\n"
"{\n"
"     float nDotVP;\n"
"     vec3 L = normalize(osg_ModelViewMatrix*vec4(lightPos, 0.0)).xyz;\n"
"     nDotVP = max(0.0, dot(normal, L));\n"
"\n"
"     if (nDotVP > 0.0) {\n"
"       vec3 E = normalize(-ecPos);\n"
"       vec3 R = normalize(reflect(L, normal));\n"
"       specular = pow(max(dot(R, E), 0.0), 16.0) * lightSpecular;\n"
"     }\n"
"     ambient  = lightAmbient;\n"
"     diffuse  = lightDiffuse * nDotVP;\n"
"}\n"
"#endif\n"
"\n"
"void main()\n"
"{\n"
"    gl_Position = osg_ModelViewProjectionMatrix * osg_Vertex;\n"
"#if defined(LIGHTING)\n"
"    vec3 normal = normalize(osg_NormalMatrix * osg_Normal);\n"
"    vec4 ecPos = osg_ModelViewMatrix * osg_Vertex;\n"
"    vec4 ambiCol = vec4(0.0);\n"
"    vec4 diffCol = vec4(0.0);\n"
"    vec4 specCol = vec4(0.0);\n"
"    DirectionalLight(normal, ecPos.xyz, ambiCol, diffCol, specCol);\n"
"    vertColor = osg_Color * (ambiCol + diffCol + specCol);\n"
"#else\n"
"    vertColor = osg_Color;\n"
"#endif\n"
"}\n";

const char* ColorShaderFrag =
"#version 300 es\n"
"in lowp vec4 vertColor;\n"
"out lowp vec4 fragColor;\n"
"void main()\n"
"{\n"
"    fragColor = vertColor;\n"
"}\n";

#elif OSG_GLES2_FEATURES

const char* ColorShaderVert =
"#version 100\n"
"#pragma import_defines( LIGHTING )\n"
"attribute vec4 osg_Vertex;\n"
"attribute vec4 osg_Color;\n"
"uniform mat4 osg_ModelViewProjectionMatrix;\n"
"varying vec4 vertColor;\n"
"\n"
"#if defined(LIGHTING)\n"
"attribute vec3 osg_Normal;\n"
"uniform mat4 osg_ModelViewMatrix;\n"
"uniform mat3 osg_NormalMatrix;\n"
"const vec3 lightPos = vec3(0.0, 0.0, 10.0);\n"
"const vec4 lightAmbient = vec4(0.1, 0.1, 0.1, 1.0);\n"
"const vec4 lightDiffuse = vec4(0.4, 0.4, 0.4, 1.0);\n"
"const vec4 lightSpecular =vec4(0.8, 0.8, 0.8, 1.0);\n"
"void DirectionalLight(in vec3 normal,\n"
"                      in vec3 ecPos,\n"
"                      inout vec4 ambient,\n"
"                      inout vec4 diffuse,\n"
"                      inout vec4 specular)\n"
"{\n"
"     float nDotVP;\n"
"     vec3 L = normalize(osg_ModelViewMatrix*vec4(lightPos, 0.0)).xyz;\n"
"     nDotVP = max(0.0, dot(normal, L));\n"
"\n"
"     if (nDotVP > 0.0) {\n"
"       vec3 E = normalize(-ecPos);\n"
"       vec3 R = normalize(reflect(L, normal));\n"
"       specular = pow(max(dot(R, E), 0.0), 16.0) * lightSpecular;\n"
"     }\n"
"     ambient  = lightAmbient;\n"
"     diffuse  = lightDiffuse * nDotVP;\n"
"}\n"
"#endif\n"
"\n"
"void main()\n"
"{\n"
"    gl_Position = osg_ModelViewProjectionMatrix * osg_Vertex;\n"
"#if defined(LIGHTING)\n"
"    vec3 normal = normalize(osg_NormalMatrix * osg_Normal);\n"
"    vec4 ecPos = osg_ModelViewMatrix * osg_Vertex;\n"
"    vec4 ambiCol = vec4(0.0);\n"
"    vec4 diffCol = vec4(0.0);\n"
"    vec4 specCol = vec4(0.0);\n"
"    DirectionalLight(normal, ecPos.xyz, ambiCol, diffCol, specCol);\n"
"    vertColor = osg_Color * (ambiCol + diffCol + specCol);\n"
"#else\n"
"    vertColor = osg_Color;\n"
"#endif\n"
"}\n";

const char* ColorShaderFrag =
"#version 100\n"
"varying lowp vec4 vertColor;\n"
"void main()\n"
"{\n"
"    gl_FragColor = vertColor;\n"
"}\n";

#else

const char* ColorShaderVert = NULL;
const char* ColorShaderFrag = NULL;

#endif


//
// texture shader
//
#if OSG_GLES3_FEATURES

const char* TextureShaderVert =
"#version 300 es\n"
"in vec4 osg_Vertex;\n"
"in vec4 osg_MultiTexCoord0;\n"
"out vec4 texcoord0;\n"
"uniform mat4 osg_ModelViewProjectionMatrix;\n"
"void main()\n"
"{\n"
"    gl_Position = osg_ModelViewProjectionMatrix * osg_Vertex;\n"
"    texcoord0 = osg_MultiTexCoord0;\n"
"}\n";

const char* TextureShaderFrag =
"#version 300 es\n"
"in lowp vec4 texcoord0;\n"
"uniform sampler2D texture0;\n"
"out lowp vec4 fragColor;\n"
"void main()\n"
"{\n"
"    fragColor = texture(texture0, texcoord0.xy);\n"
"}\n";

#elif OSG_GLES2_FEATURES

const char* TextureShaderVert =
"#version 100\n"
"attribute vec4 osg_Vertex;\n"
"attribute vec4 osg_MultiTexCoord0;\n"
"uniform mat4 osg_ModelViewProjectionMatrix;\n"
"varying vec4 texcoord0;\n"
"void main()\n"
"{\n"
"    gl_Position = osg_ModelViewProjectionMatrix * osg_Vertex;\n"
"    texcoord0 = osg_MultiTexCoord0;\n"
"}\n";

const char* TextureShaderFrag =
"#version 100\n"
"varying lowp vec4 texcoord0;\n"
"uniform sampler2D texture0;\n"
"void main()\n"
"{\n"
"    gl_FragColor = texture2D(texture0, texcoord0.xy);\n"
"}\n";

#else

const char* TextureShaderVert = NULL;
const char* TextureShaderFrag = NULL;

#endif
