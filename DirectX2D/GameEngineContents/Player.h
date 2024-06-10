#pragma once
#include "BaseCharacter.h"
#include <GameEngineCore/GameEngineState.h>

enum class FSM_State
{
	Idle,
	Run,
	Jump,
	Fall,
	RunToIdle,
	IdleToRun,
	Roll,
	PreCrouch,
	PostCrouch,
	DoorKick,
	Attack,
	Death,
};

class Player : public BaseCharacter
{
public:
	Player();
	~Player();

	// delete Function
	Player(const Player& _Other) = delete;
	Player(Player&& _Other) noexcept = delete;
	Player& operator=(const Player& _Other) = delete;
	Player& operator=(Player&& _Other) noexcept = delete;

protected:
	virtual void Start() override;
	virtual void Update(float _Delta) override;

	virtual void LevelStart(GameEngineLevel* _PrevLevel) override;
	virtual void LevelEnd(GameEngineLevel* _NextLevel) override;

private:
	//FSM
	void FSM_Idle();
	void FSM_Run();
	void FSM_Jump();
	void FSM_Fall();
	void FSM_RunToIdle();
	void FSM_IdleToRun();
	void FSM_Roll();
	void FSM_PreCrouch();
	void FSM_PostCrouch();
	void FSM_Death();
	void FSM_Attack();

	GameEngineState FSM_PlayerState;

	float4 LeftOffset = { -40.0f, 0.0f };
	float4 RightOffset = { 40.0f, 0.0f };
	float4 UpOffset = { 0.0f, 40.0f };
	float4 DownOffset = { 0.f, -40.0f };

private:
	void DirectionUpdate();

	Direction CurrentDir;

	bool IsRolling = false;

	bool CheckCanChangeDir();

private:
	std::shared_ptr<GameEngineSpriteRenderer> PlayerSpriteRenderer;

	float MoveSpeed = 200.f;

	GameEngineColor LastGroundPixelColor;
};

