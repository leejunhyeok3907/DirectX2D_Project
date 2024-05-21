#pragma once
#include "GameEngineShaderResHelper.h"

// 설명 :
// 랜더링의 최소단위
// 이녀석을 돌리면 랜더링이 된다.
// 더이상 상속 못내린다.
class GameEngineRenderUnit final : public GameEngineObjectBase, std::enable_shared_from_this<GameEngineRenderUnit>
{
public:
	// constrcuter destructer
	GameEngineRenderUnit();
	~GameEngineRenderUnit();

	void SetMesh(std::string_view _Name);
	void SetMaterial(std::string_view _Name);

	void ResSetting();
	void Draw();

	void SetParentRenderer(class GameEngineRenderer* _Renderer)
	{
		ParentRenderer = _Renderer;
	}

	class GameEngineRenderer* GetParentRenderer()
	{
		return ParentRenderer;
	}

	/*
	class std::shared_ptr<class GameEngineMaterial> GetMaterial()
	{
		return Material;
	}
	*/
	GameEngineShaderResHelper ShaderResHelper;

private:
	class GameEngineRenderer* ParentRenderer = nullptr;

	// 최적화 대상이 될수 있다.
	// 이걸 어떻게든 관리할 방법을 찾는게 맞다.
	std::shared_ptr<class GameEngineInputLayOut> LayOut = nullptr;


	std::shared_ptr<class GameEngineMesh> Mesh = nullptr;
	std::shared_ptr<class GameEngineMaterial> Material = nullptr;
};

