STD=-std=c++11
INCLUDE=-I ./
LIB=-lpthread
TARGET=my-wget
OBJ=main.o ftp.o

.cpp.o:
	g++ ${STD} ${INCLUDE} -c $<

${TARGET}:${OBJ}
	@echo "============开始编译============"
	g++ -o $@ $?
	rm -f ${OBJ}
	@echo "============编译结束============"

clean:
	rm -f *.o