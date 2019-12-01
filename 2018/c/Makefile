TARGET := main

$(TARGET): main.c
	c++ -o $@ -O3 main.c

clean:
	rm $(TARGET)

run: $(TARGET)
	./main

rtags:
	$(MAKE) clean
	bear $(MAKE)
