#pragma once
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEnginePlatform/GameEngineSound.h>
#include <GameEngineCore/GameEngineGUI.h>

class TestGUIWindow : public GameEngineGUIWindow
{
public:
	int Select = 0;
	std::shared_ptr<GameEngineObject> SelectObject = nullptr;

	void Start() override;
	void OnGUI(GameEngineLevel* _Level, float _DeltaTime) override;
};

class TitleLevel : public GameEngineLevel
{
public:
	// constrcuter destructer
	TitleLevel();
	~TitleLevel();

	// delete Function
	TitleLevel(const TitleLevel& _Other) = delete;
	TitleLevel(TitleLevel&& _Other) noexcept = delete;
	TitleLevel& operator=(const TitleLevel& _Other) = delete;
	TitleLevel& operator=(TitleLevel&& _Other) noexcept = delete;

protected:
	virtual void Start() override;
	virtual void Update(float _Delta) override;

	virtual void LevelStart(GameEngineLevel* _PrevLevel) override;
	virtual void LevelEnd(GameEngineLevel* _NextLevel) override;

private:
	GameEngineSoundPlayer BGMPlayer;
	GameEngineSoundPlayer AmbiencePlayer;
};
