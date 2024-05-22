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

void BaseMap::InitMap(std::string_view _MapName, std::string_view _CollisionMapName)
{
	MapName = _MapName;
	CollisionMapName = _CollisionMapName;

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

		GameEngineTexture::Load(FilePath.PlusFilePath(_CollisionMapName));
		GameEngineSprite::CreateSingle(_CollisionMapName);

	}

	CollisionMapRenderer = CreateComponent<GameEngineSpriteRenderer>(static_cast<int>(ContentsRenderType::Map));
	CollisionMapRenderer->SetSprite(_CollisionMapName);

	std::shared_ptr<GameEngineTexture> CollisionMapTexture = GameEngineTexture::Find(_CollisionMapName);

	float4 HScaleDebug = CollisionMapTexture->GetScale().Half();
	HScaleDebug.Y *= -1.0f;

	CollisionMapRenderer->Transform.SetLocalPosition(HScaleDebug);
	CollisionMapRenderer->Off();

	MapTexture = GameEngineTexture::Find(MapName);
	CollisionMapTexture = GameEngineTexture::Find(CollisionMapName);
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
		CollisionMapRenderer->Off();
	}
	else if (true == DebugMode)
	{
		MapRenderer->Off();
		CollisionMapRenderer->On();
	}
}
