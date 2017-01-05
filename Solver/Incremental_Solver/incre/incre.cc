#include <iostream>
#include <map>
#include <fstream>
#include <algorithm>
#include <string>
#include <cmath>
#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include "incre/incre.h"
#include "simp/SimpSolver.h"
#include "incre/tools.h"
#include "incre/dict.h"
#include "utils/System.h"
#include "utils/ParseUtils.h"
#include "utils/Options.h"
#include "core/Dimacs.h"
#include "incre/resource.h"
using namespace Minisat;
using namespace Incre;
using namespace std;

//=================================================================================================
// declear static 



lbool IncreSolver::ret = l_False;
bool IncreSolver::debug = false;
bool IncreSolver::out_file = false;
bool IncreSolver::time_limit = false;

clock_t IncreSolver::start = clock();									
clock_t IncreSolver::totoal_all = 0;								    
clock_t IncreSolver::total_sub = 0;										

const char * IncreSolver::Orac_file_path;
const char * IncreSolver::Came_file_path;
const char * IncreSolver::target_cnf = "target.cnf";
const char * IncreSolver::Solver_solution = "NULL";

SimpSolver IncreSolver::S;
SimpSolver IncreSolver::S_final;
progress_t IncreSolver::bar;                          

int IncreSolver::time_bound = 3600;
int IncreSolver::niter = 1;                        
int IncreSolver::miterOutIndex = 0;
int IncreSolver::cktTotVarNum = 0;                
int IncreSolver::camVarNum = 0;                    

vector<int> IncreSolver::nodes2grab;
vector<int> IncreSolver::camCBindex;              
vector<int> IncreSolver::miterCBindex;            
vector<int> IncreSolver::camCB2index;          
vector<int> IncreSolver::camPIndex;               
vector<int> IncreSolver::camPOindex;

vector<string> IncreSolver::camCNFile;            

map<int, string> IncreSolver::indexVarDict;       
map<string, int> IncreSolver::varIndexDict;       


vector<map<int, string> > IncreSolver::OracPIs;   
vector<map<int, string> > IncreSolver::OracPOs;                
//=================================================================================================
//implementation of IncreSolver
IncreSolver::IncreSolver()
{
//	cout << "IncreSolver is created" << endl;
}

IncreSolver::~IncreSolver()
{
//	cout << "IncreSolver is deleted" << endl;
}

lbool IncreSolver::check_ret()
{
    return ret;
}


void IncreSolver::print_state()
{
    
	cout << "============================[ Problem Statistics ]=============================" << endl;
	cout << "|                                                                              " << endl;
	cout << "|\tTotal CPU time:      \t\t\t" << ((float)totoal_all)/CLOCKS_PER_SEC << " s" << endl;
	cout << "|\tMain CPU time:       \t\t\t" << ((float)totoal_all - (float)total_sub)/CLOCKS_PER_SEC << " s" << endl;
    cout << "|\tNum of iteration(s)  \t\t\t" << niter << endl;
    cout << "|\tPhysical Memory used:\t\t\t" << get_memUsage_mb() << " Mb" << endl;
    cout << "|\tCPU usage:           \t\t\t" << getCurrentValue() << " %" << endl;
	cout << "===============================================================================" << endl;
}


vector<string> IncreSolver::assign_value(map<int, string> &value_map, vector<int> what)
{
    vector<string> result;
    vector<int>::iterator position = what.begin();
    for(map<int, string>::iterator index = value_map.begin(); index != value_map.end(); ++index)
    {
        if(index->second == "1") result.push_back(tostring(*position) + " 0\n");
        else if(index->second == "0") result.push_back("-" + tostring(*position) + " 0\n");
        position++;
    }
    return result;
}

vector<int> IncreSolver::get_index(vector<int> source, int correction)
{
    vector<int> target;
    for(vector<int>::iterator index = source.begin(); index != source.end(); ++index)
    {
        target.push_back(*index + correction);
    }
    return target;
}


vector<string> IncreSolver::duplicateCircuit(vector<string> cnFile, int start_index)
{
    vector<string> cnFile2;
    for(vector<string>::iterator iter = cnFile.begin(); iter != cnFile.end(); ++iter)
    {
        string tmpClause = *iter;
        vector<string> intIndexLs;
        SplitString(*iter, intIndexLs, " ");
        string newTmpCls;
        for(vector<string>::iterator j = intIndexLs.begin(); j != intIndexLs.end() - 1; ++j)
        {
            string tmpInt(*j);
            if(tmpInt.find("-") != string::npos)
            {
                strip_all(tmpInt, "-");
                newTmpCls += "-" + tostring(stoi(tmpInt) + start_index) + " ";
            }
            else
            {
                newTmpCls += tostring(stoi(tmpInt) + start_index) + " ";
            }
        }
        cnFile2.push_back(newTmpCls + "0\n");
    }  
    return cnFile2;
}

vector<string> IncreSolver::connectNets(vector<int> &piVec, int start_index)           
{
    vector<string> new_cnFile;
    for(vector<int>::iterator i = piVec.begin(); i != piVec.end(); ++i)
    {
        string piConsLine1 = tostring(*i) + " -" + tostring(*i + start_index) + " 0\n";
        new_cnFile.push_back(piConsLine1);
        string piConsLine2 = "-" + tostring(*i) + " " + tostring(*i + start_index) + " 0\n";
        new_cnFile.push_back(piConsLine2);
    } 
    return  new_cnFile; 
}


void IncreSolver::grabnodes()     
{
    if(ret == l_True)
    {

        grab(camPIndex, PItemp);
        grab(camCBindex, CB1temp);
        grab(camCB2index, CB2temp);

        OracPIs.push_back(PItemp);

//        print_map(PItemp);
    }

}

void IncreSolver::grab(vector<int> &list, map<int,string> &target)
{
    for(vector<int>::iterator node = list.begin(); node != list.end(); ++node)
    {
        if(S.model[*node - 1] != l_Undef)
        {
            if(S.model[*node - 1] == l_True) target.insert(std::pair<int,string>(*node, "1"));
            else target.insert(std::pair<int, string>(*node, "0"));
        }
    }
}

void IncreSolver::print_progress(string info, int progress)
{
    progress_show(&bar, progress/100.0f);
    cout << "\r" << info << flush;

}


//=================================================================================================
//implementation of MiterSolver
MiterSolver::MiterSolver()
{
    if(!debug) print_progress("Start...", 1);
}

MiterSolver::~MiterSolver()
{
}

vector<string> MiterSolver::connectPO_xor(vector<int> &posIndex, int &camVarNum, int &xorInt)
{
    xorInt = camVarNum * 2;
    vector<string> cnfLines;
    for(vector<int>::iterator po = posIndex.begin(); po != posIndex.end(); ++po)
    {
        xorInt++;
        int inV1 = *po;
        int inV2 = *po + camVarNum;
        int outV = xorInt;
        cnfLines.push_back("-" + tostring(inV1) + " -" + tostring(inV2) + " -" + tostring(outV) + " 0\n");
        cnfLines.push_back(tostring(inV1) + ' ' + tostring(inV2) + " -" + tostring(outV) + " 0\n");
        cnfLines.push_back(tostring(inV1) + " -" + tostring(inV2) + " " + tostring(outV) + " 0\n");
        cnfLines.push_back("-" + tostring(inV1) + " " + tostring(inV2) + " " + tostring(outV) + " 0\n");
    }
//    print_vector(cnfLines,"connectPO_xor");
    return cnfLines;
}

vector<string> MiterSolver::forbidden_bits(string line, vector<int> target)
{
    // acquire allow info
    int set_length;
    vector<string> allowed_list;
    vector<string> complete_list;
    vector<string> forbidden_list;
    vector<string> final;
    if(line.find("RE__ALLOW") != string::npos)
    {
        smatch result;
        regex pattern("(\\().*(\\))");
        regex_search(line, result, pattern);
        string allow_string = result[0].str();
        strip_all(allow_string,"(");
        strip_all(allow_string, ")");
        SplitString(allow_string, allowed_list, ",");
    
        // get forbidden info
        set_length = allowed_list[0].length();
        for(int i = 0; i < pow(2,set_length); i++)
        {
            complete_list.push_back(toBinary(i, set_length));
        }

        for(vector<string>::iterator CB = complete_list.begin(); CB != complete_list.end(); ++CB)
        {
            auto iter = find(allowed_list.begin(), allowed_list.end(), *CB);
            if(iter == allowed_list.end())
            {
                forbidden_list.push_back(*CB);
            }
        }
        // assign
        for( auto type = forbidden_list.begin(); type != forbidden_list.end(); ++type)
            {
                for(auto group = target.begin(); group != target.end(); )
                {
                    string temp = "";
                    for(unsigned int value = 0; value < (*type).size(); value++ )
                    {
                        if((*type)[value] == '1')
                        {
                            temp += "-" + to_string(*group) + " ";
                        }
                        else if((*type)[value] == '0')
                        {
                            temp += to_string(*group) + " ";
                        }
                        group++;
                    }
                    temp += " 0\n";
                    final.push_back(temp);
                }
            }
        return final;
    }
    else
    {
        return final;
    }
}

void MiterSolver::formatCheck(vector<string> netlist)
{
	bool format_error = false;
	for(vector<string>::iterator it = netlist.begin(); it != netlist.end(); ++it)
	{
		string line = *it;
		if(line.find("input") != string::npos)
		{
			string next_line = *(it+1);

			if(next_line.find("RE__ALLOW") != string::npos)
			{
				vector<string> allow_list;
				vector<string> net_list;
				// get net list in this line
				strip_all(line, "input");
				strip_all(line, " ");
				strip_all(line, "\t");
				strip_all(line, "\n");
        		SplitString(line, net_list, ",");
        		// get allow bits list in next line
				smatch result;
        		regex pattern("(\\().*(\\))");
        		regex_search(next_line, result, pattern);
        		string allow_string = result[0].str();
        		strip_all(allow_string,"(");
        		strip_all(allow_string, ")");
        		SplitString(allow_string, allow_list, ",");
        		// compare whether the length is matching
        		for(auto pair: allow_list)
        		{
        			if(pair.length() != net_list.size()) 
        			{
        				cerr << "Format Error: length of allow bits mistach" << endl;
        				cerr << "At: " << "input " + line << endl;
        				format_error = true;
        			}
        		}
			}

			else if(next_line.find("RE__PI") != string::npos) continue;
			else 
			{
				strip_all(line, "\t");
				strip_all(line, "\n");
				cerr << "Format Error: input should be marked" << endl;
				cerr << "At: " << line << endl;
        		format_error = true;

			}

		}
	}
	if(format_error) exit(10);
}
void MiterSolver::genCameCNF(char const * CamePath)
{
    vector<string> cnfLines;
    vector<string> Vlines;

    vector<vector<int> > inputs;
    map<string, int> gateTypeDict;
    vector<string> cnFile;
    vector<int> posIndex;
    camVarNum = 0;
    int varIndex = 1;
    int gateCnt = 0;
    bool has_const1 = false;
    bool has_const0 = false;
    load_gateTypeDict(gateTypeDict);

    if(debug == true) clog << "Reading data from " << CamePath << endl;
    if(!debug) print_progress("Generate Miter\t", 2);
    SplitString(stripComments(Readall(CamePath)), Vlines,";");
    vector<string> netlist = Vlines;
    formatCheck(netlist);
    // cout << stripComments(Readall(CamePath)) << endl;
//======================================================================================================================
//parse the first camouflaged circuit 

    for(vector<string>::iterator iter = Vlines.begin(); iter != Vlines.end(); ++iter)
    {
        string line= *iter;
        strip_all(line, "\r");
        strip_all(line, "\n");
        if((line.find("input") != string::npos) && ((*(iter + 1)).find("RE__PI") != string::npos))
        {     
            if(!debug) print_progress("Parse PI  \t", 3);

            string next_line = *(iter + 1);
            vector<string> PIs;         
            vector<int> tmpPis;

            if(line.find("CONST1") != string::npos) has_const1 = true;
            if(line.find("CONST0") != string::npos) has_const0 = true;

            strip_all(line, "input");
            strip_all(line, " ");
            SplitString(line, PIs, ","); 

            for(vector<string>::iterator pi = PIs.begin(); pi != PIs.end(); ++pi)
            {
                strip_all(*pi, "[");
                strip_all(*pi, "]");
                strip_all(*pi, " ");
                strip_all(*pi, "\t");
                // cout << *pi << " " << varIndex << endl;
                varIndexDict.insert(std::pair<string, int>(*pi, varIndex));
                indexVarDict.insert(std::pair<int, string>(varIndex, *pi));
                tmpPis.push_back(varIndex);
                ++varIndex;  
            }  
            camPIndex = tmpPis;
        }

        else if((line.find("input") != string::npos) && (*(iter + 1)).find("RE__ALLOW") != string::npos)
        {    
            if(!debug) print_progress("Parse CB  \t", 3);

            string next_line = *(iter + 1);
            string line_with_allow;
            bool has_allowBits = false;
            vector<string> PIs;         
            vector<int> tmpPis;

            if(line.find("CONST1") != string::npos) has_const1 = true;
            if(line.find("CONST0") != string::npos) has_const0 = true;

            if(next_line.find("RE__ALLOW") != string::npos)
            {
                has_allowBits = true;
               line_with_allow = line + next_line;
            }

            strip_all(line, "input");
            strip_all(line, " ");
            SplitString(line, PIs, ","); 

            for(vector<string>::iterator pi = PIs.begin(); pi != PIs.end(); ++pi)
            {
                strip_all(*pi, "[");
                strip_all(*pi, "]");
                strip_all(*pi, " ");
                strip_all(*pi, "\t");
                // cout << *pi << " " << varIndex << endl;

                varIndexDict.insert(std::pair<string, int>(*pi, varIndex));
                indexVarDict.insert(std::pair<int, string>(varIndex, *pi));
                tmpPis.push_back(varIndex);
                varIndex++;  
            }  
            camCBindex += tmpPis;

            if(has_allowBits)
            {
                forbidden_string += forbidden_bits(line_with_allow, tmpPis);
            }
        }

        else if((line.find("output") != string::npos) && (line.find("//") == string::npos))
        {

            if(!debug) print_progress("Parse Outputs \t", 5);

            vector<string> POs;

            strip_all(line, "output");
            strip_all(line, " ");

            SplitString(line, POs, ",");
            for(vector<string>::iterator po = POs.begin(); po != POs.end(); ++po)
            {
                strip_all(*po, "[");
                strip_all(*po, "]"); 
                strip_all(*po, " ");
                strip_all(*po, "\t");
//              cout << *po << endl;
	            // cout << *po << " " << varIndex << endl;
             
                posIndex.push_back(varIndex);
                varIndexDict.insert(std::pair<string,int>(*po, varIndex));
                indexVarDict.insert(std::pair<int, string>(varIndex, *po));
                varIndex++;             
            }
        }

        else if((line.find("wire") != string::npos) && (line.find("//") == string::npos))
        {

            if(!debug) print_progress("Parse Wires   \t", 7);

            vector<string> wires;

            strip_all(line, "wire");
            strip_all(line, " ");
            SplitString(line, wires, ",");

            for(vector<string>::iterator w = wires.begin(); w != wires.end(); ++w)
            {
	            // cout << *w << " " << varIndex << endl;

                strip_all(*w, " ");
                strip_all(*w, "\t");
                varIndexDict.insert(std::pair<string,int>(*w, varIndex));
                indexVarDict.insert(std::pair<int, string>(varIndex, *w));
                varIndex++;             
            }
        }
        

        else if((line != "") && (line.find("RE__") == string::npos) &&(line.find("module") == string:: npos))
        {
            if(!debug) print_progress("Parse Gates   \t", 10);
            string report = line;
            string gate;
            vector<string> cnfLines;
            vector<string> netname;
            vector<int> lineIn;
            int lineOut;

            if((line.find(".") != string::npos) && (line.find("(") != string::npos))
            {
                gate = find_gatetype(line);
            }
            else
            {
                cerr << "error: Verilog format is not acceptable!!!" << endl;
                cerr << "illegal line is: " << report << endl;
                exit(3);
            }


            netname = find_netname(line);
            lineOut = varIndexDict[netname.back()];
            for(vector<string>::iterator iter = netname.begin(); iter != netname.end() - 1; ++iter)
            {
                lineIn.push_back(varIndexDict[*iter]);
            }

            strip_all(gate," ");
            strip_all(gate,"\t");
            int caseNo = gateTypeDict[gate];

            cnfLines = transGATE(caseNo, lineIn, lineOut);
            for(vector<string>::iterator iter = cnfLines.begin(); iter != cnfLines.end(); ++iter)
            {
                cnFile.push_back(*iter);
            }
            gateCnt++;
        }   
    }

    camVarNum = varIndex - 1;
    camCNFile = cnFile;
//========================================================================================================================
//duplicate another camouflage circuit 
    if(!debug) print_progress("Duplicating   \t", 20);

    vector<string> cnFile2 =  duplicateCircuit(cnFile, camVarNum);
    cnFile.push_back("c The second camouflage circuit:\n");
    vector<string> cnFile_merged;
    cnFile_merged = cnFile + cnFile2;
//    print_vector(cnFile_merged, "cnFile_merged");


//========================================================================================================================
//add constrains
//1, connect PIs
    if(!debug) print_progress("Connect PIs   \t", 25);

    cnFile_merged.push_back("c Force PIs of 2 ckts to be the same:\n"); 
    vector<string> connection = connectNets(camPIndex, camVarNum); 
    vector<string> cnFile_PIconnected;          
    cnFile_PIconnected = cnFile_merged + connection;
//    print_vector(cnFile_PIconnected, "cnFile_PIconnected");
//========================================================================================================================
//2, add forbidden bits

//========================================================================================================================
//3, use XOR connect POs
    if(!debug) print_progress("Connect POs   \t", 26);

    int xorInt = 0;
    cnFile_PIconnected.push_back("c XOR outputs of 2 ckts:\n");
    vector<string> XOR_connection = connectPO_xor(posIndex, camVarNum, xorInt);
    vector<string> cnFile_POconnected;
    cnFile_POconnected = cnFile_PIconnected + XOR_connection;
//   print_vector(cnFile_POconnected, "cnFile_POconnected");
//========================================================================================================================
//4, use Or to connect all the XOR
    cnFile_POconnected.push_back("c The last OR gate of the miter:\n");
    int orIndex = xorInt + 1;
    vector<int> XOR_outputs;
    for (int XOR_index = camVarNum*2 + 1; XOR_index < orIndex; XOR_index++)
    {
        XOR_outputs.push_back(XOR_index);
    }
    vector<string> OR_connection = transGATE(2, XOR_outputs, orIndex);
    OR_connection.push_back(tostring(orIndex) + " 0\n");
    vector<string> final_miter;
    final_miter = cnFile_POconnected + OR_connection;

    int varNum = orIndex;
//========================================================================================================================
//5, add CONST constrain
    final_miter.push_back("c The CONST constrains");
    if(has_const0) final_miter.push_back("-" + tostring(varIndexDict["CONST0"]) + " 0\n");
    if(has_const1) final_miter.push_back(tostring(varIndexDict["CONST1"]) + " 0\n");
//========================================================================================================================
//get POindex, camCBindex, VarNum


    baseCnfMtrLs = final_miter;
//    camPIndex = inputs.front();									///////////////////////
//   for(vector<vector<int> >::iterator iter = inputs.begin() + 1; iter != inputs.end(); ++iter)			/////////////
//    {
//        camCBindex += *iter;
//    }
    camPOindex = posIndex;
    baseMtrVarNum = varNum;
    miterOutIndex = varNum; 


}


void MiterSolver::buildmiter()
{
    genCameCNF(Came_file_path);
    if(debug == true) clog << "Start to buildmiter" << endl;

//========================================================================================================================
//add both Oracle and miter to single circuit
    cktTotVarNum = baseMtrVarNum;
//========================================================================================================================
//connect PIs oracle and miter
// baseCnfMtrls includes two camouflages, PI connection, PO connection, OR, CONST
    string cmmtline1 = "c this file is generated by buildmiter\n";
    string cmmtline2 = "c generated on " + get_localtime();
//    string problemLn = "p cnf " + tostring(cktTotVarNum) + " " + tostring(baseCnfMtrLs.size() + forbidden_string.size() - 5) + "\n";
//    baseCnfMtrLs.insert(baseCnfMtrLs.begin(), problemLn);
    baseCnfMtrLs.insert(baseCnfMtrLs.begin(), cmmtline2);
    baseCnfMtrLs.insert(baseCnfMtrLs.begin(), cmmtline1);
    string cmmtline3 = "c this is forbidden constrains\n";
    forbidden_string.insert(forbidden_string.begin(), cmmtline3);
    baseCnfMtrLs += forbidden_string;
    print_vector(baseCnfMtrLs, target_cnf);
//========================================================================================================================
//update miterCBindex to include duplication's CB
    for(vector<int>::iterator pbitIndex = camCBindex.begin(); pbitIndex != camCBindex.end(); ++pbitIndex)
    {
        camCB2index.push_back(*pbitIndex + camVarNum);

    } 
    miterCBindex = camCBindex + camCB2index;
//========================================================================================================================
//nodes2grab now includes miterCBindex, camPIndex
    nodes2grab = camPIndex; 
    nodes2grab += miterCBindex;
    nodes2grab.push_back(miterOutIndex);
//    print_vector(camPIndex, "camPIndex");
//    print_vector(camCBindex, "camCBindex");
//    print_vector(miterCBindex, "miterCBindex");
//    print_vector(nodes2grab, "nodes2grab");
    if(!debug) print_progress("Miter Built   \t", 26);
    if(debug) print_vector(baseCnfMtrLs, "miter");
}

//=================================================================================================
//implementation of AddonSolver

AddonSolver::AddonSolver():IncreSolver()
{

}

AddonSolver::~AddonSolver()
{
//    cout << "AddonSolver is deleted" << endl;
}

void AddonSolver::freeze()
{
    static int already_frozen = 0;
    if(already_frozen == 0)
    {
        for(vector<int>::iterator index = nodes2grab.begin(); index != nodes2grab.end(); ++index)
        {
            S.setFrozen(*index - 1, true);
        }        
        already_frozen++;
    }

}

void AddonSolver::start_solving()
{
    //solve miter, grabnodes

    if(!debug) print_progress("Solve Addons  \t", 27);
    solve();    

    grabnodes();                // grab CB1 CB2 info and PItemp

    if(!debug) print_progress("Export PIs   \t", 27);
    export_PI();        
    

}

void AddonSolver::queryOrac()
{
	remove("PO.txt");

    if(!debug) print_progress("Query Oracle  \t", 27);
	run_shell();

    if(!debug) print_progress("Parse POs     \t", 27);
	parse_PO();
	if(debug == true) print_solution("Solution_temp");
}

void AddonSolver::run_shell()
{
	const char *full_path = Orac_file_path;
	const char *sh = "sh ";
    const char *dum = " > /dev/null 2>&1";
	char buffer[1024];
	strncpy(buffer, sh, sizeof(buffer));
	strncat(buffer, full_path, sizeof(buffer));
    if(!debug)
    {
        strncat(buffer, dum, sizeof(buffer));
 
    }
	if(debug == true) clog << "Runing: " << buffer << endl; 
	clock_t start_branch = clock();
    int status = system(buffer);
    if(debug == true)
    {
        if(status < 0)
        {
            printf("cmd: %s\t error: %s", buffer, strerror(errno) );
        }
        if(WIFEXITED(status))
        {
            cerr << "normal termination, exit status = " << WEXITSTATUS(status) << endl;
        }
        else if(WIFSIGNALED(status))
        {
            cerr << "abnormal termination, signal number = " << WTERMSIG(status) << endl;
        }
        else if(WIFSTOPPED(status))
        {
            cerr << "process stopped, signal number = " << WSTOPSIG(status) << endl;
        }
    }
	clock_t end_branch = clock();
	total_sub += end_branch - start_branch;
}

void AddonSolver::print_solution(const char * path)
{

    ofstream outfile(path);
    outfile << "------The found PI vector that differentiate CB" << endl;
    clog << "------The found PI vector that differentiate CB is" << endl;
    print_map(PItemp, outfile);
    outfile << "The corresponding primary outputs are:" << endl;
    clog << "The corresponding primary outputs are:" << endl;
    print_map(POtemp, outfile);    
    
}

void AddonSolver::print_map(map<int,string> &container, ofstream &outfile)
{
    for(map<int, string>::iterator index = container.begin(); index != container.end(); ++index)
    {
        outfile << index->first << "\t";
        clog << indexVarDict[index->first] << "\t";
    }
    outfile << endl;
    clog << endl;
    for(map<int, string>::iterator value = container.begin(); value != container.end(); ++value)
    {
        outfile << value->second << "\t";
        clog << value->second << "\t";
    }
    outfile << endl; 
    clog << endl;
}

void AddonSolver::solve()   // used to solve both miter and addons
{

    gzFile in = gzopen(target_cnf, "rb");
    parse_DIMACS(in, S);
    gzclose(in);
    freeze();
    S.eliminate(true);
    if(!S.okay())
    {
        if(debug == true) cerr << "UNSAT\n" << endl;
    }


    vec<Lit> dummy;
    ret = S.solveLimited(dummy);

	niter++;

}

void AddonSolver::continue_solving()
{
    if(ret == l_True)
    {
    	if(debug == true) clog << "Continue Solving" << endl;
        if(!debug) print_progress("Continue Solve\t", 27);
        OracPOs.push_back(POtemp);

        //========================================================================================================================
        //create 2 duplication of original cam circuit
        int correction1 = cktTotVarNum;
        int correction2 = cktTotVarNum + camVarNum;
        vector<string> dupCkt1 =  duplicateCircuit(camCNFile, correction1);
        vector<string> dupCkt2 =  duplicateCircuit(camCNFile, correction2);
        dupCkt1.insert(dupCkt1.begin(), "c this is 1st duplication\n");
        dupCkt2.insert(dupCkt2.begin(), "c this is 2nd duplication\n");

        cktTotVarNum = correction2 + camVarNum; // update the total variable number
        //========================================================================================================================
        //connect CB of the two new duplications to miter
        vector<string> connect_CB1 = connectNets(camCBindex, correction1); 
        vector<string> connect_CB2 = connectNets(camCB2index, correction1); 
        connect_CB1.insert(connect_CB1.begin(), "c connect CB for 1st\n");
        connect_CB2.insert(connect_CB2.begin(), "c connect CB fot 2nd\n");
        addon_CB1 = get_index(camCBindex, correction1);
        addon_CB2 = get_index(camCBindex, correction2);
        addon_PI1 = get_index(camPIndex, correction1);
        addon_PI2 = get_index(camPIndex, correction2);
        addon_PO1 = get_index(camPOindex, correction1);
        addon_PO2 = get_index(camPOindex, correction2);

        //========================================================================================================================
        //assign new temp PI vector to new duplicated circuit
        vector<string> assign_PI1 = assign_value(PItemp, addon_PI1);
        vector<string> assign_PI2 = assign_value(PItemp, addon_PI2);
        assign_PI1.insert(assign_PI1.begin(), "c assign PI for 1st\n");
        assign_PI2.insert(assign_PI2.begin(), "c assign PI for 2nd\n");
        //========================================================================================================================
        //assign new temp PO vector to new duplicated circuit
        vector<string> assign_PO1 = assign_value(POtemp, addon_PO1);
        vector<string> assign_PO2 = assign_value(POtemp, addon_PO2);
        assign_PO1.insert(assign_PO1.begin(), "c assign PO for 1st\n");
        assign_PO2.insert(assign_PO2.begin(), "c assign PO for 2nd\n");
        //========================================================================================================================
        //merge all
        dupCkt1 += connect_CB1 + assign_PI1 + assign_PO1;
        dupCkt2 += connect_CB2 + assign_PI2 + assign_PO2;
        vector<string> addon = dupCkt1 + dupCkt2;
        int clauseNum = addon.size() - 8;
        string cmmtline1 = "c this file is generated by AddonSolver::addconstrains\n";
        string cmmtline2 = "c generated on " + get_localtime();
        string firstLine = "p cnf " + tostring(cktTotVarNum) + " " + tostring(clauseNum) + "\n";
        addon.insert(addon.begin(), firstLine);
        addon.insert(addon.begin(), cmmtline2);
        addon.insert(addon.begin(), cmmtline1); 
        print_vector(addon, target_cnf);
    }
}

void AddonSolver::export_PI()                                                         
{
	ofstream outfile;
	outfile.open("PI.txt", std::ios::out);
    for(map<int, string>::iterator index = PItemp.begin(); index != PItemp.end(); ++index)
    {
        outfile << indexVarDict[index->first] << "\t";
    }
    outfile << endl;
    for(map<int, string>::iterator index = PItemp.begin(); index != PItemp.end(); ++index)
    {
    	outfile << index->second << "\t";
    }
    outfile << endl;
}

void AddonSolver::parse_PO()  
{                                                       
	ifstream infile("PO.txt");
	//infile.open("PO.txt", std::ios::in);
	string first_line;
	string second_line;

	getline(infile, first_line);
	getline(infile, second_line);

	vector<string> name_temp;
	vector<string> value_temp;
	SplitString(first_line, name_temp, "\t");
	SplitString(second_line, value_temp, "\t");

	vector<string>::iterator value = value_temp.begin();
	for(vector<string>::iterator name = name_temp.begin(); name != name_temp.end(); ++name)
	{
		POtemp.insert(pair<int, string>(varIndexDict[*name], *value));
		value++;
	}
}

//========================================================================================================================




//========================================================================================================================
//implementation of soluFinder
SoluFinder::SoluFinder()
{
    if(!debug) print_progress("Final Solve   \t", 80);
}

SoluFinder::~SoluFinder()
{
}

void SoluFinder::freeze()
{
    for(vector<int>::iterator index = camCBindex.begin(); index != camCBindex.end(); ++index)
    {
        S.setFrozen(*index - 1, true);
    }            
}

void SoluFinder::find_solu()
{

    num2dup = OracPIs.size();
    totVarNum = num2dup*camVarNum;

    if(num2dup > 1) case_1();
    else case_2();
    if(!debug) print_progress("Almost done   \t", 90);

    string cmmtLine1 = "c This file is generated by SoluFinder\n";
    string cmmtLine2 = "c Generated on " + get_localtime();
    string firstLine = "p cnf " + tostring(totVarNum) + " " + tostring(clauseNum) + "\n";
    finalCNF.insert(finalCNF.begin(), firstLine);
    finalCNF.insert(finalCNF.begin(), cmmtLine1);
    finalCNF.insert(finalCNF.begin(), cmmtLine2);
    print_vector(finalCNF, "finalSolu.cnf");

    solve_it();

    remove("PI.txt");
    remove("finalSolu.cnf");
    remove("target.cnf");
}

void SoluFinder::case_1()
{
    vector<vector<string> > tmpCnfLs;
    for(int iteration = 0; iteration < num2dup; iteration++)
    {
        vector<string> duplication = duplicateCircuit(camCNFile, (iteration)*camVarNum);
        duplication.insert(duplication.begin(), "c this is the duplication\n");
        if(iteration != 0) 
        {
            vector<string> connection = connectNets(camCBindex, iteration*camVarNum);
            connection.insert(connection.begin(), "c this is connection\n");
            duplication += connection;
        }
        vector<int> PI_index = get_index(camPIndex, iteration*camVarNum);
        vector<int> PO_index = get_index(camPOindex, iteration*camVarNum);
        vector<string> assign_PI = assign_value(OracPIs.at(iteration), PI_index);
        assign_PI.insert(assign_PI.begin(), "c this is assign PI\n");
        vector<string> assign_PO= assign_value(OracPOs.at(iteration), PO_index);
        assign_PO.insert(assign_PO.begin(), "c this is assign PO\n");
        finalCNF+=duplication  + assign_PI + assign_PO;
    }

    totVarNum = num2dup * camVarNum;
    clauseNum = finalCNF.size() - 3*num2dup - num2dup + 1;
}

void SoluFinder::case_2()
{
    vector<string> duplication = camCNFile;
    vector<string> assign_PI = assign_value(OracPIs.at(0), camPIndex);
    vector<string> assign_PO = assign_value(OracPOs.at(0), camPOindex);
    camCNFile.insert(camCNFile.begin(), "c this is duplication\n");
    assign_PI.insert(assign_PI.begin(), "c this is assign_PI\n");
    assign_PO.insert(assign_PO.begin(), "c this is assign_PO\n");
    finalCNF+= duplication + assign_PI + assign_PO;
    totVarNum = camVarNum;
    clauseNum = finalCNF.size() - 3;
}

void SoluFinder::solve_it()
{
    if(debug == true) clog << "Start finding finalSolu" << endl;
    gzFile in = gzopen("finalSolu.cnf", "rb");
    parse_DIMACS(in, S_final);
    gzclose(in);
    freeze();
    S_final.eliminate(true);
    string content = "";
    if(!S_final.okay())
    {
        content += "UNSAT\n";
        if(debug == true) cerr << "Pre-process: UNSAT" << endl;
    }

    vec<Lit> dummy;
    lbool ret = S_final.solveLimited(dummy);

    if(ret == l_True)
    {
        content += "SAT\n";
        if(debug == true) clog << "SAT" << endl;
        for(int i = 0; i < S_final.nVars(); i++)
        {
            if(S_final.model[i] != l_Undef)
            {
                content += " ";
                if(S_final.model[i] == l_True) content += "" + tostring(i + 1);
                else content += "-" + tostring(i + 1);
                
            }
        }
        content += " 0\n";
    }
    else if(ret == l_False) content += "UNSAT\n";
    else    content += "INDET\n";

    if (debug) 
    {
    	ofstream outfile("EC_solution");
    	outfile << content;
    	outfile.close();   
    }

    if(!debug)
     {
        print_progress("FINISH SOLVING\t", 100);
        cout << endl;
    }

    totoal_all = clock() - start;

} 

void SoluFinder::print_solution()
{
	vector<string> content;
    clog << "SOLUTION IS:" << endl;
    for(vector<int>::iterator node = camCBindex.begin(); node != camCBindex.end(); ++node)
    {
        if(S_final.model[*node - 1] != l_Undef)
        {
            if(S_final.model[*node - 1] == l_True) Solution.insert(std::pair<int,string>(*node, "1"));
            else Solution.insert(std::pair<int, string>(*node, "0"));
        }
    }

    for(map<int, string>::iterator index = Solution.begin(); index != Solution.end(); ++index)
    {
        clog << indexVarDict[index->first] << "\t";
        content.push_back(indexVarDict[index->first] + "\t");
    } 
    clog << endl;
    content.push_back("\n");
    for(map<int, string>::iterator index = Solution.begin(); index != Solution.end(); ++index)
    {
        clog << index->second << "\t";
        content.push_back(index->second + "\t");
    }
    clog << endl;
    content.push_back("\n");

    if(out_file)
    {
        print_vector(content, Solver_solution);
        cout << "\nSolution saved in: " << Solver_solution << endl;
    }

}                                                                          

//========================================================================================================================
//implementation of support 
Support::Support(int one, char ** two)
{
    init();
    string title(" ");
    progress_init(&bar, strdup(title.c_str()), 100, PROGRESS_NUM_STYLE);
	int argc = one;
	char **argv = two;
    a.add("debug", 'd', "change to debug mode");
    a.add<string>("outfile", 'o', "write solution to outfile", false, "NULL");
    a.add("help-verb", 0, "print verbose help message");
    a.footer("<Cam.v> <Orac.sh>");
    a.parse_check(argc, argv);

    if(a.exist("help-verb"))
    {   cout << endl;
        cout << "SOLVER is a SAT-based Incremental Solver, it takes <Cam.v> and <Orac.sh> as inputs. " << endl;
        cout << "During each iteration, Solver will generate 'PI.txt' as outfile, and read in 'PO.txt'." << endl;
        cout << "For detailed information please visit:" << endl;
        cout << "\t\thttp://broccoli.ecs.umass.edu/decam\n" << endl;
        cout << "<Cam.v>       \t\tcamouflaged netlist to be solved" << endl;
        cout << "<Orac.sh>     \t\tshell script to query Oracle" << endl;
        cout << "-d, --debug   \t\tchange to debug mode, solver will generate log messages and log files" << endl;
        cout << "-o, --outfile \t\texport solution to this file" << endl;
        exit(0);
    }
    bool ok = a.parse(argc, argv);

    if(argc == 1 || a.exist("help")) {cerr << a.usage(); exit(4);}
    if(!ok) {cerr << a.error() << endl << a.usage(); exit(5);}
    if(a.rest().size() < 2) {cout << "Please provide both <Cam.v> and <Orac.sh>" << endl; exit(6);}
    else
    {
	    if((access(a.rest()[0].c_str(), 04)) != -1){Came_file_path = realpath(a.rest()[0].c_str(), NULL);}
	    else{cout << "error: Camouflage Circuit is not existed or read prohibited!!!\n"; exit(7);}
	    if((access(a.rest()[1].c_str(), 04)) != -1){Orac_file_path = realpath(a.rest()[1].c_str(), NULL);}
	    else{cout << "error: Shell file is not existed or read prohibited!!!\n"; exit(8);}
    }
    debug = a.exist("debug");
    if(a.exist("outfile"))
    {
        Solver_solution = a.get<string>("outfile").c_str();   
        out_file = true;   
    }

}





