#include "PreCompile.h"
#include "BaseLevel.h"

BaseLevel::BaseLevel()
{
}

BaseLevel::~BaseLevel()
{
}

void BaseLevel::FocusCamera(float _Delta)
{
	float4 MapScale = GameEngineTexture::Find(MapObject->GetMapName())->GetScale();
	float4 ClampScale = MapScale / 2.0f - GameEngineCore::MainWindow.GetScale().Half();

	CameraLimit_Middle = { MapScale.X / 2.0f, -MapScale.Y / 2.0f };
	CameraLimit_Max = { CameraLimit_Middle.X - ClampScale.X, CameraLimit_Middle.Y + ClampScale.Y };
	CameraLimit_Min = { CameraLimit_Middle.X + ClampScale.X, CameraLimit_Middle.Y - ClampScale.Y };

	float4 CameraCurrentPos = GetMainCamera()->Transform.GetWorldPosition();

	//만약 카메라 이동을 할 필요가 있을경우 이걸로 업데이트
	float4 CameraNextPos;

	//플레이어의 좌표를 중심으로, 카메라를 천천히 부드럽게 이동시킴.
	if (MainPlayer != nullptr)
	{
		float4 PlayerPos = MainPlayer->Transform.GetWorldPosition();
		CameraNextPos = float4::LerpClamp(CameraCurrentPos, PlayerPos, _Delta);
	}

	//카메라의 범위조절
	if (CameraNextPos.X < CameraLimit_Max.X)
	{
		CameraNextPos.X = CameraLimit_Max.X;
	}

	if (CameraNextPos.X > CameraLimit_Min.X)
	{
		CameraNextPos.X = CameraLimit_Min.X;
	}

	if (CameraNextPos.Y < CameraLimit_Max.Y)
	{
		CameraNextPos.Y = CameraLimit_Max.Y;
	}

	if (CameraNextPos.Y > CameraLimit_Min.Y)
	{
		CameraNextPos.Y = CameraLimit_Min.Y;
	}

	GetMainCamera()->Transform.SetWorldPosition(CameraNextPos);
}

void BaseLevel::Start()
{
}

void BaseLevel::Update(float _Delta)
{
}

void BaseLevel::LevelStart(GameEngineLevel* _PrevLevel)
{
}

void BaseLevel::LevelEnd(GameEngineLevel* _NextLevel)
{
}

void BaseLevel::InitCamera()
{
	float4 CameraInitPos = { 640.0f, -360.0f, -500.0f };

	GetMainCamera()->Transform.SetLocalPosition(CameraInitPos);
	GetMainCamera()->SetProjectionType(EPROJECTIONTYPE::Orthographic);
}
