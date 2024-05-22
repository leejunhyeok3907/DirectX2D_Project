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
	//�߷��� ������ �޴°�
	bool EffectiveGravity = true;

	//���� ĳ���Ͱ� ���� ����ִ���
	bool IsGround;

	bool IsJumping = false;

	//�⺻������ ���� �߷� �ӵ�
	float GravityPower = 100.0f;

	//������ �߷� �ӵ�
	float4 GravityVector = float4::ZERO;

	std::shared_ptr<class GameEngineTexture> CollisionMapTexture;
};

