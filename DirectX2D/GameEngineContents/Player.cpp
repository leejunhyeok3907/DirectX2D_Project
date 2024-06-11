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
		PlayerSpriteRenderer->CreateAnimation("WallSlide", "spr_dragon_wallslide");

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
		FSM_WallSlide();

		FSM_PlayerState.ChangeState(FSM_State::Idle);
	}
}

void Player::Update(float _Delta)
{
	BaseCharacter::Update(_Delta);

	CheckPixel(LeftOffset);
	CheckPixel(RightOffset);
	CheckPixel(UpOffset);
	CheckPixel(DownOffset);

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

				if (GroundColor == GameEngineColor::RED || GroundColor == GameEngineColor::BLUE)
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
			float4 Offset = float4::ZERO;

			DirectionUpdate();
			SetGravity(true);

			if (GameEngineInput::IsPress('W', this))
			{
				FSM_PlayerState.ChangeState(FSM_State::Jump);
				return;
			}

			if (GameEngineInput::IsPress('A', this))
			{
				Offset = LeftOffset;
				MovePos = { float4::LEFT * MoveSpeed * _Delta };
			}

			if (GameEngineInput::IsPress('D', this))
			{
				Offset = RightOffset;
				MovePos = { float4::RIGHT * MoveSpeed * _Delta };
			}

			if (GameEngineInput::IsDown('S', this))
			{
				GameEngineColor GroundColor = CheckGroundPixel();

				if (GroundColor == GameEngineColor::RED || GroundColor == GameEngineColor::BLUE)
				{
					FSM_PlayerState.ChangeState(FSM_State::PreCrouch);
					return;
				}
			}

			if (GravityVector.Y < -20.f)
			{
				FSM_PlayerState.ChangeState(FSM_State::Fall);
				return;
			}

			//키를 누르지 않아 값이 바뀌지않았다면 입력이 없던것
			if (MovePos == float4::ZERO)
			{
				FSM_PlayerState.ChangeState(FSM_State::RunToIdle);
				return;
			}
			else
			{
				GameEngineColor Color = CheckPixel(Offset);
				GameEngineColor CollsionColor = CheckPixel(DownOffset + (float4::UP * 2.f));
				GameEngineColor CollsionColor2 = CheckPixel(DownOffset + (float4::DOWN * 2.f));

				if ((CollsionColor == GameEngineColor::RED || CollsionColor == GameEngineColor::BLUE)
					&& (CollsionColor2 == GameEngineColor::RED || CollsionColor2 == GameEngineColor::BLUE))
				{
					MovePos += (float4::UP * 300.f * _Delta);
				}

				if (CheckWall(Offset) == false)
				{
					Transform.AddLocalPosition(MovePos);
				}
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

			IsJumping = true;

			//std::shared_ptr<Fx> NewFx = GetLevel()->CreateActor<Fx>();
			//NewFx->SetFxData(EFx_Type::JumpCloud, float4::ZERO);
			//NewFx->Transform.SetLocalPosition({ Transform.GetLocalPosition().X, Transform.GetLocalPosition().Y + 10.0f });
		};

	Jump_Param.Stay = [=](float _Delta, class GameEngineState* _Parent)
		{
			float4 MovePos = float4::ZERO;
			float4 UpPos = float4::ZERO;
			float4 Offset = float4::ZERO;
			float4 CheckOffset = float4::ZERO;

			DirectionUpdate();
			SetGravity(true);

			Offset = UpOffset;
			UpPos = { 0.f, float4::UP.Y * 400.f * _Delta };

			if (GameEngineInput::IsPress('A', this))
			{
				Offset = LeftOffset;
				CheckOffset = LeftOffset.X - 5.f;
				MovePos = (float4::LEFT * MoveSpeed * _Delta);
			}

			if (GameEngineInput::IsPress('D', this))
			{
				Offset = RightOffset;
				CheckOffset = LeftOffset.X + 5.f;
				MovePos = (float4::RIGHT * MoveSpeed * _Delta);
			}

			GameEngineColor Color = CheckPixel(UpOffset);

			if (Color == GameEngineColor::WHITE || Color == GameEngineColor::BLUE)
			{
				Transform.AddLocalPosition(UpPos);

				if (CheckWall(Offset) == false)
				{
					Transform.AddLocalPosition(MovePos);
				}
			}
			else if (Color == GameEngineColor::RED)
			{
				FSM_PlayerState.ChangeState(FSM_State::Fall);
				return;
			}

			if (CheckWall(CheckOffset, GameEngineColor::GREEN))
			{
				if (GameEngineInput::IsPress('A', this) || GameEngineInput::IsPress('D', this))
				{
					FSM_PlayerState.ChangeState(FSM_State::WallSlide);
					return;
				}
			}

			if (GravityVector.Y <= -50.0f)
			{
				FSM_PlayerState.ChangeState(FSM_State::Fall);
				return;
			}
		};

	Jump_Param.End = [=](class GameEngineState* _Parent)
		{
			IsJumping = false;
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
			float4 Offset = float4::ZERO;
			float4 CheckOffset = float4::ZERO;

			DirectionUpdate();
			SetGravity(true);

			if (GameEngineInput::IsPress('A', this))
			{
				Offset = LeftOffset;
				CheckOffset = LeftOffset.X - 5.f;
				MovePos = (float4::LEFT * MoveSpeed * _Delta);
			}

			if (GameEngineInput::IsPress('D', this))
			{
				Offset = RightOffset;
				CheckOffset = LeftOffset.X + 5.f;
				MovePos = (float4::RIGHT * MoveSpeed * _Delta);
			}

			GameEngineColor DownColor = CheckPixel(DownOffset);

			if(IsPassingGround)
			{
				//낙하로 파란색 벽 범위 밖으로 벗어남
				if (DownColor == GameEngineColor::WHITE)
				{
					IsPassingGround = false;
				}
			}
			else
			{
				if (CheckGround())
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

			if (CheckWall(Offset) == false)
			{
				Transform.AddLocalPosition(MovePos);
			}

			if (CheckWall(CheckOffset, GameEngineColor::GREEN))
			{
				if (GameEngineInput::IsPress('A', this) || GameEngineInput::IsPress('D', this))
				{
					FSM_PlayerState.ChangeState(FSM_State::WallSlide);
					return;
				}
			}
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

				if (GroundColor == GameEngineColor::RED || GroundColor == GameEngineColor::BLUE)
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

			if (GravityVector.Y < -20.f)
			{
				FSM_PlayerState.ChangeState(FSM_State::Fall);
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
			float4 Offset = float4::ZERO;

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
				Offset = LeftOffset;
				MovePos = { float4::LEFT * MoveSpeed * _Delta };
			}

			if (GameEngineInput::IsPress('D', this))
			{
				Offset = RightOffset;
				MovePos = { float4::RIGHT * MoveSpeed * _Delta };
			}

			if (GameEngineInput::IsDown('S', this))
			{
				GameEngineColor GroundColor = CheckGroundPixel();

				if (GroundColor == GameEngineColor::RED || GroundColor == GameEngineColor::BLUE)
				{
					FSM_PlayerState.ChangeState(FSM_State::PreCrouch);
					return;
				}
			}

			if (GravityVector.Y < -20.f)
			{
				FSM_PlayerState.ChangeState(FSM_State::Fall);
				return;
			}

			//키를 누르지 않아 값이 바뀌지않았다면 입력이 없던것
			if (MovePos == float4::ZERO)
			{
				FSM_PlayerState.ChangeState(FSM_State::RunToIdle);
				return;
			}
			else
			{
				GameEngineColor Color = CheckPixel(Offset);
				GameEngineColor CollsionColor = CheckPixel(DownOffset + (float4::UP * 2.f));
				GameEngineColor CollsionColor2 = CheckPixel(DownOffset + (float4::DOWN * 2.f));

				if ((CollsionColor == GameEngineColor::RED || CollsionColor == GameEngineColor::BLUE)
					&& (CollsionColor2 == GameEngineColor::RED || CollsionColor2 == GameEngineColor::BLUE))
				{
					MovePos += (float4::UP * 200.f * _Delta);
				}

				if (CheckWall(Offset) == false)
				{
					Transform.AddLocalPosition(MovePos);
				}
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
			GameEngineColor CollsionColor = CheckPixel(DownOffset + (float4::UP * 2.f));
			GameEngineColor CollsionColor2 = CheckPixel(DownOffset + (float4::DOWN * 2.f));

			if ((CollsionColor == GameEngineColor::RED || CollsionColor == GameEngineColor::BLUE)
				&& (CollsionColor2 == GameEngineColor::RED || CollsionColor2 == GameEngineColor::BLUE))
			{
				MovePos += (float4::UP * 400.f * _Delta);
			}

			if (CheckWall(Offset) == false)
			{
				Transform.AddLocalPosition(MovePos);
			}

			if (PlayerSpriteRenderer->IsCurAnimationEnd())
			{
				if (GravityVector.Y < -20.f)
				{
					FSM_PlayerState.ChangeState(FSM_State::Fall);
				}
				else if (GameEngineInput::IsPress('A', this) || GameEngineInput::IsPress('D', this))
				{
					FSM_PlayerState.ChangeState(FSM_State::Run);
				}
				else if (GameEngineInput::IsPress('S', this))
				{
					FSM_PlayerState.ChangeState(FSM_State::PreCrouch);
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
			float4 Offset = float4::ZERO;

			DirectionUpdate();
			SetGravity(true);

			Offset = DownOffset;

			if (GameEngineInput::IsPress('A', this) || GameEngineInput::IsPress('D', this))
			{
				FSM_PlayerState.ChangeState(FSM_State::Roll);
				return;
			}

			GameEngineColor Color = CheckPixel(Offset);

			if ((GameEngineInput::IsDown(VK_SPACE, this) && Color == GameEngineColor::BLUE) || GravityVector.Y < -20.f)
			{
				FSM_PlayerState.ChangeState(FSM_State::Fall);
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

			if (GravityVector.Y < -20.f)
			{
				FSM_PlayerState.ChangeState(FSM_State::Fall);
				return;
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

void Player::FSM_WallSlide()
{
	CreateStateParameter WallSlide_Param;

	WallSlide_Param.Start = [=](class GameEngineState* _Parent)
		{
			PlayerSpriteRenderer->ChangeAnimation("WallSlide");
		};

	WallSlide_Param.Stay = [=](float _Delta, class GameEngineState* _Parent)
		{
			float4 MovePos = float4::ZERO;
			float4 DownPos = float4::ZERO;
			float4 Offset = float4::ZERO;
			float4 CheckOffset = float4::ZERO;

			DirectionUpdate();
			SetGravity(false);

			if (GameEngineInput::IsPress('A', this))
			{
				MovePos = (float4::LEFT * MoveSpeed * _Delta);
			}

			if (GameEngineInput::IsPress('D', this))
			{
				MovePos = ( float4::RIGHT * MoveSpeed * _Delta );
			}

			if (CurrentDir == Direction::Left)
			{
				Offset = LeftOffset;
				CheckOffset = LeftOffset.X - 5.f;
			}
			else if (CurrentDir == Direction::Right)
			{
				Offset = RightOffset;
				CheckOffset = LeftOffset.X + 5.f;
			}

			if (GameEngineInput::IsDown('W', this))
			{
				IsWallJump = true;
				LastWallDir = CurrentDir;
				FSM_PlayerState.ChangeState(FSM_State::Jump);
				return;
			}

			//중력대신에 내려갈 값
			DownPos += (float4::DOWN * 30.f * _Delta);

			if (CheckWall(Offset) == false)
			{
				Transform.AddLocalPosition(MovePos);
			}

			if (CheckWall(CheckOffset, GameEngineColor::GREEN))
			{
				Transform.AddLocalPosition(DownPos);
			}
			else
			{
				FSM_PlayerState.ChangeState(FSM_State::Fall);
			}
		};

	FSM_PlayerState.CreateState(FSM_State::WallSlide, WallSlide_Param);
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

bool Player::CheckWall(float4 _DirOffset)
{
	GameEngineColor OffsetColor, OffsetDownColor, OffsetUpColor;

	//가려는 방향과 위, 아래 값이 모두 픽셀에 충돌된 상태라면 벽에 충돌된 상태
	OffsetColor = CheckPixel(_DirOffset);
	OffsetDownColor = CheckPixel(_DirOffset + DownOffset);
	OffsetUpColor = CheckPixel(_DirOffset + UpOffset);

	bool Result = ((OffsetColor == GameEngineColor::RED || OffsetColor == GameEngineColor::BLUE)
		&& (OffsetUpColor == GameEngineColor::RED || OffsetUpColor == GameEngineColor::BLUE)
		&& (OffsetDownColor == GameEngineColor::RED || OffsetDownColor == GameEngineColor::BLUE));

	return Result;
}

bool Player::CheckWall(float4 _DirOffset, GameEngineColor _Color)
{
	GameEngineColor OffsetColor, OffsetDownColor, OffsetUpColor;

	//가려는 방향과 위, 아래 값이 모두 픽셀에 충돌된 상태라면 벽에 충돌된 상태
	OffsetColor = CheckPixel(_DirOffset);
	OffsetDownColor = CheckPixel(_DirOffset + DownOffset);
	OffsetUpColor = CheckPixel(_DirOffset + UpOffset);

	bool Result = ((OffsetColor == _Color)
		&& (OffsetUpColor == _Color)
		&& (OffsetDownColor == _Color));

	return Result;
}

bool Player::CheckInStair(float4 _DirOffset)
{
	GameEngineColor DownColor, OffsetDownColor, OffsetDownHalfColor, DirOffsetColor;

	DownColor = CheckPixel(DownOffset);
	OffsetDownColor = CheckPixel(_DirOffset + DownOffset);
	OffsetDownHalfColor = CheckPixel(_DirOffset + (DownOffset * 0.5f));
	DirOffsetColor= CheckPixel(_DirOffset);

	bool Result = ((DownColor == GameEngineColor::RED || DownColor == GameEngineColor::BLUE)
		&& (OffsetDownColor == GameEngineColor::RED || OffsetDownColor == GameEngineColor::BLUE)
		&& (OffsetDownHalfColor == GameEngineColor::RED || OffsetDownHalfColor == GameEngineColor::BLUE)
		&& (DirOffsetColor == GameEngineColor::WHITE));

	return Result;
}

bool Player::CheckOutStair(float4 _DirOffset)
{
	if (IsInStair)
	{
		GameEngineColor DownColor, OffsetDownColor, DownCheckColor, OffsetDownCheckColor, OffsetDownHalfColor;
		float4 CheckOffset = float4::UP * 2.f;

		DownColor = CheckPixel(DownOffset);
		OffsetDownColor = CheckPixel(_DirOffset + DownOffset);
		DownCheckColor = CheckPixel(DownOffset + CheckOffset);
		OffsetDownCheckColor = CheckPixel(_DirOffset + DownOffset + CheckOffset);
		OffsetDownHalfColor = CheckPixel(_DirOffset + (DownOffset * 0.5f));

		//밑, 보고있는 방향은 충돌했지만 그로부터 2픽셀 위로 올린 지점과 Half지점은 충돌하지않았을 경우 경사로에서 벗어난것으로 판단
		bool Result = ((DownColor == GameEngineColor::RED || DownColor == GameEngineColor::BLUE)
			&& (OffsetDownColor == GameEngineColor::RED || OffsetDownColor == GameEngineColor::BLUE)
			&& (DownCheckColor == GameEngineColor::WHITE)
			&& (OffsetDownCheckColor == GameEngineColor::WHITE)
			&& (OffsetDownHalfColor == GameEngineColor::WHITE));

		return Result;
	}

	return false;
}

bool Player::CheckInAir()
{
	GameEngineColor LeftColor, RightColor, DownColor, UpColor;
	GameEngineColor LeftUPColor, RightUPColor, LeftDownColor, RightDownColor;

	LeftColor = CheckPixel(LeftOffset);
	RightColor = CheckPixel(RightOffset);
	DownColor = CheckPixel(DownOffset);
	UpColor = CheckPixel(UpOffset);

	LeftUPColor = CheckPixel(LeftOffset + UpOffset);
	RightUPColor = CheckPixel(RightOffset + UpOffset);
	LeftDownColor = CheckPixel(DownOffset + DownOffset);
	RightDownColor = CheckPixel(UpOffset + DownOffset);


	//8방향 모두 White일때
	bool Result = (LeftColor == GameEngineColor::WHITE
		&& RightColor == GameEngineColor::WHITE
		&& DownColor == GameEngineColor::WHITE
		&& UpColor == GameEngineColor::WHITE
		&& LeftUPColor == GameEngineColor::WHITE
		&& RightUPColor == GameEngineColor::WHITE
		&& LeftDownColor == GameEngineColor::WHITE
		&& RightDownColor == GameEngineColor::WHITE);

	return Result;
}
