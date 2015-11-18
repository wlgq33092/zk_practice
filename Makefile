src=zk_main.cpp
objs=zk_main.o

cc=g++
AR=ar
link=g++

include_dir=-I/home/parallels/open_source/zookeeper-3.4.6/src/c/include \
            -I/home/parallels/open_source/zookeeper-3.4.6/src/c/generated

link_lib=-L /home/parallels/open_source/zookeeper-3.4.6/src/c/.libs

%.o:%.cpp
	$(cc) $(include_dir) -c -o $@ $^ -g

zk_practice:$(objs)
	$(cc) $(objs) -o zk_practice $(link_lib) -lzookeeper_mt

clean:
	rm zk_practice *.o
