#pragma once
#include "AppState.h"

namespace JimmyGod
{

	struct AppConfig
	{
		std::string appName = "JimmyGod";
		uint32_t windowWidth = 1280;
		uint32_t windowHeight = 720;
	};
	struct SpriteCommand
	{
		SpriteCommand(TextureId inTextureId, const Math::Vector2& inPosiiton, float inRotation)
			: textureId(inTextureId)
			, sourceRect({ 0.0f, 0.0f, 0.0f, 0.0f })
			, position(inPosiiton)
			, rotation(inRotation)
		{}

		SpriteCommand(TextureId inTextureId, const Math::Vector2& inPosiiton, float inRotation, Pivot pivot, Flip flip)
			: textureId(inTextureId)
			, sourceRect({ 0.0f, 0.0f, 0.0f, 0.0f })
			, position(inPosiiton)
			, rotation(inRotation)
			, pivot(pivot)
			, flip(flip)
		{}

		SpriteCommand(TextureId inTextureId, const Math::Rect& inSourceRect, const Math::Vector2& inPosiiton, float inRotation)
			: textureId(inTextureId)
			, sourceRect(inSourceRect)
			, position(inPosiiton)
			, rotation(inRotation)
		{}

		TextureId textureId = 0;
		Math::Rect sourceRect{};
		Math::Vector2 position{ 0.0f };
		float rotation{ 0.0f };
		Pivot pivot = Pivot::Center;
		Flip flip = Flip::None;
	};

	struct TextCommand
	{
		TextCommand(std::wstring inStr, float inSize, float inX, float inY, uint32_t inColor)
			: str(std::move(inStr))
			, size(inSize)
			, x(inX)
			, y(inY)
			, color(inColor)
		{}

		std::wstring str;
		float size, x, y;
		uint32_t color;
	};

	class App
	{
	public:
		//std::string msg = "Pater is awesome";
		//std::string_view sv = msg; -> string_view stores 2 pointers of stirng
		template<class StateType, class = std::void_t<std::is_base_of<AppState,StateType>>>

		void AddState(std::string name)
		{
			mAppStates.emplace(std::move(name), std::make_unique<StateType>());
		}
		
		void ChangeState(const std::string& name);

		void Run(AppConfig appConfig);
		void Quit() { mRunning = false; };

		TextureId LoadTexture(const std::filesystem::path& fileName)
		{
			return TextureManager::Get()->Load(fileName, false);
		}

		void ClearAllTextures()
		{
			return TextureManager::Get()->Clear();
		}

	private:
		AppConfig mAppConfig;
		Core::Window mWindow;
		bool mRunning = false;

		std::map<std::string, std::unique_ptr<AppState>> mAppStates;
		AppState* mCurrentState = nullptr;
		AppState* mNextState = nullptr;

		std::vector<SpriteCommand> mySpriteCommands;
		std::vector<TextCommand> myTextCommands;

		//Font myFont;
	};
} // namespace JimmyGod


//#define NONCOPYABLE(Class)
//	private:\
//		Class(const Class&);\
//		Class& operator=(const Class&);\


//class Car								|
//{										|
//public:								|
//	Car()								|
//	{									|
//		engine = new Engine();			|
//	}									|
//	~Car()								|
//	{									|
//		delete engine;					| Special Member Functions - Constructor, Destructor, Copy Constructor, Assignment Operator, Move Constructor, Move Assignment Operator
//	}									|
//	car(const car& rhs)					|
//	{									|	
//	engine = new engine();				|
//	*engine = *rhs.engine;				|
//	}									|	
//	Car& operator=(const Car&)			|
//	{									|
//		if (&rhs != this)				|
//		{								|
//			delete engine;				|
//			engine = new Engine();		|
//			*engine = *rhs.engine;		|
//		}								|
//		return *this;					|
//	}									|
//	Car(Car&& rhs)						|
//	{									|
//		engine = rhs.engine;			|
//		rhs.engine = nullptr;			|
//	}									|
//	Car& operator= (Car&& rhs)			|
//	{									|
//		if (&rhs != this)				|
//		{								|
//			engine = rhs.engine;		|
//			rhs.engine = nullptr;		|
//		}								|
//		return *this;					|
//	}									|
//										|
//										|
////Before C++11						|
//private:								|
//	Car(const Car&)						|
//	Car& operator= (const Car&);		|
//	Engine* engine;						|
//};									|
// 										|
//Car myCar;							|
//Car yourCar = myCar;					| 
//myCar = yourCar						|
//
//class Instructor
//{
//public:
//	void SetName(const std::string& str) // Constructor, Destructor, and Copy Constructor are temperary
//	{
//		name = str;
//	}
//	void SetName(std::string&& str)
//	{
//		name = std::move(str);
//	}
//private:
//	std::string name;
//};
//void Main()
//{
//	std::unique_ptr<std::string> ptr = std::make_unique<std::string>("Peter");
//	Instructor instr;
//	instr.SetName(ptr);
//}
// Constants = 1) Literal (i.g. 42) 2) Symbolic (i.g. const int)
// Move Semantic(std::move()) : 1)intentinal shallow copy 2)ownership transfer