#include<iostream>
#include<unistd.h>
namespace Incre {

	int existFile(const char * path)
	{
		int access_result = access(path, F_OK);
		if(access_result == -1) std::cout <<"file: " << path << " is not exist!\n";
		else return 1;

	}
}



