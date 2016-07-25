#include <iostream>
#include <string>
#include <string.h>
#include <assert.h>
#include "zookeeper.h"
#include "zookeeper_log.h"

using namespace std;

#define RETURN_OK 1
#define RETURN_ERROR 0
#define LOCKROOT "/ZK_DemoRoot"

struct ZKConfig {
    string host;
    int timeout;
};

typedef void (*ZK_WATCHER)(zhandle_t* zh, int type, int state, const char* path, void* watcherCtx);

int ZK_CreateNode(zhandle_t* zkhandle, const char* path, const char* value);
zhandle_t* ZK_InitZKRoot(ZK_WATCHER watcher, ZKConfig config);
void ZK_DeleteNode(const char* node);

