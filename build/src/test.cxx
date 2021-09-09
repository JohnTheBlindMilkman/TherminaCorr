#include "sayHello.h"

int main(int argc, char **argv)
{
    sayHello say;

    if(argc > 1)
        say.printMsg(argv[1]);

    return 1;
}