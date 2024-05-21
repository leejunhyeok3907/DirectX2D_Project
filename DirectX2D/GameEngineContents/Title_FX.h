#pragma once
class Title_FX : public GameEngineActor
{
public:
	Title_FX();
	~Title_FX();

	Title_FX(const Title_FX& _Other) = delete;
	Title_FX(Title_FX&& _Other) noexcept = delete;
	Title_FX& operator=(const Title_FX& _Other) = delete;
	Title_FX& operator=(Title_FX&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _Delta) override;

private:
	std::shared_ptr<GameEngineUIRenderer> RainRenderer;

	float RainDeathPos = 0.f;
};