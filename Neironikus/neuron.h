#pragma once
class neuron //Класс нейрон 
{
public:
	double weight;
	double input[2];
	double output(double in[]);
};
