#include "header.h"

int main(void){
	producer * prod[producer_num];
	system("clear");	
	for (int i = 0; i < producer_num; ++i) {
		prod[i] = new producer(i);
	}
	
	for (int i = 0; i < producer_num; ++i) {
		pthread_join((prod[i]->thread),NULL);
	}
	
	for (int i = 0; i < producer_num; ++i) {
		delete prod[i];
	}
	return 0;	

}

// Producer class method
producer::producer(int id){
	this->product_id = id;
	this->pid = pthread_create(&this->thread, NULL, producer::start_producer, this);
}
producer::~producer(){
	
}
void * producer::start_producer(void * args){
	while(true){
		((producer *)args)->generate_material(((producer *)args)->product_id);	
	}
	return NULL;
}
void producer::generate_material(int product_id){
	// For each generator to generate material, 
	// it requests a random time interval between 0.01 seconds to 1 seconds
	srand(static_cast<unsigned int>(time(NULL)));
	int interval = 10*(rand()%100 + 1);
	usleep(interval);
	//printf("This is thread #%d\n",product_id);

	pthread_mutex_lock(&in_buf_mutex);
	if (in_buf_size <= in_buf_cnt){
		pthread_exit(NULL);
	}else{
		in_buf[in_buf_cnt++] = product_id;
		printf("Product #%c\n",material[product_id]);
	}
	pthread_mutex_unlock(&in_buf_mutex);

};


