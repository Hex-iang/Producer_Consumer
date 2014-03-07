#include "header.h"

int main(void){
	srand(static_cast<unsigned int>(time(NULL)));
	producer * prod[producer_num];

	int keyboard_iret = pthread_create(&keyboard_thread, NULL, keyboard_signal_handler, NULL);
	
	for (int i = 0; i < producer_num; ++i) {
		prod[i] = new producer(i);
	}
	
	while(~done){
		while(pause_the_program);
		
		if(update_screen){
			system("clear");	
			pthread_mutex_lock(&in_buf_mutex);
			pthread_mutex_lock(&screen_mutex);
			update_screen = false;
			pthread_mutex_unlock(&screen_mutex);
			cout<<"================================================="<<endl;
			cout<<"|\tMaterial Buffer: ";
			for(int i = 0; i < in_buf.size(); i++){
				cout<<material[in_buf[i]]<<" ";
			}
			cout<<endl;
			cout<<"|\tMaterial Buffer Size:"<<in_buf.size()<<endl;
			cout<<"================================================="<<endl;
			pthread_mutex_unlock(&in_buf_mutex);
		}

	}
	
	
	for (int i = 0; i < producer_num; ++i) {
		delete prod[i];
	}
	return 0;	

}

// Producer class method
producer::producer(int id){
	this->product_id = id;
	this->iret = pthread_create(&this->thread, NULL, producer::start_producer, this);
	if(this->iret){
		fprintf(stderr, "THERE IS AN ERROR WHEN CREATING THREADS : %s\n", strerror( errno ));
	}
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
	while(true){
		
		while(pause_the_program);
		
		int interval = 1000*(rand()%1000 + 1);
		usleep(interval);
		
		pthread_mutex_lock(&in_buf_mutex);
		if (in_buf.size() >= in_buf_size){
			pthread_exit(NULL);
		}else{
			pthread_mutex_lock(&screen_mutex);
			in_buf.push_back(product_id);
			update_screen = true;
			pthread_mutex_unlock(&screen_mutex);
		}
		pthread_mutex_unlock(&in_buf_mutex);
	}

};
//Consumer class method
void * consumer::start_consumer(void * args){
	while(true){
		((consumer *)args)->produce_product(((producer *)args)->product_id);	
	}
	return NULL;
}

void consumer::produce_product(){
	// For each operator to produce product, 
	while(true){

		while(pause_the_program);

		pthread_mutex_lock(&in_buf_mutex);
		if (in_buf.size() >= in_buf_size){
			pthread_mutex_lock(&screen_mutex);
			int mater1, mater2;
			mater1 = in_buf.pop_back();
			for(int i = 0; i < in_buf.size(); i++){
				mater2 = in_buf[i];
				if (mater2 == mater1){
					in_buf.erase(in_buf.begin()+i);
					break;
				}
			}
			update_screen = true;
			pthread_mutex_unlock(&screen_mutex);
		}else{

		}
		pthread_mutex_unlock(&in_buf_mutex);
	}

};

// Function for keyboard handling
void * keyboard_handler(void * sig){
	while(true){
		char ch;
		cin.get(ch);
		if(ch == RETURN){
			if(!pause_the_program){
				pause_the_program = true;
			}else{
				pause_the_program = false;
			}		
		}
	}
}

