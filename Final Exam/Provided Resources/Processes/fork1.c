#include <unistd.h>

int main() {    
	fork();
    sleep(3);
	write(1, "Hi \n", 4);
}
