#include <iostream>
#include <unistd.h>

#include "pthread.h"
#include "zk.h"

using namespace std;

#if 0
void ZK_CreateRootNode(zhandle_t* zkhandle);
void ZK_CreateNode(zhandle_t* zkhandle, const char* path);
zhandle_t* ZK_InitZKRoot(ZK_WATCHER watcher);
void ZK_DeleteNode(const char* node);
#endif

void test_watcher(zhandle_t* zh, int type, int state, const char* path, void* watcherCtx) {
    cout << "global watcher!" << endl;
    if (2 == state) {
        cout << "global watcher state 2" << endl;
    }
}

void test_find_node(zhandle_t* zhandle, int type, int state, const char* path, void* watcherCtx) {
    cout << "find node " << path << " exist!" << endl;
    if (ZOO_CREATED_EVENT == type) {
        cout << "find node " << path << " is created!" << endl;
        zoo_wexists(zhandle, "/test3", test_find_node, NULL, NULL);
    } else if (ZOO_SESSION_EVENT == type) {
        cout << "session event" << endl;
        // zoo_wexists(zhandle, "/test3", test_find_node, NULL, NULL);
    } else if (ZOO_DELETED_EVENT == type) {
        cout << "delete event" << endl;
        zoo_wexists(zhandle, "/test3", test_find_node, NULL, NULL);
    }
    cout << "state: " << state << endl;
    cout << "type: " << type << endl;
}

int main(int argc, char** argv) {
    ZKConfig zkconfig;
    zkconfig.host = "127.0.0.1:2181";
    zkconfig.timeout = 30000;

    char* path = argv[1];
    char* value = argv[2];

    ZK_WATCHER watcher = test_watcher;
    zhandle_t* zhandle = ZK_InitZKRoot(watcher, zkconfig);
    ZK_CreateNode(zhandle, path, value);

    zoo_wexists(zhandle, "/test3", test_find_node, NULL, NULL);

    while (1) { sleep(10); }

    return 0;
}
