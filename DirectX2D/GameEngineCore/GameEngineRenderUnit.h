#pragma once
#include "GameEngineShaderResHelper.h"

// ���� :
// �������� �ּҴ���
// �̳༮�� ������ �������� �ȴ�.
// ���̻� ��� ��������.
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

	// ����ȭ ����� �ɼ� �ִ�.
	// �̰� ��Ե� ������ ����� ã�°� �´�.
	std::shared_ptr<class GameEngineInputLayOut> LayOut = nullptr;


	std::shared_ptr<class GameEngineMesh> Mesh = nullptr;
	std::shared_ptr<class GameEngineMaterial> Material = nullptr;
};
