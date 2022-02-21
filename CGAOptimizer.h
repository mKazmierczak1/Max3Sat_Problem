#pragma once

#include "CMax3SatProblem.h"
#include <random>
#include <vector>

using namespace std;

class  CGAIndividual
{
public:
	CGAIndividual(int iGenotypeSize);
	~CGAIndividual() { delete[] pb_genotype; }

	CGAIndividual& operator=(const CGAIndividual& cOther);

	static bool bCrossover(CGAIndividual* pcParent1, CGAIndividual* pcParent2, CGAIndividual* pcChild1, CGAIndividual* pcChild2, float fCrossoverProbability);
	void vMutation(float fMutationProbability);
	int iFitness(CMax3SatProblem* pcProblem);
	int iSingleFitness(CMax3SatProblem* pcProblem, int iIndex);
	void vSetGene(bool bGene, int iIndex);
	void vGrayBoxOptimization(CMax3SatProblem* pcProblem);

private:
	int i_genotype_size;
	bool* pb_genotype;

};//class  CGIndividual

class CGAOptimizer
{
public:
	CGAOptimizer(CMax3SatProblem* pcProblem, int iPopulationSize, float fCrossoverProbability, float fMutationProbability, int iVarsNumber);
	~CGAOptimizer();

	void vInitialize();
	void vRunIteration();

	CGAIndividual* pcGetBestFound() { return pc_best_found; }

private:
	CGAIndividual* pcPickParent();
	bool bIfBetterFitness(CGAIndividual* cIndividual);
	bool bIfIndividualInPopulation(CGAIndividual* cIndividual);

	CMax3SatProblem* pc_problem;
	CGAIndividual* pc_best_found;

	vector<CGAIndividual*>  v_population;

	int i_populaton_size, i_best_fitness, i_vars_number;
	float f_crossover_probability, f_mutation_probability;
};//class CGOptimizer

class CRandom
{
public:
	CRandom() { c_rand_engine = mt19937(rd()); }

	float fRandomFloat(float fMin, float fMax);
	int iRandomInt(int iMin, int iMax);
	bool bRandomBool();

private:
	random_device rd;
	mt19937 c_rand_engine;
};//class CRandom