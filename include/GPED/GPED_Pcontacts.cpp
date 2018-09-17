#include "GPED_Pcontacts.h"
using namespace GPED;

void GPED::ParticleContact::resolve(real duration)
{
	resolveVelocity(duration);
	resolveInterpenetration(duration);
}

real GPED::ParticleContact::calculateSepartingVelocity() const
{
	glm::vec3 relativeVelocity = particle[0]->getVelocity();
	if (particle[1]) relativeVelocity -= particle[1]->getVeclocity();
	return glm::dot(relativeVelocity, contactNormal);
}

void GPED::ParticleContact::resolveVelocity(real duration)
{
	// contact ������ �ӵ��� ���Ѵ�
	real separatingVelocity = calculateSepartingVelocity();

	// �ذ��� �ʿ䰡 �ִ��� ������ Ȯ���Ѵ�
	if (separatingVelocity > 0)
	{
		//contact�� �и��Ǿ��ְų� �����Ǿ��־ �ƹ��� �͵� �ʿ����� �ʴ�
		return;
	}

	// ���Ӱ� �и��� �ӵ��� ����Ѵ�
	real newSepVelocity = -separatingVelocity * restitution;

	// ���Ӹ����� �ӵ� ������ �����ض�
	glm::vec3 accCausedVelocity = particle[0]->getAcceleration();
	if (particle[1]) accCausedVelocity -= particle[1]->getAcceleration();
	real accCausedSepVelocity = glm::dot(accCausedVelocity, contactNormal) * duration;

	// ���ӵ� ��� ������ closing �ӵ��� �ִٸ�,
	// ���ο� �и� �ӵ����� �����Ѵ�
	if (accCausedSepVelocity < 0)
	{
		newSepVelocity += restitution * accCausedSepVelocity;

		// �츮�� ���� �� ������ �� ���� �ű��� �ʾҴ��� Ȯ���ض�
		if (newSepVelocity < 0) newSepVelocity = 0;
	}

	real deltaVelocity = newSepVelocity - separatingVelocity;

	// �� ������ ����Ͽ� �� ��ü�� �ӵ� ��ȭ�� �����Ѵ�
	// (��, �� ������ ���� ��ü�� �ӵ� ��ȭ�� ����)
	real totalInverseMass = particle[0]->getInverseMass();
	if (particle[1]) totalInverseMass += particle[1]->getInverseMass();

	// ��� ���ڰ� ���� ������ ���´ٸ�, ����� ȿ���� ����
	if (totalInverseMass <= 0) return;

	// ������ �浹�� ����ض�
	real impulse = deltaVelocity / totalInverseMass;
	if (particle[0] && particle[1])
	{
		impulse *= particle[0]->getInverseMass()*particle[1]->getInverseMass();
	}

	// �� ������ ���� �� �浹 ���� ã�´�
	glm::vec3 impulsePerIMass = contactNormal * impulse;

	// ����� ���ض�: �׵��� ������ �������� ����ǰ� �� ������ ����Ѵ�
	particle[0]->setVelocity
	(
		particle[0]->getVeclocity() + impulsePerIMass * particle[0]->getInverseMass()
	);
	if (particle[1])
	{
		// ���� 1�� �ݴ� �������� ����
		particle[1]->setVelocity
		(
			particle[1]->setVelocity() + impulsePerIMass * -particle[0]->getInverseMass()
		);
	}
}

void GPED::ParticleContact::resolveInterpenetration(real duration)
{
	// ħ���� ���ٸ� �� �ܰ踦 �ǳʶڴ�
	if (penetration <= 0) return;

	// �� ��ü�� �������� �� ������ ������� �Ѵ�
	real totalInverseMass = particle[0]->getInverseMass();
	if (particle[1]) totalInverseMass += particle[1]->getInverseMass();

	// ��� ���ڰ� ���� ������ ������, �ƹ��͵� ���� �ʴ´�
	if (totalInverseMass <= 0) return;

	// �� ������ ������ ħ�� ���ؾ��� �ľ��Ѵ�
	glm::vec3 movePerIMass = contactNormal * (penetration / totalInverseMass);

	// �̵����� ����Ѵ�
	particleMovement[0] = movePerIMass * particle[0]->getInverseMass();
	if (particle[1])
		particleMovement[1] = movePerIMass * -particle[1]->getInverseMass();
	else
		particleMovement[1] = glm::vec3(0.0);

	// ħ�� �ػ� ����
	particle[0]->setPosition
	(
		particle[0]->getPosition() + particleMovement[0]
	);
	if (particle[1])
	{
		particle[1]->setPosition
		(
			particle[1]->getPosition() + particleMovement[1]
		);
	}
}

GPED::ParticleContactResolver::ParticleContactResolver(unsigned iterations)
	:iterations(iterations), iterationUsed(0)
{ }

void GPED::ParticleContactResolver::setIterations(unsigned iterations)
{
	ParticleContactResolver::iterations = iterations;
}

void GPED::ParticleContactResolver::resolveContacts(ParticleContact* contactArray, unsigned numContacts, real duration)
{
	unsigned i;

	iterationUsed = 0;
	while (iterationUsed < iterations)
	{
		// ���� ū closing �ӵ��� ������ ã�ƶ�
		real max = REAL_MAX;
		unsigned maxIndex = numContacts;
		for (i = 0; i < numContacts; ++i)
		{
			real sepVel = contactArray[i].calculateSepartingVelocity();
			if (sepVel < max && (sepVel < 0 || contactArray[i].penetration>0))
			{
				max = sepVel;
				maxIndex = i;
			}
		}

		// �ذ��Ҹ��� ��ġ�� �ִ� ���� �ִ��� ����
		if (maxIndex == numContacts) break;

		// �� contact�� �ذ��Ѵ�
		contactArray[maxIndex].resolve(duration);

		// ��� ��ƼŬ�� ���� ��ȣ ħ���� ������Ʈ�Ѵ�
		glm::vec3* move = contactArray[maxIndex].particleMovement;
		for (i = 0; i < numContacts; ++i)
		{
			if (contactArray[i].particle[0] == contactArray[maxIndex].particle[0])
			{
				contactArray[i].penetration -= glm::dot(move[0], contactArray[i].contactNormal);
			}
			else if (contactArray[i].particle[0] == contactArray[maxIndex].particle[1])
			{
				contactArray[i].penetration -= glm::dot(move[1], contactArray[i].contactNormal);
			}

			if (contactArray[i].particle[1])
			{
				if (contactArray[i].particle[1] == contactArray[maxIndex].particle[0])
				{
					contactArray[i].penetration += glm::dot(move[0], contactArray[i].contactNormal);
				}
				else if (contactArray[i].particle[1] == contactArray[maxIndex].particle[1])
				{
					contactArray[i].penetration += glm::dot(move[1], contactArray[i].contactNormal);
				}
			}
		}
		++iterationUsed;
	}
}