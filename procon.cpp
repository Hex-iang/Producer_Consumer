#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <unistd.h>

using namespace std;

//Total variety of materials
const char material[] = {'A','B','C'};
const int mater_cnt = 3;
//Total variety of products
const char product[] = {'X','Y','Z'};
const int prod_cnt = 3;
//Total producer numbers
const int producer_num = 3;
const int in_buf_size = 10;

void pause();
void resume();
void init();

class producer{
private:
	int id;
	int iret;
	pthread_t thread;
public:
	producer(int num);
	~producer(){}
	void * gen_mater();
	void wake_up_consumer();
	void go_sleeping();
};

class consumer{
private:
	int id;
	int iret;
	pthread_t thread;
public:
	consumer(int num):id(num){}
	~consumer(){}
	void * gen_prod();
	void wake_up_producer();
};

vector<int> in_buf;
producer * pro[producer_num];
int iret[producer_num];

int main(void)
{
	srand(time(NULL));
	pthread_t pro_thread[producer_num];
	for(int i = 0; i < producer_num;++i){
		pro[i] = new producer(i);
	}


    for(int i = 0; i < producer_num;++i){
		delete pro[i];
        pro[i] = NULL;
	}
    
}

void pause(){
	
}

void resume(){
	
}

producer::producer(int num):id(num){
	this.iret = pthread_create( &thread, NULL, gen_mater, NULL);
}
void * producer::gen_mater(){
	int t = 10*(rand()%100 + 1);
	usleep(t);
	if(in_buf.size() == in_buf_size){
		this.go_sleeping();
		wake_up_consumer();
	}
	else{
		in_buf.push_back(id);
	}
    return 0;
}
void producer::wake_up_consumer(){

}

void producer::go_sleeping(){

}

void * comsumer::gen_prod(){

}
void consumer::wake_up_producer(){

}