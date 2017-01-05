#ifndef     INCRE_INCRE_DICT_H
#define     INCRE_INCRE_DICT_H

#include <string>
#include <vector>
#include <sstream>




using namespace std;


namespace Incre{

inline string tostring(int i)
{
	stringstream ss;
	ss << i;
	return ss.str();
}


inline vector<string> transGATE(int type, vector<int> & lineIn, int & lineOut)
{
    vector<string> cnfLines;
    string outV = tostring(lineOut);
    string firstLine = "";

    switch(type)
    {
        case 1: //  and
        {
 //           cout << "enter case 1\n";
            for (std::vector<int>::iterator iter = lineIn.begin(); iter != lineIn.end(); ++iter)
            {
                string inVstr = tostring(*iter);
                firstLine += "-" + inVstr + " ";
                cnfLines.push_back(inVstr + " -" + outV + " 0\n");
            }
            firstLine += outV + " 0\n";
            cnfLines.insert(cnfLines.begin(), firstLine);
            break;

        }

        case 2: // or
        {
//            cout << "enter case 2\n";
            for(std::vector<int>::iterator iter = lineIn.begin(); iter != lineIn.end(); ++iter)
            {
                string inVstr = tostring(*iter);
                firstLine += inVstr + " ";
                cnfLines.push_back("-" + inVstr + " " + outV + " 0\n");
            }
            firstLine += "-" + outV + " 0\n";
            cnfLines.insert(cnfLines.begin(), firstLine);
            break;
        }

        case 3: //xor
        {
//            cout << "enter case 3\n";
            string inV1 = tostring(*lineIn.begin());
            string inV2 = tostring(*(lineIn.begin() + 1));
            //  string outV = tostring(lineOut);
            cnfLines.push_back("-" + inV1 + " -" + inV2 + " -" + outV + " 0\n");
            cnfLines.push_back(inV1 + ' ' + inV2 + " -" + outV + " 0\n");
            cnfLines.push_back(inV1 + " -" + inV2 + " " + outV + " 0\n");
            cnfLines.push_back("-" + inV1 + " " + inV2 + " " + outV + " 0\n");
            break;
        }
        case 4: //inv
        {
//            cout << "enter case 4\n";
            string inV = tostring(lineIn[0]);
            cnfLines.push_back(inV + " " + outV + " 0\n");
            cnfLines.push_back("-" + inV + " -" + outV + " 0\n");
            break;
        }

        case 5: //buf
        {
//            cout << "enter case 5\n";
            string inV = tostring(lineIn[0]);
            cnfLines.push_back(inV + " -" + outV + " 0\n");
            cnfLines.push_back("-" + inV + " " + outV + " 0\n");
            break;
        }

        case 6: //nand
        {
//            cout << "enter case 6\n";
            for(std::vector<int>::iterator iter = lineIn.begin(); iter != lineIn.end(); ++iter)
            {
                string inVstr = tostring(*iter);
                firstLine += "-" + inVstr + " ";
                cnfLines.push_back(inVstr + " " + outV + " 0\n");
            }
            firstLine += "-" + outV + " 0\n";
            cnfLines.insert(cnfLines.begin(), firstLine);
            break;
        }

        case 7: //nor
        {
//            cout << "enter case 7\n";
            for(std::vector<int>::iterator iter = lineIn.begin(); iter != lineIn.end(); ++iter)
            {
                string inVstr = tostring(*iter);
                firstLine += inVstr + " ";
                cnfLines.push_back("-" + inVstr + " -" + outV + " 0\n");
            }
            firstLine += outV + " 0\n";
            cnfLines.insert(cnfLines.begin(), firstLine);
            break;
        }

        case 8: //one, force certain( here is lineOut ) to be 1
        {
//            cout << "enter case 8\n";
            cnfLines.push_back(tostring(lineOut) + " 0\n");
            break;
        }

        case 9: //zero, , force certain( here is lineOut ) to be 1
        {
//            cout << "enter case 9\n";
            cnfLines.push_back("-" + tostring(lineOut) + " 0\n");
            break;
        }

        default:
        {
            cout << "gate " << type << " is not acceptable!!!\n";
            break;
        }
    }

    return cnfLines;

}



}

#endif
