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
vector<int> in_buf;
vector<int> out_buf;

pthread_mutex_t in_buf_mutex;
pthread_cond_t in_buf_cond;
pthread_mutex_t out_buf_mutex;
pthread_cond_t out_buf_cond;

class producer{
public:
	pthread_t thread;
	int pid;
	int product_id;

	producer(int id = 0);
	~producer();
	static void * start_producer(void * args);
	void generate_material(int product_id);

};

class consumer{
	pthread_t thread;
	int pid;
};