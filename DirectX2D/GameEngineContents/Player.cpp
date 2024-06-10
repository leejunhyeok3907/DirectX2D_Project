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
		PlayerSpriteRenderer->CreateAnimation("Fall", "spr_dragon_fall");
		PlayerSpriteRenderer->CreateAnimation("Attack", "spr_dragon_attack");

		PlayerSpriteRenderer->CreateAnimation("IdleToRun", "spr_dragon_idle_to_run", 0.1f, 0, 2, false);
		PlayerSpriteRenderer->CreateAnimation("RunToIdle", "spr_dragon_run_to_idle", 0.1f, 0, 3, false);
		PlayerSpriteRenderer->CreateAnimation("DoorKick", "spr_dragon_doorkick", 0.1f, 0, 5, false);
		PlayerSpriteRenderer->CreateAnimation("PreCrouch", "spr_dragon_precrouch", 0.1f, 0, 1, false);
		PlayerSpriteRenderer->CreateAnimation("PostCrouch", "spr_dragon_postcrouch", 0.1f, 0, 1, false);
		PlayerSpriteRenderer->CreateAnimation("Roll", "spr_dragon_roll", 0.1f, 0, 6, false);

		PlayerSpriteRenderer->AutoSpriteSizeOn();
	}

	//Initalize FSM
	{
		FSM_Idle();
		FSM_Run();
		FSM_Jump();
		FSM_Fall();
		FSM_RunToIdle();
		FSM_IdleToRun();
		FSM_Roll();
		FSM_PreCrouch();
		FSM_PostCrouch();
		FSM_Death();
		FSM_Attack();

		FSM_PlayerState.ChangeState(FSM_State::Idle);
	}
}

void Player::Update(float _Delta)
{
	BaseCharacter::Update(_Delta);

	float4 CurPlayerPos = Transform.GetWorldPosition();

	Transform.SetWorldPosition(CurPlayerPos);

	FSM_PlayerState.Update(_Delta);
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
			float4 MovePos = float4::ZERO;

			DirectionUpdate();
			SetGravity(true);

			if (GameEngineInput::IsDown('W', this))
			{
				FSM_PlayerState.ChangeState(FSM_State::Jump);
				return;
			}

			if (GameEngineInput::IsDown('S', this))
			{
				GameEngineColor GroundColor = CheckGroundPixel();

				if (GroundColor==GameEngineColor::BLUE)
				{
					FSM_PlayerState.ChangeState(FSM_State::Fall);
					return;
				}
				else if (GroundColor == GameEngineColor::RED)
				{
					FSM_PlayerState.ChangeState(FSM_State::PreCrouch);
					return;
				}
			}

			if (GameEngineInput::IsPress('A', this) || GameEngineInput::IsPress('D', this))
			{
				FSM_PlayerState.ChangeState(FSM_State::IdleToRun);
				return;
			}
	};

	FSM_PlayerState.CreateState(FSM_State::Idle, Idle_Param);
}

void Player::FSM_Run()
{
	CreateStateParameter Run_Param;

	Run_Param.Start = [=](class GameEngineState* _Parent)
		{
			PlayerSpriteRenderer->ChangeAnimation("Run");
		};

	Run_Param.Stay = [=](float _Delta, class GameEngineState* _Parent)
		{
			float4 MovePos = float4::ZERO;

			DirectionUpdate();
			SetGravity(true);

			if (GameEngineInput::IsPress('W', this))
			{
				FSM_PlayerState.ChangeState(FSM_State::Jump);
				return;
			}

			if (GameEngineInput::IsPress('A', this))
			{

				MovePos = { float4::LEFT * MoveSpeed * _Delta };
			}

			if (GameEngineInput::IsPress('D', this))
			{

				MovePos = { float4::RIGHT * MoveSpeed * _Delta };
			}

			if (GameEngineInput::IsDown('S', this))
			{
				GameEngineColor GroundColor = CheckGroundPixel();
				MovePos = { 0.f, -MoveSpeed * _Delta };

				if (GroundColor == GameEngineColor::BLUE)
				{
					Transform.AddLocalPosition(MovePos);
					FSM_PlayerState.ChangeState(FSM_State::Fall);
					return;
				}
				else if(GroundColor == GameEngineColor::RED)
				{
					FSM_PlayerState.ChangeState(FSM_State::PreCrouch);
					return;
				}
			}

			if (CheckGroundPixel() == GameEngineColor::WHITE)
			{
				FSM_PlayerState.ChangeState(FSM_State::Fall);
			}

			//키를 누르지 않아 값이 바뀌지않았다면 입력이 없던것
			if (MovePos == float4::ZERO)
			{
				FSM_PlayerState.ChangeState(FSM_State::RunToIdle);
				return;
			}
			else
			{
				Transform.AddLocalPosition(MovePos);
			}
		};

	FSM_PlayerState.CreateState(FSM_State::Run, Run_Param);
}

void Player::FSM_Jump()
{
	CreateStateParameter Jump_Param;

	Jump_Param.Start = [=](class GameEngineState* _Parent)
		{
			PlayerSpriteRenderer->ChangeAnimation("Jump");

			//std::shared_ptr<Fx> NewFx = GetLevel()->CreateActor<Fx>();
			//NewFx->SetFxData(EFx_Type::JumpCloud, float4::ZERO);
			//NewFx->Transform.SetLocalPosition({ Transform.GetLocalPosition().X, Transform.GetLocalPosition().Y + 10.0f });
		};

	Jump_Param.Stay = [=](float _Delta, class GameEngineState* _Parent)
		{
			float4 MovePos = float4::ZERO;

			DirectionUpdate();
			SetGravity(true);

			MovePos = { 0.f, float4::UP.Y * 3000.f * _Delta };

			if (GameEngineInput::IsDown('A', this))
			{
				MovePos = { (float4::LEFT + float4::UP) * 300.f * _Delta };
			}

			if (GameEngineInput::IsDown('D', this))
			{
				MovePos = { (float4::RIGHT + float4::UP) * 300.f * _Delta };
			}

			GameEngineColor Color = CheckGroundPixel();

			if (Color == GameEngineColor::WHITE || Color == GameEngineColor::BLUE)
			{
				Transform.AddLocalPosition(MovePos);
			}
			else
			{
				FSM_PlayerState.ChangeState(FSM_State::Fall);
				return;
			}

			if (GravityVector.Y <= -200.0f);
			{
				FSM_PlayerState.ChangeState(FSM_State::Fall);
				return;
			}
		};

	FSM_PlayerState.CreateState(FSM_State::Jump, Jump_Param);
}

void Player::FSM_Fall()
{
	CreateStateParameter Fall_Param;

	Fall_Param.Start = [=](class GameEngineState* _Parent)
		{
			PlayerSpriteRenderer->ChangeAnimation("Fall");

			LastGroundPixelColor = CheckGroundPixel();

			if (LastGroundPixelColor == GameEngineColor::BLUE)
			{
				IsPassingGround = true;
			}
		};

	Fall_Param.Stay = [=](float _Delta, class GameEngineState* _Parent)
		{
			float4 MovePos = float4::ZERO;

			DirectionUpdate();
			SetGravity(true);

			if (GameEngineInput::IsPress('A', this))
			{

				MovePos = { float4::LEFT * MoveSpeed * _Delta };
			}

			if (GameEngineInput::IsPress('D', this))
			{

				MovePos = { float4::RIGHT * MoveSpeed * _Delta };
			}

			GameEngineColor GroundColor = CheckGroundPixel();
			{
				//낙하로 파란색 벽 범위 밖으로 벗어남
				if (GroundColor != LastGroundPixelColor && LastGroundPixelColor == GameEngineColor::BLUE)
				{
					IsPassingGround = false;
				}

				if (GroundColor == GameEngineColor::RED)
				{
					if (GameEngineInput::IsPress('A', this) || GameEngineInput::IsPress('D', this))
					{
						FSM_PlayerState.ChangeState(FSM_State::Run);
					}
					else
					{
						FSM_PlayerState.ChangeState(FSM_State::Idle);
					}
					return;
				}
			}

			Transform.AddLocalPosition(MovePos);
		};

	FSM_PlayerState.CreateState(FSM_State::Fall, Fall_Param);
}

void Player::FSM_RunToIdle()
{
	CreateStateParameter RunToIdle_Param;

	RunToIdle_Param.Start = [=](class GameEngineState* _Parent)
		{
			PlayerSpriteRenderer->ChangeAnimation("RunToIdle");
		};

	RunToIdle_Param.Stay = [=](float _Delta, class GameEngineState* _Parent)
		{
			float4 MovePos = float4::ZERO;

			DirectionUpdate();
			SetGravity(true);

			//이 애니메이션이 끝나면 Idle상태로 전환
			if (PlayerSpriteRenderer->IsCurAnimationEnd() == true)
			{
				FSM_PlayerState.ChangeState(FSM_State::Idle);
				return;
			}

			//Idle 상태와 같은 행동 가능
			if (GameEngineInput::IsDown('W', this))
			{
				FSM_PlayerState.ChangeState(FSM_State::Jump);
				return;
			}

			if (GameEngineInput::IsDown('S', this))
			{
				GameEngineColor GroundColor = CheckGroundPixel();
				MovePos = { 0.f, -MoveSpeed * _Delta };

				if (GroundColor == GameEngineColor::BLUE)
				{
					Transform.AddLocalPosition(MovePos);
					FSM_PlayerState.ChangeState(FSM_State::Fall);
					return;
				}
				else if (GroundColor == GameEngineColor::RED)
				{
					FSM_PlayerState.ChangeState(FSM_State::PreCrouch);
					return;
				}
			}

			if (GameEngineInput::IsDown('A', this) || GameEngineInput::IsDown('D', this))
			{
				FSM_PlayerState.ChangeState(FSM_State::IdleToRun);
				return;
			}
		};

	FSM_PlayerState.CreateState(FSM_State::RunToIdle, RunToIdle_Param);
}

void Player::FSM_IdleToRun()
{
	CreateStateParameter IdleToRun_Param;

	IdleToRun_Param.Start = [=](class GameEngineState* _Parent)
		{
			PlayerSpriteRenderer->ChangeAnimation("RunToIdle");
		};

	IdleToRun_Param.Stay = [=](float _Delta, class GameEngineState* _Parent)
		{
			float4 MovePos = float4::ZERO;

			DirectionUpdate();
			SetGravity(true);

			//이 애니메이션이 끝나면 Run상태로 전환
			if (PlayerSpriteRenderer->IsCurAnimationEnd() == true)
			{
				FSM_PlayerState.ChangeState(FSM_State::Run);
				return;
			}

			//Run 상태와 같은 행동 가능
			if (GameEngineInput::IsPress('W', this))
			{
				FSM_PlayerState.ChangeState(FSM_State::Jump);
				return;
			}

			if (GameEngineInput::IsPress('A', this))
			{

				MovePos = { float4::LEFT * MoveSpeed * _Delta };
			}

			if (GameEngineInput::IsPress('D', this))
			{

				MovePos = { float4::RIGHT * MoveSpeed * _Delta };
			}

			if (GameEngineInput::IsDown('S', this))
			{
				GameEngineColor GroundColor = CheckGroundPixel();
				MovePos = { 0.f, -MoveSpeed * _Delta };

				if (GroundColor == GameEngineColor::BLUE)
				{
					Transform.AddLocalPosition(MovePos);
					FSM_PlayerState.ChangeState(FSM_State::Fall);
					return;
				}
				else if (GroundColor == GameEngineColor::RED)
				{
					FSM_PlayerState.ChangeState(FSM_State::PreCrouch);
					return;
				}
			}

			//키를 누르지 않아 값이 바뀌지않았다면 입력이 없던것
			if (MovePos == float4::ZERO)
			{
				FSM_PlayerState.ChangeState(FSM_State::RunToIdle);
				return;
			}
			else
			{
				Transform.AddLocalPosition(MovePos);
			}
		};

	FSM_PlayerState.CreateState(FSM_State::IdleToRun, IdleToRun_Param);
}

void Player::FSM_Roll()
{
	CreateStateParameter Roll_Param;

	Roll_Param.Start = [=](class GameEngineState* _Parent)
		{
			DirectionUpdate();

			PlayerSpriteRenderer->ChangeAnimation("Roll");

			IsRolling = true;
		};

	Roll_Param.Stay = [=](float _Delta, class GameEngineState* _Parent)
		{
			float4 MovePos = float4::ZERO;
			float4 Offset = float4::ZERO;

			DirectionUpdate();
			SetGravity(true);

			if (CurrentDir == Direction::Left)
			{
				MovePos = { float4::LEFT * (MoveSpeed * 2) * _Delta };
				Offset = LeftOffset;
			}

			if (CurrentDir == Direction::Right)
			{
				MovePos = { float4::RIGHT * (MoveSpeed * 2) * _Delta };
				Offset = RightOffset;
			}

			GameEngineColor Color = CheckPixel(Offset);

			if (Color == GameEngineColor::WHITE)
			{
				Transform.AddLocalPosition(MovePos);
			}

			if (PlayerSpriteRenderer->IsCurAnimationEnd())
			{
				if (GameEngineInput::IsPress('A', this) || GameEngineInput::IsPress('D', this))
				{
					FSM_PlayerState.ChangeState(FSM_State::Run);
				}
				else
				{
					FSM_PlayerState.ChangeState(FSM_State::Idle);
				}
				IsRolling = false;
				return;
			}
		};

	FSM_PlayerState.CreateState(FSM_State::Roll, Roll_Param);
}

void Player::FSM_PreCrouch()
{
	CreateStateParameter PreCrouch_Param;

	PreCrouch_Param.Start = [=](class GameEngineState* _Parent)
		{
			PlayerSpriteRenderer->ChangeAnimation("PreCrouch");
		};

	PreCrouch_Param.Stay = [=](float _Delta, class GameEngineState* _Parent)
		{
			float4 MovePos = float4::ZERO;

			DirectionUpdate();
			SetGravity(true);

			if (GameEngineInput::IsPress('A', this) || GameEngineInput::IsPress('D', this))
			{
				FSM_PlayerState.ChangeState(FSM_State::Roll);
				return;
			}

			if (GameEngineInput::IsFree('S', this))
			{
				FSM_PlayerState.ChangeState(FSM_State::PostCrouch);
				return;
			}
		};

	FSM_PlayerState.CreateState(FSM_State::PreCrouch, PreCrouch_Param);
}

void Player::FSM_PostCrouch()
{
	CreateStateParameter PostCrouch_Param;

	PostCrouch_Param.Start = [=](class GameEngineState* _Parent)
		{
			PlayerSpriteRenderer->ChangeAnimation("PostCrouch");
		};

	PostCrouch_Param.Stay = [=](float _Delta, class GameEngineState* _Parent)
		{
			//Idle 상태와 할수있는건 같지만 애니메이션이 끝나면 State를 Idle로 전환
			float4 MovePos = float4::ZERO;

			DirectionUpdate();
			SetGravity(true);

			if (GameEngineInput::IsDown('W', this))
			{
				FSM_PlayerState.ChangeState(FSM_State::Jump);
				return;
			}

			if (GameEngineInput::IsDown('S', this))
			{
				GameEngineColor GroundColor = CheckGroundPixel();

				if (GroundColor == GameEngineColor::BLUE)
				{
					FSM_PlayerState.ChangeState(FSM_State::Fall);
					return;
				}
				else if (GroundColor == GameEngineColor::RED)
				{
					FSM_PlayerState.ChangeState(FSM_State::PreCrouch);
					return;
				}
			}

			if (GameEngineInput::IsPress('A', this) || GameEngineInput::IsPress('D', this))
			{
				FSM_PlayerState.ChangeState(FSM_State::IdleToRun);
				return;
			}

			if (PlayerSpriteRenderer->IsCurAnimationEnd())
			{
				FSM_PlayerState.ChangeState(FSM_State::Idle);
				return;
			}
		};

	FSM_PlayerState.CreateState(FSM_State::PostCrouch, PostCrouch_Param);
}

void Player::FSM_Death()
{
}

void Player::FSM_Attack()
{
}

void Player::DirectionUpdate()
{
	// 방향을 결정하는 키들이 모두 프리라면 그상태 그대로 유지. 아래의 D가 프리일때 Left가 되는 것을 방지.
	if ((GameEngineInput::IsFree('A', this) && true == GameEngineInput::IsFree('D', this)) || CheckCanChangeDir() == false)
	{
		return;
	}

	// A가 눌렸거나 D가 프리이라면 Left로 방향전환 인데 가만히있어도 Left를 바라보는 현상이 생김.
	if (GameEngineInput::IsDown('A', this) || GameEngineInput::IsFree('D', this))
	{
		PlayerSpriteRenderer->LeftFlip();
		CurrentDir = Direction::Left;
		return;
	}

	// D가 눌렸거나 A가 프리이면 Right로 방향 전환.
	if (GameEngineInput::IsDown('D', this) || GameEngineInput::IsFree('A', this))
	{
		PlayerSpriteRenderer->RightFlip();
		CurrentDir = Direction::Right;
		return;
	}
}

bool Player::CheckCanChangeDir()
{
	bool Check = !IsRolling;

	return Check;
}
