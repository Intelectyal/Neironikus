#include "pch.h"
#include <cmath>
#include "neuron.h"
#define activation_function(x) (1/(1+exp(-x))) //Фунция активации

//Класс нейрон 


	
	float neuron::output(float in[])
	{
		float x = 0;
		for (int i = 0; i < 2; i++)
		{
			x += in[i];
		}
		return(activation_function(x));
	}

