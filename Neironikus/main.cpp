#include "pch.h"
#include <iostream>
#include <ctime>
#include <cmath>
#include <fstream>
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
void info(int j, double net_answers[])
{
	cout<<"Номер сета-> "<<j+1<<endl;
	cout << "Output O\n"<< net_answers[j] << endl;
}
void weights_calibration(input_neuron arr_input_neurons[], neuron arr_neurons[], shift_neuron arr_shift_neuron[],output_neuron answerer,bool true_answer[], double net_answers[],int j, double &u_output, double u_hidden[], double grad[], double u_w[], double u_winput[2][2], double grad_input[2][2], double &grad_shift, double &u_wshift, double grad_d_shift[], double u_d_wshift[],double E,double A)
{
	double H1_in[2] = { 0,0 };
	u_output = (true_answer[j] - net_answers[j])*derivative_function(answerer.input);
	for (int i = 0; i < 2; i++)
	{
		for (int k = 0; k < 2; k++)
		{
			H1_in[i] += arr_input_neurons[k].weight[i] * arr_input_neurons[k].input;
		}
		H1_in[i] += arr_shift_neuron[0].weight[i] * arr_shift_neuron[0].input;
	}
	for (int i = 0; i < 2; i++)
	{
		u_hidden[i] =derivative_function(H1_in[i])*(arr_neurons[i].weight*u_output);
		grad[i] = u_output * (arr_neurons[i].output(arr_neurons[i].input)*arr_neurons[i].weight);
		u_w[i] = E*grad[i]+A*u_w[i];
		arr_neurons[i].weight += u_w[i];
	}
	grad_shift = arr_shift_neuron[1].input*u_output;
	u_wshift = E * grad_shift + A * u_wshift;
	arr_shift_neuron[1].weight[0] += u_wshift;
	for (int i = 0; i < 2; i++)
	{
		grad_d_shift[i] = arr_shift_neuron[0].input*u_hidden[i];
		u_d_wshift[i] = E * grad_d_shift[i] + A * u_d_wshift[i];
		arr_shift_neuron[0].weight[i] += u_d_wshift[i];
	}
	for (int i = 0; i < 2; i++)
	{
		for (int k = 0; k < 2; k++)
		{
			grad_input[i][k] = arr_input_neurons[i].input*u_hidden[k];
			u_winput[i][k] = E * grad_input[i][k] + A * u_winput[i][k];
			arr_input_neurons[i].weight[k] += u_winput[i][k]; 
		}
	}
}

int main()
{
	setlocale(LC_ALL, "Rus");
	double mse, net_answers[4] = { 0,0,0,0 }, u_output = 0, u_hidden[2] = { 0,0 }, grad[2] = { 0,0 }, u_w[2] = { 0,0 }, u_winput[2][2] = { {0,0},{0,0} }, grad_input[2][2] = { {0,0},{0,0} }, grad_shift = 0, u_wshift = 0, grad_d_shift[2] = { 0,0 }, u_d_wshift[2] = { 0,0 },E,A;
	int era = 0, max_era,plus=0;
	bool set[4][2] = { {0,0}, {1,0}, {0,1}, {1,1} }, true_answer[4] = { 0,1,1,0 };
	const int value_neuron = 2, value_input_neuron = 2,value_shift_neuron=2;
	input_neuron arr_input_neurons[value_input_neuron];// Количество вхоных нейронов
	neuron arr_neurons[value_neuron]; //Количество нейронов 
	output_neuron answerer; //Выходной неирон
	shift_neuron arr_shift_neuron[value_shift_neuron];// Неройны сдвига
	answerer.input = 0;
	ifstream file_in("D:\\file\\out_file.txt");
	ifstream set_file("D:\\file\\set.txt");
	if (set_file.is_open())//Установка сета из файла(Если файл есть)
	{
		for (int i = 0; i < 4; i++)
		{
			set_file >> true_answer[i];
		}
	}
	else//Если файла нет, сет XOR
	{
		cout<<"Файл set.txt не найдет\nУстановлен сет XOR"<<endl;
	}
	if (!file_in.is_open())//Установка скорости обучения,момента,макс эпохи,весов (если нет соответствующего файла)
	{
		cout << "Введите значение для эпохи (max_era)" << endl;
		cin >> max_era;
		cout << "Введите значение для скорости обучения (E)" << endl;
		cin >> E;
		cout << "Введите значение для момента (A)" << endl;
		cin >> A;
		if (max_era == 0)
		{
			cout << "Недопустимое значение для эпохи (max_era)" << endl;
			max_era = 1;
		}
		if (E == 0)
		{
			cout << "Недопустимое значение для скорости обучения (E)" << endl;
			E = 0.3;
		}
		if (A == 0)
		{
			cout << "Недопустимое значение для момента (A)" << endl;
			A = 0.3;
		}
		if (era == 0)
		{
			for (int i = 0; i < value_neuron; i++)/*Заполнение весов нейрона, случайными числами*/
			{
				arr_neurons[i].weight =  rand() / double(RAND_MAX);
			}

			for (int i = 0; i < value_input_neuron; i++)/*Заполнение весов входного нейрона, случайными числами*/
			{
				for (int k = 0; k < value_neuron; k++)
				{
					arr_input_neurons[i].weight[k] =  rand() / double(RAND_MAX);
				}
			}
			for (int i = 0; i < value_shift_neuron; i++)/*Заполнение весов нейрона сдвига, случайными числами*/
			{
				for (int k = 0; k < 2; k++)
				{
					arr_shift_neuron[i].weight[k] =rand() / double(RAND_MAX);
				}
			}
			arr_shift_neuron[1].weight[1] = arr_shift_neuron[1].weight[0];
		}
	}
	else//Если файл есть, установка из него весов,скорости обучения,момента,эпохи
	{
		file_in >> arr_shift_neuron[0].weight[0];//вес в нейрона сдвига
		file_in >> arr_shift_neuron[0].weight[1];//вес в нейрона сдвига
		file_in >> arr_shift_neuron[1].weight[0];//вес в нейрона сдвига
		for (int i = 0; i < value_input_neuron; i++)//веса нейронов входа
		{
			for (int k = 0; k < 2; k++)
			{
				file_in >> arr_input_neurons[i].weight[k];
			}
		}
		for (int i = 0; i < value_neuron; i++)//веса нейронов скрытого слоя
		{
			file_in >> arr_neurons[i].weight;
		}
		file_in >> E;
		file_in >> A;
		file_in >> max_era;
		file_in >> era;
		for (int i = 0; i < 2; i++)//дельта весов сдвига
		{
			file_in >> u_d_wshift[i];
		}
		file_in >> u_wshift;//дельта весов сдвига
		for (int i = 0; i < 2; i++)//дельта весов входного
		{
			for (int k = 0; k < 2; k++)
			{
				file_in >> u_winput[i][k];
			}
		}
		for (int i = 0; i < 2; i++)//дельта весов скрытого
		{
			file_in >> u_w[i];
		}
		cout << "Эпоха-> " << era << " Макс эпоха-> " << max_era << endl;
		cin >> plus;
		max_era += plus;
	}
	do
	{
		for (int j = 0; j < 4; j++)
		{
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
			input_hidden(value_neuron, arr_neurons, arr_input_neurons, arr_shift_neuron);//Передача вход->скрытый
			hidden_output(value_neuron, answerer, arr_neurons);//скрытый->выходной
			ansewer_output(answerer, arr_shift_neuron);//выходной->ответ
			net_answers[j] = answerer.output;//сохранение ответа
			info(j, net_answers);//вывод в консоль
			weights_calibration(arr_input_neurons, arr_neurons, arr_shift_neuron, answerer, true_answer, net_answers, j, u_output, u_hidden, grad, u_w, u_winput, grad_input, grad_shift, u_wshift, grad_d_shift, u_d_wshift,E,A);//алгоритм обучения
			answerer.input = 0;//обнуления для выхоного
		}
		era++;//подсчет эпохи
		mse = (pow((true_answer[0] - net_answers[0]), 2) + pow((true_answer[1] - net_answers[1]), 2) + pow((true_answer[2] - net_answers[2]), 2) + pow((true_answer[3] - net_answers[3]), 2)) / 4;//Среднеквадратичная ошибка 
		cout <<"Cреднеквадратичная ошибка-> "<< mse<<"\nЭпоха->"<<era<<"\n--------------------------------------------------------------" << endl;//Вывод в консоль
		ofstream file("D:\\file\\weight.txt",ios_base::app);//запись ответов и весов в файл 256-280
		file << "Веса слоя сдвига->" << endl;
		file << arr_shift_neuron[0].weight[0] << endl;
		file << arr_shift_neuron[0].weight[1] << endl;
		file << arr_shift_neuron[1].weight[0] << endl;
		file << "Веса входного слоя->" << endl;
		for (int i = 0; i < value_input_neuron; i++)
		{
			for (int k = 0; k < 2; k++)
			{
				file << arr_input_neurons[i].weight[k] << endl;
			}
		}
		file << "Веса скрытого слоя->" << endl;
		for (int i = 0; i < value_neuron; i++)
		{
			file << arr_neurons[i].weight << endl;
		}
		file << "Ответы->" << endl;
		for (int i = 0; i < 4; i++)
		{
			file << net_answers[i] << endl;
		}
		file <<"Mse-> "<<mse<<endl;
		file << "Эпоха-> " << era <<"\n---------------------"<< endl;
	} while (era<max_era);
	ofstream file("D:\\file\\weight.txt", ios_base::app);//Запись в файл скорости обучение и момента
	file<<"E="<<E<<"\n A="<<A<<endl;
	file.close();
	ofstream outputfile("D:\\file\\out_file.txt"); //сохранения весов для последующего использования 
	outputfile << arr_shift_neuron[0].weight[0] << endl;//вес в нейрона сдвига
	outputfile << arr_shift_neuron[0].weight[1] << endl;//вес в нейрона сдвига
	outputfile << arr_shift_neuron[1].weight[0] << endl;//вес в нейрона сдвига
	for (int i = 0; i < value_input_neuron; i++)//веса нейронов входа
	{
		for (int k = 0; k < 2; k++)
		{
			outputfile << arr_input_neurons[i].weight[k] << endl;
		}
	}
	for (int i = 0; i < value_neuron; i++)//веса нейронов скрытого слоя
	{
		outputfile << arr_neurons[i].weight << endl;
	}
	outputfile << E << "\n" << A << "\n" << max_era << "\n" << era << endl;
	for (int i = 0; i < 2; i++)//дельта весов скрытого слоя
	{
		outputfile << u_d_wshift[i] << endl;
	}
	outputfile << u_wshift << endl;//дельта весов скрытого слоя
	for (int i = 0; i < 2; i++)//дельта весов входа
	{
		for (int k = 0; k < 2; k++)
		{
			outputfile << u_winput[i][k] << endl;
		}
	}
	for (int i = 0; i < 2; i++)//дельта весов скрытого
	{
		outputfile << u_w[i] << endl;
	}
	outputfile.close();
	system("pause");
}

