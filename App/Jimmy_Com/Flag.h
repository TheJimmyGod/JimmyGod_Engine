#pragma once

namespace JimmyCom
{
	enum class Flag
	{
		Ally,
		Neutral,
		Enemy,
	};

	enum class UnitType
	{
		None,
		Soldier,
		Mutant
	};

	enum class Status
	{
		Move,
		Attack,
		Standby,
		TurnOver,
		Dead
	};
}
