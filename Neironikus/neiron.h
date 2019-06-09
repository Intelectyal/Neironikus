#pragma once
class neiron //Класс нейрон 
{
public:
	float weight;
	float in[2];
	float axon(float in[], float weight);
};
