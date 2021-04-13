#pragma once

#include <JimmyGodEngine/Inc/JimmyGodEngine.h>

namespace FighterGround
{
	class UI
	{
	public:
		static void StaticInitialize(std::map<std::string, size_t> data);
		static void StaticTerminate();
		static UI* Get();
	public:

		struct LeaderBoardItem
		{
			std::string name;
			size_t score;
		};

		void Initialize(std::map<std::string, size_t> data);
		void Terminate();
		void Update(size_t score);
		void Render();
	private:
		size_t mTotalScore = 0;
		JimmyGod::Graphics::TextureId mBackGround;
		JimmyGod::Graphics::TextureId mGameOver;
		std::vector<LeaderBoardItem> mLeader;
	};
}