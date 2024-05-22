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
		PlayerSpriteRenderer->CreateAnimation("Run", "spr_dragon_run");
		PlayerSpriteRenderer->CreateAnimation("Jump", "spr_dragon_jump");
		PlayerSpriteRenderer->CreateAnimation("Roll", "spr_dragon_roll");
		PlayerSpriteRenderer->CreateAnimation("Attack", "spr_dragon_attack");

		PlayerSpriteRenderer->CreateAnimation("IdleToRun", "spr_dragon_idle_to_run", 0.1f, 0, 2, false);
		PlayerSpriteRenderer->CreateAnimation("RunToIdle", "spr_dragon_run_to_idle", 0.1f, 0, 3, false);
		PlayerSpriteRenderer->CreateAnimation("DoorKick", "spr_dragon_doorkick", 0.1f, 0, 5, false);
		PlayerSpriteRenderer->CreateAnimation("PreCrouch", "spr_dragon_precrouch", 0.1f, 0, 1, false);
		PlayerSpriteRenderer->CreateAnimation("PostCrouch", "spr_dragon_postcrouch", 0.1f, 0, 1, false);

		PlayerSpriteRenderer->ChangeAnimation("RunToIdle");
		//FSM_PlayerState.ChangeState(FSM_State::Idle);

		PlayerSpriteRenderer->AutoSpriteSizeOn();
	}
}

void Player::Update(float _Delta)
{
	BaseCharacter::Update(_Delta);

	float4 CurPlayerPos = Transform.GetWorldPosition();

	if (GameEngineInput::IsPress('W', this) == true)
	{
		CurPlayerPos.Y += 100.f * _Delta;

		IsJumping = true;
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

void Player::FSM_Idle()
{
	CreateStateParameter Idle_Param;

	Idle_Param.Start = [=](class GameEngineState* _Parent)
	{
			PlayerSpriteRenderer->ChangeAnimation("Idle");
	};

	Idle_Param.Stay = [=](float _Delta, class GameEngineState* _Parent)
	{
			SetGravity(true);
			if (GameEngineInput::IsDown('W', this))
			{
				FSM_PlayerState.ChangeState(FSM_State::Jump);
				return;
			}

			if (GameEngineInput::IsDown('S', this))
			{
				FSM_PlayerState.ChangeState(FSM_State::Fall);
				return;
			}
	};

	FSM_PlayerState.CreateState(FSM_State::Idle, Idle_Param);
}

void Player::FSM_Run()
{
}

void Player::FSM_Jump()
{
}

void Player::FSM_Fall()
{
}

void Player::FSM_RunToIdle()
{
}

void Player::FSM_IdleToRun()
{
}

void Player::FSM_Death()
{
}
