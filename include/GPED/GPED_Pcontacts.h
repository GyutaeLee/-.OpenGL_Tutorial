#ifndef __GPED_PCONTACTS_H__
#define __GPED_PCONTACTS_H__

#include "GPED_Particle.h"

namespace GPED
{
	/*
	Contact�� �� ��ü�� ��Ÿ���ϴ� (�� ��� �� ���� ���ڸ� ��Ÿ���� ParticleContact)
	��ȣ�ۿ��� �ذ��ϸ� ��ȣ ħ���� ���ŵǰ� ���� �����ϵ��� ����� �浿�� �ش�.
	�浹�ϴ� ��ü�� ���ٿ��� �� �ִ�.
	
	Contact���� ȣ�� �� �� �ִ� ����� ������ Contact�� �����ϸ� �����Ѵ�.
	Contact ��Ʈ�� �ذ��Ϸ��� Particle contact resolver Ŭ������ ����ض�.
	*/
	class ParticleContact
	{
		/*
		Contact resolver ��ü�� contact�� ��Ʈ�ϰ� ������ �ֱ� ����
		contact�� ���� ������ �ʿ��ϴ�.
		*/
		friend class ParticleContactResolver;

	public:
		/*
		Contact�� ���õ� ��ƼŬ�� �����Ѵ�.
		�� ��° �׸��� scenery�� ���õ� contact�� ��� NULL�� �� �� �ִ�.
		*/
		GPEDParticle* particle[2];

		/*
		contact�� �������� �ݹ� �Լ��� �����Ѵ�.
		*/
		real restitution;

		/*
		world ��ǥ�迡�� contact�� ������ �����Ѵ�.
		*/
		glm::vec3 contactNormal;

		/*
		contact���� ħ���� ���̸� �����Ѵ�.
		*/
		real penetration;

		/*
		��ȣ ħ�� �ذ��� �߻��ϴ� ���� ���ŵ� �� ��ƼŬ�� ���� �����Ѵ�.
		*/
		glm::vec3 particleMovement[2];

	protected:
		/*
		�ӵ��� ��ȣ ħ���� ���� �� contact�� �ذ��Ѵ�.
		*/
		void resolve(real duration);

		/*
		contact���� �и� �ӵ��� ����Ѵ�.
		*/
		real calculateSepartingVelocity() const;

	private:
		/*
		�� �浹�� ��� ����� �����Ѵ�.
		*/
		void resolveVelocity(real duration);

		/*
		�� contact�� ��ȣ �ۿ� �ذ��� �����Ѵ�.
		*/
		void resolveInterpenetration(real duration);
	};

	/*
	��ƼŬ contact�� ��ŭ contact �ذ��� ��ƾ�� ����.
	�ϳ��� reslover �ν��Ͻ��� ��ü �ùķ��̼ǿ��� ������ �� �ִ�.
	*/
	class ParticleContactResolver
	{
	protected:
		/*
		�㰡�� iteration�� ���ڸ� �����Ѵ�.
		*/
		unsigned iterations;

		/*
		�̰��� traking value�� �����̴� - �츮�� ���ǰ� �ִ� iteration�� ���� ���ڸ� ����� ���̴�.
		*/
		unsgiend iterationUsed;

	public:
		/*
		���ο� contact resolver�� �����Ѵ�.
		*/
		ParticleContactResolver(unsigned iterations);

		/*
		��밡���� iteration�� ���� �����Ѵ�.
		*/
		void setIterations(unsigned iterations);

		/*
		ħ�� �� �ӵ��� ���� ���� ���� ��Ʈ�� �ؼ��Ѵ�.		
		*/
		void resolveContacts(ParticleContact* contactArray, unsigned numContacts, real duration);
	};

	/*
	�̰��� ��ƼŬ�� �����ϴ� ���� �������� �⺻ ���� �������̽��̴�.
	*/
	class ParticleContactGenerator
	{
	public:
		/*
		�־��� contact ������ ������ contact�� ä���.
		contact pointer�� contact �迭�� ù ��° ��� ������ ������
		�����Ѿ��Ѵ�. ���⼭ limit�� �� �� �ִ� �迭�� �ִ� ���� ���̴�.
		�޼ҵ�� ������ �Լ� ���� ��ȯ�Ѵ�.
		*/
		virtual unsigned addContact(ParticleContact* contact, unsigned limit) const = 0;
	};

}