#include<iostream>
#include"messageQueue.h"
#include<pthread.h>
#include<string>

#define READTHREADS 4
#define WRITETHREADS 4
struct block{
	MessageQueue* Qptr;
	int id;
};

void* readThread(void* argument){
	MessageQueue* ptrQ = (MessageQueue*) argument;
	for(int i = 0; i < 100; i++){
	std::cout << ptrQ->ReadMessage().printMessage();
	}
	pthread_exit(NULL);
}

void* writeThread(void* argument){
	struct block* bptr = (struct block* ) argument;
	MessageQueue* ptrQ = bptr->Qptr;
	int ID = bptr->id;
	for(int i = 0; i < 100; i++){
		std::string str = "message: " + std::to_string(i) + " in thread " + std::to_string(ID) + "\n";
		Message a(i,str);
		ptrQ->WriteMessage(a);
	}
	pthread_exit(NULL);
}
	
int main(){
	MessageQueue q;
	pthread_t readThreads[READTHREADS];
	pthread_t writeThreads[WRITETHREADS];
	int result;
		
	for(int i = 0; i < WRITETHREADS; i++){
		struct block* bptr = new block;
		bptr->Qptr = &q;
		bptr->id = i;
		result = pthread_create(&writeThreads[i], NULL, writeThread, bptr);	
	}		
		
	for(int i = 0; i < READTHREADS; i++)	
		result = pthread_create(&readThreads[i], NULL, readThread, &q);
			
	for(int i = 0; i < WRITETHREADS; i++)
		pthread_join(writeThreads[i], NULL);	
			
	for(int i = 0; i < READTHREADS; i++)
		pthread_join(readThreads[i], NULL);

	pthread_exit(NULL);
}
