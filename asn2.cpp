#include <iostream>
#include <thread>

using namespace std;

const char material[] = {'A','B','C'};
const int mater_cnt = 3;
const char product[] = {'X','Y','Z'};
const int prod_cnt = 3;

class generator{
private:
	int id;
public:
	generator(int num):id(num){}
	~generator(){}
	int get_id()const{return id;}
	int gen_mater(){return this.get_id();}
};

class operator{
private:
	int id;
public:
	operator(int num):id(num){}
	~operator(){}
	int get_id()const{return id;}
	int gen_prod(int * buf);
};

operator::gen_prod(int * buf){
	int mater1 = id % mater_cnt;
	int mater2 = (id + 1) % mater_cnt;
	
}

void pause(){
	
}

void resume(){
	
}

int main(void)
{

}