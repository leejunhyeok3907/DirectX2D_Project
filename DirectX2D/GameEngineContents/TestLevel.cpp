#include "PreCompile.h"
#include "TestLevel.h"

#include "Player.h"

TestLevel::TestLevel()
{
}

TestLevel::~TestLevel()
{
}

void TestLevel::Start()
{
	GameEngineInput::AddInputObject(this);

	InitCamera();

	GameEngineLevel::OnDebug();
}

void TestLevel::Update(float _Delta)
{
	FocusCamera(_Delta);
}

void TestLevel::LevelStart(GameEngineLevel* _PrevLevel)
{
	float4 HalfWindowScale = GameEngineCore::MainWindow.GetScale().Half();

	MapObject = CreateActor<BaseMap>();
	MapObject->InitMap("Map_MainLevel1_Origin.png", "Map_MainLevel1.png");
	MapObject->InitBackGround("Skyline.png");

	std::shared_ptr<Player> player = CreateActor<Player>();
	MainPlayer = player;
	player->Transform.SetLocalPosition({ HalfWindowScale.X, -HalfWindowScale.Y });
	player->SetCollisionMapTexture(MapObject->GetCollisionMapName());
}

void TestLevel::LevelEnd(GameEngineLevel* _NextLevel)
{
}
