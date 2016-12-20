STUDENT_ID=2630058

CMD=./process_sort

process_sort: process_sort.c
	gcc -g -Wall -o process_sort process_sort.c
	
tar: clean
	make clean
	mkdir $(STUDENT_ID)-lab-7                                                           
	cp process_sort.c makefile $(STUDENT_ID)-lab-7/
	tar czvf $(STUDENT_ID)-lab-7.tar.gz $(STUDENT_ID)-lab-7
	rm -rf $(STUDENT_ID)-lab-7

clean:
	rm -f *.o process_sort

