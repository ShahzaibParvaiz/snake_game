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

    //Giving seed to rand() function by time(NULL) which returns current time
    srand(time(NULL));
    //Our grid is w*h = 640*480 -> Height is 16*(40) and Width is 12*(40) - This is the concept behind enum grid
    enum {GRID_SIDE=40,X_FACTORS=16, Y_FACTORS=12}grid;
    //I've made food the same as the dimensions of snake head for the ease of grid
    SDL_Rect food;
    food.x = (rand()%X_FACTORS)*GRID_SIDE;
    food.y = (rand()%Y_FACTORS)*GRID_SIDE;
    food.w = GRID_SIDE;
    food.h = GRID_SIDE;

    //snake_head is called object of the struct, SDL_Rect that we are using.
    SDL_Rect snake_head;
    snake_head.x = 320;
    snake_head.y = 240;
    snake_head.w = GRID_SIDE;
    snake_head.h = GRID_SIDE;

    int score = 0;

    //Making body as an array
    //here is the issue. Score=0 and we try to access elements like 1,2,3 etc in loop
    //The fix seems to declare a large array like 100 elements or DMA - Baaki zaanaan Rabb
    //TODO - Dynamic Memory Allocation
    // int body_elements=0;
    SDL_Rect *snake_body;
    snake_body=(SDL_Rect*)malloc(sizeof(SDL_Rect));


    int running = 1;
    int body_elements = 1;
    snake_body[0].x = snake_head.x - GRID_SIDE;
    snake_body[0].y = snake_head.y;

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

        for (int i=0; i<score; i++) {
            snake_body[i].w = GRID_SIDE;
            snake_body[i].h = GRID_SIDE;
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        //TODO - why RenderClear after SetRenderDrawColor
        //Bcz it clears the complete back buffer with the color we provide in the latest SetRenderDrawColor
        SDL_RenderClear(renderer);
        SDL_RenderDrawRect(renderer, &snake_head); //TODO - Food Bana
        SDL_RenderDrawRect(renderer, &food);
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);//here it fills the brush for snake head, nothing with the window
        SDL_RenderFillRect(renderer, &snake_head);//here it paints the snake head on canvas

        for (int elements=0; elements<body_elements; elements++) {
            SDL_SetRenderDrawColor(renderer, 47, 62, 70, 255);
            SDL_RenderDrawRect(renderer, &snake_body[elements]);
            SDL_RenderFillRect(renderer, &snake_body[elements]);
        }
        SDL_SetRenderDrawColor(renderer, 255, 192, 203, 255);//here it changes the brush to pink color
        SDL_RenderFillRect(renderer, &food);//here it draws the food on the canvas using pink brush
        SDL_RenderPresent(renderer); //This function presents everything (the canvas) onto the screen - if it's not called, nothing draws
        SDL_Delay(60); //Why delay in loop? Ans: FPS, mismatch in CPU and Screen speed - barabar krna hai, Snake was too fast (+40)

        //do baar SDL_RenderPresent() use kiya tha - galati - 2x overlap

        //TODO: Motion of Snake_Head - Now incorporating Events (Button i/p from keyboard)
        switch (direction) {
            case UP:

                for (int a=body_elements-1; a>=0; a--){
                    if (a!=0) {
                        snake_body[a].y = snake_body[a-1].y;
                        snake_body[a].x = snake_body[a-1].x;
                    }
                    else {
                        snake_body[a].y = snake_head.y;
                        snake_body[a].x = snake_head.x;
                    }
                }
                snake_head.y = snake_head.y - GRID_SIDE;
                break;

            case DOWN:

                for (int a=body_elements-1; a>=0; a--){
                    if (a!=0) {
                        snake_body[a].y = snake_body[a-1].y;
                        snake_body[a].x = snake_body[a-1].x;
                    }
                    else {
                        snake_body[a].y = snake_head.y;
                        snake_body[a].x = snake_head.x;
                    }
                }
                snake_head.y = snake_head.y + GRID_SIDE;
                break;

            case LEFT:

                for (int a=body_elements-1; a>=0; a--){

                    if (a!=0) {
                        snake_body[a].y = snake_body[a-1].y;
                        snake_body[a].x = snake_body[a-1].x;
                    }
                    else {
                        snake_body[a].y = snake_head.y;
                        snake_body[a].x = snake_head.x;
                    }
                }
                snake_head.x = snake_head.x - GRID_SIDE;
                break;

            case RIGHT:

                for (int a=body_elements-1; a>=0; a--){
                    if (a!=0) {
                        snake_body[a].y = snake_body[a-1].y;
                        snake_body[a].x = snake_body[a-1].x;
                    }
                    else {
                        snake_body[a].y = snake_head.y;
                        snake_body[a].x = snake_head.x;
                    }
                }
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

                                    for (int a=body_elements-1; a>=0; a--){
                                        if (a!=0) {
                                            snake_body[a].y = snake_body[a-1].y;
                                            snake_body[a].x = snake_body[a-1].x;
                                        }
                                        else {
                                            snake_body[a].y = snake_head.y;
                                            snake_body[a].x = snake_head.x;
                                        }
                                    }
                                    snake_head.y -= GRID_SIDE; //why minus? inverted coordinate system on screen
                                }break;

                            case SDLK_DOWN:
                                if (direction != UP) {
                                    direction = DOWN;

                                    for (int a=body_elements-1; a>=0; a--){
                                        if (a!=0) {
                                            snake_body[a].y = snake_body[a-1].y;
                                            snake_body[a].x = snake_body[a-1].x;
                                        }
                                        else {
                                            snake_body[a].y = snake_head.y;
                                            snake_body[a].x = snake_head.x;
                                        }
                                    }
                                    printf("Arrow Down key is pressed.\n");
                                    snake_head.y += GRID_SIDE;
                                }break;

                            case SDLK_LEFT:
                                if (direction != RIGHT){
                                    direction = LEFT;

                                    for (int a=body_elements-1; a>=0; a--){
                                        if (a!=0) {
                                            snake_body[a].y = snake_body[a-1].y;
                                            snake_body[a].x = snake_body[a-1].x;
                                        }
                                        else {
                                            snake_body[a].y = snake_head.y;
                                            snake_body[a].x = snake_head.x;
                                        }
                                    }
                                    printf("Arrow Left key is pressed.\n");
                                    snake_head.x -= GRID_SIDE;
                                }break;

                            case SDLK_RIGHT:
                                if (direction != LEFT) {
                                    direction = RIGHT;

                                    for (int a=body_elements-1; a>=0; a--){
                                        if (a!=0) {
                                            snake_body[a].y = snake_body[a-1].y;
                                            snake_body[a].x = snake_body[a-1].x;
                                        }
                                        else {
                                            snake_body[a].y = snake_head.y;
                                            snake_body[a].x = snake_head.x;
                                        }
                                    }
                                    printf("Arrow Right key is pressed.\n");
                                    snake_head.x += GRID_SIDE;
                                }break;
                        }
            }
        }
        //Made a function - Boundary Check - Call by Reference - To make change in actual memory location
        boundary_check(&snake_head.x, &snake_head.y);
        //Changing the Position of food if Snake Head arrives at the same coordinates - Eats food, New Food
        if (snake_head.x == food.x && snake_head.y == food.y) {
            food.x = (rand()%X_FACTORS)*GRID_SIDE;
            food.y = (rand()%Y_FACTORS)*GRID_SIDE;
            score+=1;
            body_elements+=1;
            snake_body=(SDL_Rect*)realloc(snake_body,body_elements*sizeof(SDL_Rect));
        }//this if statement end
        //Checking for collision with body
        for (int a=0; a<body_elements;a++) {
            if ((snake_head.x==snake_body[a].x)&&(snake_head.y==snake_body[a].y)) {
                running = 0;
                printf("Game Over!\n");
                break;
            }
        }
    }//first while end
    printf("Score: %d\n", score);
    free(snake_body);
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