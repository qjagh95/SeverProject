#pragma once

#define JEONG_BEGIN namespace JEONG {
#define JEONG_END }

JEONG_BEGIN

enum GAME_MODE
{
	GM_2D,
	GM_3D,
};

enum COMPONENT_TYPE
{
	CT_NONE,
	CT_TRANSFORM,
	CT_RENDER,
	CT_MATERIAL,
	CT_ANIMATION2D,
	CT_CAMERA,
	CT_FREECAMERA,
	CT_UI,
	CT_COLLIDER,
	CT_TITLEBAR,
	CT_TEXT,
	CT_TILE2D,
	CT_TILEIMAGE,
	CT_STAGE2D,
	CT_BACKCOLOR,
	CT_GARVITY,
	CT_NUMBER,
	CT_PLAYER,
	CT_EATTING,
	CT_OTHER_PLAYER,
	CT_MAX,
};

//각 축의 방향
enum AXIS
{
	AXIS_X,
	AXIS_Y,
	AXIS_Z,
	AXIS_MAX,
};

enum CBUFFER_SHADER_TYPE
{
	CST_VERTEX = 0x1,
	CST_PIXEL = 0x2,
};

enum CAMERA_TYPE
{
	CT_PERSPECTIVE,
	CT_ORTHO,
};

enum RENDER_STATE
{
	RS_BLEND,
	RS_RASTERIZER,
	RS_DEPTHSTENCIL,
	RS_END
};

enum MOVE_DIR
{
	MD_RIGHT = 1,
	MD_LEFT = -1,
	MD_UP = 1,
	MD_DOWN = -1
};

enum ANIMATION2D_TYPE
{
	A2D_ATLS,
	A2D_FRAME,
};

enum ANIMATION_OPTION
{
	AO_LOOP,
	AO_ONCE_DESTROY,
};

enum COLLIDER_TYPE
{
	CT_RECT,
	CT_POINT,
	CT_CIRCLE,
	CT_OBB2D,
	CT_PIXEL,
};

enum COLLSION_CALLBACK_TYPE
{
	CCT_FIRST,
	CCT_DOING,
	CCT_END,
	CCT_MAX,
};

enum UI_TYPE
{
	UT_PANNAL,
	UT_BUTTON,
	UT_CHECKBOX,
	UT_RADIOBUTTON,
	UT_BAR,
	UT_ICON,
	UT_ICONSLOT,
};

enum BUTTON_STATE
{
	BS_DISABLE,
	BS_NORMAL,
	BS_MOUSEOVER,
	BS_CLICK,
	BS_MAX,
};

enum CHECKBOX_STATE
{
	CBS_NORMAL,
	CBS_TRUE,
	CBS_OVER,
	CBS_FALSE,
	CBS_MAX,
};

enum BAR_DIR
{
	BD_LEFT,
	BD_RIGHT,
	BD_UP,
	BD_DOWN,
};

enum RADIO_DIR
{
	RD_DOWN,
	RD_RIGHT,
};

enum TRANSFORM_PARENT_FLAG
{
	TPF_SCALE = 0x1,
	TPF_ROT = 0x2,
	TPF_POS = 0x4
};

enum STAGE2D_TILE_TYPE
{
	STT_NONE,
	STT_TILE,
	STT_ISO,
};

enum RENDER_GROUP
{
	RG_LANDSCAPE,
	RG_NORMAL,
	RG_ALPHA1,
	RG_ALPHA2,
	RG_ALPHA3,
	RG_UI,
	RG_HUD,
	RG_END
};

enum TILE2D_OPTION
{
	T2D_NORMAL,
	T2D_NOMOVE,
};

enum SEVER_DATA_TYPE
{
	SST_NEW_CLIENT,
	SST_CREATE_EAT_OBJECT,
	SST_CREATE_PLAYER,
	SST_CREATE_OTHER_PLAYER,
	SST_PLAYER_DATA,
	SST_DELETE_EAT_OBJECT,
	SST_NONE,
};

enum PROJECT_TYPE
{
	MY_IS_SEVER,
	MY_IS_CLIENT,
};

JEONG_END