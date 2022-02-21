#include "CMax3SatProblem.h"
#include <fstream>

CClause::CClause()
{
	for (int i = 0; i < 3; i++)
	{
		i_log_vars[i] = 0;
		b_var_sign[i] = 1;
	}
}

void CClause::vSetLogVar(std::string number, int iIndex)
{
	if (iIndex >= 0 && iIndex < 3)
	{
		int nr = atoi(number.c_str());

		if (nr < 0)
		{
			nr *= -1;
			b_var_sign[iIndex] = 0;
		}

		i_log_vars[iIndex] = nr;
	}
}

int CClause::iGetLogVar(int iIndex)
{
	if (iIndex >= 0 && iIndex < 3)
		return i_log_vars[iIndex];
	else
		return -1;
}

bool CClause::bGetVarSign(int iIndex)
{
	if (iIndex >= 0 && iIndex < 3)
		return b_var_sign[iIndex];
	else
		return NULL;
}

bool CClause::bIfFulfilled(bool *pbIndividual)
{
	if (pbIndividual[i_log_vars[0]] == b_var_sign[0] || pbIndividual[i_log_vars[1]] == b_var_sign[1] || pbIndividual[i_log_vars[2]] == b_var_sign[2])
		return true;
	else
		return false;
}

CMax3SatProblem::~CMax3SatProblem()
{
	for (int i = v_clauses.size() - 1; i >= 0; i--)
		delete v_clauses[i];
}

bool CMax3SatProblem::bLoad(std::string sSourcePath)
{
	std::fstream file;
	file.open(sSourcePath, std::ios::in);

	if (!file.good()) return false;

	std::string instance;

	while (!file.eof())
	{
		getline(file, instance);

		if (instance != "")
		{
			v_clauses.push_back(new CClause());
			int spaces = 0;
			int index = 2;
			int logVarsIndex = 0;
			std::string logVar = "";

			while (spaces < 5)
			{
				if (instance[index] == ' ')
				{
					spaces++;

					if (logVar != "")
					{
						(v_clauses[v_clauses.size() - 1])->vSetLogVar(logVar, logVarsIndex);
						logVarsIndex++;
						logVar = "";
					}

				}
				else
					logVar += instance[index];

				index++;
			}
		}
	}

	vOrganiseClauses();

	file.close();
	return true;
}

CMax3SatProblem::CMax3SatProblem(int iVarsNumber)
{
	i_vars_number = iVarsNumber;

	for (int i = 0; i < i_vars_number; i++)
		v_clauses_by_number.push_back(vector<CClause*>());
}

int CMax3SatProblem::iCompute(bool *pbIndividual)
{
	int quality = 0;

	for (int i = 0; i < v_clauses.size(); i++)
	{
		if (v_clauses[i]->bIfFulfilled(pbIndividual))
			quality++;
	}

	return quality;
}

int CMax3SatProblem::iSingleCompute(bool* pbIndividual, int iIndex)
{
	int quality = 0;

	if (iIndex >= 0 && iIndex < i_vars_number)
	{
		for (int i = 0; i < v_clauses_by_number[iIndex].size(); i++)
		{
			if (v_clauses_by_number[iIndex].at(i)->bIfFulfilled(pbIndividual))
				quality++;
		}
	}

	return quality;
}

void CMax3SatProblem::vOrganiseClauses()
{
	for (int i = 0; i < v_clauses.size(); i++)
	{
		for (int j = 0; j < 3; j++)
			v_clauses_by_number[v_clauses[i]->iGetLogVar(j)].push_back(v_clauses[i]);
	}
		
}
