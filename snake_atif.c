#include <stdio.h>
#include <SDL2/SDL.h>

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

    int running = 1;

    //For direction manipulation
    typedef enum {UP, DOWN, LEFT, RIGHT} Direction;
    Direction direction = RIGHT;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("Hello World", 
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

/*
 */
    while (running) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        //TODO - why RenderClear after SetRenderDrawColor
        //Bcz it clears the complete back buffer with the color we provide in the latest SetRenderDrawColor
        SDL_RenderClear(renderer);
        SDL_RenderDrawRect(renderer, &snake_head);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); //here it fills the rectangle, nothing with the window
        SDL_RenderFillRect(renderer, &snake_head);
        SDL_RenderPresent(renderer); //This function presents everything onto the screen - if it's not called, nothing draws

        SDL_Delay(1); //Why delay in loop? Ans: FPS, mismatch in CPU and Screen speed - barabar krna hai

        //TODO: Motion of Snake_Head - Now incorporating Events (Button i/p from keyboard)


        switch (direction) {
            case UP:
                printf("Up key is pressed.\n");
                snake_head.y = snake_head.y - 10;
                break;

            case DOWN:
                printf("Down key is pressed.\n");
                snake_head.y = snake_head.y + 10;
                break;

            case LEFT:
                printf("Left key is pressed.\n");
                snake_head.x = snake_head.x - 10;
                break;

            case RIGHT:
                printf("Right key is pressed.\n");
                snake_head.x = snake_head.x + 10;
                break;
        }
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT) {
                 running = 0;
            }  //why & in event? - Bcz, it stores the event at that address - structure of events
         //SDL_PollEvent - checks event, if present then 1, stores the event in &event, clears the queue, else 0

            // NOTES on top


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
                                    snake_head.y -= 10; //why minus? inverted coordinate system on screen
                                }break;

                            case SDLK_DOWN:
                                if (direction != UP) {
                                    direction = DOWN;
                                    snake_head.y += 10;
                                    printf("Arrow Down key is pressed.\n");
                                }break;

                            case SDLK_LEFT:
                                if (direction != RIGHT){
                                    direction = LEFT;
                                    snake_head.x -= 10;
                                    printf("Arrow Left key is pressed.\n");
                                }break;

                            case SDLK_RIGHT:
                                if (direction != LEFT) {
                                    direction = RIGHT;
                                    snake_head.x += 10;
                                    printf("Arrow Right key is pressed.\n");
                                }break;
                        // }
                    }
            }

            //Checking Boundary Conditions

        }
        if(snake_head.x < 0 ) snake_head.x = 600;
        else if(snake_head.x > 600) snake_head.x = 0;
        if (snake_head.y < 0) snake_head.y = 440;
        else if(snake_head.y > 440) snake_head.y = 0;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    //todo
    return 0;
}