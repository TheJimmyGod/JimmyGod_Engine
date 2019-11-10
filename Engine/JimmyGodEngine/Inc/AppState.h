#pragma once

namespace JimmyGod {
	class AppState
	{
	public:
		virtual void Initialize() {}
		virtual void Terminate() {}
		
		virtual void Update(float deltaTime) {}
		virtual void Render() {}
		virtual void DebugUI() {}

	};
} //  namespace JimmyGod

//class Cat { public: int hp; };
////Explicit template specialization
//template<>
//Cat Add(Cat a, Cat b)
//{
//	return a.hp + b.hp;
//}