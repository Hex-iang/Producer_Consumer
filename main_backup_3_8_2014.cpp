#include "header.h"

int main(void){
	srand(static_cast<unsigned int>(time(NULL)));

	get_input();

	if(pthread_create(&keyboard_thread, NULL, keyboard_handler, NULL)){
        fprintf(stderr, "THERE IS AN ERROR WHEN CREATING THREADS : %s\n", strerror( errno ));
    }

    init_producer_consumer();

	while(~done){

	}
	
	destroy_producer_consumer();

	return 0;	

}

// Producer class method
producer::producer(int id){
	this->product_id = id;
	if(pthread_create(&this->thread, NULL, producer::start_producer, this)){
		fprintf(stderr, "THERE IS AN ERROR WHEN CREATING THREADS : %s\n", strerror( errno ));
	}
}
producer::~producer(){
	pthread_exit(NULL);
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
			pthread_cond_wait(&producer_cond,&in_buf_mutex);
		}else{
			in_buf.push_back(product_id);
			pthread_cond_signal(&consumer_cond);
			print_info_to_screen("Generator Thread");
		}
		pthread_mutex_unlock(&in_buf_mutex);
	}

};
//Consumer class method
consumer::consumer(){
	if(pthread_create(&this->thread, NULL, consumer::start_consumer, this)){
		fprintf(stderr, "THERE IS AN ERROR WHEN CREATING THREADS : %s\n", strerror( errno ));
	}
}
consumer::~consumer(){
	pthread_exit(NULL);
}
void * consumer::start_consumer(void * args){
	while(true){
		((consumer *)args)->produce_product();	
	}
	return NULL;
}

void consumer::produce_product(){
	// For each operator to produce product, 
	while(true){

		while(pause_the_program);

		pthread_mutex_lock(&in_buf_mutex);
		if (in_buf.size() > 0){
			int mater1, mater2;
			mater1 = in_buf[in_buf.back()];
			in_buf.pop_back();
			for(int i = 0; i < in_buf.size(); i++){
				mater2 = in_buf[i];
				if (mater2 == mater1){
					in_buf.erase(in_buf.begin()+i);
					break;
				}
			}
			pthread_cond_signal(&producer_cond);
			print_info_to_screen("Operator Thread");
		}else{
			pthread_cond_wait(&consumer_cond,&in_buf_mutex);
		}
		pthread_mutex_unlock(&in_buf_mutex);
	}

};

// Function for the main process
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

void get_input(){

	printf("Please Input the Number of consumers:");
	cin>>consumer_num;
	cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
	printf("Please Input the Number of tools:");
	cin>>tool_num;
	cin.ignore(numeric_limits<std::streamsize>::max(), '\n');

}

void print_info_to_screen(const char * thread_type){
	system("clear");	
	cout<<"====================================================="<<endl;
	cout<<"|\tMaterial In Buffer: ";
	for(int i = 0; i < in_buf.size(); i++){
		cout<<material[in_buf[i]]<<" ";
	}
	cout<<endl;
	cout<<"|\tMaterial Buffer Size:"<<in_buf.size()<<endl;
	cout<<"|\tProduct In Buffer: ";
	int out_buf_size = 10;
	if(out_buf_size > out_buf.size()) out_buf_size = out_buf.size();
	for(int i = 0; i < out_buf_size; i++){
		cout<<product[out_buf[i]]<<" ";
	}
	cout<<endl;
	cout<<"|\tProduct Buffer Size:"<<out_buf.size()<<endl;
	cout<<"====================================================="<<endl;
}

void init_producer_consumer(){
	for (int i = 0; i < producer_num; ++i) {
		prod[i] = new producer(i);
	}

	for (int i = 0; i < consumer_num; ++i) {
		consumer* tmp;
		tmp = new consumer();
		cons.push_back(tmp);
	}
}
void destroy_producer_consumer(){
	for (int i = 0; i < producer_num; ++i) {
		delete prod[i];
	}

	for (int i = 0; i < consumer_num; ++i) {
		delete cons[i];
	}
}