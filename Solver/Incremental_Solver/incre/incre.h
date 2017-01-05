#ifndef INCRE_INCRE_INCRE_H
#define INCRE_INCRE_INCRE_H

#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <time.h>
#include "simp/SimpSolver.h"
#include "utils/System.h"
#include "utils/ParseUtils.h"
#include "utils/Options.h"
#include "core/Dimacs.h"
#include "incre/cmdline.h"
#include "incre/progress.h"



namespace Incre
{


class IncreSolver
{

public:
	static Minisat::lbool ret;									// indicator: indicate whether this iteration in addon is sat or not

	std::map<int, std::string> Solution;						// container: store final Solution

	std::map<int, std::string> PItemp;							// container: store temporary (only in this iteration) miter PI index->value
	std::map<int, std::string> POtemp;							// container: store temporary (only in this iteration) oracle PO index->value

	static std::vector<std::map<int, std::string> > OracPIs;	// container: store all temp PIs 
	static std::vector<std::map<int, std::string> > OracPOs;	// container: store all temp POs

	IncreSolver();
	~IncreSolver();

	static Minisat::lbool check_ret();							// tools: check ret before any instanization
	static void print_state();									// tools: print info inlcuding CPU, Memroy, time ,iterations



protected:


	static bool debug;							// indicator: level of verb
	static bool out_file;						// indicator: exist solution file or not
	static bool time_limit;						// indicator: set time limited or not

	static int niter;							// indicator: number of iterations
	static int time_bound;						// value: time limited
	
	static const char  * Came_file_path;		// path: input Camouflage file path
	static const char  * Orac_file_path;		// path: input Oracle SHELL file path
	static const char  * target_cnf;			// path: output of buildmiter, input of solver, and output of addon
	static const char  * Solver_solution;		// path: final solution path

	static std::vector<int> camPIndex;					// container: miter first circuit's PI, and also it the oracle's PI
	static std::vector<int> camPOindex;					// container: PO index list
	static std::vector<int> camCBindex;					// container: CB except duplicated circuit
	static std::vector<int> miterCBindex;				// container: CB include duplicated circuit 
	static std::vector<int> camCB2index;				// container: suplication's CB
	static std::vector<int> nodes2grab;					// container: variable need to be frozen during incremental solving

	static std::map<int, std::string> indexVarDict;		// map: store map of index to netname
    static std::map<std::string, int> varIndexDict;		// map: store map of netname to index

	std::map<int, std::string> CB1temp;					// map: store temporary (only in this iteration) original CB index->value 
	std::map<int, std::string> CB2temp;					// map: store temporary (only in this iteration) duplication CB index->value

	std::vector<int> addon_CB1;						// container: store temporary (only in this iteration) first duplication CB index
	std::vector<int> addon_CB2;						// container: store temporary (only in this iteration) second duplication CB index
	std::vector<int> addon_PI1;						// container: store temporary (only in this iteration) first duplication CB index	
	std::vector<int> addon_PI2;						// container: store temporary (only in this iteration) second duplication PI index
	std::vector<int> addon_PO1;						// container: store temporary (only in this iteration) first duplication PO index
	std::vector<int> addon_PO2;						// container: store temporary (only in this iteration) second duplication PO index

	static int cktTotVarNum;					// values: number of wire including miter and oracle circuit 
	static int camVarNum;						// values: total number of wires + inputs + CBs + outputs in the original cam ckt
	static int miterOutIndex; 					// values: last index of miter

	static std::vector<std::string> camCNFile;			// CNF: original Camouflaged circuit CNF
	
	static progress_t bar;								// indicator: process bar
	static Minisat::SimpSolver S;						// object: used for solve add on
	static Minisat::SimpSolver S_final;					// object: used for solve finalSolue

	static clock_t start;						// indicator: starting time
	static clock_t totoal_all;					// indicator: all thread total time
	static clock_t total_sub;					// indicator: sub-thread total time

	void print_progress(std::string info, int progress);												// tools: print progress
	void grab(std::vector<int> &list, std::map<int,std::string> &target);								// tools: searching elements in list in solution list(S.model[], index left shifted by 1), assign into target
	void grabnodes();																					// main: obtain values from "S"  
																	
	std::vector<std::string> duplicateCircuit(std::vector<std::string> cnFile, int start_index);					// tools: duplicate a circuit based on "cnFile", index start from "start_index"
	std::vector<std::string> assign_value(std::map<int, std::string> &value_map, std::vector<int> what);			// tools: use "value_map" value to assign elements in "what"
	std::vector<std::string> connectNets(std::vector<int> &piVec, int start_index);					// tools  using known start_index to connect two circuit
	std::vector<int> get_index(std::vector<int> source, int correction);							// tools: used to get duplication's PI, PO, CB index. based on "source", calculate with "correction", store in "target"

	std::map<std::string, int> netname_to_value(std::vector<std::string> nets);								// tools: used to search nets' value;
};


class MiterSolver : public IncreSolver 
{

private:
	int baseMtrVarNum;																					// values: total variable number (original + duplicated + XOR + OR)

	std::vector<std::string> baseCnfMtrLs;																// CNF: completed miter (including original Cam, duplicated Cam, XOR, Or)
	std::vector<std::vector<int> > inputs;																// container: same to inputs (includes PI and CBs)
	std::vector<std::string> forbidden_string;															// CNF: forbidden string
		
public:
	MiterSolver();																									// constructor: initialize base class and milterSolver
	~MiterSolver();																									// deconstructor
	void buildmiter();																								// main: build CNF formatted miter and export to Miter_file_path
private:
	void genOracCNF(char const * OracPath, int start);																// main: parse "OracPath", generate CNF, index start on "start"
	void genCameCNF(char const * CamePath);																			// main: parse "CamePath" and generate CNF
	std::vector<std::string> forbidden_bits(std::string line, std::vector<int> target);								// main: process forbidden options
	std::vector<std::string> connectPO_xor(std::vector<int> &posIndex, int &camVarNum, int &xorInt);				// tools: connect POs using xor, used only for two duplicated circuit
	void formatCheck(std::vector<std::string> netlist);
};



class SoluFinder : public IncreSolver 
{

public:
	SoluFinder();
	~SoluFinder();
	void find_solu();																				// main: trigger final solution
	void print_solution();																			// main: used to print final solution

private:
	int num2dup = 0;
	int totVarNum = 0;
	int clauseNum = 0;

	std::vector<std::string> finalCNF;																		// CNF: store CNF to find final solution

	void case_1();																					// main: build circuit, for the case that need more than one duplication
	void case_2();																					// main: build circuit, for the case that only one duplication needed
	void solve_it();																				// main: used to solve and find finalSolution
	void freeze();																					// main: used to setFrozen for camCBindex, so they will not be removed during simplification 
};



class AddonSolver: public IncreSolver 
{
	
public:
	AddonSolver();											
	~AddonSolver();
	void start_solving();																// main: start a iteration
	void continue_solving();															// main: based on solution, generate new addon circuit
	void queryOrac();

private:
	void print_solution(const char * path);																// tools: in current iteration, print solution for this iteration into "path"
	void freeze();																						// main: used to setFrozen for node2grab, so they will not be removed during simplification
	void print_map(std::map<int,std::string> &container, std::ofstream &outfile);						// tools: export content of "container" to "outfile"
	void solve();																						// main: used to solve both miter and addconstrains
	void export_PI();																					// main: export PI to "PI.txt"
	void parse_PO();																					// main: parser from "PO.txt"
	void run_shell();																					// main: run shell

};

class Support: public IncreSolver
{
private:
	cmdline::parser a;

public:
	Support(int argc, char ** argv);

	void parse_argv();

};

}

#endif
