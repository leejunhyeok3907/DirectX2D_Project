#pragma once
class BaseMap : public GameEngineActor
{
public:
	BaseMap();
	~BaseMap();

	BaseMap(const BaseMap& _Other) = delete;
	BaseMap(BaseMap&& _Other) noexcept = delete;
	BaseMap& operator=(const BaseMap& _Other) = delete;
	BaseMap& operator=(BaseMap&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _Delta) override;

public:
	void InitMap(std::string_view _MapName, std::string_view _DebugMapName);
	void InitBackGround(std::string_view _BackGroundName);
	void ToggleMode();

private:
	bool DebugMode = false;

	std::string_view MapName;
	std::string_view BackGroundName;
	std::string_view DebugMapName;

	std::shared_ptr<GameEngineTexture> MapTexture;
	std::shared_ptr<GameEngineTexture> DebugMapTexture;

	std::shared_ptr<GameEngineSpriteRenderer> MapRenderer;
	std::shared_ptr<GameEngineSpriteRenderer> BackGroundRenderer;
	std::shared_ptr<GameEngineSpriteRenderer> DebugMapRenderer;

public:
	std::string_view GetMapName() const { return MapName; }
	std::string_view GetDebugMapName() const { return DebugMapName; }
};

