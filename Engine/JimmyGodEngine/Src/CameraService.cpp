#include "Precompiled.h"
#include "CameraService.h"

using namespace JimmyGod;

META_DERIVED_BEGIN(CameraService, Service)
	META_NO_FIELD
META_CLASS_END

void JimmyGod::CameraService::Initialize()
{
	mActiveCamera = AddCamera("Default");
}

Graphics::Camera * CameraService::AddCamera(const char * name)
{
	return &mCameraMap[name];
}

Graphics::Camera * CameraService::FindCamera(const char * name)
{
	auto iter = mCameraMap.find(name);
	return (iter==mCameraMap.end())? nullptr : &(iter->second);
}

void CameraService::SetActiveCamera(const char * name)
{
	auto camera = FindCamera(name);
	if (camera)
		mActiveCamera = camera;
}

Graphics::Camera & CameraService::GetActiveCamera()
{
	ASSERT(mActiveCamera, "CameraService -- No active camera!");
	return *mActiveCamera;
}

const Graphics::Camera & CameraService::GetActiveCamera() const
{
	ASSERT(mActiveCamera, "CameraService -- No active camera!");
	return *mActiveCamera;
}
