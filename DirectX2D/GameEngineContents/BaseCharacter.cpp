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

	GameEngineColor Color = CheckGroundPixel();
	GameEngineColor LeftColor, RightColor;

	LeftColor = CheckGroundPixel({ 15.f, -5.f });
	RightColor = CheckGroundPixel({ -15.f, -5.f });

	//땅을 밟고있는 상태
	if ((Color == GameEngineColor::RED || Color == GameEngineColor::BLUE) && IsPassingGround == false)
	{
		GravityVector = float4::ZERO;

		if (IsJumping)
		{
			IsJumping = false;
		}


		//비탈길인 경우(올라갈때)
		if ((LeftColor == GameEngineColor::RED) || (RightColor == GameEngineColor::RED))
		{
			Transform.AddWorldPosition(float4::UP * _Delta * 300.f);
		}
	}
	else
	{
		GravityVector += {float4::DOWN* GravityPower* _Delta};

		GameEngineColor HoverColor = CheckGroundPixel({ 0.f, 40.f });

		//공중에 떠있는 상태
		if (HoverColor != GameEngineColor::WHITE)
		{
			//비탈길인 경우(내려갈때)
			if ((LeftColor == GameEngineColor::RED) || (RightColor == GameEngineColor::RED))
			{
				Transform.AddWorldPosition(float4::DOWN * _Delta * 300.f);
			}
		}
	}

	Transform.AddWorldPosition(GravityVector * _Delta);
}

void BaseCharacter::ResetGravity()
{
	GravityVector = float4::ZERO;
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

	GameEngineTransform Trans;
	Trans.SetWorldPosition({ Transform.GetWorldPosition().X - _Offset.X, Transform.GetWorldPosition().Y - 40.0f - _Offset.Y });

	GameEngineDebug::DrawBox2D(Trans, TempColor);

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
