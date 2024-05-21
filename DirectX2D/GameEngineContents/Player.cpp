#include "PreCompile.h"
#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Start()
{
	GameEngineInput::AddInputObject(this);

	{
		GameEngineDirectory Dir;
		Dir.MoveParentToExistsChild("GameEngineResources");
		Dir.MoveChild("ContentsResources");
		Dir.MoveChild("Texture");
		Dir.MoveChild("Player");
		std::vector<GameEngineDirectory> Directorys = Dir.GetAllDirectory();

		for (size_t i = 0; i < Directorys.size(); i++)
		{
			GameEngineDirectory& Dir = Directorys[i];

			GameEngineSprite::CreateFolder(Dir.GetStringPath());
		}
	}

	{
		PlayerSpriteRenderer = CreateComponent<GameEngineSpriteRenderer>(ContentsRenderType::Play);

		PlayerSpriteRenderer->CreateAnimation("Idle", "spr_dragon_idle");

		PlayerSpriteRenderer->ChangeAnimation("Idle");

		PlayerSpriteRenderer->AutoSpriteSizeOn();
	}
}

void Player::Update(float _Delta)
{
	float4 CurPlayerPos = Transform.GetWorldPosition();

	if (GameEngineInput::IsPress('W', this) == true)
	{
		CurPlayerPos.Y += 100.f * _Delta;
	}

	if (GameEngineInput::IsPress('S', this) == true)
	{
		CurPlayerPos.Y -= 100.f * _Delta;
	}

	if (GameEngineInput::IsPress('A', this) == true)
	{
		CurPlayerPos.X -= 100.f * _Delta;
	}

	if (GameEngineInput::IsPress('D', this) == true)
	{
		CurPlayerPos.X += 100.f * _Delta;
	}

	Transform.SetWorldPosition(CurPlayerPos);
}

void Player::LevelStart(GameEngineLevel* _PrevLevel)
{
}

void Player::LevelEnd(GameEngineLevel* _NextLevel)
{
}
