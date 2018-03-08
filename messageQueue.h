#include<queue>
#include<string>
#include<pthread.h>

#define HIGH_WATER_MARK 0.9
#define LOW_WATER_MARK 0.1
#define MAX_MESSAGE 1000

class Message{
	private:
		int priority;
		std::string message;
		
	public:
		Message (){}
		
		Message(const int pri){
			priority = pri;
		}
		
		Message(const std::string str){
			message = str;
		}
		
		Message(const char* cstr){
			std::string str(cstr);
			message = str;
		}
		Message(const int pri, const std::string str){
			priority = pri;
			message = str;
		}
		
		Message(const int pri, const char* cstr){
			std::string str(cstr);
			priority = pri;
			message = str;
		}
		
		void setPriority(const int pri){
			priority = pri;
		}
		
		void setMessage(const std::string str){
			message = str;
		}
		
		void setMessage(const char* cstr){
			std::string str(cstr);
			message = str;
		}	 
		 
		std::string printMessage() const{
			return message;
		}
		
		bool operator<(const Message& b) const{
			return priority < b.priority;
		}		
};

class CompareMessage{
	public:
		bool operator()(const Message& a, const Message& b){
			return a < b;
		}
};
			

class MessageQueue{
	private:
		std::priority_queue<Message, std::vector<Message>, CompareMessage> messageQ;
		pthread_mutex_t muteks = PTHREAD_MUTEX_INITIALIZER;
		pthread_cond_t condition_var = PTHREAD_COND_INITIALIZER;
	
	public:
		MessageQueue(){	
		}
		
		~MessageQueue(){
			while(!messageQ.empty())
				messageQ.pop();
		}
		
		bool WriteMessage(const Message& a){	
			pthread_mutex_lock(&muteks);
			if(HighWaterMark()){
				pthread_cond_wait(&condition_var,&muteks);	
			}
			else if(LowWaterMark()){
				pthread_cond_signal(&condition_var);
			}
			messageQ.push(a);
			pthread_mutex_unlock(&muteks);
			return false;
		}
		
		Message ReadMessage(){
			Message result;
			if(messageQ.empty()){
				result = Message(0,"...NO_MESSAGE...\n");
			}
			else{
				pthread_mutex_lock(&muteks);
				result = messageQ.top();
				messageQ.pop();
				pthread_mutex_unlock(&muteks);
			}
			return result;
		}
		
		
		bool HighWaterMark(){
			float percent = (float)messageQ.size() / (float)MAX_MESSAGE; 
			return percent >= HIGH_WATER_MARK;
		}
		
		bool LowWaterMark(){
			float percent = (float)messageQ.size() / (float)MAX_MESSAGE;
			return percent <= LOW_WATER_MARK;
		}

};
