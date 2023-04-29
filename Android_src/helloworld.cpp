#include "helloworld.h"

Zevel::Zevel(int count):_junk(count),_zevel(0)
	{
		std::cout<<"Zevel(count cot)"<<std::endl;
		std::cout<<"_junk "<<_junk<<std::endl;
		std::cout<<"_zevel "<<_zevel<<std::endl;
		_ptr = new int[15];
	}

	Zevel::Zevel(Zevel && other_zevel)
	{
		_ptr = other_zevel._ptr;
		other_zevel._ptr = nullptr;
		_zevel = other_zevel._zevel;
		_junk = other_zevel._junk;
		std::cout<<" call Zevel(Zevel && )"<<std::endl;
	}
	


/*
int main(int argc, char *argv[]){
	int f = 9;
	std::vector<int> zevel;
	zevel.push_back(7);
	auto other_vec = std::move(zevel);
	zevel.push_back(7);
   std::cout << "Hello World!" <<f<< std::endl;
   std::cout << "zevel size is: "<<zevel.size()<<std::endl;
   std::cout << "other_vec size is: "<<other_vec.size()<<std::endl;
   
  // Zevel zevel_instance(4);
  // auto moved_zevel = std::move(zevel_instance);
   
   
   return 0;
}
*/