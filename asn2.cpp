#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

//Total variety of materials
const char material[] = {'A','B','C'};
const int mater_cnt = 3;
//Total variety of products
const char product[] = {'X','Y','Z'};
const int prod_cnt = 3;
//Total generator numbers
const int generator_num = 3;
const int in_buf_size = 10;
vector<int> in_buf;

class generator{
private:
	int id;
public:
	generator(int num):id(num){}
	~generator(){}
	int gen_mater();
	void wake_up_operator();
};
int generator::gen_mater(){
	int t = 10*(rand()%100 + 1);

	if(in_buf.size() == in_buf_size){
		
		wake_up_operator();
	}
    return 0;
}

void pause(){
	
}

void resume(){
	
}

int main(void)
{
	srand(time(NULL));
    
	generator * gen[generator_num];
    
	for(int i = 0; i < generator_num;++i){
		gen[i] = new generator(i);
	}
    
    
    for(int i = 0; i < generator_num;++i){
		delete gen[i];
        gen[i] = NULL;
	}
}