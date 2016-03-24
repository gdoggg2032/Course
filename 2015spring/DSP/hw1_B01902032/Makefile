.PHONY: all clean

CFLAGS+=
LDFLAGS+=-lm     # link to math library

TARGET=train test

all: $(TARGET)
# type make/make all to compile test_hmm
 
run: 
	./train  $(ITER)  model_init.txt  seq_model_01.txt model_01.txt
	./train  $(ITER)  model_init.txt  seq_model_02.txt model_02.txt
	./train  $(ITER)  model_init.txt  seq_model_03.txt model_03.txt
	./train  $(ITER)  model_init.txt  seq_model_04.txt model_04.txt
	./train  $(ITER)  model_init.txt  seq_model_05.txt model_05.txt
	./test modellist.txt testing_data1.txt result1.txt
	./test modellist.txt testing_data2.txt result2.txt
	./count_accuracy ./result1.txt ./testing_answer.txt ./acc.txt

	

clean:
	$(RM) $(TARGET)   # type make clean to remove the compiled file
