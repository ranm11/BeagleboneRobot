#include<iostream>
#include <vector>
//#ifdef EMSCRIPTEN
//	#include <emscripten.h>
//#endif
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


class Zevel
{
	public:
	Zevel(int count);
	Zevel(Zevel & zevel);
	Zevel(Zevel && other_zevel);
	
	
	private:
	int _zevel;
	int _junk;
	int* _ptr;
};