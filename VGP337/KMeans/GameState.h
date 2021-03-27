#pragma once

#include <JimmyGodEngine/Inc/JimmyGodEngine.h>
#include <ImGui/Inc/Applog.h>

class GameState : public JimmyGod::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void DebugUI() override;

private:

	struct Unit
	{
		ImVec2 position;
		ImVec2 velocity;
	};


	std::vector<Unit> mUnits;
	JimmyGod::Graphics::Texture mUnitTexture;

	
	bool mDraw;
};