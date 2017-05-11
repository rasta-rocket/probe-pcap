probe-pcap : main.o probe.o arguments.o
	gcc -o probe-pcap main.o probe.o arguments.o -lpcap
main.o : main.c probe.h arguments.h
	gcc -c main.c
probe.o : probe.c probe.h
	gcc -c probe.c
arguments.o : arguments.c arguments.h
	gcc -c arguments.c
clean :
	rm *.o probe-pcap 2>/dev/null || true
