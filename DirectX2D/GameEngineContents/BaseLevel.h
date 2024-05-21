#pragma once
#include "BaseMap.h"
#include "Player.h"

class BaseLevel : public GameEngineLevel
{
public:
	// constrcuter destructer
	BaseLevel();
	~BaseLevel();

	// delete Function
	BaseLevel(const BaseLevel& _Other) = delete;
	BaseLevel(BaseLevel&& _Other) noexcept = delete;
	BaseLevel& operator=(const BaseLevel& _Other) = delete;
	BaseLevel& operator=(BaseLevel&& _Other) noexcept = delete;

protected:
	// 카메라 이동
	void FocusCamera(float _Delta);

	float4 CameraLimit_Middle;
	float4 CameraLimit_Max;
	float4 CameraLimit_Min;

protected:
	virtual void Start() override;

	virtual void Update(float _Delta) override;

	virtual void LevelStart(GameEngineLevel* _PrevLevel) override;
	virtual void LevelEnd(GameEngineLevel* _NextLevel) override;

	void InitCamera();

protected:
	GameEngineSoundPlayer BGMPlayer;

	std::shared_ptr<BaseMap> MapObject;

	std::shared_ptr<Player> MainPlayer;
};



