#后缀为.h的文件都在 ../headers文件夹下寻找
vpath %.h ../headers
#a.out的依赖文件
OBJECTS1 = main.o list.o tri_list.o network.o fun.o cal_value.o function.o free_space.o file.o 
#*.o的依赖文件
OBJECTS2 = config.h 
#编译器
CC = g++
#编译器参数
CFLAGs = -O2 -c -Wall -std=c++11

a.out : $(OBJECTS1)
		$(CC) -O2 -o a.out $(OBJECTS1)
#-fexceptions -g

main.o : main.cpp $(OBJECTS2)
		$(CC) $(CFLAGs) main.cpp
				
list.o : list.cpp $(OBJECTS2)
		$(CC) $(CFLAGs) list.cpp 
				
tri_list.o : tri_list.cpp $(OBJECTS2)
		$(CC) $(CFLAGs) tri_list.cpp 
				
network.o : network.cpp $(OBJECTS2)
		$(CC) $(CFLAGs) network.cpp 
				
cal_value.o : cal_value.cpp $(OBJECTS2)
		$(CC) $(CFLAGs) cal_value.cpp 
		
function.o : function.cpp $(OBJECTS2)
		$(CC) $(CFLAGs) function.cpp 

fun.o : fun.cpp $(OBJECTS2)
		$(CC) $(CFLAGs) fun.cpp 
		
free_space.o : free_space.cpp $(OBJECTS2)
		$(CC) $(CFLAGs) free_space.cpp

file.o : file.cpp $(OBJECTS2)
		$(CC) $(CFLAGs) file.cpp

.PHONY : clean
clean :
	-rm a.out $(OBJECTS1)