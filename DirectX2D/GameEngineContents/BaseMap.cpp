#include "PreCompile.h"
#include "BaseMap.h"

BaseMap::BaseMap()
{
}

BaseMap::~BaseMap()
{
}

void BaseMap::Start()
{
	GameEngineInput::AddInputObject(this);
}

void BaseMap::Update(float _Delta)
{
	if (true == GameEngineInput::IsDown(VK_F2, this))
	{
		ToggleMode();
	}
}

void BaseMap::InitMap(std::string_view _MapName, std::string_view _DebugMapName)
{
	MapName = _MapName;
	DebugMapName = _DebugMapName;

	{
		GameEnginePath FilePath;
		FilePath.SetCurrentPath();
		FilePath.MoveParentToExistsChild("ContentsResources");
		FilePath.MoveChild("ContentsResources\\Texture\\Map\\");

		GameEngineTexture::Load(FilePath.PlusFilePath(_MapName));
		GameEngineSprite::CreateSingle(_MapName);
	}

	MapRenderer = CreateComponent<GameEngineSpriteRenderer>(static_cast<int>(ContentsRenderType::Map));
	MapRenderer->SetSprite(_MapName);

	std::shared_ptr<GameEngineTexture> MapTexture = GameEngineTexture::Find(_MapName);

	float4 HScale = MapTexture->GetScale().Half();
	HScale.Y *= -1.0f;

	MapRenderer->Transform.SetLocalPosition(HScale);
	MapRenderer->On();


	{
		GameEnginePath FilePath;
		FilePath.SetCurrentPath();
		FilePath.MoveParentToExistsChild("ContentsResources");
		FilePath.MoveChild("ContentsResources\\Texture\\Map\\");

		GameEngineTexture::Load(FilePath.PlusFilePath(_DebugMapName));
		GameEngineSprite::CreateSingle(_DebugMapName);

	}

	DebugMapRenderer = CreateComponent<GameEngineSpriteRenderer>(static_cast<int>(ContentsRenderType::Map));
	DebugMapRenderer->SetSprite(_DebugMapName);

	std::shared_ptr<GameEngineTexture> DebugMapTexture = GameEngineTexture::Find(_DebugMapName);

	float4 HScaleDebug = DebugMapTexture->GetScale().Half();
	HScaleDebug.Y *= -1.0f;

	DebugMapRenderer->Transform.SetLocalPosition(HScaleDebug);
	DebugMapRenderer->Off();

	MapTexture = GameEngineTexture::Find(MapName);
	DebugMapTexture = GameEngineTexture::Find(DebugMapName);
}

void BaseMap::InitBackGround(std::string_view _BackGroundName)
{
	BackGroundName = _BackGroundName;

	{
		GameEnginePath FilePath;
		FilePath.SetCurrentPath();
		FilePath.MoveParentToExistsChild("ContentsResources");
		FilePath.MoveChild("ContentsResources\\Texture\\Map\\");

		GameEngineTexture::Load(FilePath.PlusFilePath(_BackGroundName));
		GameEngineSprite::CreateSingle(_BackGroundName);
	}

	BackGroundRenderer = CreateComponent<GameEngineSpriteRenderer>(static_cast<int>(ContentsRenderType::BackGround));
	BackGroundRenderer->SetSprite(_BackGroundName);

	std::shared_ptr<GameEngineTexture> BackGroundTexture = GameEngineTexture::Find(_BackGroundName);

	float4 HScale = BackGroundTexture->GetScale().Half();
	HScale.Y *= -1.0f;

	BackGroundRenderer->Transform.SetLocalPosition(HScale);
	BackGroundRenderer->On();
}

void BaseMap::ToggleMode()
{
	DebugMode = !DebugMode;

	if (false == DebugMode)
	{
		MapRenderer->On();
		DebugMapRenderer->Off();
	}
	else if (true == DebugMode)
	{
		MapRenderer->Off();
		DebugMapRenderer->On();
	}
}
