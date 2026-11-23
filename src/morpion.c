#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>

#define RADIUS 100

void draw_background(SDL_Renderer *renderer){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    for (int i = 0; i<3; i++){
        SDL_RenderDrawLine(renderer, 200*i, 0, 200*i, 600);
        SDL_RenderDrawLine(renderer, 0, i*200, 600, i*200);
    }
}

void draw_cross(SDL_Renderer *renderer, int x, int y){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(renderer, x*200, y*200, x*200+200, y*200+200);
    SDL_RenderDrawLine(renderer, x*200+200, y*200, x*200, y*200+200);
}

void draw_circle(SDL_Renderer *renderer, int x_case, int y_case){
    int x = RADIUS - 1;
    int y = 0;
    int tx = 1;
    int ty = 1;
    int error = tx - (2 * RADIUS);

    int center_x = x_case * 200 + 100;
    int center_y = y_case * 200 + 100;
    SDL_Point *points = malloc(8 * RADIUS * 2 * sizeof(SDL_Point)); 
    int count = 0;

    while (x >= y) {
        // Add 8 symmetric points for each octant
        points[count++] = (SDL_Point){ center_x + x, center_y - y };
        points[count++] = (SDL_Point){ center_x + x, center_y + y };
        points[count++] = (SDL_Point){ center_x - x, center_y - y };
        points[count++] = (SDL_Point){ center_x - x, center_y + y };
        points[count++] = (SDL_Point){ center_x + y, center_y - x };
        points[count++] = (SDL_Point){ center_x + y, center_y + x };
        points[count++] = (SDL_Point){ center_x - y, center_y - x };
        points[count++] = (SDL_Point){ center_x - y, center_y + x };

        if (error <= 0) {
            y++;
            error += ty;
            ty += 2;
        }
        if (error > 0) {
            x--;
            tx += 2;
            error += (tx - (2 * RADIUS));
        }
    }

    SDL_RenderDrawPoints(renderer, points, count);
    free(points);
}

void draw_game(SDL_Renderer * renderer, int game[3][3]){
    for (int y = 0; y<3; y++){
        for (int x = 0; x<3; x++){
            if (game[y][x] == 1){
                draw_cross(renderer, x, y);
            }
            else if (game[y][x] == 2){
                draw_circle(renderer, x, y);
            }
        }
    }
}

bool rules(int game[3][3], int player){
    int player_win[3] = {player, player, player};
    int col_tab[3];
    int diag_un[3] = {game[0][0], game[1][1], game[2][2]};
    int diag_deux[3] = {game[0][2], game[1][1], game[2][0]};
    for (int i = 0; i<3; i++){
        if (memcmp(player_win, game[i], sizeof(player_win)) == 0){
            return false;
        }
        for (int j = 0; j<3; j++){
            col_tab[j] = game[j][i];
        }
        if (memcmp(player_win, col_tab, sizeof(col_tab)) == 0){
            return false;
        }
    }
    if ((memcmp(player_win, diag_un, sizeof(diag_un)) == 0) | (memcmp(player_win, diag_deux, sizeof(diag_deux)) == 0)){
        return false;
    }
    return true;
}

void display_matrice(int matrice[3][3]){
    for (int i = 0; i<3; i++){
        printf("\n");
        for (int j = 0; j<3; j++){
            printf("%d", matrice[i][j]);
        }
    }
    printf("\n");
}
int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Window* win = SDL_CreateWindow("GAME", 
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    600, 600, 0);

    if (!win) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(
        win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!renderer) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    SDL_Event event;
    bool running = true;
    int x, y;
    int game[3][3] = {{0, 0, 0},
                      {0, 0, 0},
                      {0, 0, 0}};
    int player = 1;
    while(running){
        while(SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT){
                running = false;
            }
            if (event.type == SDL_KEYDOWN){
                if (event.key.keysym.sym == SDLK_ESCAPE){
                    running = false;
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN){
                if (event.button.button == SDL_BUTTON_LEFT){
                    x = event.button.x;
                    y = event.button.y;
                    if (game[y/200][x/200] == 0){
                        game[y/200][x/200] = player;
                    }
                    running = rules(game, player);
                    player += 1;
                    if (player == 3){
                        player = 1;
                    }
                }
            }
        }
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        draw_background(renderer);
        draw_game(renderer, game);
        SDL_RenderPresent(renderer);
    }
    printf("Bravo joueur %d, vous avez gagné !\n", (player == 2) ? 1 : 2);
    SDL_DestroyWindow(win);
    SDL_Quit();
}