#include <stdio.h>

//void mov(int disk, char origin, char destiny , char work ) {
//
//    if (disk == 1) {
//        printf("Disco %d no pilar %c movendo para %c\n", disk, origin, destiny);
//
//    } else {
//        mov(disk-1, origin, work, destiny);
//        printf("Disco %d no pilar %c movendo para %c\n", disk, origin, destiny);
//        mov(disk-1, work, destiny, origin);
//
//    }
//
//
//}


void movHanoi(int disk, char origin, char destiny , char work ) {
    if (disk > 0) {
        movHanoi(disk-1, origin, destiny, work);
        printf("Disco %d no pilar %c movendo para %c\n", disk, origin, destiny);

        movHanoi(disk-1, destiny, work, origin);
    }
}

int main() {

    const int disk = 3;
    movHanoi(disk, 'A', 'B', 'C');

}