#include "pch.h"
#include <iostream>
#include <ctime>
#include <cmath>
#include "neuron.h"
#include "output_neuron.h"
#include "input_neuron.h"
#include "shift_neuron.h"
#define activation_function(x) (1/(1+exp(-(x)))) //Фунция активации
#define derivative_function(x) (exp(-(x))/pow(1+exp(-(x)),2)) //Производная функции
using namespace std;

void input_hidden(int value_neuron,neuron arr_neurons[],input_neuron arr_input_neurons[],shift_neuron arr_shift_neuron[])/*1->2 Выход input_neiron к Входу neiron*/
{
		for (int i = 0; i < value_neuron; i++)
		{
			for (int k = 0; k < value_neuron; k++)
			{
				arr_neurons[i].input[k] = arr_input_neurons[k].weight[i] * arr_input_neurons[k].input;
			}
			arr_neurons[i].input[0] += arr_shift_neuron[0].weight[i] * arr_shift_neuron[0].input;
		}
}
void hidden_output(int value_neuron,output_neuron &answerer, neuron arr_neurons[])/*2->3 Выход neiron к Входу output_neiron*/
{
	for (int i = 0; i < value_neuron; i++) 
	{
		answerer.input += arr_neurons[i].output(arr_neurons[i].input)*arr_neurons[i].weight;
	}
}
void ansewer_output(output_neuron &answerer,shift_neuron arr_shift_neuron[])/*Конечное значение*/
{
	answerer.input += arr_shift_neuron[1].weight[0]* arr_shift_neuron[1].input;
	answerer.output = activation_function(answerer.input);
}
void info(int value_neuron,int value_input_neuron,input_neuron arr_input_neurons[],neuron arr_neurons[],output_neuron answerer,int j,int era,float net_answers[],shift_neuron arr_shift_neuron[])
{
	cout<<"Номер сета-> "<<j+1<<endl;
	switch (j)
	{
	case(0):
		/*cout << "Сигнал-> [0:0]" << endl;*/
		break;
	case(1):
		/*cout << "Сигнал-> [1:0]" << endl;*/
		break;
	case(2):
		/*cout << "Сигнал-> [0:1]" << endl;*/
		break;
	case(3):
		/*cout << "Сигнал-> [1:1]" << endl;*/
		break;
	default:
		break;
	}
	cout << "Веса слоя сдвига->" << endl;
		cout << arr_shift_neuron[0].weight[0] << endl;
		cout << arr_shift_neuron[0].weight[1] << endl;
		cout << arr_shift_neuron[1].weight[0] << endl;
	cout << "Веса входного слоя->" << endl;
	for (int i = 0; i < value_input_neuron; i++)
	{
		for (int k = 0; k < 2; k++)
		{
			cout << arr_input_neurons[i].weight[k] << endl;
		}
	}
	cout << "Веса скрытого слоя->" << endl;
	for (int i = 0; i < value_neuron; i++)
	{
		cout << arr_neurons[i].weight << endl;
	}
	/*cout << "\n" << activation_function(0) << "<- Логический сигмоид f(x) [x=0]" << "\n" << derivative_function(0) << "<- Производная f'(x) [x=0]" << "\n" << endl;*/
	cout << "Input H" << endl;
	for (int i = 0; i < value_neuron; i++)
	{
		for (int k = 0; k < value_input_neuron; k++)
		{
			cout << arr_neurons[i].input[k] << endl;
		}
	}

	cout << "Output H" << endl;
	for (int i = 0; i < value_neuron; i++)
	{
		cout << arr_neurons[i].output(arr_neurons[i].input)*arr_neurons[i].weight << endl;
	}
	cout << "Input O\n" << answerer.input << endl;

	cout << "Output O\n"<< net_answers[j] << endl;
	cout << "Эпоха ->" << era << endl;
	cout << "--------------------------------------------------------------" << endl;
}
void weights_calibration(input_neuron arr_input_neurons[], neuron arr_neurons[], shift_neuron arr_shift_neuron[],float mse,bool true_answer[],float net_answers[])
{

}

int main()
{
	float mse, net_answers[4] = {0,0,0,0};
	setlocale(LC_ALL, "Rus");
	int era = 0;
	bool set[4][2] = { {0,0}, {1,0}, {0,1}, {1,1} }, true_answer[4] = { 0,1,1,0 };
	const int value_neuron = 2, value_input_neuron = 2,value_shift_neuron=2;
	neuron arr_neurons[value_neuron]; //Количество нейронов 
	input_neuron arr_input_neurons[value_input_neuron];// Количество вхоных нейронов
	output_neuron answerer; //Выходной неирон
	shift_neuron arr_shift_neuron[value_shift_neuron];// Неройны сдвига
	answerer.input = 0;
	//srand(time(NULL));
	if (era == 0)
	{
		for (int i = 0; i < value_neuron; i++)/*Заполнение весов нейрона, случайными числами*/
		{
			arr_neurons[i].weight = rand() / double(RAND_MAX);
		}

		for (int i = 0; i < value_input_neuron; i++)/*Заполнение весов входного нейрона, случайными числами*/
		{
			for (int k = 0; k < value_neuron; k++)
			{
				arr_input_neurons[i].weight[k] = rand() / double(RAND_MAX);
			}
		}
		for (int i = 0; i < value_shift_neuron; i++)/*Заполнение весов нейрона сдвига, случайными числами*/
		{
			for (int k = 0; k < 2; k++)
			{
				arr_shift_neuron[i].weight[k] = rand() / double(RAND_MAX);
			}
		}
		arr_shift_neuron[1].weight[1] = arr_shift_neuron[1].weight[0];
	}
	do
	{
		for (int j = 0; j < 4; j++)
		{
		/*int j = 3;*/
			switch (j)
			{
			case(0):
				for (int i = 0; i < value_input_neuron; i++)// вход нейронов 2 значения
				{
					arr_input_neurons[i].input = set[j][i];
				}
				break;
			case(1):
				for (int i = 0; i < value_input_neuron; i++)// вход нейронов 2 значения
				{
					arr_input_neurons[i].input = set[j][i];
				}
				break;
			case(2):
				for (int i = 0; i < value_input_neuron; i++)// вход нейронов 2 значения
				{
					arr_input_neurons[i].input = set[j][i];
				}
				break;
			case(3):
				for (int i = 0; i < value_input_neuron; i++)// вход нейронов 2 значения
				{
					arr_input_neurons[i].input = set[j][i];
				}
				break;
			default:
				cout << "Error, тренировочный сет закончился i>3 Программа попала в default" << endl;
				break;
			}
			input_hidden(value_neuron, arr_neurons, arr_input_neurons, arr_shift_neuron);
			hidden_output(value_neuron, answerer, arr_neurons);
			ansewer_output(answerer, arr_shift_neuron);
			net_answers[j] = answerer.output;
			info(value_neuron, value_input_neuron, arr_input_neurons, arr_neurons, answerer, j, era, net_answers, arr_shift_neuron);
			answerer.output = 0;
			answerer.input = 0;	
		}
		era++;
		mse = (pow((true_answer[0] - net_answers[0]), 2) + pow((true_answer[1] - net_answers[1]), 2) + pow((true_answer[2] - net_answers[2]), 2) + pow((true_answer[3] - net_answers[3]), 2)) / 4;//Среднеквадратичная ошибка 
		cout <<"Cреднеквадратичная ошибка-> "<< mse<<"\n--------------------------------------------------------------" << endl;
		weights_calibration(arr_input_neurons, arr_neurons, arr_shift_neuron, mse, true_answer,net_answers);
	} while (false);
	system("pause");
}

