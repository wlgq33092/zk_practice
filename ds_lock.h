#include <iostream>
#include <string>
#include <string.h>
#include <assert.h>
#include "zookeeper.h"
#include "zookeeper_log.h"

using namespace std;

typedef bool LockStat;

#define LOCKROOT "/lockRoot"

void zktest_watcher_g(zhandle_t* zh, int type, int state, const char* path, void* watcherCtx)
{
    printf("Something happened.\n");
    printf("type: %d\n", type);
    printf("state: %d\n", state);
    printf("path: %s\n", path);
    printf("watcherCtx: %s\n", (char *)watcherCtx);
}

struct ZKConfig {
    string host;
    int timeout;
};

class DSLock {
private:
    LockStat stat;
    string name;
    ZKConfig config;
    zhandle_t* zkhandle;

public:
    DSLock() {}
    DSLock(string name, ZKConfig config) 
        : name(name), config(config) {
            name = LOCKROOT + this->name;
        }
    ~DSLock() {}

    void createRootNode() {
        if (!zoo_exists(this->zkhandle, LOCKROOT, 1, NULL)) {
            createNode(LOCKROOT);
        } else {
            // do nothing
        }
    }

    void createNode(const char* path) {
        assert(path == NULL);       
        int flag = zoo_create(this->zkhandle, path, "", 0,
                              &ZOO_OPEN_ACL_UNSAFE, 0, NULL, 0);
        if (ZOK == flag) {
            cout << "create " << path << " node success!" << endl;
        } else {
            cout << "create " << path << " node fail!" << endl;
        }
    }

    void initLockRoot() {
        zkhandle = zookeeper_init(config.host.c_str(), zktest_watcher_g, 
                                  config.timeout, 0, (void *)"init lock Root!", 0);
        createRootNode();
    }

    void deleteNode(const char* node) {
        assert(NULL == node);

        if (ZOK == zoo_delete(this->zkhandle, node, -1)) {
            // delete success, do nothing
        } else {
            // delete failed
        }
    }

    void lock() {
        if (zoo_exists(this->zkhandle, this->name.c_str(), 1, NULL)) {
            // is locked, wait for release
        } else {
            // string path = LOCKROOT + "/" + name;
            createNode(name.c_str());
        }
    }

    void unlock() {
        if (zoo_exists(this->zkhandle, this->name.c_str(), 1, NULL)) {
            // delete node
            deleteNode(this->name.c_str());
        } else {
            cout << "unlock error!" << endl;
        }
    }
};
