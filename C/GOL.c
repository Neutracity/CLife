#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define haut 8
#define largeur 30

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
    int count = 0;
    //int limy = 1;
    //int limx = 1;
    for(int i = y-1;i<=y+1;i++){
        for(int j = x-1;j<=x+1;j++){
            int nx = j;
            int ny = i;
            if(x >=largeur){nx = 0;ny = ny-3;} // teleporte Droite -> Gauche Work
            else if(x<0){nx = largeur-1;ny = ny-3;}; // teleporte Gauche -> Droite  Work Mais descent 
            if(y >=haut-1){ny = 0;} // teleporte Bas -> Haut
            else if(y<0){ny = haut-1;}; // teleporte Haut -> Bas
            if(screen[ny][nx] == 1){count++;}; // Incrémente valeur de la grile est vivante
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

void make_grid_voisin(int (*grid)[largeur],int (*grid_voisin)[largeur]){
    for(int i = 0;i<haut;i++){
        for(int j = 0;j<largeur;j++){
            grid_voisin[i][j] = voisin(grid,j,i);
            /*printf("i: %d , j : %d \n",i,j);
            print_scr(grid_voisin);
            getchar();*/
            /*if (grid[i][j] == 1){
                for(int a = i-1;a<=i+1;a++){
                    for(int b = j-1;b<=j+1;b++){ 
                        grid_voisin[a][b] = voisin(grid,b,a);
                        printf("a: %d , b : %d \n",a,b);
                        print_scr(grid_voisin);
                        getchar();
                    }
                }
            }*/
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
            }else if((grid_voisin[i][j] == 3 || grid_voisin[i][j] == 2) && grid[i][j] == 1){
                grid[i][j] = 1;
            }else{
                grid[i][j] = 0;
            }
        }
    }
}

void glidder(int x, int y,int (*grid)[largeur]){
    change_value(grid,x,y);
    change_value(grid,x,y+1);
    change_value(grid,x,y+2);
    change_value(grid,x+1,y+2);
    change_value(grid,x+2,y+1);
}
/*
 ####
#   #
    #
#  #
*/
void lwss(int x, int y,int (*grid)[largeur]){
    change_value(grid,x,y);
    change_value(grid,x,y+1);
    change_value(grid,x,y+2);
    change_value(grid,x-1,y+3);
    change_value(grid,x-4,y+3);
    change_value(grid,x-4,y+1);
    change_value(grid,x-3,y);
    change_value(grid,x-2,y);
    change_value(grid,x-1,y);
}
void glidder2(int x, int y,int (*grid)[largeur]){
    change_value(grid,x,y);
    change_value(grid,x+1,y);
    change_value(grid,x+2,y);
    change_value(grid,x,y+1);
    change_value(grid,x+1,y+2);
}

int main(){
    time_t seconds;
    time(&seconds);
    srand((unsigned int) seconds);
    int screen[haut][largeur];
    init_scr(screen);

    // change_value(screen,10,4);
    // change_value(screen,10,5);
    // change_value(screen,10,6);
    // change_value(screen,11,6);
    // change_value(screen,12,5);
    // change_value(screen,6,6);
    // change_value(screen,7,7);
    //glidder(2,2,screen);
    // glidder2(7,3,screen);
    lwss(7,2,screen);
    //scramble(screen);
    high_print_scr(screen);
    printf("\n");
    int grid_voisin[haut][largeur];
    init_scr(grid_voisin);
    while(1==1){
        init_scr(grid_voisin);
        make_grid_voisin(screen,grid_voisin);
        print_scr(grid_voisin);
        apply_rule(screen,grid_voisin);
        high_print_scr(screen);
        getchar();
    }
    return 0;
}