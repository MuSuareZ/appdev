#define UNICODE
// constant definitions
enum COLORS{BLACK=30, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE};
#define bg(c) (c+10)	// macro definition
// function declarations
void clearScreen(void);
void setColors(short, short);
void resetColors(void);
void barChart(int []);
