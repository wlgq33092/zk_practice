#include <stdio.h>
#include <string.h>
#include "zookeeper.h"
#include "zookeeper_log.h"

#include "ds_lock.h"

using namespace std;

/*
void zktest_watcher_g(zhandle_t* zh, int type, int state, const char* path, void* watcherCtx)
{
    printf("Something happened.\n");
    printf("type: %d\n", type);
    printf("state: %d\n", state);
    printf("path: %s\n", path);
    printf("watcherCtx: %s\n", (char *)watcherCtx);
}
*/

//自己的监听函数
void watcher_myself(zhandle_t *zh,int type,int state,const char *path,void *watcherCtx)
{
    printf("just for testing\n");
    printf("path:%s\n",path);
}

//同步方式创建节点
void create(zhandle_t *zkhandle,char *str)
{
    char path_buffer[64];
    int bufferlen=sizeof(path_buffer);
    printf("同步方式创建节点-----------------------\n");
    int flag = zoo_create(zkhandle,str,"hahah",5,
                          &ZOO_OPEN_ACL_UNSAFE,0,
                          path_buffer,bufferlen);

    if (flag!=ZOK)
    {
        printf("节点创建失败 \n");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("创建的节点名称为：%s\n",path_buffer);
    }
}

//同步方式获取节点数据
void get(zhandle_t* zkhandle, char* path)
{
    printf("同步方式获取节点数据-----------------------\n");
    char buffer[64] = {0};
    int bufferlen=sizeof(buffer);

    int flag = zoo_get(zkhandle, path, 0,
                      buffer, &bufferlen, NULL);
    if (flag == ZOK) {
        printf("节点%s的数据为: %s\n", path, buffer);
    }
}

//exists
void exists(zhandle_t *zkhandle,char *str)
{
    int flag = zoo_exists(zkhandle,str,1,NULL);
}

void wexists(zhandle_t *zkhandle,char *str)
{
    int flag=zoo_wexists(zkhandle, str, watcher_myself, NULL, NULL);
}

//同步方式获得子节点信息
void getChildren(zhandle_t *zkhandle,char *str)
{
    struct String_vector strings;
    struct Stat stat;
    int flag = zoo_wget_children2(zkhandle,str,
                                  watcher_myself, NULL,
                                  &strings, &stat);
    if (flag==ZOK)
    {
        printf("get children ok!\n");
        for (int i = 0; i < strings.count; ++i)
            printf("%s\n", strings.data[i]);
    }
}

//获取ACL信息
void getACL(zhandle_t *zkhandle,char *str)
{
    struct ACL_vector acl;
    struct Stat stat;
    int flag = zoo_get_acl(zkhandle,str,&acl,&stat);
    if (flag==ZOK)
    {
        printf("-----------------the ACL of %s:\n------------",str);
        printf("%d\n",acl.count);
        printf("%d\n",acl.data->perms);
        printf("%s\n",acl.data->id.scheme);
        printf("%s\n",acl.data->id.id);
    }
}

//删除节点
void delete_node(zhandle_t *zkhandle,char *str)
{
    int flag = zoo_delete(zkhandle,str,-1);
    if (flag==ZOK) {
        printf("delete node success\n");
    }
}

int main(int argc, const char *argv[])
{
    const char* host = "127.0.0.1:2181";
    int timeout = 30000;
    char buffer[512];
    int *bufferlen;

    zoo_set_debug_level(ZOO_LOG_LEVEL_WARN); //设置日志级别,避免出现一些其他信息
    zhandle_t* zkhandle = zookeeper_init(host,zktest_watcher_g, timeout, 0, (void *)"hello, zookeeper!", 0);
    if (zkhandle ==NULL)
    {
        fprintf(stderr, "Error when connecting to zookeeper servers...\n");
        exit(EXIT_FAILURE);
    }


    char str[]="/data";
    // wexists(zkhandle,str);
    // printf("---------------\n");
    create(zkhandle,str);
    get(zkhandle, str);
    //getChildren(zkhandle,str);
    //getACL(zkhandle,str);
    delete_node(zkhandle,str);
}

