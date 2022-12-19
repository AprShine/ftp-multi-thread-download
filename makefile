INCLUDE=-I ./
LIB=-lpthread
TARGET=my-wget
OBJ=main.o ftp.o m_thread.o

.cpp.o:
	g++ ${INCLUDE} -c $<

${TARGET}:${OBJ}
	@echo "============开始编译============"
	g++ -o $@ $? $(LIB)
	rm -f ${OBJ}
	@echo "============编译结束============"

clean:
	rm -f *.o