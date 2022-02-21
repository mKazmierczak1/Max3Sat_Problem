#pragma once

#include <string>
#include <vector>

using namespace std;

class CClause
{
public:
	CClause();

	void vSetLogVar(string number, int iIndex);
	int iGetLogVar(int iIndex);
	bool bGetVarSign(int iIndex);
	bool bIfFulfilled(bool* pbIndividual);

private:
	int i_log_vars[3];
	bool b_var_sign[3];
};

class CMax3SatProblem
{
public:
	CMax3SatProblem(int iVarsNumber);
	virtual ~CMax3SatProblem();

	bool bLoad(string sSourcePath);
	int iCompute(bool* pbIndividual);
	int iSingleCompute(bool* pbIndividual, int iIndex);
	int iGetNumberClauses() { return v_clauses.size(); }

private:
	void vOrganiseClauses();

	vector<CClause*> v_clauses;
	vector<vector<CClause*>> v_clauses_by_number;
	int i_vars_number;
};//class CMax3SatProblem


