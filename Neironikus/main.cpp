﻿#include "pch.h"
#include <iostream>
#include <ctime>
#include <cmath>
#include "neiron.h"
#include "output_neiron.h"
#include "input_neiron.h"
#define activation_function(x) (1/(1+exp(-(x)))) //Фунция активации
#define derivative_function(x) (exp(-(x))/pow(1+exp(-(x)),2)) //Производная функции
using namespace std;

void input_hidden(int value_neiron,neiron arr[],input_neiron inarr[])/*1->2 Выход input_neiron к Входу neiron*/
{
		for (int i = 0; i < value_neiron; i++)
		{
			for (int k = 0; k < value_neiron; k++)
			{
				arr[i].in[k] = inarr[k].weight[i] * inarr[k].in;
			}
		}
}
void hidden_output(int value_neiron,output_neiron &ansewer, neiron arr[])/*2->3 Выход neiron к Входу output_neiron*/
{
	for (int i = 0; i < value_neiron; i++) 
	{
		ansewer.in += arr[i].axon(arr[i].in)*arr[i].weight;
	}
}
void ansewer_output(output_neiron &ansewer)/*Конечное значение*/
{
	ansewer.out = activation_function(ansewer.in);
}
void info(int value_neiron,int value_input_neiron,input_neiron inarr[],neiron arr[],output_neiron ansewer,int j,int era,float net_ansewer[])
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
	cout << "Веса входного слоя->" << endl;
	for (int i = 0; i < value_input_neiron; i++)
	{
		for (int k = 0; k < 2; k++)
		{
			cout << inarr[i].weight[k] << endl;
		}
	}
	cout << "Веса скрытого слоя->" << endl;
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
		cout << arr[i].axon(arr[i].in)*arr[i].weight << endl;
	}
	cout << "входные данные отдаточного слоя\n" << ansewer.in << endl;

	cout << "Выходы отдаточного слоя\n"<< net_ansewer[j] << endl;
	cout << "Эпоха ->" << era << endl;
	cout << "--------------------------------------------------------------" << endl;
}
void weights_calibration()
{

}

int main()
{
	float mse, net_ansewer[4] = {0,0,0,0};
	setlocale(LC_ALL, "Rus");
	int era = 0;
	bool set[4][2] = { {0,0}, {1,0}, {0,1}, {1,1} }, true_out[4] = { 0,1,1,0 };
	const int value_neiron = 2, value_input_neiron = 2;
	neiron arr[value_neiron]; //Количество нейронов 
	input_neiron inarr[value_input_neiron];// Количество вхоных нейронов
	output_neiron ansewer; //Выходной неирон
	ansewer.in = 0;
	//srand(time(NULL));
	if (era == 0)
	{
		for (int i = 0; i < value_neiron; i++)/*Заполнение весов нейрона, случайными числами*/
		{
			arr[i].weight = -1000 + rand() % 2000;
			arr[i].weight /= 100;
		}

		for (int i = 0; i < value_input_neiron; i++)/*Заполнение весов входного нейрона, случайными числами*/
		{
			for (int k = 0; k < value_neiron; k++)
			{
				inarr[i].weight[k] = -1000 + rand() % 2000;
				inarr[i].weight[k] /= 100;
			}
		}
	}
	do
	{
		for (int j = 0; j < 4; j++)
		{
		/*int j = 3;*/
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
			input_hidden(value_neiron, arr, inarr);
			hidden_output(value_neiron, ansewer, arr);
			ansewer_output(ansewer);
			net_ansewer[j] = ansewer.out;
			ansewer.out = 0;
			ansewer.in = 0;
			info(value_neiron, value_input_neiron, inarr, arr, ansewer, j,era, net_ansewer);
		}
		era++;
		mse = (pow((true_out[0] - net_ansewer[0]), 2) + pow((true_out[1] - net_ansewer[1]), 2) + pow((true_out[2] - net_ansewer[2]), 2) + pow((true_out[3] - net_ansewer[3]), 2)) / 4;//Среднеквадратичная ошибка 
		cout <<"Cреднеквадратичная ошибка-> "<< mse<<"\n--------------------------------------------------------------" << endl;

	} while (false);
	system("pause");
}

