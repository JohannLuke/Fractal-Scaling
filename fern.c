#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define NUM_THREADS 4

// Initializing two structures
typedef struct {
    double scale;
    double offsetX;
    double offsetY;
    SDL_Renderer *renderer;
    unsigned long iter_per_thread;
    unsigned int seed;
    SDL_mutex *render_mutex;
    SDL_Surface *surface;
} ThreadData;

typedef struct {
    double scale;
    double offsetX;
    double offsetY;
} FractalView;

//Function to generate the Barnsley Fern thread
void* barnsleyFernThread(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    double x0 = 0, y0 = 0, x1, y1;
    int diceThrow;
    int centerX = WINDOW_WIDTH / 2;
    int centerY = WINDOW_HEIGHT / 2;
    unsigned int seed = data->seed;

    Uint32 green = SDL_MapRGB(data->surface->format, 0, 255, 0);

    for (unsigned long i = 0; i < data->iter_per_thread; i++) {
        diceThrow = rand_r(&seed) % 100;

        if (diceThrow == 0) {
            x1 = 0;
            y1 = 0.16 * y0;
        }
        else if (diceThrow >= 1 && diceThrow <= 7) {
            x1 = -0.15 * x0 + 0.28 * y0;
            y1 = 0.26 * x0 + 0.24 * y0 + 0.44;
        }
        else if (diceThrow >= 8 && diceThrow <= 15) {
            x1 = 0.2 * x0 - 0.26 * y0;
            y1 = 0.23 * x0 + 0.22 * y0 + 1.6;
        }
        else {
            x1 = 0.85 * x0 + 0.04 * y0;
            y1 = -0.04 * x0 + 0.85 * y0 + 1.6;
        }

        int screenX = (int)(data->scale * x1 + data->offsetX + centerX);
        int screenY = (int)(data->scale * y1 + data->offsetY + centerY);

        if (screenX >= 0 && screenX < WINDOW_WIDTH && screenY >= 0 && screenY < WINDOW_HEIGHT) {
            SDL_LockMutex(data->render_mutex);
            Uint32 *pixels = (Uint32 *)data->surface->pixels;
            pixels[screenY * data->surface->w + screenX] = green;
            SDL_UnlockMutex(data->render_mutex);
        }

        x0 = x1;
        y0 = y1;
    }

    return NULL;
}

//Function for the Barnsley Fern 
void barnsleyFern(SDL_Renderer *renderer, unsigned long iter, FractalView *view) {
    pthread_t threads[NUM_THREADS];
    ThreadData threadData[NUM_THREADS];
    unsigned long iter_per_thread = iter / NUM_THREADS;
    unsigned int seed = (unsigned int)time(NULL);

    SDL_Surface *surface = SDL_CreateRGBSurface(0, WINDOW_WIDTH, WINDOW_HEIGHT, 32,
                                                0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    if (!surface) {
        printf("Surface creation failed: %s\n", SDL_GetError());
        return;
    }

    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 0, 0));

    SDL_mutex *render_mutex = SDL_CreateMutex();
    if (!render_mutex) {
        printf("Mutex creation failed: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        threadData[i].renderer = renderer;
        threadData[i].scale = view->scale;
        threadData[i].offsetX = view->offsetX;
        threadData[i].offsetY = view->offsetY;
        threadData[i].iter_per_thread = iter_per_thread;
        threadData[i].seed = seed + i;
        threadData[i].render_mutex = render_mutex;
        threadData[i].surface = surface;

        if (i == NUM_THREADS - 1) {
            threadData[i].iter_per_thread += iter % NUM_THREADS;
        }

        if (pthread_create(&threads[i], NULL, barnsleyFernThread, &threadData[i])) {
            printf("Thread creation failed!\n");
            SDL_DestroyMutex(render_mutex);
            SDL_FreeSurface(surface);
            return;
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        printf("Texture creation failed: %s\n", SDL_GetError());
    } else {
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_DestroyTexture(texture);
    }

    SDL_DestroyMutex(render_mutex);
    SDL_FreeSurface(surface);
}

int main() {
    unsigned long num = 50000;
    SDL_Event e;
    FractalView view = { 80.0, 0.0, -100.0 };
    int isDragging = 0;
    int prevX, prevY;

    printf("Enter number of iterations (default 50000): ");
    if (scanf("%lu", &num) != 1) {
        num = 50000;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Barnsley Fern",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
                                                SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
            else if (e.type == SDL_MOUSEWHEEL) {
                double zoomFactor = e.wheel.y > 0 ? 1.1 : 1/1.1;
                view.scale *= zoomFactor;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                isDragging = 1;
                prevX = e.button.x;
                prevY = e.button.y;
            }
            else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT) {
                isDragging = 0;
            }
            else if (e.type == SDL_MOUSEMOTION && isDragging) {
                view.offsetX += e.motion.x - prevX;
                view.offsetY += e.motion.y - prevY;
                prevX = e.motion.x;
                prevY = e.motion.y;
            }
            else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_r) {
                    view.scale = 80.0;
                    view.offsetX = 0.0;
                    view.offsetY = -100.0;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        barnsleyFern(renderer, num, &view);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
