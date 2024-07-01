// /*
// hakjfjkas
// ajklfdj
// ejewkf
// */

// // 这是一行注释

// int a \
//     = 1;

    // #include "1_preprocess.h" // 这是一行注释

// int sub(int a, int b) { return a - b; }
// int main()
// {
//     int a = 1;
//     int b = 2;
//     int c = sub(a, b);
    // printf("%s %d %s %s %s", __FILE__, __LINE__, __DATE__, __TIME__, __FUNCTION__);
//     return 0;
// }

#define ADD(x, y) (x+y)
int add = ADD(1,2);

#define CONCAT(x,y) x##y
int xy = CONCAT(1,2);

// #define STRY( )

#define STRINGIFY( x ) #x
char* str = STRINGIFY(1+2);

#define PRINT(...) printf(__VA_ARGS__)
PRINT(1, 2, 3);

#define DEBUG 1

// // 仅支持单参数判断
// #if DEBUG
// #pragma message("DEBUG MODE")
// #else
// #pragma message("RELEASE MODE")
// #endif

// #ifndef CCC
#define CCC 
// #endif

// #ifdef CCC
// #pragma message("CCC is defined")
// #endif
// #undef CCC


