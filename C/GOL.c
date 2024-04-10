#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define haut 16
#define largeur 128

void print_scr(int (*array)[largeur]){
    for(int i = 0;i<haut;i++){
        for(int j = 0;j<largeur;j++){
            printf("%d",array[i][j]);
        }
        printf("\n");
    }
}

void high_print_scr(int (*array)[largeur]){
    for(int i = 0;i<haut;i++){
        for(int j = 0;j<largeur;j++){
            if(array[i][j]==1){printf("█");}
            else{printf(" ");}
        }
        printf("\n");
    }
}

void init_scr(int (*screen)[largeur]){
    for(int i = 0;i<haut;i++){
        for(int j = 0;j<largeur;j++){
            screen[i][j] = 0 ;
        }
    }
}

int voisin(int (*screen)[largeur],int x,int y){
    int count = 0 ;
    for(int i = y-1;i<=y+1;i++){
        for(int j = x-1;j<=x+1;j++){
            if(screen[i][j] == 1){
                count++;
            }
            //printf("i: %d , j : %d \n",i,j);
        }
    }
    return count - screen[y][x] ;
}

void change_value(int (*screen)[largeur],int x, int y){
    if(x >= largeur){
        printf("x trop grand.\n");
    }else if (x < 0){
        printf("x trop petit.\n");
    }else if (y >= haut){
        printf("y trop grand.\n");
    }else if (y < 0){
        printf("y trop petit.\n");
    }else{
        screen[y][x] = 1 ;
    }
}

int make_grid_voisin(int (*grid)[largeur],int (*grid_voisin)[largeur]){
    for(int i = 1;i<haut-1;i++){
        for(int j = 1;j<largeur-1;j++){
            if (grid[i][j] == 1){
                for(int a = i-1;a<=i+1;a++){
                    for(int b = j-1;b<=j+1;b++){
                        grid_voisin[a][b] = voisin(grid,b,a);
                        //printf("a: %d , b : %d \n",a,b);
                    }
                }
            }
        }
    }
}

void scramble(int (*grid)[largeur]){
    for(int i = 0;i<haut;i++){
        for(int j = 0;j<largeur;j++){
            grid[i][j] = abs(rand()%2) ;
        }
    }
}

void apply_rule(int (*grid)[largeur],int (*grid_voisin)[largeur]){
    for(int i = 0;i<haut;i++){
        for(int j = 0;j<largeur;j++){
            if(grid_voisin[i][j] == 3 && grid[i][j] == 0){
                grid[i][j] = 1;
            }else if((grid_voisin[i][j] == 3 || grid_voisin[i][j] == 4) && grid[i][j] == 1){
                grid[i][j] = 1;
            }else{
                grid[i][j] = 0;
            }
        }
    }
}

int main(){
    time_t seconds;
    time(&seconds);
    srand((unsigned int) seconds);
    int screen[haut][largeur];
    init_scr(screen);
    scramble(screen);
    high_print_scr(screen);
    printf("\n");
    int grid_voisin[haut][largeur];
    init_scr(grid_voisin);
    while(1==1){
        make_grid_voisin(screen,grid_voisin);
        apply_rule(screen,grid_voisin);
        print_scr(grid_voisin);
        high_print_scr(screen);
        getchar();
    }
    return 0;
}