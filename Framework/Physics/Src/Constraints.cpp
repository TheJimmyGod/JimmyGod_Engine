#include "Precompiled.h"
#include "Constraints.h"

using namespace JimmyGod;
using namespace JimmyGod::Physics;

Spring::Spring(Particle * a, Particle * b, float restLength)
	: mParticleA(a), mParticleB(b), mRestLength(restLength)
{
	if (restLength <= 0.0f)
		mRestLength = Math::Magnitude(a->position - b->position);
}

void Spring::Apply() const
{
	Math::Vector3 delta = mParticleB->position - mParticleA->position;
	float dist = Math::Magnitude(delta);
	float diff = (dist - mRestLength) / (dist * (mParticleA->invMass + mParticleB->invMass));
	mParticleA->position += delta * diff * mParticleA->invMass;
	mParticleB->position -= delta * diff * mParticleB->invMass;
}

void Spring::DebugDraw() const
{
	Graphics::SimpleDraw::AddLine(
		mParticleA->position,
		mParticleB->position,
		Graphics::Colors::Cyan
	);
}

Fixed::Fixed(Particle * p) : Fixed(p,p->position)
{
}

Fixed::Fixed(Particle * p, const Math::Vector3 & position)
	: mParticle(p), mPosition(position)
{
}

void Fixed::Apply() const
{
	mParticle->SetPosition(mPosition);
}

void Fixed::DebugDraw() const
{
	Graphics::SimpleDraw::AddAABB(mPosition, mParticle->radius, Graphics::Colors::Cyan);
}

void Fixed::SetPosition(const Math::Vector3 & position)
{
	mPosition = position;
}

// Homework:
// Add the following:
// - Tetrahedrons
// - Cubes (how many constraints do you need?)
// - Ball and Chain
// - Cloth
// - And if you are up for the challenge, build 