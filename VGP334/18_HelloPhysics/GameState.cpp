#include "GameState.h"

#include <ImGui/Inc/imgui.h>

using namespace JimmyGod::Graphics;
using namespace JimmyGod::Input;
using namespace JimmyGod::Math;
using namespace JimmyGod::Physics;

namespace
{
	std::random_device sRandomDevice{};
	std::mt19937 sRandomEngine{ sRandomDevice() };

	float Randf(float min, float max)
	{
		return std::uniform_real_distribution<float>{ min, max }(sRandomEngine);
	}

}

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mCamera.SetNearPlane(0.1f);
	mCamera.SetFarPlane(500.0f);
	mCamera.SetPosition({ 0.0f, 10.0f, -30.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	JimmyGod::Physics::PhysicsWorld::Settings settings;
	settings.gravity = { 0.0f, -9.8f, 0.0f };
	settings.timeStep = 1.0f / 60.0f;
	settings.drag = 0.3f;
	settings.iterations = 1;
	mPhysicsWorld.Initialize(settings);
	mPhysicsWorld.AddPlane({Vector3::YAxis, 0.0f});
	mPhysicsWorld.AddOBB({ { 0.0f,2.0f,0.0f }, { 4.0f,0.5f,5.0f }, Quaternion::RotationAxis(Vector3::ZAxis, 10.0f * Constants::DegToRad) });
	mPhysicsWorld.AddStaticOBB({ { 0.0f,2.0f,0.0f }, { 4.0f,0.5f,5.0f }, Quaternion::RotationAxis(Vector3::ZAxis, 10.0f * Constants::DegToRad) });
}

void GameState::Terminate()
{
	mPhysicsWorld.Clear();
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();

	const float kMoveSpeed = inputSystem->IsKeyDown(KeyCode::LSHIFT) ? 100.0f : 10.0f;
	const float kTurnSpeed = 1.0f;

	if (inputSystem->IsKeyDown(KeyCode::W))
		mCamera.Walk(kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::S))
		mCamera.Walk(-kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::D))
		mCamera.Strafe(kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::A))
		mCamera.Strafe(-kMoveSpeed * deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(inputSystem->GetMouseMoveX() * kTurnSpeed * deltaTime);
		mCamera.Pitch(inputSystem->GetMouseMoveY() * kTurnSpeed * deltaTime);
	}
	mPhysicsWorld.Update(deltaTime);
	mTime += deltaTime;
}

void GameState::Render()
{
	
	mPhysicsWorld.DebugDraw();
	SimpleDraw::AddGroundPlane(30.0f,Colors::Aqua);
	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
	ImGui::Begin("Physics", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::Button("Particles!"))
	{
		mPhysicsWorld.Clear(true);
		for (int i = 0; i < 100; ++i)
		{
			auto p = new Particle();
			p->SetPosition({ 0.0f,5.0f,0.0f });
			p->SetVelocity({ Randf(-0.05f,0.05f),Randf(0.1f,0.4f),Randf(-0.05f,0.05f) });
			p->radius = 0.1f;
			p->bounce = 0.3f;
			mPhysicsWorld.AddParticle(p);
		}
	}
	if (ImGui::Button("Sticks!"))
	{
		mPhysicsWorld.Clear(true);
		for (int i = 0; i < 50; ++i)
		{
			auto p1 = new Particle();
			auto p2 = new Particle();
			p1->SetPosition({ 0.5f,5.0f,0.0f });
			p1->SetVelocity({ Randf(-0.05f,0.05f),Randf(0.1f,0.4f),Randf(-0.05f,0.05f) });
			p1->radius = 0.1f;
			p1->bounce = 0.3f;
			

			p2->SetPosition({ -0.5f,5.0f,0.0f });
			p2->SetVelocity({ Randf(-0.05f,0.05f),Randf(0.1f,0.4f),Randf(-0.05f,0.05f) });
			p2->radius = 0.1f;
			p2->bounce = 0.3f;
			mPhysicsWorld.AddParticle(p1);
			mPhysicsWorld.AddParticle(p2);
			auto c = new Spring(p1, p2);
			mPhysicsWorld.AddConstraint(c);
		}
	}
	if (ImGui::Button("Fixed!"))
	{
		mPhysicsWorld.Clear(true);
		for (int i = 0; i < 10; ++i)
		{
			auto p = new Particle();
			p->SetPosition({ 0.5f,5.0f,0.5f });
			p->SetVelocity({ Randf(-0.05f,0.05f),Randf(0.1f,0.4f),Randf(-0.05f,0.05f) });
			p->radius = 2.0f;
			p->bounce = 0.3f;
			mPhysicsWorld.AddParticle(p);
			auto f = new Fixed(p);
			mPhysicsWorld.AddConstraint(f);
		}
	}
	if (ImGui::Button("Tetrahedron!"))
	{
		mPhysicsWorld.Clear(true);
		float size = 20.0f;
		for (int i = 0; i < 20; ++i)
		{
			auto p1 = new Particle();
			auto p2 = new Particle();
			auto p3 = new Particle();
			auto p4 = new Particle();

			p1->SetPosition({ -1.5f,0.0f + size,0.0f });
			p1->SetVelocity({ Randf(-0.05f,0.05f),Randf(0.1f,0.4f),Randf(-0.05f,0.05f) });
			p1->radius = 0.1f;
			p1->bounce = 0.3f;

			p2->SetPosition({ 1.5f,0.0f + size,0.0f });
			p2->SetVelocity({ Randf(-0.05f,0.05f),Randf(0.1f,0.4f),Randf(-0.05f,0.05f) });
			p2->radius = 0.1f;
			p2->bounce = 0.3f;
			//Top
			p3->SetPosition({ 0.0f,1.5f + size,0.0f });
			p3->SetVelocity({ Randf(-0.05f,0.05f),Randf(0.1f,0.4f),Randf(-0.05f,0.05f) });
			p3->radius = 0.1f;
			p3->bounce = 0.3f;

			p4->SetPosition({ 0.0f,0.0f + size,-1.5f });
			p4->SetVelocity({ Randf(-0.05f,0.05f),Randf(0.1f,0.4f),Randf(-0.05f,0.05f) });
			p4->radius = 0.1f;
			p4->bounce = 0.3f;

			mPhysicsWorld.AddParticle(p1);
			mPhysicsWorld.AddParticle(p2);
			mPhysicsWorld.AddParticle(p3);
			mPhysicsWorld.AddParticle(p4);

			mPhysicsWorld.AddConstraint(new Spring(p1, p2, 5.0f));
			mPhysicsWorld.AddConstraint(new Spring(p2, p3, 5.0f));
			mPhysicsWorld.AddConstraint(new Spring(p3, p4, 5.0f));

			mPhysicsWorld.AddConstraint(new Spring(p4, p1, 5.0f));
			mPhysicsWorld.AddConstraint(new Spring(p1, p3, 5.0f));
			mPhysicsWorld.AddConstraint(new Spring(p2, p4, 5.0f));

		}
	}
	if (ImGui::Button("Cloth!"))
	{
		particles.clear();
		float size = 7.0f;
		mPhysicsWorld.Clear(true);
		const float OffsetX = (size) * 0.5f;
		const float OffsetY = (size) * 0.5f + 20.0f;
		for (float y = 0; y < size; y++)
		{
			for (float x = 0; x < size; x++)
			{
				auto p1 = new Particle({ Vector3{ -OffsetX + (static_cast<float>(x)), OffsetY - (static_cast<float>(y)),0.0f }});
				p1->radius = 0.1f;
				p1->SetVelocity(Randf(0.01f, 0.1f));
				mPhysicsWorld.AddParticle(p1);
				particles.push_back(p1);
			}
		}

		for (float y = 0; y < size; y++)
		{
			for (float x = 0; x < size; x++)
			{
				if(y == 0 && (x == size -1 || x == 0))
					mPhysicsWorld.AddConstraint(new Fixed(particles[static_cast<size_t>(y * size + x)]));
				if(x + 1 < size)
					mPhysicsWorld.AddConstraint(new Spring(particles[static_cast<size_t>(y*size + x)], particles[static_cast<size_t>(y*size + x + 1)]));
				if (y + 1 < size)
					mPhysicsWorld.AddConstraint(new Spring(particles[static_cast<size_t>(y*size + x)], particles[static_cast<size_t>((y + 1)*size + x)]));
			}
		}
	}
	if (ImGui::Button("Cube!"))
	{
		particles.clear();
		float size = 5.0f;
		mPhysicsWorld.Clear(true);

		auto P1 = new Particle();
		P1->radius = 0.1f;
		P1->SetVelocity(0.01f);
		P1->SetPosition({ -size,size + 20.0f,size });
		auto P2 = new Particle();
		P2->radius = 0.1f;
		P2->SetVelocity(0.01f);
		P2->SetPosition({ size,size + 20.0f,size });
		auto P3 = new Particle();
		P3->radius = 0.1f;
		P3->SetVelocity(0.01f);
		P3->SetPosition({ -size,size + 20.0f,-size });
		auto P4 = new Particle();
		P4->radius = 0.1f;
		P4->SetVelocity(0.01f);
		P4->SetPosition({ size,size + 20.0f,-size });

		auto P5 = new Particle();
		P5->radius = 0.1f;
		P5->SetVelocity(0.01f);
		P5->SetPosition({ size,-size + 20.0f,-size });
		auto P6 = new Particle();
		P6->radius = 0.1f;
		P6->SetVelocity(0.01f);
		P6->SetPosition({ -size,-size + 20.0f,-size });
		auto P7 = new Particle();
		P7->radius = 0.1f;
		P7->SetVelocity(0.01f);
		P7->SetPosition({ size,-size + 20.0f,size });
		auto P8 = new Particle();
		P8->radius = 0.1f;
		P8->SetVelocity(0.01f);
		P8->SetPosition({ -size,-size + 20.0f,size });


		mPhysicsWorld.AddParticle(P1);
		mPhysicsWorld.AddParticle(P2);
		mPhysicsWorld.AddParticle(P3);
		mPhysicsWorld.AddParticle(P4);

		mPhysicsWorld.AddParticle(P5);
		mPhysicsWorld.AddParticle(P6);
		mPhysicsWorld.AddParticle(P7);
		mPhysicsWorld.AddParticle(P8);
		//Top
		mPhysicsWorld.AddConstraint(new Spring(P1, P2,1.0f));
		mPhysicsWorld.AddConstraint(new Spring(P2, P3,1.0f));
		mPhysicsWorld.AddConstraint(new Spring(P3, P4,1.0f));
		mPhysicsWorld.AddConstraint(new Spring(P4, P1,1.0f));
		//Bottom
		mPhysicsWorld.AddConstraint(new Spring(P5, P6,1.0f));
		mPhysicsWorld.AddConstraint(new Spring(P6, P7,1.0f));
		mPhysicsWorld.AddConstraint(new Spring(P7, P8,1.0f));
		mPhysicsWorld.AddConstraint(new Spring(P8, P5,1.0f));
		//Connect
		mPhysicsWorld.AddConstraint(new Spring(P1, P5,1.0f));
		mPhysicsWorld.AddConstraint(new Spring(P2, P6,1.0f));
		mPhysicsWorld.AddConstraint(new Spring(P3, P7,1.0f));
		mPhysicsWorld.AddConstraint(new Spring(P4, P8,1.0f));
		//
		mPhysicsWorld.AddConstraint(new Spring(P1, P7, 5.0f));
		mPhysicsWorld.AddConstraint(new Spring(P3, P5, 5.0f));
		mPhysicsWorld.AddConstraint(new Spring(P2, P8, 5.0f));
		mPhysicsWorld.AddConstraint(new Spring(P4, P6, 5.0f));
	}
	if (ImGui::Button("Chain and ball!"))
	{
		mPhysicsWorld.Clear(true);
		particles.clear();
		size_t size = 20;
		for (size_t i = 0; i < size; i++)
		{
			if (i != 0)
			{
				auto p1 = new Particle();
				p1->SetPosition(Vector3{ 0.0f, static_cast<float>(size+10) - (static_cast<float>(i) * 0.5f), 0.0f });
				p1->SetVelocity(0.01f);
				p1->radius = 0.1f;
				p1->bounce = 0.3f;
				mPhysicsWorld.AddParticle(p1);
				particles.push_back(p1);
			}
			else
			{
				auto p2 = new Particle();
				p2->SetPosition(Vector3{ 0.0f, static_cast<float>(size+10), 0.0f });
				p2->SetVelocity(0.01f);
				p2->radius = 0.5f;
				p2->bounce = 0.3f;
				mPhysicsWorld.AddParticle(p2);
				particles.push_back(p2);
			}
		}

		for (size_t i = 0; i < particles.size(); i++)
		{
			if (i != 0)
			{
				mPhysicsWorld.AddConstraint(new Spring(particles[i], particles[i - 1]));
			}
			else
			{
				mPhysicsWorld.AddConstraint(new Fixed(particles[i]));
			}
		}

		auto p3 = new Particle();
		p3->SetPosition(Vector3{ 0.0f,static_cast<float>(size),0.0f });
		p3->SetVelocity(1.2f);
		p3->radius = 1.0f;
		p3->bounce = 0.3f;
		mPhysicsWorld.AddParticle(p3);
		mPhysicsWorld.AddConstraint(new Spring(particles[particles.size() - 1], p3));
	}
	ImGui::End();
}