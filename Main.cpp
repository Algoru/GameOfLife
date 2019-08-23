#include <SDL2/SDL.h>

#define WINDOW_WIDTH 384
#define WINDOW_HEIGHT 384
#define WINDOW_TITLE "LIFE"

#define CELL_SIZE 32
#define GRID_WIDTH WINDOW_WIDTH / CELL_SIZE
#define GRID_HEIGHT WINDOW_HEIGHT / CELL_SIZE
#define GRID_SIZE GRID_WIDTH *GRID_HEIGHT

void draw_grid(SDL_Renderer *, const int, const int, const int);
void draw_cells(SDL_Renderer *, int[GRID_WIDTH][GRID_HEIGHT]);
void update_cells_state(int[GRID_WIDTH][GRID_HEIGHT]);
void init_cells(int cells[GRID_WIDTH][GRID_HEIGHT]);
int get_alive_neighbours(int[GRID_WIDTH][GRID_HEIGHT], int, int);

int main(const int argc, const char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *m_window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    int cells[GRID_WIDTH][GRID_HEIGHT] = {{0}};
    init_cells(cells);

    SDL_Event evt;
    bool running = true;

    Uint32 anim_speed = 200;
    bool update_cells = true;

    while (running)
    {
        while (SDL_PollEvent(&evt))
        {
            if (evt.type == SDL_QUIT)
            {
                running = false;
            }
            else if (evt.type == SDL_KEYDOWN)
            {
                switch (evt.key.keysym.sym)
                {
                case SDLK_UP:
                    anim_speed -= 100;
                    if (anim_speed < 0)
                    {
                        anim_speed = 0;
                    }
                    break;
                case SDLK_DOWN:
                    anim_speed += 100;
                    break;
                case SDLK_SPACE:
                    update_cells = !update_cells;
                    break;
                }
            }
        }

        if (update_cells)
        {
            update_cells_state(cells);
        }

        SDL_RenderClear(m_renderer);

        draw_cells(m_renderer, cells);
        draw_grid(m_renderer, WINDOW_WIDTH, WINDOW_HEIGHT, CELL_SIZE);
        SDL_Delay(anim_speed);

        SDL_RenderPresent(m_renderer);
    }

    SDL_DestroyRenderer(m_renderer);
    m_renderer = nullptr;
    SDL_DestroyWindow(m_window);
    m_window = nullptr;

    SDL_Quit();
    return 0;
}

void draw_grid(SDL_Renderer *renderer, const int width, const int height, const int size)
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    for (int x = size; x < width; x += size)
    {
        SDL_RenderDrawLine(renderer, x, 0, x, height);
    }

    for (int y = size; y < height; y += size)
    {
        SDL_RenderDrawLine(renderer, 0, y, width, y);
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
}

void draw_cells(SDL_Renderer *renderer, int cells[GRID_WIDTH][GRID_HEIGHT])
{
    for (int y = 0; y < GRID_HEIGHT; y++)
    {
        for (int x = 0; x < GRID_WIDTH; x++)
        {
            SDL_Rect rect = {x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE};
            switch (cells[y][x])
            {
            case 0: /* Dead */
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                break;
            case 1: /* Alive */
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
                break;
            }

            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

void update_cells_state(int cells[GRID_WIDTH][GRID_HEIGHT])
{
    for (int y = 0; y < GRID_HEIGHT; y++)
    {
        for (int x = 0; x < GRID_WIDTH; x++)
        {
            int alive = get_alive_neighbours(cells, y, x);
            if (cells[y][x] == 1)
            {
                if (alive <= 1 || alive >= 4)
                {
                    cells[y][x] = 0;
                }
            }
            else
            {
                if (alive >= 3)
                {
                    cells[y][x] = 1;
                }
            }
        }
    }
}

int get_alive_neighbours(int cells[GRID_WIDTH][GRID_HEIGHT], int y, int x)
{
    int alive = 0;

    alive += &cells[y - 1][x - 1] != NULL ? cells[y - 1][x - 1] : 0;
    alive += &cells[y - 1][x] != NULL ? cells[y - 1][x] : 0;
    alive += &cells[y - 1][x + 1] != NULL ? cells[y - 1][x + 1] : 0;
    alive += &cells[y][x + 1] != NULL ? cells[y][x + 1] : 0;
    alive += &cells[y + 1][x + 1] != NULL ? cells[y + 1][x + 1] : 0;
    alive += &cells[y + 1][x] != NULL ? cells[y + 1][x] : 0;
    alive += &cells[y + 1][x - 1] != NULL ? cells[y + 1][x - 1] : 0;
    alive += &cells[y][x - 1] != NULL ? cells[y][x - 1] : 0;

    return alive;
}

void init_cells(int cells[GRID_WIDTH][GRID_HEIGHT])
{
    const int middle = GRID_WIDTH / 2;
    cells[middle][middle] = 1;
    cells[middle + 1][middle] = 1;
    cells[middle][middle + 1] = 1;
}
