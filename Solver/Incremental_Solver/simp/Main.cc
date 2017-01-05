/*****************************************************************************************[Main.cc]
Copyright (c) 2003-2006, Niklas Een, Niklas Sorensson
Copyright (c) 2007,      Niklas Sorensson

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************************************/

#include <errno.h>
#include <signal.h>
#include <zlib.h>
#include <sys/resource.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include "utils/System.h"
#include "utils/ParseUtils.h"
#include "utils/Options.h"
#include "incre/incre.h"
#include "core/Dimacs.h"
#include "simp/SimpSolver.h"
using namespace Incre;
using namespace Minisat;
using namespace std;


//static Solver* solver;
int main(int argc, char** argv)
{

try
{
    Support support(argc, argv);

//=================================================================================================================================
// build miter based on income cam file

	MiterSolver *MTR = new MiterSolver;
	MTR->buildmiter();
	delete MTR;	
//=================================================================================================================================
// incrementally solve


    while(1)
    {
     	AddonSolver *ADD = new AddonSolver;

    	ADD->start_solving();  
    	if(IncreSolver::check_ret() == l_True)
    	{
            ADD->queryOrac();
	    ADD->continue_solving();
    	} 	
    	else 
        {
            break;
        }
            delete ADD;
    }
//================================================================================================================================
// find final Solution
    SoluFinder *finder = new SoluFinder;
    finder->find_solu();
    IncreSolver::print_state();
    finder->print_solution();
    delete finder; 
    return 0;
} catch(...)
{
    cerr << "\n\n===============================================================================" << endl;
    cerr << "FATAL ERROR, EXIT" << endl;
    exit(0);

}
}
