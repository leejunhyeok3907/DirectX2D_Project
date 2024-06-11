#include "PreCompile.h"
#include "BaseCharacter.h"

BaseCharacter::BaseCharacter()
{
}

BaseCharacter::~BaseCharacter()
{
}

void BaseCharacter::Start()
{
}

void BaseCharacter::Update(float _Delta)
{
	UpdateGravity(_Delta);

	//Debug
	GameEngineColor Color = GetCollisionMapColor({ Transform.GetWorldPosition().X, Transform.GetWorldPosition().Y - 40.f });
	float4 TempColor = { static_cast<float>(Color.R), static_cast<float>(Color.G), static_cast<float>(Color.B), static_cast<float>(Color.A) };

	GameEngineDebug::DrawBox2D(Transform, TempColor);
}

void BaseCharacter::LevelStart(GameEngineLevel* _PrevLevel)
{
}

void BaseCharacter::LevelEnd(GameEngineLevel* _NextLevel)
{
}

void BaseCharacter::UpdateGravity(float _Delta)
{
	//중력 적용하지않음
	if (EffectiveGravity == false) return;

	//땅을 밟고있는 상태
	if (CheckGround() && IsPassingGround == false)
	{
		if (IsJumping == false)
		{
			GravityVector = float4::ZERO;
		}
	}
	else
	{
		GravityVector += (float4::DOWN * GravityPower * _Delta);
	}

	Transform.AddWorldPosition(GravityVector * _Delta);
}

void BaseCharacter::ResetGravity()
{
	GravityVector = float4::ZERO;
}

bool BaseCharacter::CheckGround()
{
	GameEngineColor Color = CheckGroundPixel();
	GameEngineColor LeftColor, RightColor;

	LeftColor = CheckGroundPixel({ 20.f, 0.f });
	RightColor = CheckGroundPixel({ -20.f, 0.f });

	bool Result = (Color == GameEngineColor::RED || Color == GameEngineColor::BLUE)
		&& ((LeftColor == GameEngineColor::RED || LeftColor == GameEngineColor::BLUE)
		|| (RightColor == GameEngineColor::RED || RightColor == GameEngineColor::BLUE));

	return Result;
}

void BaseCharacter::SetCollisionMapTexture(std::string_view _CollisionMapTextureName)
{
	CollisionMapTexture = GameEngineTexture::Find(_CollisionMapTextureName);

	if (CollisionMapTexture == nullptr)
	{
		MsgBoxAssert("Error : 픽셀충돌용 텍스쳐가 존재하지 않음.");
	}
}

GameEngineColor BaseCharacter::CheckGroundPixel(float4 _Offset)
{
	GameEngineColor GroundColor;

	GroundColor = GetCollisionMapColor({ Transform.GetWorldPosition().X - _Offset.X, Transform.GetWorldPosition().Y - 40.0f - _Offset.Y });

	GameEngineColor Color = GetCollisionMapColor({ Transform.GetWorldPosition().X - _Offset.X, Transform.GetWorldPosition().Y - 40.0f - _Offset.Y });
	float4 TempColor = { static_cast<float>(Color.R), static_cast<float>(Color.G), static_cast<float>(Color.B), static_cast<float>(Color.A) };

	//GameEngineTransform Trans;
	//Trans.SetWorldPosition({ Transform.GetWorldPosition().X - _Offset.X, Transform.GetWorldPosition().Y - 40.0f - _Offset.Y });

	//GameEngineDebug::DrawBox2D(Trans, TempColor);

	return GroundColor;
}

GameEngineColor BaseCharacter::CheckPixel(float4 _Offset)
{
	GameEngineColor WallColor;

	WallColor = GetCollisionMapColor({ Transform.GetWorldPosition().X - _Offset.X, Transform.GetWorldPosition().Y - _Offset.Y });

	GameEngineColor Color = GetCollisionMapColor({ Transform.GetWorldPosition().X - _Offset.X, Transform.GetWorldPosition().Y - _Offset.Y });
	float4 TempColor = { static_cast<float>(Color.R), static_cast<float>(Color.G), static_cast<float>(Color.B), static_cast<float>(Color.A) };

	GameEngineTransform Trans;
	Trans.SetWorldPosition({ Transform.GetWorldPosition().X - _Offset.X, Transform.GetWorldPosition().Y - _Offset.Y });

	GameEngineDebug::DrawBox2D(Trans, TempColor);

	return WallColor;
}

GameEngineColor BaseCharacter::GetCollisionMapColor(float4 _Pos, GameEngineColor _DefaultColor)
{
	if (CollisionMapTexture == nullptr)
	{
		MsgBoxAssert("Error : 픽셀충돌용 텍스쳐가 존재하지 않음.");
	}

	_Pos.Y *= -1;

	return CollisionMapTexture->GetColor(_Pos, _DefaultColor);
}
