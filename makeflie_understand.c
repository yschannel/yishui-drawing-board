object = helloworld.c //生成c文件名字
demo=helloworld //exe名字
%(demo) :$(object)  //$声明变量
	gcc -o $(demo) $(object) -I include -L lib -lraylib -lgdi32 -lwinmm

.PHONY : clean//伪代码

clean :
	rm $(object)	//删除程序输入make clean