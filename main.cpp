#include "header.h"

int main(void){
	producer * prod[producer_num];	
	for (int i = 0; i < producer_num; ++i) {
		prod[i] = new producer(i);
	}
	
	pthread_join((prod[0]->thread),NULL);
	pthread_join((prod[1]->thread),NULL);
	pthread_join((prod[2]->thread),NULL);
	
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
	((producer *)args)->generate_material(((producer *)args)->product_id);
	return NULL;
}
void producer::generate_material(int product_id){
	// For each generator to generate material, 
	// it requests a random time interval between 0.01 seconds to 1 seconds
	srand(static_cast<unsigned int>(time(NULL)));
	int interval = 10*(rand()%100 + 1);
	usleep(interval);
	
	cout<<"This is thread"<<pid<<", Which produce product"<<material[product_id]<<endl;

};

