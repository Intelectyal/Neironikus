#pragma once
class neuron //����� ������ 
{
public:
	double weight;
	double input[2];
	double output(double in[]);
};
