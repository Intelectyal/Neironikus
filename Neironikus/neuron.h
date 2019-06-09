#pragma once
class neuron //Класс нейрон 
{
public:
	float weight;
	float input[2];
	float output(float in[]);
};
