#include "CMax3SatProblem.h"
#include "CGAOptimizer.h"
#include "Timer.h"
#include <iostream>

using namespace TimeCounters;
using namespace std;

#define dMAX_TIME 3

void vRunExperiment(CMax3SatProblem *pcProblem, int iPopulationSize, float fCrossoverProbability, float fMutationProbability, int iVarsNumber)
{
	try
	{
		CTimeCounter c_time_counter;

		double d_time_passed;

		CGAOptimizer c_optimizer(pcProblem, iPopulationSize, fCrossoverProbability, fMutationProbability, iVarsNumber);

		c_time_counter.vSetStartNow();

		c_optimizer.vInitialize();

		c_time_counter.bGetTimePassed(&d_time_passed);

		while (d_time_passed <= dMAX_TIME)
		{
			c_optimizer.vRunIteration();
			cout << c_optimizer.pcGetBestFound()->iFitness(pcProblem) << endl;

			c_time_counter.bGetTimePassed(&d_time_passed);
		}//while (d_time_passed <= dMAX_TIME)
	}//try
	catch (exception &c_exception)
	{
		cout << c_exception.what() << endl;
	}//catch (exception &c_exception)
}//void vRunExperiment()


int main(int iArgCount, char **ppcArgValues)
{
	int i_population_size = 100, i_vars_number = 50;
	float f_crossover_probability = 0.35, f_mutation_probability = 0.12;
	CMax3SatProblem c_problem(i_vars_number);

	if (c_problem.bLoad("max3sat/50/m3s_50_0.txt") == true)			//type the path to the file
	{
		vRunExperiment(&c_problem, i_population_size, f_crossover_probability, f_mutation_probability, i_vars_number);
	}
}//void main(int iArgCount, char **ppcArgValues)