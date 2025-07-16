#include "common.h"
#include "os.h"
#include "gbi.h"
#include "mbi.h"

extern Vp D_800D9A60[];

Gfx dl_0[] = {
	gsSPViewport(D_800D9A60),
	gsSPClearGeometryMode(G_SHADE | G_SHADING_SMOOTH | G_CULL_BOTH | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR | G_LOD),
	gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_OFF),
	gsSPSetGeometryMode(G_SHADE | G_SHADING_SMOOTH),
	gsSPEndDisplayList(),
};

Gfx dl_1[] = {
	gsDPSetCycleType(G_CYC_1CYCLE),
	gsDPPipelineMode(G_PM_1PRIMITIVE),
	gsDPSetScissor(G_SC_NON_INTERLACE, 0, 0, 320, 240),
	gsDPSetTextureLOD(G_TL_TILE),
	gsDPSetTextureLUT(G_TT_NONE),
	gsDPSetTextureDetail(G_TD_CLAMP),
	gsDPSetTexturePersp(G_TP_NONE),
	gsDPSetTextureFilter(G_TF_BILERP),
	gsDPSetTextureConvert(G_TC_FILT),
	gsDPSetCombineMode(G_CC_DECALRGB, G_CC_DECALRGB),
	gsDPSetCombineKey(G_CK_NONE),
	gsDPSetAlphaCompare(G_AC_NONE),
	gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
	gsDPSetColorDither(G_CD_DISABLE),
	gsDPPipeSync(),
	gsSPEndDisplayList(),
};

Gfx dl_2[] = {
	gsDPPipeSync(),
	gsSPClearGeometryMode(G_SHADE | G_SHADING_SMOOTH | G_CULL_BOTH | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR | G_LOD),
	gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
	gsSPSetGeometryMode(G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH | G_CULL_BACK),
	gsDPSetCycleType(G_CYC_1CYCLE),
	gsDPSetCombineMode(G_CC_MODULATEI, G_CC_MODULATEI),
	gsDPSetTextureLOD(G_TL_TILE),
	gsDPSetTextureLUT(G_TT_RGBA16),
	gsDPSetTextureDetail(G_TD_CLAMP),
	gsDPSetTexturePersp(G_TP_PERSP),
	gsDPSetTextureFilter(G_TF_BILERP),
	gsDPSetTextureConvert(G_TC_FILT),
	gsDPSetCombineKey(G_CK_NONE),
	gsDPSetAlphaCompare(G_AC_NONE),
	gsDPSetRenderMode(G_RM_RA_ZB_OPA_SURF, G_RM_RA_ZB_OPA_SURF2),
	gsDPNoOp(),
	gsDPSetColorDither(G_CD_BAYER),
	gsDPSetPrimColor(0, 0, 39, 151, 87, 119),
	gsDPSetEnvColor(151, 87, 39, 68),
	gsDPSetBlendColor(87, 39, 151, 0),
	gsSPFogFactor(0x3200, 0xCF00),
	gsDPSetFogColor(228, 222, 246, 0),
	gsSPClipRatio(FRUSTRATIO_1),
	gsDPPipeSync(),
	gsSPEndDisplayList(),
};
