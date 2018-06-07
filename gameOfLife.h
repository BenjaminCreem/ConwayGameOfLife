typedef struct
{
    int x;
    int y;
    int value;
} Cell;
Cell** allocGrid(Cell **mat, int n);
void freeGrid(Cell **mat, int n);
void assignGrid(Cell **mat, int n);
void printGrid(int **mat, int n, int g);
