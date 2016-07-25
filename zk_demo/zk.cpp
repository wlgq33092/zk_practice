#include "zk.h"


/*
void zktest_watcher_g(zhandle_t* zh, int type, int state, const char* path, void* watcherCtx) {
}
*/

int ZK_CreateNode(zhandle_t* zkhandle, const char* path, const char* value) {
    assert(path != NULL);       
    int flag = zoo_create(zkhandle, path, value, 0,
                          &ZOO_OPEN_ACL_UNSAFE, ZOO_EPHEMERAL, NULL, 0);
    if (ZOK == flag) {
        cout << "create " << path << " node success!" << endl;
        return RETURN_OK;
    } else {
        cout << "create " << path << " node fail!" << endl;
        return RETURN_ERROR;
    }
}

zhandle_t* ZK_InitZKRoot(ZK_WATCHER watcher, ZKConfig config) {
    zhandle_t* zkhandle = zookeeper_init(config.host.c_str(), watcher, 
                              config.timeout, 0, (void *)"init lock Root!", 0);
    if (NULL == zkhandle) {
        cout << "init zk error!" << endl;
        return NULL;
    }

    return zkhandle;
}

void ZK_DeleteNode(zhandle_t* zkhandle, const char* node) {
    assert(NULL != node);

    cout << "delete node is " << node << endl;
    if (ZOK == zoo_delete(zkhandle, node, -1)) {
        // delete success, do nothing
    } else {
        // delete failed
        cout << "delete node failed!" << endl;
    }
}

