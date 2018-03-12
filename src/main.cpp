#include <iostream>
#include <SDL.h>
#include <complex.h>

#define WIDTH 1280
#define HEIGHT 600
#define ITERATIONS 200
#define WHITE 0xffffffff
#define BLACK 0xff000000

struct Camera{
    double x;
    double y;
    double w;
    double h;
};

void putPixel(SDL_Surface* surface, int x, int y, Uint32 val){
    SDL_LockSurface(surface);
    Uint32* pixels = (Uint32*)surface->pixels;
    pixels[y*WIDTH+x] = val;
    SDL_UnlockSurface(surface);
}

bool isInMandelbrotSet(double x, double y, int* numOfIter = nullptr){
    std::complex<double> point(x,y);
    if(abs(point) > 2){
        return false;
    }
    std::complex<double> value(0,0);
    for(int i = 0; i < ITERATIONS; i++){
        value = std::pow(value, 2)+point;
        if(abs(value) > 2){
            if(numOfIter){
                *numOfIter = i;
            }
            return false;
        }
    }
    return true;
}

void drawMandelbrot(SDL_Surface* surface, Camera camera){
    for(int x = 0; x < WIDTH; x++){
        for(int y = 0; y < HEIGHT; y++){
            double manX = camera.x+(double)x/WIDTH*camera.w;
            double manY = camera.y+(double)y/HEIGHT*camera.h;
            int numOfIter = 0;
            if(isInMandelbrotSet(manX, manY, &numOfIter)){
                putPixel(surface, x, y, WHITE);
            }
            else{
                double fill = 0xff*1.0* numOfIter / ITERATIONS;
                Uint8 blue = static_cast<Uint8>(fill*10);
                Uint8 red = static_cast<Uint8>(fill*100);
                Uint32 colour = red << 4 | blue << 6;
                putPixel(surface, x, y, BLACK | colour);
            }
        }
    }
}

bool isInJuliaSet(double x, double y, int n, std::complex<double> c, int* numOfIter = nullptr){
    std::complex<double> value(x,y);
    for(int i = 0; i < ITERATIONS; i++){
        value = std::pow(value, n)+c;
        if(abs(value) > 2){
            if(numOfIter){
                *numOfIter = i;
            }
            return false;
        }
    }
    return true;
}

void drawJulia(SDL_Surface* surface, Camera camera, int n, std::complex<double> c){
    for(int x = 0; x < WIDTH; x++){
        for(int y = 0; y < HEIGHT; y++){
            double manX = camera.x+(double)x/WIDTH*camera.w;
            double manY = camera.y+(double)y/HEIGHT*camera.h;
            int numOfIter = 0;
            if(isInJuliaSet(manX, manY,n, c, &numOfIter)){
                putPixel(surface, x, y, WHITE);
            }
            else{
                double fill = 0xff*1.0* numOfIter / ITERATIONS;
                Uint8 blue = static_cast<Uint8>(fill*10);
                Uint8 red = static_cast<Uint8>(fill*100);
                Uint32 colour = red << 4 | blue << 6;
                putPixel(surface, x, y, BLACK | colour);
            }
        }
    }
}

int main() {
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* win = SDL_CreateWindow("Fractals", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0);
    SDL_Surface* surface = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32,
            0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
    SDL_Surface* winSurf = SDL_GetWindowSurface(win);
    if(surface == NULL){
        std::cout << SDL_GetError();
        exit(-1);
    }

    bool running = true;
    bool changed = true;
    Camera mainCamera{-2, -1, 3, 2};

    while(running){
        if(changed){
            //drawMandelbrot(surface, mainCamera);
            drawJulia(surface, mainCamera, 2, std::complex<double>(0.285,0.01));
            if(SDL_BlitSurface(surface, NULL, winSurf, NULL) < 0){
                std::cout << SDL_GetError();
                exit(-1);
            }
            changed = false;
        }

        SDL_UpdateWindowSurface(win);

        SDL_Event event{};
        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    changed = true;
                    switch (event.key.keysym.sym) {
                        case SDLK_KP_PLUS:
                            mainCamera.w *= 0.8;
                            mainCamera.h *= 0.8;
                            break;
                        case SDLK_MINUS:
                            mainCamera.w *= 1.25;
                            mainCamera.h *= 1.25;
                            break;
                        case SDLK_RIGHT:
                            mainCamera.x += 0.1*mainCamera.w;
                            break;
                        case SDLK_LEFT:
                            mainCamera.x -= 0.1*mainCamera.w;
                            break;
                        case SDLK_UP:
                            mainCamera.y -= 0.1*mainCamera.h;
                            break;
                        case SDLK_DOWN:
                            mainCamera.y += 0.1*mainCamera.h;
                            break;
                        default:
                            changed = false;
                            break;
                    }
                default:
                    break;
            }
        }
    }

    SDL_Quit();
    return 0;
}