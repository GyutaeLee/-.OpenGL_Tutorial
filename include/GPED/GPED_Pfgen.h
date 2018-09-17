#ifndef __GPED_PFGEN_H__
#define __GPED_PFGEN_H__

/*
�� ������(force generator)�� ���� �ϳ����� �߰����� �� ���� ��ƼŬ�� �߰������� ���´�
*/

#include "GPED_Particle.h"
#include <vector>

namespace GPED
{
	class ParticleForceGenerator
	{
		/*
		�־��� ��ƼŬ�� ����� ���� ����ϰ� ������Ʈ�ϱ� ����
		�������̽��� �������� �����ε��ض�
		*/
		virtual void updateForce(GPEDParticle* particle, real duration) = 0;
	};

	/*
	�߷��� ���ϴ� �� ������
	�ϳ��� �ν��Ͻ��� ���� ���� ���ڿ� ����� �� �ִ�
	*/
	class ParticleGravity : public ParticleForceGenerator
	{
		/*	�߷� ���ӵ��� �����Ѵ� */
		glm::vec3 gravity;
	public:
		/* �־��� ���ӵ��� �߻��⸦ �����Ѵ� */
		ParticleGravity(const glm::vec3& gravity);

		/* �־��� ��ƼŬ�� �߷� ���� �����Ѵ� */
		virtual void updateForce(GPEDParticle* particle, real duration);
	};

	/*
	�� �߻���� �������� �����Ѵ�
	�ϳ��� �ν��Ͻ��� ���� ��ƼŬ�� �̿�� �� �ִ�
	*/
	class ParticleDrag : public ParticleForceGenerator
	{
		/* �ӵ� ���� ����� ���� */
		real k1;

		/* �ӵ� ���� ���� ����� ���� */
		real k2;

	public:
		/* �־��� ����� �°� �߻��⸦ �����Ѵ� */
		ParticleDrag(real k1, real k2);

		/* �־��� ��ƼŬ�� �������� �����Ѵ� */
		virtual void updateForce(GPEDParticle* particle, real duration);
	};

	/*
	�������� ���� ���ϴ� �� �߻���
	*/
	class ParticleSpring : public ParticleForceGenerator
	{
		/* �������� �ٸ� ���� �ִ� ���� */
		GPEDParticle* other;

		/* ������ ����� �����Ѵ� */
		real springConstant;

		/* �������� ������ ���̸� �����Ѵ� */
		real restlength;

	public:
		/* �־��� �Ű��������� ���ο� �������� �����Ѵ� */
		ParticleSpring(GPEDParticle* other, real springConstant, real restLength);

		/* �־��� ��ƼŬ�� �������� ���� �����Ѵ� */
		virtual void updateForce(GPEDParticle* particle, real duration);
	};

	/*
	���� ���� ������ ���� ���� �����Ǵ� ������ ���� �����ϴ� �� �߻���
	*/
	class ParticleAnchoredSpring : public ParticleForceGenerator
	{
	protected:
		/* ������ ���� ���� ��ġ */
		glm::vec3* anchor;

		/*������ ����� �����Ѵ� */
		real springConstant;

		/* �������� ������ ���̸� �����Ѵ� */
		real restLength;

	public:
		/* �־��� �Ű��������� ���ο� �������� �����Ѵ� */
		ParticleAnchoredSpring(glm::vec3* anchor, real springConstant, real restLength);

		/* �־��� ��ƼŬ�� �������� ���� �����Ѵ� */
		virtual void updateForce(GPEDParticle* particle, real duration);

		/* ��Ŀ ����Ʈ �˻� */
		const glm::vec3* getAnchor() const { return anchor; }
	};

	/*
	Ȯ��Ǿ��� ���� ������ ���� �����ϴ� �� �߻���
	*/
	class ParticleBungee : public ParticleForceGenerator
	{
		/* �������� �ٸ� ���� �ִ� ��ƼŬ */
		GPEDParticle* other;

		/* ������ ����� �����Ѵ� */
		real springConstant;

		/*
		���� �߻���Ű�� ������ �������� ������ ���̸� �����Ѵ�
		*/
		real restLength;

	public:
		/* �־��� �Ű��������� ���ο� ������ �����Ѵ� */
		ParticleBungee(GPEDParticle* other, real springConstant, real restLength);

		/* �־��� ��ƼŬ�� ������ ���� �����Ѵ� */
		virtual void updateForce(GPEDParticle* particle, real duration);
	};

	/*
	���� ���� �����ϴ� �� �߻���.
	���� ���� ������ ���� ���� ����ȴ�
	*/
	class ParticleAnchoredBungee : public ParticleAnchoredSpring
	{
	public:
		/* ParticleAnchoredSpring�� �ʱ�ȭ ���α׷��� ����ض� */
		using ParticleAnchoredSpring::ParticleAnchoredSpring;

		/* �־��� ��ƼŬ�� ������ ���� �����ض� */
		virtual void updateForce(GPEDParticle* particle, real duration);
	};

	/*
	XZ ��鿡 ������ ��ü ��鿡 ���� �η��� �����ϴ� �� �߻���
	*/
	class ParticleBuoyancy : public ParticleForceGenerator
	{
		/*
		��ü�� �ִ� �η��� �߻���Ű�� ���� ��ü�� �ִ� ħ�� ����
		*/
		real maxDepth;

		/*
		��ü�� ����
		*/
		real volume;

		/*
		y = 0 ���� ���� ����� ����
		����� XZ ��鿡 �����ϴ�
		*/
		real waterHeight;

		/*
		��ü�� �е�
		������ ���� �е��� 1000kg/m^3�̴�
		*/
		real liquidDensity;

	public:
		/* �־��� �Ű� ������ ���ο� �η��� �����Ѵ� */
		ParticleBuoyancy(real maxDepth, real volume, real waterHeight, real liquidDensity = 1000.0f);

		/* �־��� ���ڿ� �η��� �����Ѵ� */
		virtual void updateForce(GPEDParticle* particle, real duration);
	};

	/*
	��� �� ������� �׵��� �����ϴ� ��ƼŬ�� �����Ѵ�
	*/
	class ParticleForceRegistry
	{
	protected:
		/*
		�־��� �� �����Ⱑ �־��� ���ڿ� ����ǵ��� ����Ѵ�
		*/
		void add(GPEDParticle* particle, ParticleForceGenerator* fg);

		/*
		������ ��� ���� ������Ʈ������ �����Ѵ�
		�� ��ϵǾ� ���� ���� ���, �� �޼ҵ�� �ƹ��͵� ���� �ʴ´�
		*/
		void remove(GPEDParticle* particle, ParticleForceGenerator* fg);

		/*
		������Ʈ���� ��� ����� �����
		�̷����ϸ� ��ƼŬ�̳� �� ������ ��ü�� �������� ������ ���� ���� ��ϸ� �����ȴ�
		*/
		void clear();

		/*
		��� �� �����⸦ ȣ���� �ش� ������ ���� ������Ʈ�Ѵ�
		*/
		void updateForces(real duration);
	};

}

#endif