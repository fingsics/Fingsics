#include "../include/SDLHelpers.h";

void checkForInput(bool& slowMotion, bool& pause, bool& quit, bool& draw, bool& drawOBBs, bool& drawAABBs, Camera*& camera, Camera* freeCamera, Camera* centeredCamera) {
    SDL_Event event;
    int xm, ym;
    SDL_GetMouseState(&xm, &ym);
    while (SDL_PollEvent(&event)) {
        camera->eventUpdate(event);
        switch (event.type) {
        case SDL_QUIT:
            quit = true;
            break;
        case SDL_KEYUP: {
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                quit = true;
                break;
            case SDLK_q:
                quit = true;
                break;
            case SDLK_p:
                pause = !pause;
                break;
            case SDLK_m:
                slowMotion = !slowMotion;
                break;
            case SDLK_f:
                draw = !draw;
                break;
            case SDLK_o:
                drawOBBs = !drawOBBs;
                break;
            case SDLK_i:
                drawAABBs = !drawAABBs;
                break;
            case SDLK_c:
                if (camera == centeredCamera) camera = freeCamera;
                else camera = centeredCamera;
                break;
            default:
                break;
            }
        }
        default:
            break;
        }
    }
}

SDL_Window* initializeSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "Failed to initialize SDL: " << SDL_GetError() << endl;
        exit(1);
    }

    atexit(SDL_Quit);

    SDL_Window* window = SDL_CreateWindow("Fingsics", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_OPENGL);
    if (window == NULL) {
        cerr << "Failed to initialize view mode: " << SDL_GetError() << endl;
        exit(1);
    }
    SDL_GL_CreateContext(window);

    return window;
}
