#后缀为.h的文件都在 ../headers文件夹下寻找
vpath %.h ../headers
#a.out的依赖文件
OBJECTS1 = main.o list.o tri_list.o network.o property.o spread.o fun.o set_parms.o cal_rho_abscissa_value1.o cal_rho_abscissa_value2.o free_space.o file.o rand.o sort.o tri_sort.o  
#*.o的依赖文件
OBJECTS2 = config.h 
#cal_rho_abscissa_value.h file.h free_space.h network.h property.h rand.h spread.h
#编译器
CC = g++
#编译器参数
CFLAGs = -O2 -c -Wall -std=c++11

#config.h cal_rho_abscissa_value.h file.h/
#free_space.h network.h property.h rand.h spread.h

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

property.o : property.cpp $(OBJECTS2)
		$(CC) $(CFLAGs) property.cpp 

spread.o : spread.cpp $(OBJECTS2)
		$(CC) $(CFLAGs) spread.cpp 

fun.o : fun.cpp $(OBJECTS2)
		$(CC) $(CFLAGs) fun.cpp 

set_parms.o : set_parms.cpp $(OBJECTS2)
		$(CC) $(CFLAGs) set_parms.cpp 

cal_rho_abscissa_value1.o : cal_rho_abscissa_value1.cpp $(OBJECTS2)
		$(CC) $(CFLAGs) cal_rho_abscissa_value1.cpp

cal_rho_abscissa_value2.o : cal_rho_abscissa_value2.cpp $(OBJECTS2)
		$(CC) $(CFLAGs) cal_rho_abscissa_value2.cpp

free_space.o : free_space.cpp $(OBJECTS2)
		$(CC) $(CFLAGs) free_space.cpp
		
file.o : file.cpp $(OBJECTS2)
		$(CC) $(CFLAGs) file.cpp

rand.o : rand.cpp $(OBJECTS2)
		$(CC) $(CFLAGs) rand.cpp

sort.o : sort.cpp $(OBJECTS2)
		$(CC) $(CFLAGs) sort.cpp

tri_sort.o : tri_sort.cpp $(OBJECTS2)
		$(CC) $(CFLAGs) tri_sort.cpp


.PHONY : clean
clean :
	-rm a.out $(OBJECTS1)