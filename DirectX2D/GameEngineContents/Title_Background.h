#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEnginePlatform/GameEngineSound.h>

class Title_Background : public GameEngineActor
{
public:
	Title_Background();
	~Title_Background();

	Title_Background(const Title_Background& _Other) = delete;
	Title_Background(Title_Background&& _Other) noexcept = delete;
	Title_Background& operator=(const Title_Background& _Other) = delete;
	Title_Background& operator=(Title_Background&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _Delta) override;



private:
	float RainCreateTimer = 0.f;

	int ButtonSelectIndex = 0;

	std::shared_ptr<GameEngineUIRenderer> RainRenderer;

	GameEngineSoundPlayer BGMPlayer;

	void NeonSoundEvent(GameEngineRenderer* _Renderer);
};

