#ifndef __GPED_PRECISON_H__
#define __GPED_PRECISON_H__

#include <limits>

namespace GPED
{
#if 1
	/*
	���Ǻ� �������� �ʿ��� ��� �ڵ忡 ���� single ���е� ���� �����Ѵ�
	*/
#define SINGLE_PRECISION
	
	/*
	real ���е��� �����Ѵ�. ����Ŭ���� �� single �Ǵ� double ���е� �������� ������ �� �� �ִ�.
	�⺻�����δ� �� ���е��� �����ȴ�.
	*/
	typedef float real;

	/* ���� ���� ���� real number�� �����Ѵ� */
#define REAL_MAX FLT_MAX

	/* square root �������� ���е��� �����Ѵ� */
#define real_sqrt sqrtf
	/* ���밪 �������� ���е��� �����Ѵ� */
#define real_abs fabsf
	/* sin �������� ���е��� �����Ѵ� */
#define real_sin sinf

	/* cos �������� ���е��� �����Ѵ� */
#define real_cos cosf

	/* exponent �������� ���е��� �����Ѵ� */
#define real_exp expf
	/* power �������� ���е��� �����Ѵ� */
#define real_pow powf
	/* floating point modulo �������� ���е��� �����Ѵ� */
#define real_fmod fmodf

	/* 1+e == 1 �� e �� �����Ѵ� */
#define real_epsilon FLT_EPSILON

#define R_PI 3.14159f

#else
#define DOUBLE_PRECISION
	typedef double real;
#define REAL_MAX DBL_MAX
#define real_sqrt sqrt
#define real_abs fabs
#define real_sin sin
#define real_cos cos
#define real_exp exp
#define real_pow pow
#define real_fmod fmod
#define real_epsilon DBL_EPSILON
#define R_PI 3.14159265358979
#endif

}

#endif