#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "gameOfLife.h"

int main(int argc, char **argv)
{
    int n = 1000;
    int numGenerations = 100;
    //Main Grid
    int **grid = (int**)malloc(n*sizeof(int*));
    //Temporary grid to store values before putting back into main grid
    int **buffer = (int**)malloc(n*sizeof(int*));
    for(int i = 0; i < n; i++){
        grid[i] = (int*)malloc(n*sizeof(int));
        buffer[i] = (int*)malloc(n*sizeof(int));
    }

    //Init
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            //The border must be of all 0's
            if(i == 0 || j == 0 || i == n-1 || j == n-1)
            {
                grid[i][j] = 0;
            }

            grid[i][j]=0;
            buffer[i][j]=0;
        }
    }

    grid[1][2] = 1;
    grid[2][2] = 1;
    grid[3][2] = 1;

    //Initial Grid
    //printGrid(grid, n, 0);
    double totalTime = 0;

    #pragma omp parallel
    {

    for(int gen = 0; gen < numGenerations; gen++)
    {
    
        //Clear buffer
        #pragma omp single
        for(int i = 0; i < n; i++)
        {
            for(int j = 0; j < n; j++)
            {
                buffer[i][j] = 0;
            }
        }
        #pragma omp barrier

    //Loop over entire grid
    double startTime = omp_get_wtime();
    #pragma omp parallel for
    for(int i = 1; i < n-1; i++){
        for(int j = 1; j < n-1; j++){
            //Count of all values in 8 pixel range
            int count = 0;
            
            //loop over neighborhood (includes center tile)
            for(int x = -1; x <= 1; x++)
            {
                for(int y = -1; y <= 1; y++)
                {
                    count = count + grid[i+x][j+y];
                }
            }

            count = count - grid[i][j]; //Subtract center tile

            //Update buffer matrix based on count for i,j
            
            int temp = grid[i][j];
            if(temp == 1) //Check if we should kill it
            {
                if(count>3) temp = 0;
                if(count<2) temp = 0;
            }
            else //Check if we should give it life
            {
                if(count==3) temp = 1;
            }
            buffer[i][j]=temp;
        }
    }
    //Swap pointers
    #pragma omp single
    {
    int **temp = grid;
    grid = buffer;
    buffer = temp;
    }
    double endTime = omp_get_wtime();

    totalTime += endTime - startTime;
    
    #pragma omp barrier
    //printGrid(grid, n, gen+1);

    }//end of generation loop
    }
    printf("Total Time: %.4f\n", totalTime);
}

Cell** allocGrid(Cell **mat, int n)
{
    mat = (Cell**)malloc(n*sizeof(*mat));
    for(int i = 0; i < n; i++)
    {
        mat[i]=(Cell*)malloc(n*sizeof(*mat[i]));
    }
    return mat;
}

void freeGrid(Cell **mat, int n)
{
    for(int i = 0; i < n; i++)
    {
        free(mat[i]);
    }
    free(mat);
}

void assignGrid(Cell **mat, int n)
{
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            if(i == 0 || j == 0 || i == n-1 || j == n-1)
            {
                //mat[i][j] = 0;
            }
            else
            {
                //mat[i][j] = 1;
            }
        }
    }
}

void printGrid(int **mat, int n, int g)
{
    printf("Generation: %d\n", g);
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}


