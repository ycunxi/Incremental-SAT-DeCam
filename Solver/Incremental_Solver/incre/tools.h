#ifndef INCRE_INCRE_TOOLS_H
#define INCRE_INCRE_TOOLS_H

#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <regex>



using namespace std;

namespace Incre
{


inline string get_localtime()
    {
        time_t nowtime;
        nowtime = time(NULL);
        struct tm *local;
        local = localtime(&nowtime);
        return asctime(local);
    }
inline void SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c)   //source, result, seperator
    {
      std::string::size_type pos1, pos2;
      pos2 = s.find(c);
      pos1 = 0;
      while(std::string::npos != pos2)      //if we can find c in s
      {
        v.push_back(s.substr(pos1, pos2-pos1));
     
        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
      }
      if(pos1 != s.length())
        v.push_back(s.substr(pos1));
    }
inline string stripComments(string in)
{
    regex pattern("\\/\\/(?!RE__).*\\n");
    smatch m;
    regex_search(in,m,pattern);

    while(regex_search(in,m,pattern))
    {
        in = in.replace(in.find(m[0].str()), m[0].str().length(), "");
    }

    return in;
}
inline string toBinary(int n, int length)
{
    string r;
    string result;
    if( n == 0)
    {
        r = "0";
    }
    while( n!= 0)
    {
        r = (n%2 == 0 ? "0":"1") + r;
        n/=2;
    }
    result = r;
    for(unsigned int i = 0; i < length - r.length(); i++)
    {
        result = "0" + result;
    }
    return result;
}

inline string Readall(const char* path)
    {

        fstream in;
        in.open(path, ios::in);
        istreambuf_iterator<char> beg(in), end;
        string strdata(beg, end);
        in.close(); 

        return strdata;
    }


inline void strip_all(string& str,const string old_value)
    {
    	while(true)
    	{
    		if(str.find(old_value) != string::npos)
    		{
    			str.replace(str.find(old_value), old_value.length(), "");
    		}
    		else
    		{
    			break;
    		}
    	}

    }

inline    void load_gateTypeDict(map<string, int>& gateTypeDict)
    {
        gateTypeDict.insert(std::pair<string, int>("and",1));
        gateTypeDict.insert(std::pair<string, int>("or", 2));
        gateTypeDict.insert(std::pair<string, int>("xor", 3));
        gateTypeDict.insert(std::pair<string, int>("inv", 4));
        gateTypeDict.insert(std::pair<string, int>("buf", 5));
        gateTypeDict.insert(std::pair<string, int>("nand", 6));
        gateTypeDict.insert(std::pair<string, int>("nor", 7));
        gateTypeDict.insert(std::pair<string, int>("one", 8));
        gateTypeDict.insert(std::pair<string, int>("zero", 9));
    }

inline void print_map(map<int, string>& input_map)
{
    cout << "map contain:" << endl;
    for(map<int, string>::iterator it = input_map.begin(); it != input_map.end(); ++it)
    {
        cout << it->first << " " << it->second << endl;
    }
}

 inline   vector<string> ReadByColon(const char* path)
    {
    	vector<string> result;
    	string read_all;
    	read_all = Readall(path);
    	strip_all(read_all, "\r");
    	SplitString(read_all, result, ";");
    	return result;
    }
inline vector<string> find_netname(string gate)

    {
    	string s = gate;
    	vector<string> container;
    	vector<string> netnames;
    	SplitString(s, container, "(");
    	for (vector<string>::iterator iter = container.begin(); iter != container.end(); ++iter) {
    		smatch nets;
    		regex pattern("([^\\)]+)([\\)])");
    		regex_search(*iter, nets, pattern);
    		for (unsigned int i = 0; i < nets.size(); i++) {
    			if (nets[i].str().find(")") == string::npos) netnames.push_back(nets[i].str());
    		}


    	}

    	for (vector<string>::iterator iter = netnames.begin(); iter != netnames.end(); ++iter) {
    	}
    	return netnames;

    }

inline string find_gatetype(string line)
    {
//        cout << line << endl;
        strip_all(line, " ");
        regex pattern("^([a-z]*)([0-9]*)");
        smatch result;
        regex_search(line, result, pattern);
        return result[1].str();
    }

inline void print_vector(vector<string> list, const char * path)
    {
        ofstream outfile(path);
        for(vector<string>::iterator iter = list.begin(); iter != list.end(); ++iter)
        {
            outfile << *iter;
        }
    }
inline void print_vector(vector<int> list, const char * path)
{
        ofstream outfile(path);
        for(vector<int>::iterator iter = list.begin(); iter != list.end(); ++iter)
        {
            outfile << *iter << " ";
        }
}


template <typename T>
std::vector<T> operator+(const std::vector<T> &A, const std::vector<T> &B)
{
    std::vector<T> AB;
    AB.reserve( A.size() + B.size() );                // preallocate memory
    AB.insert( AB.end(), A.begin(), A.end() );        // add A;
    AB.insert( AB.end(), B.begin(), B.end() );        // add B;
    return AB;
}

template <typename T>
std::vector<T> &operator+=(std::vector<T> &A, const std::vector<T> &B)
{
    A.reserve( A.size() + B.size() );                // preallocate memory without erase original data
    A.insert( A.end(), B.begin(), B.end() );         // add B;
    return A;                                        // here A could be named AB
}
}

#endif
