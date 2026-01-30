#include <init.h>
#include <io.h>
int main(){
    init();
    for (int i = 0; i < 100; i++){
        printf("iteration: %i\n",i);
    }
    return 0;
}