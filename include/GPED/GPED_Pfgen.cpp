#include "GPED_Pfgen.h"

using namespace GPED;

GPED::ParticleGravity::ParticleGravity(const glm::vec3 & gravity)
{
	ParticleGravity::gravity = gravity;
}

void GPED::ParticleGravity::updateForce(GPEDParticle* particle, real duration)
{
	// ���� ������ ������ Ȯ���ض�
	if (!particle->hasFiniteMass()) return;

	// ��ƼŬ�� mass-scaled �����Ѵ�
	particle->addForce(gravity*particle->getMass());
}

GPED::ParticleDrag::ParticleDrag(real k1, real k2)
{
	ParticleDrag::k1 = k1;
	ParticleDrag::k2 = k2;
}

void GPED::ParticleDrag::updateForce(GPEDParticle * particle, real duration)
{
	glm::vec3 force = particle->getVelocity();

	// �� ���� ����� ����ض�
	real dragCoeff = glm::length(force);
	dragCoeff = k1 * dragCoeff + k2 * dragCoeff * dragCoeff;

	// ���� ���� ����ϰ� �����ض�
	force = glm::normalize(force);
	force *= -dragCoeff;
	particle->addForce(force);
}

GPED::ParticleSpring::ParticleSpring(GPEDParticle* other, real springConstant, real restLength)
{
	ParticleSpring::other = other;
	ParticleSpring::springConstant = springConstant;
	ParticleSpring::restlength = restlength;
}

void GPED::ParticleSpring::updateForce(GPEDParticle* particle, real duration)
{
	// �������� ���͸� ����Ѵ�
	glm::vec3 force = particle->getPosition();
	force -= *anchor;

	// ���� ũ�⸦ ����Ѵ�
	real magnitude = glm::length(force);
	magnitude = real_abs(magnitude - restLength); // �������� ���δ� cable rope bridge�� �����ϰ� ������ �� ������ �����ؾ��Ѵٰ� �����Ѵٰ� �Ѵ�
	magnitude *= springConstant;

	// ������ ���� ����ϰ� �����ض�
	force = glm::normalize(force);
	force *= -magnitude;
	particle->addForce(force);
}

GPED::ParticleBungee::ParticleBungee(GPEDParticle* other, real springConstant, real restLength)
{
	ParticleBungee::other = other;
	ParticleBungee::springConstant = springConstant;
	ParticleBungee::restLength = restLength;
}

void GPED::ParticleBungee::updateForce(GPEDParticle* particle, real duration)
{
	// �������� ���͸� ����Ѵ�
	glm::vec3 force = particle->getPosition();
	force -= other->getPosition();

	// ������ ����Ǿ� �ִ��� Ȯ���ض�
	real magnitude = glm::length(force);
	if (magnitude <= restLength) return;

	// ���� ũ�⸦ ����ض�
	magnitude = springConstant * (restLength - magnitude);

	// ������ ���� ����ϰ� �����ض�
	force = glm::normalize(force);
	force *= -magnitude;
	particle->addForce(force);
}

void GPED::ParticleAnchoredBungee::updateForce(GPEDParticle* particle, real duration)
{
	// �������� ���͸� ����Ѵ�
	glm::vec3 force = particle->getPosition();
	force -= *anchor;
	
	// ���� ũ�⸦ ����Ѵ�
	real magnitude = glm::length(force);
	if (magnitude < restLength) return;

	magnitude = springConstant * (magnitude - restLength);

	// ������ ���� ����ϰ� �����Ѵ�
	force = glm::normalize(force);
	force *= -magnitude;
	particle->addFroce(force);
}

GPED::ParticleBuoyancy::ParticleBuoyancy(real maxDepth, real volume, real waterHeight, real liquidDensity)
{
	ParticleBuoyancy::maxDepth = maxDepth;
	ParticleBuoyance::volume = volume;
	ParticleBuoyance::waterHeight = waterHeight;
	ParticleBuoyancy::liquidDensity = liquidDensity;
}

void GPED::ParticleBuoyany::updateForce(GPEDParticle* particle, real duration)
{
	// ��� ���̸� ����Ѵ�
	real depth = particle->getPosition().y;

	// �� ������ �������� �����Ѵ�
	if (depth >= waterHeight + maxDepth) return;
	glm::vec3 force(0.0);

	// �ִ� ���̿� �ִ��� �ƴ��� �˻��Ѵ�
	if (depth <= waterHeight - maxDepth)
	{
		force.y = liquidDensity * volume;
		particle->addForce(force);
		return;
	}

	// �׷��� �ʴٸ� �츮�� �κ������� ���ӿ� ��� ���̴�
	force.y = liquidDensity * volume*(depth - maxDepth - waterHeight) / 2 * maxDepth;
	particle->addForce(force);
}

void GPED::ParticleForceRegistry::add(GPEDParticle* particle, ParticleForceGenerator* fg)
{
	registrations.push_back(ParticleForceRegistration{ particle,fg });
}

void GPED::ParticleForceRegistry::remove(GPEDParticle* particle, ParticleForceGenerator* fg)
{
	Registry::iterator i = registrations.begin();
	for (; i != registrations.end(); ++i)
		if (i->particle == particle && i->fg == fg)
			registrations.erase(i);
}

void GPED::ParticleForceRegistry::clear()
{
	registrations.clear();
}

void GPED::ParticleForceRegistry::updateForces(real duration)
{
	Registry::iterator i = registrations.begin();
	for (; i != registrations.end(); ++i)
		i->fg->updateForce(i->particle, duration);
}