#include "PreCompile.h"
#include "ContentsCore.h"

#include "TitleLevel.h"
#include "TestLevel.h"


ContentsCore::ContentsCore()
{
}

ContentsCore::~ContentsCore()
{
}

void ContentsCore::Start()
{
	GameEngineCore::CreateLevel<TitleLevel>("TitleLevel");
	GameEngineCore::CreateLevel<TestLevel>("TestLevel");

	GameEngineCore::ChangeLevel("TestLevel");
}

void ContentsCore::Update(float _Delta)
{

}

void ContentsCore::Release()
{
}