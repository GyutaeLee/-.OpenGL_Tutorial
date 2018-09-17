#ifndef __GPED_PWORLD_H__
#define __GPED_PWORLD_H__

#include <vector>

#include "GPED_Pcontact.h"
#include "GPED_Pfgen.h"

namespace GPED
{
	/*
	��ƼŬ ��Ʈ�� �����ϰ� ��� ��ƼŬ�� ������Ʈ �� �� �ִ� ����� �����Ѵ�
	*/
	class ParticleWorld
	{
	public:
		typedef std::vector<GPEDParticle*> Particles;
		typedef std::vector<ParticleContactGenerator*> ContactGenerators;
	
	protected:
		/*
		��ƼŬ���� �����Ѵ�
		*/
		Particles particles;

		/*
		���谡 �� �����ӿ��� ���� �����ڿ��� �ִ� �ݺ� Ƚ����
		����ؾ��ϴ� ��쿡 true�̴�
		*/
		bool calculaterations;

		/*
		��ƼŬ�� �� �߻��⸦ �����Ѵ�
		*/
		ParticleForceRegistry registry;

		/*
		contact�� ���� resolver�� �����Ѵ�
		*/
		ParticleForceRegistry resolver;

		/*
		contact �߻���
		*/
		ContactGenerators contactGenerators;

		/*
		contact�� ����� �����Ѵ�
		*/
		ParticleContact* contacts;

		/*
		���Ǵ� �ִ� contact ���� ��Ÿ���� (��, contact �迭�� ũ��)
		*/
		unsigned maxContacts;
	
	public:
		/*
		������ �� �־��� ������ ������ ó���� �� �ִ� ���ο� ��ƼŬ �ùķ����͸� �����Ѵ�
		����, ���������� ����� contact Ȯ�� �ݺ� Ƚ���� ������ �� �ִ�
		�ݺ� Ƚ���� �������� ������ contact ���� �� �� ���ȴ�
		*/
		ParticleWorld(unsigned maxContacts, unsigned iterations = 0);
		~ParticleWorld();

		/*
		��ϵ� �� contact �����ڿ��� ������ �ش� contact�� �����Ѵ�
		������ contact ���� ��ȯ�Ѵ�
		*/
		unsigned generateContacts();


		/*
		�� ������ ��� ��ƼŬ�� �־��� �ð���ŭ ������ �����Ѵ�
		*/
		void runPhysics(real duration);

		/*
		�ùķ��̼� �������� ���� ���踦 �ʱ�ȭ�Ѵ�
		�̰��� ������ ��ƼŬ�� ���� ������ ���� �����Ѵ�
		�̰��� ȣ�� �� �Ŀ� ��ƼŬ�� �� �����ӿ� ���� ���� ���� �� �ִ�
		*/
		void startFrame();


		/*
		��ƼŬ�� ����� �����Ѵ�
		*/
		Particles& getParticles();

		/*
		contact ������ ����� �����Ѵ�
		*/
		ContactGenerators& getContactGenerators();

		/*
		force ������Ʈ���� �����ش�
		*/
		ParticleForceRegistry& getForceRegistry();
	};

	/*
	��ƼŬ �������� STL ���͸� �����ͼ� ����� �浹��Ű�� ���� �������̴�
	*/
	class GroundContacts : public ParticleContactGenerator
	{
		ParticleWorld::Particles* particles;

	public:
		void init(ParticleWorld::Particles* particles);

		virtual unsigned addContact(ParticleContact* contact, unsigned limit) const;
	};

}

#endif