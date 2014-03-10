#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <unistd.h>
#include <semaphore.h>

using namespace std;

//Total variety of materials
const char material[3] = {'A','B','C'};
const int mater_cnt = 3;
//Total variety of products
const char product[3] = {'X','Y','Z'};
const int prod_cnt = 3;
//Total producer numbers
const int producer_num = 3;
int consumer_num = 3;
int tool_num = 3;
const int max_interval = 4;
int tool_cnt = 0;
int product_cnt[3] = { 0, 0, 0 };
int material_cnt[3] = { 0, 0, 0};
//keyboard key value
const int ESCAPE=27;
const int RETURN=10;

static bool done = false;
static bool pause_the_program = false;
const int in_buf_size = 10;

char sys_info[100];

vector<int> in_buf;
vector<int> out_buf;

pthread_t keyboard_thread;
pthread_mutex_t in_buf_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t out_buf_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t producer_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t consumer_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t tool_mutex = PTHREAD_MUTEX_INITIALIZER;

class producer{
public:
	pthread_t thread;
	int producer_id;
	int product_id;

	producer(int id = 0);
	~producer();
	static void * start_producer(void * args);
	void generate_material(int product_id);
	bool material_check();
};

class consumer{
public:	
	int consumer_id;
	pthread_t thread;
	consumer(int id = 0);
	~consumer();
	static void * start_consumer(void *args);
	void produce_product();
	int product_check(int material_1, int material_2);
	bool get_in_buf_material(int* ptr_m1, int* ptr_m2);
};

producer * prod[producer_num];
vector<consumer *> cons;


void * keyboard_handler(void * sig);
void get_input();
void print_info_to_screen(const char * thread_type);
void init_producer_consumer();
void destroy_producer_consumer();