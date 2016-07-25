src=zk.cpp main.cpp
objs=main.o zk.o

cc=g++
AR=ar
link=g++

include_dir=-I/home/parallels/open_source/zookeeper-3.4.6/src/c/include \
            -I/home/parallels/open_source/zookeeper-3.4.6/src/c/generated

link_lib=-L /home/parallels/open_source/zookeeper-3.4.6/src/c/.libs

%.o:%.cpp
	$(cc) $(include_dir) -c -o $@ $^ -g

zk_practice:$(objs)
	$(cc) $(objs) -o zk_demo $(link_lib) -lzookeeper_mt -lpthread

clean:
	rm zk_demo *.o
