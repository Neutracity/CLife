#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define pixel_size 4
#define blocked 1
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
/*#define haut (SCREEN_HEIGHT / pixel_size)*/
/*#define largeur (SCREEN_WIDTH / pixel_size)*/
const int haut = 400 ;
const int largeur = 400;


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
    for(int i = y-1;i<=y+1;i++){
        for(int j = x-1;j<=x+1;j++){
            if(blocked == 1){
                if(i>=0 && j>=0 && i<=haut-1 && j<=largeur-1){
                    if(screen[i][j] == 1){count++;};
                }     
            }else{
                int nx = j;
                int ny = i;
                if(x >=largeur){nx = 0;ny = ny;} // teleporte Droite -> Gauche Work
                else if(x<0){nx = largeur-1;ny = ny;}; // teleporte Gauche -> Droite  Work Mais descent 
                if(y >=haut-1){ny = 0;} // teleporte Bas -> Haut
                else if(y<0){ny = haut-1;}; // teleporte Haut -> Bas
                if(screen[ny][nx] == 1){count++;}; // Incrémente valeur de la cellule est vivante
                //printf("i: %d , j : %d \n",i,j);
            }
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
/*
####
#   #
#   
 # #
*/
void lwss_reverse(int x, int y,int (*grid)[largeur]){
    change_value(grid,x,y);
    change_value(grid,x,y+1);
    change_value(grid,x,y+2);
    change_value(grid,x+1,y+3);
    change_value(grid,x+4,y+3);
    change_value(grid,x+4,y+1);
    change_value(grid,x+3,y);
    change_value(grid,x+2,y);
    change_value(grid,x+1,y);
}
void glidder2(int x, int y,int (*grid)[largeur]){
    change_value(grid,x,y);
    change_value(grid,x+1,y);
    change_value(grid,x+2,y);
    change_value(grid,x,y+1);
    change_value(grid,x+1,y+2);
}

int setWindowColor(SDL_Renderer *renderer, SDL_Color color)
{
    if(SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) < 0)
        return -1; 
    if(SDL_RenderClear(renderer) < 0)
        return -1;
    return 0;  
}

void draw_pix(SDL_Renderer *renderer,int x,int y, SDL_Color color){
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawPoint(renderer, x, y);
}

void print_to_screen(SDL_Renderer *renderer,int (*grid)[largeur],SDL_Color vivant,SDL_Color mort){
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, mort.r, mort.g, mort.b, mort.a);
    SDL_SetRenderDrawColor(renderer, vivant.r, vivant.g, vivant.b, vivant.a);
    SDL_Rect r;
    r.x = 0;
    r.y = 0;
    r.w = pixel_size;
    r.h = pixel_size;
    for(int y = 0; y<haut; y++){
        for(int x = 0; x<largeur; x++){
            if(grid[y][x]==1){
                SDL_RenderFillRect(renderer,&r); 
            };
            r.x+=pixel_size;
        };
        r.x = 0;
        r.y+=pixel_size;
        //printf("x= %d , y= %d\n",r.x,r.y);
    }
    SDL_RenderPresent(renderer);
}


int main(int argc, char * argv[]){
    if(argc < 2 || strcmp(argv[1], "-t") == 0){
      printf("Terminal ui \n"); 
      struct winsize w;
      ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
      time_t seconds;
      time(&seconds);
      srand((unsigned int) seconds);
      /*haut = w.ws_row ;*/
      /*largeur = w.ws_col ; */
      int screen[haut][largeur];
      init_scr(screen);
      //glidder(2,2,screen);
      //glidder2(300,300,screen);
      //lwss_reverse(3,2,screen);
      scramble(screen);
      //high_print_scr(screen);
      //printf("\n");
      int grid_voisin[haut][largeur];
      init_scr(grid_voisin);
      while(1==1){
        init_scr(grid_voisin);
        make_grid_voisin(screen,grid_voisin);
        apply_rule(screen,grid_voisin);
        high_print_scr(screen);
      };

  }else if(strcmp(argv[1], "-g") == 0){ 
    //Setup SDL2
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    int statut = EXIT_FAILURE;
    //SDL_Color orange = {255, 127, 40, 255};
    SDL_Color noir = {0, 0, 0, 255};
    SDL_Color blanc = {255, 255, 255, 255};
    window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderPresent(renderer);
    
    // GOL
    time_t seconds;
    time(&seconds);
    srand((unsigned int) seconds);
    int screen[haut][largeur];
    init_scr(screen);
    //glidder(2,2,screen);
    //glidder2(300,300,screen);
    //lwss_reverse(3,2,screen);
    scramble(screen);
    //high_print_scr(screen);
    //printf("\n");
    int grid_voisin[haut][largeur];
    init_scr(grid_voisin);
    SDL_Event event;
    SDL_bool quit = SDL_FALSE;
    while(!quit){
        SDL_Event event;
        if (SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                quit = SDL_TRUE; 
            }else{
                if(SDL_GetWindowFlags(window) !=1){ 
                    init_scr(grid_voisin);
                    make_grid_voisin(screen,grid_voisin);
                    apply_rule(screen,grid_voisin);
                    SDL_SetRenderDrawColor(renderer, noir.r, noir.g, noir.b, noir.a);
                    SDL_RenderClear(renderer);
                    print_to_screen(renderer,screen,blanc,noir);
                }
            }
        }

    };
    statut = EXIT_SUCCESS;
  
    
    

    }
}
