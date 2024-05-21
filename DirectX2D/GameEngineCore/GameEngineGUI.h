#pragma once
#include "GameEngineObject.h"

class GameEngineGUIWindow : public GameEngineObjectBase
{
public:
	void Begin();
	void End();
	// ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)

	virtual void Start() = 0;
	virtual void OnGUI(GameEngineLevel* _Level, float _DeltaTime) = 0;
};

// Ό³Έν :
class GameEngineGUI
{
public:
	static void Start();
	static void Release();

	static void GUIRender(GameEngineLevel* _Level, float _DeltaTime);

	// constrcuter destructer
	GameEngineGUI();
	~GameEngineGUI();

	// delete Function
	GameEngineGUI(const GameEngineGUI& _Other) = delete;
	GameEngineGUI(GameEngineGUI&& _Other) noexcept = delete;
	GameEngineGUI& operator=(const GameEngineGUI& _Other) = delete;
	GameEngineGUI& operator=(GameEngineGUI&& _Other) noexcept = delete;

	template<typename WindowType>
	static std::shared_ptr<WindowType> CreateGUIWindow(std::string_view _Name)
	{
		std::string UpperName = GameEngineString::ToUpperReturn(_Name);

		if (true == GUIWindows.contains(UpperName))
		{
			return nullptr;
		}

		std::shared_ptr<WindowType> NewWindow = std::make_shared<WindowType>();
		NewWindow->SetName(UpperName);
		NewWindow->Start();
		GUIWindows[UpperName] = NewWindow;

		return NewWindow;
	}


protected:

private:
	static std::map<std::string, std::shared_ptr<GameEngineGUIWindow>> GUIWindows;

};

