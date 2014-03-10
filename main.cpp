#include "header.h"

int main(void){
	srand(static_cast<unsigned int>(time(NULL)));
    
	get_input();
    
	if(pthread_create(&keyboard_thread, NULL, keyboard_handler, NULL)){
        fprintf(stderr, "THERE IS AN ERROR WHEN CREATING THREADS : %s\n", strerror( errno ));
    }
    
    init_producer_consumer();
    
	while(~done){
		for(int i = 0; i < producer_num; ++i)
			pthread_join(prod[i]->thread,NULL);
        
		for(int i = 0; i < consumer_num; ++i)
			pthread_join(cons[i]->thread,NULL);
	}
	
	destroy_producer_consumer();
    
	return 0;
    
}

// Producer class method
producer::producer(int id){
	this->producer_id = id;
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
bool producer::material_check(){
	if(material_cnt[product_id] == 0){
		return true;
	}
    
	if(in_buf.size() >= 8){
		for(int i = 0;i < 3;++i){
			if(material_cnt[i] == 0){
				return false;
			}
		}
	}
    
	return true;
}
void producer::generate_material(int product_id){
	// For each generator to generate material,
	// it requests a random time interval between 0.01 seconds to 1 seconds
	while(true){
		int interval = max_interval*1000*(rand()%1000 + 1);
        
		pthread_mutex_lock(&in_buf_mutex);
		if (in_buf.size() >= in_buf_size){
			pthread_cond_broadcast(&consumer_cond);
			sprintf(sys_info,"Generator Thread %d Goes to sleep",producer_id);
			print_info_to_screen(sys_info);
			pthread_cond_wait(&producer_cond,&in_buf_mutex);
		}else if(material_check()){
			material_cnt[product_id]++;
			in_buf.push_back(product_id);
			pthread_cond_broadcast(&consumer_cond);
			sprintf(sys_info,"Generator Thread %d Producing Material %c",producer_id,material[product_id]);
			print_info_to_screen(sys_info);
		}
		pthread_mutex_unlock(&in_buf_mutex);
        
		usleep(interval);
	}
    
};
//Consumer class method
consumer::consumer(int id){
	this->consumer_id = id;
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
bool consumer::get_in_buf_material(int* ptr_m1, int* ptr_m2){
	int i,j;
	int item;
	for(i = 0; i < in_buf.size() - 1;i++){
		for(j = i + 1; j < in_buf.size(); j++){
			if(in_buf[i] != in_buf[j]){
				*ptr_m1 = in_buf[i];
				*ptr_m2 = in_buf[j];
				item = product_check(*ptr_m1,*ptr_m2);
				if( item != -1 && (out_buf.empty() || out_buf.back() != item)){
					material_cnt[*ptr_m1]--;
					material_cnt[*ptr_m2]--;
					swap(in_buf[j],in_buf.back());
					in_buf.pop_back();
					swap(in_buf[i],in_buf.back());
					in_buf.pop_back();
					
					return true;
				}
			}
		}
	}
	return false;
}
int consumer::product_check(int material_1, int material_2){
	int check_result = -1;
	if ( (material_1 == 0 && material_2 == 1) || (material_1 == 1 && material_2 == 0) )
	{
		check_result = 0;
	}
	if ( (material_1 == 0 && material_2 == 2) || (material_1 == 2 && material_2 == 0) )
	{
		check_result = 1;
	}
	if ( (material_1 == 2 && material_2 == 1) || (material_1 == 1 && material_2 == 2) )
	{
		check_result = 2;
	}
	
	if(check_result != -1){
		for (int i = 0; i < 3; ++i)
		{
			if(product_cnt[check_result] - product_cnt[i] >= 10)
				check_result = -1;
		}
	}
    
	return check_result;
    
}
void consumer::produce_product(){
	// For each operator to produce product,
	while(true){
		int interval = max_interval*1000*(rand()%1000 + 1);
		pthread_mutex_lock(&tool_mutex);
		if(tool_cnt <= 1){
			pthread_mutex_unlock(&tool_mutex);
			continue;
		}
		tool_cnt -= 2;
		pthread_mutex_unlock(&tool_mutex);
		pthread_mutex_lock(&in_buf_mutex);
		if (in_buf.size() > 2){
			int material_1, material_2;
			pthread_mutex_lock(&out_buf_mutex);
			bool tmp_flag = get_in_buf_material(&material_1, &material_2);
			pthread_mutex_unlock(&out_buf_mutex);
			if(false == tmp_flag){
				pthread_cond_broadcast(&producer_cond);
				sprintf(sys_info,"Operator Thread %d Goes to Sleep",consumer_id);
				print_info_to_screen(sys_info);
				pthread_cond_wait(&consumer_cond,&in_buf_mutex);
			}
            
			int item = product_check(material_1,material_2);
            
			product_cnt[item]++;
			pthread_cond_broadcast(&producer_cond);
			pthread_mutex_lock(&out_buf_mutex);
			out_buf.push_back(item);
			pthread_mutex_unlock(&out_buf_mutex);
            
			sprintf(sys_info,"Operator Thread %d Producing Product %c",consumer_id,product[item]);
			print_info_to_screen(sys_info);
		}else{
			pthread_cond_broadcast(&producer_cond);
			sprintf(sys_info,"Operator Thread %d Goes to Sleep",consumer_id);
			print_info_to_screen(sys_info);
			pthread_cond_wait(&consumer_cond,&in_buf_mutex);
		}
		pthread_mutex_unlock(&in_buf_mutex);
		
		pthread_mutex_lock(&tool_mutex);
		tool_cnt += 2;
		pthread_mutex_unlock(&tool_mutex);
		usleep(interval);
	}
    
};

// Function for the main process
void * keyboard_handler(void * sig){
	while(true){
		char ch;
		cin.get(ch);
		if(ch == RETURN){
			if(!pause_the_program){
				pthread_mutex_lock(&in_buf_mutex);
				pthread_mutex_lock(&out_buf_mutex);
				pause_the_program = true;
				print_info_to_screen("Now Pausing...");
			}else{
				pthread_mutex_unlock(&in_buf_mutex);
				pthread_mutex_unlock(&out_buf_mutex);
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

void print_info_to_screen(const char * extra_info){
	system("clear");
	cout<<"=========================================================="<<endl;
	cout<<"|\t"<<extra_info<<endl;
	cout<<"|\tCurrent Tools Remaining: "<<tool_cnt<<endl;
	cout<<"|\tMaterial In Buffer: ";
	for(int i = 0; i < in_buf.size(); i++){
		cout<<material[in_buf[i]]<<" ";
	}
	cout<<endl;
	cout<<"|\tLast 10 Products In Buffer: ";
	int out_buf_size = 10;
	if(out_buf_size > out_buf.size()) out_buf_size = out_buf.size();
	for(int i = (int)out_buf.size() - out_buf_size; i < (int)out_buf.size(); i++){
		cout<<product[out_buf[i]]<<" ";
	}
	cout<<endl;
	cout<<"|\tProduct Buffer Size:"<<out_buf.size()<<endl;
	cout<<"|\tProducts Number in Buffer:"<<endl;
	cout<<"|\tX: "<<product_cnt[0]<<"\tY: "<<product_cnt[1]<<"\tZ: "<<product_cnt[2]<<endl;
	cout<<"|\tMaterials Number in Buffer:"<<endl;
	cout<<"========================================================="<<endl;
	cout<<"=> Press Return to Pause/Resume"<<endl;
	cout<<"=> Press Ctrl + C to exit the process"<<endl;
}

void init_producer_consumer(){
	tool_cnt = tool_num;
    
	for (int i = 0; i < producer_num; ++i) {
		prod[i] = new producer(i);
	}
    
	for (int i = 0; i < consumer_num; ++i) {
		consumer* tmp;
		tmp = new consumer(i);
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