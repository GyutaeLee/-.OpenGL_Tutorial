#version 330 core
out vec4 FragColor;
in vec3 WorldPos;

uniform samplerCube environmentMap;

const float PI = 3.14159265359;

void main()
{
	// ���� ���ʹ� ������ ���� ǥ���� �������� �ۿ��� WorldPos�� ���ĵȴ�
	// �� ������ ������ ��, ȯ���� ��� ������ ���� ����ض�
	// �� ������ ����� �츮�� PBR ���̴����� ��緮�� ���ø��ϴµ� ����ϴ�
	// Normal direction���� ���� ���� �����̴�
	vec3 N = normalize(WorldPos);

	vec3 irradiance = vec3(0.0);

	// tangent space calculation from origin point
	vec3 up = vec3(0.0, 1.0, 0.0);
	vec3 right = cross(up, N);
	up = cross(N, right);

	float sampleDelta = 0.025;
	float nrSamples = 0.0;
	for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
	{
		for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
		{
			// �������� ���� ��ǥ (ź��Ʈ ��������)
			vec3 tangentSample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
			// ���踦 ���� ���� ��������
			vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N;

			irradiance += texture(environmentMap, sampleVec).rgb * cos(theta) * sin(theta);
			nrSamples++;
		}
	}
	irradiance = PI * irradiance * (1.0 / float(nrSamples));

	FragColor = vec4(irradiance, 1.0);
}