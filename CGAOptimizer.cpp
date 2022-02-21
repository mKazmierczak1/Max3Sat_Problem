#include "CGAOptimizer.h"
#include <iostream>

using namespace std;

CGAIndividual::CGAIndividual(int iGenotypeSize)
{
	i_genotype_size = iGenotypeSize;
	pb_genotype = new bool[i_genotype_size];
}

CGAIndividual& CGAIndividual::operator=(const CGAIndividual& cOther)
{
	i_genotype_size = cOther.i_genotype_size;

	if (pb_genotype != nullptr)
		delete[] pb_genotype;

	pb_genotype = new bool[i_genotype_size];

	for (int i = 0; i < i_genotype_size; i++)
		pb_genotype[i] = cOther.pb_genotype[i];

	return *this;
}

bool CGAIndividual::bCrossover(CGAIndividual* pcParent1, CGAIndividual* pcParent2, CGAIndividual* pcChild1, CGAIndividual* pcChild2, float fCrossoverProbability)
{
	CRandom rand;
	
	if (rand.fRandomFloat(0, 1) < fCrossoverProbability)
	{
		for (int i = 0; i < pcChild1->i_genotype_size; i++)
			rand.bRandomBool() ? pcChild1->pb_genotype[i] = pcParent1->pb_genotype[i] : pcChild1->pb_genotype[i] = pcParent2->pb_genotype[i];

		for (int i = 0; i < pcChild2->i_genotype_size; i++)
			pcChild1->pb_genotype[i] ? pcChild2->pb_genotype[i] = 0 : pcChild2->pb_genotype[i] = 1;

		return true;
	}
	else
	{
		*pcChild1 = *pcParent1;
		*pcChild2 = *pcParent2;

		return false;
	}

}

void CGAIndividual::vMutation(float fMutationProbability)
{
	CRandom rand;

	for (int i = 0; i < i_genotype_size; i++)
	{
		if (rand.fRandomFloat(0, 1) < fMutationProbability)
			pb_genotype[i] ? pb_genotype[i] = 0 : pb_genotype[i] = 1;
	}
}

int CGAIndividual::iFitness(CMax3SatProblem* pcProblem)
{
	return pcProblem->iCompute(pb_genotype);
}

int CGAIndividual::iSingleFitness(CMax3SatProblem* pcProblem, int iIndex)
{
	return pcProblem->iSingleCompute(pb_genotype, iIndex);
}

void CGAIndividual::vSetGene(bool bGene, int iIndex)
{
	if (iIndex >= 0 && iIndex < i_genotype_size)
		pb_genotype[iIndex] = bGene;
}

void CGAIndividual::vGrayBoxOptimization(CMax3SatProblem* pcProblem)
{
	for (int i = 0; i < i_genotype_size; i++)
	{
		int fitnessBefore = iSingleFitness(pcProblem, i);

		pb_genotype[i] ? pb_genotype[i] = 0 : pb_genotype[i] = 1;

		if (fitnessBefore > iSingleFitness(pcProblem, i))
			pb_genotype[i] ? pb_genotype[i] = 0 : pb_genotype[i] = 1;
	}
}

CGAOptimizer::CGAOptimizer(CMax3SatProblem* pcProblem, int iPopulationSize, float fCrossoverProbability, float fMutationProbability, int iVarsNumber)
{
	pc_problem = pcProblem; 
	pc_best_found = nullptr;
	i_populaton_size = iPopulationSize;
	f_crossover_probability = fCrossoverProbability;
	f_mutation_probability = fMutationProbability;
	i_vars_number = iVarsNumber;
	i_best_fitness = 0;
}

CGAOptimizer::~CGAOptimizer()
{
	for (int i = v_population.size() - 1; i >= 0; i--)
		delete v_population[i];

	if (pc_best_found != nullptr)
		delete pc_best_found;
}

void CGAOptimizer::vInitialize()
{
	CRandom rand;

	for (int i = 0; i < i_populaton_size; i++)
	{
		v_population.push_back(new CGAIndividual(i_vars_number));

		for (int j = 0; j < i_vars_number; j++)
			(v_population[v_population.size() - 1])->vSetGene(rand.bRandomBool(), j);

		bIfBetterFitness(v_population[v_population.size() - 1]);
	}
}

void CGAOptimizer::vRunIteration()
{
	vector<CGAIndividual*> newPopulation;

	while (newPopulation.size() < v_population.size())
	{
		CGAIndividual* parent1 = pcPickParent();
		CGAIndividual* parent2 = pcPickParent();

		CGAIndividual* child1 = new CGAIndividual(i_vars_number);
		CGAIndividual* child2 = new CGAIndividual(i_vars_number);
		bool ifCrossed = CGAIndividual::bCrossover(parent1, parent2, child1, child2, f_crossover_probability);

		child1->vMutation(f_mutation_probability);
		child2->vMutation(f_mutation_probability);
		
		if (ifCrossed)
		{
			child1->vGrayBoxOptimization(pc_problem);
			child2->vGrayBoxOptimization(pc_problem);

			bIfBetterFitness(child1);
			bIfBetterFitness(child2);
		}

		newPopulation.push_back(child1);
		newPopulation.push_back(child2);
	}

	for (int i = v_population.size() - 1; i >= 0; i--)
		if (v_population[i] != pc_best_found)
			delete (v_population[i]);

	v_population.clear();

	for (int i = 0; i < newPopulation.size(); i++)
		v_population.push_back(newPopulation[i]);
}

CGAIndividual* CGAOptimizer::pcPickParent()
{
	CRandom rand;

	CGAIndividual* individual1 = v_population[rand.iRandomInt(0, i_populaton_size - 1)];
	CGAIndividual* individual2 = v_population[rand.iRandomInt(0, i_populaton_size - 1)];

	return individual1->iFitness(pc_problem) >= individual2->iFitness(pc_problem) ? individual1 : individual2;
}

bool CGAOptimizer::bIfBetterFitness(CGAIndividual* cIndividual)
{
	int fitness = (cIndividual->iFitness(pc_problem));

	if (fitness > i_best_fitness)
	{
		i_best_fitness = fitness;
		pc_best_found = cIndividual;
		return true;
	}
	else
		return false;
}

bool CGAOptimizer::bIfIndividualInPopulation(CGAIndividual* cIndividual)
{
	for (int i = 0; i < v_population.size(); i++)
	{
		if (v_population[i] == cIndividual)
			return true;
	}
	
	return false;
}

float CRandom::fRandomFloat(float fMin, float fMax)
{
	int min = static_cast<int>(fMin * 100);
	int max = static_cast<int>(fMax * 100);
	uniform_int_distribution<int> dist(min, max);

	return ( (float)dist(c_rand_engine) / 100.0);
}

int CRandom::iRandomInt(int iMin, int iMax)
{
	uniform_int_distribution<int> dist(iMin, iMax);
	return dist(c_rand_engine);
}

bool CRandom::bRandomBool()
{
	uniform_int_distribution<int> dist(0, 1);
	return dist(c_rand_engine);
}
