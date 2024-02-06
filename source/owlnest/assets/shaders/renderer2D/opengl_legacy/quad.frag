#version 120
#define MAX_TEXTURE 16

varying vec4 v_Color;
varying vec2 v_TexCoord;
varying float v_TilingFactor;
varying float v_TexIndex;
varying float v_EntityID;

uniform sampler2D u_Texture0;
uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;
uniform sampler2D u_Texture3;
uniform sampler2D u_Texture4;
uniform sampler2D u_Texture5;
uniform sampler2D u_Texture6;
uniform sampler2D u_Texture7;
#if (MAX_TEXTURE > 8)
uniform sampler2D u_Texture8;
uniform sampler2D u_Texture9;
uniform sampler2D u_Texture10;
uniform sampler2D u_Texture11;
uniform sampler2D u_Texture12;
uniform sampler2D u_Texture13;
uniform sampler2D u_Texture14;
uniform sampler2D u_Texture15;
#endif

void main() {
    vec4 texColor = v_Color;
    vec2 texCoord = v_TexCoord * v_TilingFactor;
    int texIndex = int(v_TexIndex);

    if (texIndex ==  0){ texColor *= texture2D(u_Texture0, texCoord); } else
    if (texIndex ==  1){ texColor *= texture2D(u_Texture1, texCoord); } else
    if (texIndex ==  2){ texColor *= texture2D(u_Texture2, texCoord); } else
    if (texIndex ==  3){ texColor *= texture2D(u_Texture3, texCoord); } else
    if (texIndex ==  4){ texColor *= texture2D(u_Texture4, texCoord); } else
    if (texIndex ==  5){ texColor *= texture2D(u_Texture5, texCoord); } else
    if (texIndex ==  6){ texColor *= texture2D(u_Texture6, texCoord); } else
    if (texIndex ==  7){ texColor *= texture2D(u_Texture7, texCoord); }
    #if (MAX_TEXTURE > 8)
    else if (texIndex ==  8){ texColor *= texture2D(u_Texture8, texCoord); }
    else if (texIndex ==  9){ texColor *= texture2D(u_Texture9, texCoord); }
    else if (texIndex == 10){ texColor *= texture2D(u_Texture10, texCoord); }
    else if (texIndex == 11){ texColor *= texture2D(u_Texture11, texCoord); }
    else if (texIndex == 12){ texColor *= texture2D(u_Texture12, texCoord); }
    else if (texIndex == 13){ texColor *= texture2D(u_Texture13, texCoord); }
    else if (texIndex == 14){ texColor *= texture2D(u_Texture14, texCoord); }
    else if (texIndex == 15){ texColor *= texture2D(u_Texture15, texCoord); }
    #endif

    gl_FragData[0] = texColor;

    // no framebuffer
    // gl_FragData[1] = vec4(v_EntityID, 0, 0, 0);// placeholder for our entity ID
}
