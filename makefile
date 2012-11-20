compile: scheduler.cc
	g++ scheduler.cc -o scheduler

test: test_scheduler.cc
	g++ test_scheduler.cc -o test_scheduler

clean:
	rm scheduler test_scheduler
