#include "pch.h"
#include <cmath>
#include "neiron.h"
#define activation_function(x) (1/(1+exp(-(x)))) //������ ���������

//����� ������ 


	
	float neiron::axon(float in[], float weight)
	{
		float x = 0;
		for (int i = 0; i < 2; i++)
		{
			x += weight * in[i];
		}
		return(activation_function(x));
	}

