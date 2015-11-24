#include <iostream>
#include <pthread.h>

#include "ds_lock.h"

using namespace std;

DSLock* g_dslock;

void* test1(void* argv) {
    g_dslock->lock();
    cout << "this is test1!" << endl;
    g_dslock->unlock();
}

void* test2(void* argv) {
    g_dslock->lock();
    cout << "this is test2!" << endl;
    g_dslock->unlock();
}

int main() {
    ZKConfig zkconfig;
    zkconfig.host = "127.0.0.1:2181";
    zkconfig.timeout = 30000;
    string name("test");

    DSLock *dslock = new DSLock(name, zkconfig);
    
    g_dslock = dslock;
    
    pthread_t tid[2];
    pthread_create(&tid[0], NULL, test1, NULL);
    pthread_create(&tid[1], NULL, test2, NULL);
    
    for (int i = 0; i < 2; i++) {
        pthread_join(tid[i], NULL);
    }

    return 0;
}
