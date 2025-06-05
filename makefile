

z: sources/*.c
	clang -framework CoreAudio -O3 -I../cer0/include -Iinclude ../cer0/library/*.o sources/*.c -o z

clean:
	-rm z
