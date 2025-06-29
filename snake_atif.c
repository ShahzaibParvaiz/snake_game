#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<SDL2/SDL.h>

void boundary_check(int*,int*);
//Main Code Starts here
int main(int argc, char *argv[]) { //why main taking args - can take when main is taking things from terminal
    SDL_Window *window; //why pointer? - heap data structure
    SDL_Renderer *renderer;
    SDL_Event event; //study event - event polling - alternate methods instead of polling

    //snake_head is called object of the struct, SDL_Rect that we are using.
    SDL_Rect snake_head;
    snake_head.x = 320;
    snake_head.y = 240;
    snake_head.w = 40;
    snake_head.h = 40;

    //Giving seed to rand() function by time(NULL) which returns current time
    srand(time(NULL));
    //Our grid is w*h = 640*480 -> Height is 16*(40) and Width is 12*(40) - This is the concept behind enum grid
    enum {GRID_SIDE=40,X_FACTORS=16, Y_FACTORS=12}grid;
    //I've made food the same as the dimensions of snake head for the ease of grid
    SDL_Rect food;
    food.x = (rand()%X_FACTORS)*GRID_SIDE;
    food.y = (rand()%Y_FACTORS)*GRID_SIDE;
    food.w = 40;
    food.h = 40;

    int score = 0;
    int running = 1;

    //For direction manipulation
    typedef enum {UP, DOWN, LEFT, RIGHT} Direction;
    Direction direction = RIGHT;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("SNAKE GAME",
                              SDL_WINDOWPOS_CENTERED, 
                              SDL_WINDOWPOS_CENTERED, 
                              640, 480, 
                              SDL_WINDOW_SHOWN);
    
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);; //GPU accelerated with the help pf processor
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // if (snake_head==NULL) {
    //     printf("Snake is empty\n");
    //     SDL_Quit();
    // }

    while (running) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        //TODO - why RenderClear after SetRenderDrawColor
        //Bcz it clears the complete back buffer with the color we provide in the latest SetRenderDrawColor
        SDL_RenderClear(renderer);
        SDL_RenderDrawRect(renderer, &snake_head); //TODO - Food Bana
        SDL_RenderDrawRect(renderer, &food);
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);//here it fills the brush for snake head, nothing with the window
        SDL_RenderFillRect(renderer, &snake_head);//here it paints the snake head on canvas
        SDL_SetRenderDrawColor(renderer, 255, 192, 203, 255);//here it changes the brush to pink color
        SDL_RenderFillRect(renderer, &food);//here it draws the food on the canvas using pink brush
        SDL_RenderPresent(renderer); //This function presents everything (the canvas) onto the screen - if it's not called, nothing draws
        SDL_Delay(60); //Why delay in loop? Ans: FPS, mismatch in CPU and Screen speed - barabar krna hai, Snake was too fast (+40)

        //do baar SDL_RenderPresent() use kiya tha - galati - 2x overlap

        //TODO: Motion of Snake_Head - Now incorporating Events (Button i/p from keyboard)
        switch (direction) {
            case UP:
                snake_head.y = snake_head.y - GRID_SIDE;
                break;

            case DOWN:
                snake_head.y = snake_head.y + GRID_SIDE;
                break;

            case LEFT:
                snake_head.x = snake_head.x - GRID_SIDE;
                break;

            case RIGHT:
                snake_head.x = snake_head.x + GRID_SIDE;
                break;
        }
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT) {
                 running = 0;
            }  //why & in event? - Bcz, it stores the event at that address - structure of events
         //SDL_PollEvent - checks event, if present then 1, stores the event in &event, clears the queue, else 0

            switch (event.type) {
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_KEYDOWN:
                        switch (event.key.keysym.sym){
                            case SDLK_UP:
                                if (direction != DOWN) {
                                    direction = UP;
                                    printf("Arrow Up key is pressed.\n");
                                    snake_head.y -= GRID_SIDE; //why minus? inverted coordinate system on screen
                                }break;

                            case SDLK_DOWN:
                                if (direction != UP) {
                                    direction = DOWN;
                                    snake_head.y += GRID_SIDE;
                                    printf("Arrow Down key is pressed.\n");
                                }break;

                            case SDLK_LEFT:
                                if (direction != RIGHT){
                                    direction = LEFT;
                                    snake_head.x -= GRID_SIDE;
                                    printf("Arrow Left key is pressed.\n");
                                }break;

                            case SDLK_RIGHT:
                                if (direction != LEFT) {
                                    direction = RIGHT;
                                    snake_head.x += GRID_SIDE;
                                    printf("Arrow Right key is pressed.\n");
                                }break;
                        }
            }
        }
        //Made a function - Boundary Check - Call by Reference - To make change in actual memory location
        boundary_check(&snake_head.x, &snake_head.y);
        //Changing the Position of food if Snake Head arrives at the same coordinates - Eats food, New Food
        if (snake_head.x == food.x && snake_head.y == food.y) {
            // srand(time(NULL));
            food.x = (rand()%X_FACTORS)*GRID_SIDE;
            food.y = (rand()%Y_FACTORS)*GRID_SIDE;
            score+=10;
        }//this if statement end
    }//first while end
    printf("Score: %d\n", score);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}//main end

void boundary_check(int* x_ptr, int* y_ptr) {
    if(*x_ptr < 0 ) *x_ptr = 600;
    else if(*x_ptr > 600) *x_ptr = 0;
    if (*y_ptr < 0) *y_ptr = 440;
    else if(*y_ptr > 440) *y_ptr = 0;
}