#include <iostream>
#include <string>
#include <string.h>
#include <assert.h>
#include "zookeeper.h"
#include "zookeeper_log.h"

using namespace std;

typedef bool LockStat;

#define RETURN_OK 1
#define RETURN_ERROR 0
#define LOCKROOT "/lockRoot"

void zktest_watcher_g(zhandle_t* zh, int type, int state, const char* path, void* watcherCtx)
{
/*
    printf("Something happened.\n");
    printf("type: %d\n", type);
    printf("state: %d\n", state);
    printf("path: %s\n", path);
    printf("watcherCtx: %s\n", (char *)watcherCtx);
*/
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
        : config(config) {
            this->initLockRoot();
            this->name = string(LOCKROOT) + string("/") + name;
        }
    ~DSLock() {}

    void createRootNode() {
        if (ZNONODE == zoo_exists(this->zkhandle, LOCKROOT, 1, NULL)) {
            createNode(LOCKROOT);
        } else {
            // do nothing
            cout << "lock root node exists!" << endl;
        }
    }

    int createNode(const char* path) {
        assert(path != NULL);       
        int flag = zoo_create(this->zkhandle, path, "", 0,
                              &ZOO_OPEN_ACL_UNSAFE, 0, NULL, 0);
        if (ZOK == flag) {
            cout << "create " << path << " node success!" << endl;
            return RETURN_OK;
        } else {
            cout << "create " << path << " node fail!" << endl;
            return RETURN_ERROR;
        }
    }

    void initLockRoot() {
        this->zkhandle = zookeeper_init(config.host.c_str(), zktest_watcher_g, 
                                  config.timeout, 0, (void *)"init lock Root!", 0);
        if (NULL == this->zkhandle) {
            cout << "init zk error!" << endl;
        }
        createRootNode();
    }

    void deleteNode(const char* node) {
        assert(NULL != node);

        cout << "delete node is " << node << endl;
        if (ZOK == zoo_delete(this->zkhandle, node, -1)) {
            // delete success, do nothing
        } else {
            // delete failed
            cout << "delete node failed!" << endl;
        }
    }

    void lock() {
        cout << "lock, node is " << this->name << endl;
        while (1) {
            if (ZNONODE == zoo_exists(this->zkhandle, this->name.c_str(), 1, NULL)) {
                if (createNode(this->name.c_str())) {
                    // lock success!
                    break;
                } else {
                    // lock fail! do nothing, blocking....
                }
            }
        }
    }

    void unlock() {
        if (ZOK == zoo_exists(this->zkhandle, this->name.c_str(), 1, NULL)) {
            // delete node
            deleteNode(this->name.c_str());
        } else {
            cout << "unlock error!" << endl;
        }
    }
};
