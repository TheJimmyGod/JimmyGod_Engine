// ============================================
// Creator : Minjang JimmyGod Jin
// 
// JimmyGod Engine
// ============================================
#pragma once
#include "Common.h"

// App headers
#include "App.h"
#include "AppState.h"

// Service headers
#include "CameraService.h"
#include "RenderService.h"

// Terrain headers
#include "Terrain.h"

// Object headers
#include "Bomb.h"
#include "Cloth.h"
#include "Rope.h"
#include "Spark.h"
#include "TextMesh.h"
#include "Planet.h"
#include "SkyDome.h"

// World Headers
#include "Component.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "GameWorld.h"
#include "Service.h"

// Component Headers
#include "AgentComponent.h"
#include "ColliderComponent.h"
#include "GridComponent.h"
#include "ModelComponent.h"
#include "TransformComponent.h"

namespace JimmyGod {
	App& MainApp();
}