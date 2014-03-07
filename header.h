#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <unistd.h>
#include <signal.h>

using namespace std;

//Total variety of materials
const char material[] = {'A','B','C'};
const int mater_cnt = 3;
//Total variety of products
const char product[] = {'X','Y','Z'};
const int prod_cnt = 3;
//Total producer numbers
const int producer_num = 3;
const int tool_num = 3;

//keyboard key value
const int ESCAPE=27;
const int RETURN=10;

static bool done = false;
static bool pause_the_program = false;
static bool update_screen = false;
const int in_buf_size = 10;

vector<int> in_buf;
vector<int> out_buf;

pthread_t keyboard_thread;
pthread_mutex_t in_buf_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t in_buf_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t out_buf_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t out_buf_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t screen_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t tool_mutex = PTHREAD_MUTEX_INITIALIZER;

class producer{
public:
	pthread_t thread;
	int iret;
	int pid;
	int product_id;

	producer(int id = 0);
	~producer();
	static void * start_producer(void * args);
	void generate_material(int product_id);

};

class consumer{
public:	
	pthread_t thread;
	int iret;
	int pid;
	consumer();
	~consumer();
	static void start_consumer(void *args);
	void produce_product();
};

void * keyboard_handler(void * sig);