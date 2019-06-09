#include "pch.h"
#include <iostream>
#include <ctime>
#include <cmath>
#include "neiron.h"
#include "output_neiron.h"
#include "input_neiron.h"
#define activation_function(x) (1/(1+exp(-(x)))) //Фунция активации
#define derivative_function(x) (exp(-(x))/pow(1+exp(-(x)),2)) //Производная функции
using namespace std;

void input_hidden(int value_input_neiron,int value_neiron,neiron arr[],input_neiron inarr[])/*1->2 Выход input_neiron к Входу neiron*/
{
	for (int i = 0; i < value_neiron; i++) 
	{
		for (int k = 0; k < value_input_neiron; k++)
		{
			arr[i].in[k] = inarr[k].weight*inarr[k].in;
		}
	}
}
void hidden_output(int value_neiron,output_neiron &ansewer, neiron arr[])/*2->3 Выход neiron к Входу output_neiron*/
{
	for (int i = 0; i < value_neiron; i++) 
	{
		ansewer.in += arr[i].axon(arr[i].in, arr[i].weight);
	}
}
void ansewer_output(output_neiron &ansewer)/*Конечное значение*/
{
	ansewer.out = activation_function(ansewer.in);
}
void info(int value_neiron,int value_input_neiron,input_neiron inarr[],neiron arr[],output_neiron ansewer,int j)
{
	cout<<"Номер сета-> "<<j+1<<endl;
	switch (j)
	{
	case(0):
		cout << "Сигнал-> [0:0]" << endl;
		break;
	case(1):
		cout << "Сигнал-> [1:0]" << endl;
		break;
	case(2):
		cout << "Сигнал-> [0:1]" << endl;
		break;
	case(3):
		cout << "Сигнал-> [1:1]" << endl;
		break;
	default:
		break;
	}
	cout << "Входной слой (веса)->" << endl;
	for (int i = 0; i < value_input_neiron; i++)
	{
		cout << inarr[i].weight << endl;
	}
	cout << "Скрытый слой (веса)->" << endl;
	for (int i = 0; i < value_neiron; i++)
	{
		cout << arr[i].weight << endl;
	}
	//cout << "\n" << activation_function(0) << "<- Логический сигмоид f(x) [x=0]" << "\n" << derivative_function(0) << "<- Производная f'(x) [x=0]" << "\n" << endl;
	cout << "Входные данные скрытого слоя" << endl;
	for (int i = 0; i < value_neiron; i++)
	{
		for (int k = 0; k < value_input_neiron; k++)
		{
			cout << arr[i].in[k] << endl;
		}
	}

	cout << "выходы скрытого слоя" << endl;
	for (int i = 0; i < value_neiron; i++)
	{
		cout << arr[i].axon(arr[i].in, arr[i].weight) << endl;
	}
	cout << "входные данные скрытого слоя\n" << ansewer.in << endl;

	cout << "Выходы скрытого слоя\n" << ansewer.out << endl;
	cout << "--------------------------------------------------------------" << endl;
}

int main()
{
	float mse;
	setlocale(LC_ALL, "Rus");
	int era = 0;
	bool set[4][2] = { {0,0}, {1,0}, {0,1}, {1,1} }, true_out[4] = { 0,1,1,0 };
	const int value_neiron = 4, value_input_neiron = 2;
	neiron arr[value_neiron]; //Количество нейронов 
	input_neiron inarr[value_input_neiron];// Количество вхоных нейронов
	output_neiron ansewer; //Выходной неирон
	ansewer.in = 0;
	//srand(time(NULL));
	if (era == 0)
	{
		for (int i = 0; i < value_neiron; i++)/*Заполнение весов нейрона, случайными числами*/
		{
			arr[i].weight = -100 + rand() % 200;
			arr[i].weight /= 100;
		}

		for (int i = 0; i < value_input_neiron; i++)/*Заполнение весов входного нейрона, случайными числами*/
		{
			inarr[i].weight = -100 + rand() % 200;
			inarr[i].weight /= 100;
		}
	}
	do
	{
		for (int j = 0; j < 4; j++)
		{
			switch (j)
			{
			case(0):
				for (int i = 0; i < value_input_neiron; i++)// вход нейронов 2 значения
				{
					inarr[i].in = set[j][i];
				}
				break;
			case(1):
				for (int i = 0; i < value_input_neiron; i++)// вход нейронов 2 значения
				{
					inarr[i].in = set[j][i];
				}
				break;
			case(2):
				for (int i = 0; i < value_input_neiron; i++)// вход нейронов 2 значения
				{
					inarr[i].in = set[j][i];
				}
				break;
			case(3):
				for (int i = 0; i < value_input_neiron; i++)// вход нейронов 2 значения
				{
					inarr[i].in = set[j][i];
				}
				break;
			default:
				cout << "Error, тренировочный сет закончился i>3 Программа попала в default" << endl;
				break;
			}
			input_hidden(value_input_neiron, value_neiron, arr, inarr);
			hidden_output(value_neiron, ansewer, arr);
			ansewer_output(ansewer);
			info(value_neiron, value_input_neiron, inarr, arr, ansewer, j);
		}
		//era++;
		
	} while (false);
	cin.get();
}

