#include "Precompiled.h"
#include "App.h"

#include "MetaRegistration.h"

using namespace JimmyGod;
using namespace JimmyGod::Core;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Input;

void App::ChangeState(const std::string & name)
{
	if (auto iter = mAppStates.find(name); iter != mAppStates.end())
		mNextState = iter->second.get();
}

void App::Run(AppConfig appConfig)
{
	LOG("App -- Running ...");

	mAppConfig = std::move(appConfig);

	LOG("App -- Registering meta types ...");
	Core::StaticMetaRegister();
	Math::StaticMetaRegister();
	JimmyGod::StaticMetaRegister();

	// Initialize timer
	TimeUtil::GetTime();

	// Setup our application window
	mWindow.Initialize(
		GetModuleHandle(NULL),
		mAppConfig.appName.c_str(),
		mAppConfig.windowWidth,
		mAppConfig.windowHeight
	);

	// Initialize input systems
	auto handle = mWindow.GetWindowHandle();
	InputSystem::StaticInitialize(handle);

	// Initialize Graphics systems
	GraphicsSystem::StaticInitialize(handle, false);
	DebugUI::StaticInitialize(handle, false, true);
	SimpleDraw::StaticInitialize();

	TextureManager::StaticInitialize("../Assets/Images");
	SpriteRenderer::StaticInitialize();
	// Initialize the starting state
	mCurrentState = mAppStates.begin()->second.get();
	mCurrentState->Initialize();

	// OnInit

	mRunning = true;
	while (mWindow.IsActive() && mRunning)
	{
		mWindow.ProcessMessage();

		if (!mWindow.IsActive())
		{
			Quit();
			continue;
		}

		if (mNextState)
		{
			mCurrentState->Terminate();
			mCurrentState = std::exchange(mNextState, nullptr);
			mCurrentState->Initialize();
		}

		auto inputSystem = InputSystem::Get();
		inputSystem->Update();

		if (inputSystem->IsKeyPressed(KeyCode::ESCAPE))
		{
			Quit();
			continue;
		}

		float deltaTime = TimeUtil::GetDeltaTime();

		//if (pasued)
		//	deltaTime = 0.0f;

		mCurrentState->Update(deltaTime);
		
		auto graphicSystem = GraphicsSystem::Get();
		graphicSystem->BeginRender();

		TextureId id = 0;
		Texture* texture = nullptr;

		//Sprite
		SpriteRenderer::Get()->BeginRender();
		for (const auto& command : mySpriteCommands)
		{
			if (id != command.textureId)
			{
				texture = TextureManager::Get()->GetTexture(command.textureId);
				id = command.textureId;
			}
			if (texture)
			{
				if (Math::IsEmpty(command.sourceRect))
				{
					SpriteRenderer::Get()->Draw(*texture, command.position, command.rotation, command.pivot, command.flip);
				}
				else
				{
					SpriteRenderer::Get()->Draw(*texture, command.sourceRect, command.position, command.rotation, command.pivot, command.flip);
				}
			}
		}
		mySpriteCommands.clear();
		SpriteRenderer::Get()->EndRender();
		// Text
		//for (const auto& command : myTextCommands)
		//{
		//	myFont.Draw(command.str.c_str(), command.size, command.x, command.y, command.color);
		//}
		//myTextCommands.clear();
		mCurrentState->Render();

		DebugUI::BeginRender();
		mCurrentState->DebugUI();
		DebugUI::EndRender();

		graphicSystem->EndRender();
	}

	mCurrentState->Terminate();

	// Termiates engine systems

	DebugUI::StaticTerminate();
	InputSystem::StaticTerminate();
	GraphicsSystem::StaticTerminate();
	SimpleDraw::StaticTerminate();
	TextureManager::StaticTerminate();
	SpriteRenderer::StaticTerminate();
	// Terminate window

	mWindow.Terminate();
}