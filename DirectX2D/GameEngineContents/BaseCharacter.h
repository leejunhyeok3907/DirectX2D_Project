#pragma once
class BaseCharacter : public GameEngineActor
{
public:
	BaseCharacter();
	~BaseCharacter();

	// delete Function
	BaseCharacter(const BaseCharacter& _Other) = delete;
	BaseCharacter(BaseCharacter&& _Other) noexcept = delete;
	BaseCharacter& operator=(const BaseCharacter& _Other) = delete;
	BaseCharacter& operator=(BaseCharacter&& _Other) noexcept = delete;

protected:
	virtual void Start() override;
	virtual void Update(float _Delta) override;

	virtual void LevelStart(GameEngineLevel* _PrevLevel) override;
	virtual void LevelEnd(GameEngineLevel* _NextLevel) override;

protected:
	void UpdateGravity(float _Delta);

	void ResetGravity();

	bool CheckGroundPixel(float4 _Offset=float4::ZERO);

	bool CheckWallPixel(float4 _Offset = float4::ZERO);

	GameEngineColor GetCollisionMapColor(float4 _Pos, GameEngineColor _DefaultColor = GameEngineColor::WHITE);

public:
	void SetGravity(bool _bEffective) { EffectiveGravity = _bEffective; }

	void SetCollisionMapTexture(std::string_view _CollisionMapTextureName);


protected:
	//중력의 영향을 받는가
	bool EffectiveGravity = true;

	//현재 캐릭터가 땅을 밟고있는지
	bool IsGround;

	bool IsJumping = false;

	//기본적으로 가할 중력 속도
	float GravityPower = 100.0f;

	//더해진 중력 속도
	float4 GravityVector = float4::ZERO;

	std::shared_ptr<class GameEngineTexture> CollisionMapTexture;
};

