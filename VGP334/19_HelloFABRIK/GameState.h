#pragma once

#include <JimmyGodEngine/Inc/JimmyGodEngine.h>

class GameState : public JimmyGod::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:

	JimmyGod::Math::Vector3 mTarget;
	std::vector<JimmyGod::Math::Vector3> mPoints;

	JimmyGod::Graphics::Camera mCamera;
	float mTime = 0.0f;
	
};