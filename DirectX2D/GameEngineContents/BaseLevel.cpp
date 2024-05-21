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

	//���� ī�޶� �̵��� �� �ʿ䰡 ������� �̰ɷ� ������Ʈ
	float4 CameraNextPos;

	//�÷��̾��� ��ǥ�� �߽�����, ī�޶� õõ�� �ε巴�� �̵���Ŵ.
	if (MainPlayer != nullptr)
	{
		float4 PlayerPos = MainPlayer->Transform.GetWorldPosition();
		CameraNextPos = float4::LerpClamp(CameraCurrentPos, PlayerPos, _Delta);
	}

	//ī�޶��� ��������
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
