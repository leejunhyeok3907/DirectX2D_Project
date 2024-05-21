#pragma once
#include "GameEngineInputLayOut.h"
// Input LayOut이란 점 1개의 구조에요.

// 설명 : 애가 제일 잘알고 있어야 합니다.
class GameEngineVertex
{
public:
	static GameEngineInputLayOutInfo VertexInfo;

public:
	float4 POSITION;
	float4 TEXCOORD;
	float4 COLOR;
	float4 NORMAL;
	float4 BINORMAL;
	float4 TANGENT;
	float4 WEIGHT;
	int BLENDINDICES[4];
};