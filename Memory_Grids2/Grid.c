#include<SDL3/SDL.h>
#include<SDL3/SDL_main.h>

#define CELL_SIZE 40U

#define START_POINT -29700
//  -29700<p<230000
typedef struct{
    SDL_Window* window;
    SDL_Renderer* renderer;
} AppState;

typedef struct{
    int w;
    int h;
    unsigned int* ptr;
} Grid;

void PaintGrid(AppState appstate,Grid grid){
    for (size_t y = 0; y < grid.h; y++)
    {
        for (size_t x = 0; x < grid.w; x++)
        {
            SDL_FRect rect = {x*CELL_SIZE,y*CELL_SIZE,CELL_SIZE,CELL_SIZE};
            char v = *grid.ptr;
            uint8_t b = (v & 3U) << (6);
            v >>= 2;
            uint8_t g = (v & 7U) << (5);
            v >>= 3;
            uint8_t r = (v & 7U) << (5);
            //SDL_Log("rect(%d,%d)color:r=%d,g=%d,b=%d",x*CELL_SIZE,y*CELL_SIZE,r,g,b);
            SDL_SetRenderDrawColor(appstate.renderer,r,g,b,0);
            SDL_RenderFillRect(appstate.renderer,&rect);
            grid.ptr++;
        }
    }
    
}

int main(int argc,char *argv[])
{
    AppState appstate;
    
    //初始化SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        return SDL_APP_FAILURE;
    }

    SDL_Event operation_event;

    //放置基准指针
    int* a = SDL_malloc(128*sizeof(int));
    memset(a,0,sizeof(a));

    //获取屏幕分辨率并赋值
    SDL_DisplayID display_id = SDL_GetPrimaryDisplay();
    SDL_DisplayMode const *display_mode = SDL_GetDesktopDisplayMode(display_id);
    int SDL_WINDOW_WIDTH = display_mode->w;
    int SDL_WINDOW_HEIGHT = display_mode->h;

    //根据屏幕分辨率适配网格

    Grid grid ={
        .w = SDL_WINDOW_WIDTH/CELL_SIZE+1,
        .h = SDL_WINDOW_HEIGHT/CELL_SIZE+1,
        .ptr = a+(START_POINT)
    };


    SDL_Log("WINDOW_WIDTH = %d, WINDOW_HEIGHT = %d",SDL_WINDOW_WIDTH,SDL_WINDOW_HEIGHT);
    SDL_Log("GRID_WIDTH = %d, GRID_HEIGHT = %d",grid.w,grid.h);
    //初始化窗口和渲染器
    if (!SDL_CreateWindowAndRenderer( "11", SDL_WINDOW_WIDTH, SDL_WINDOW_HEIGHT, SDL_WINDOW_BORDERLESS, &appstate.window, &appstate.renderer)) {
        return SDL_APP_FAILURE;
    }

    //开始绘制


    //SDL_SetRenderDrawColor(appstate.renderer,0,255,0,SDL_ALPHA_OPAQUE);
    //SDL_RenderFillRect(appstate.renderer,&(SDL_FRect){100,100,400,300});
    //SDL_RenderPresent(appstate.renderer);
    
    while ( 1 ) {
        while (SDL_PollEvent(&operation_event))
        {
            switch (operation_event.type)
            {
            default:
                break;
            }
        }
        PaintGrid(appstate,grid);
        SDL_RenderPresent(appstate.renderer);
        SDL_Delay(100);
        grid.ptr++;
        if (grid.ptr > a+(START_POINT)+259700) grid.ptr = a+(START_POINT);
    }

    SDL_DestroyRenderer(appstate.renderer);
    SDL_DestroyWindow(appstate.window);
    SDL_Quit();
}