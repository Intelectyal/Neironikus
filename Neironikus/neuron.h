#pragma once
class neuron //����� ������ 
{
public:
	float weight;
	float input[2];
	float output(float in[]);
};
