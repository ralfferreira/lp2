#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define QTD_THREADS 5

using std::cout;
using std::endl;
using std::thread;
using std::mutex;

typedef struct {
	long id;
	int count;
} param_t;

mutex cout_mutex;

void funcao_thread(param_t* my_params) {
    
    cout_mutex.lock();
	cout << "[" << my_params->id << "] iniciando contagem ";
    cout << "(" << my_params->count << ")" << endl;
    cout_mutex.unlock();

	for (int i = 0; i < my_params->count; i++) {
	    cout_mutex.lock();
		cout << "[" << my_params->id << "] " << i << endl;
		cout_mutex.unlock();
		std::this_thread::sleep_for (std::chrono::seconds(1));
	}

    cout_mutex.lock();
	cout << "[" << my_params->id << "] finalizando contagem..." << endl;
	cout_mutex.unlock();

}

int main(void) {
	thread* threads[QTD_THREADS];
	param_t params[QTD_THREADS];

	cout << "[main] criando " << QTD_THREADS << " threads..." << endl;

	for (long i = 0; i < QTD_THREADS; i++) {
		params[i].id = i;
		params[i].count = rand() % 10;
		thread* t = new thread(funcao_thread, &params[i]);
		threads[i] = t;
	}

	for (int i = 0; i < QTD_THREADS; i++) {
	    cout_mutex.lock();
	    cout << "[" << params[i].id << "]"; 
		cout <<  " join() (" << threads[i]-> get_id() << ")" << endl;	    
		cout_mutex.unlock();
		threads[i]->join();
	}

	cout << "[main] finalizando funcao main..." << endl;

	return 0;
}
