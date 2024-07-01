#define ADD(x, y) (x+y)
int add = ADD(1,2);

#define CONCAT(x,y) x##y
int xy = CONCAT(1,2);

// #define STRY( )

#define STRINGIFY( x ) #x
char* str = STRINGIFY(1+2);

#define PRINT(...) print(\
__VA_ARGS__)
PRINT(1, 2, 3);

#define DEBUG 1