#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <vector>
#include <thread>
#include <chrono>

const int HEIGHT = 260;
const int WIDTH = 240;
const int PIXEL_SIZE = 10;
const int SCREEN_HEIGHT = HEIGHT / PIXEL_SIZE;
const int SCREEN_WIDTH = WIDTH / PIXEL_SIZE;
const int CELL_SIZE = SCREEN_WIDTH / 20;

class Conway : public olc::PixelGameEngine
{
public:
    Conway()
    {
        sAppName = "Conway";
    }

    std::array<std::array<bool, HEIGHT>, WIDTH> nextCells;

    bool OnUserCreate() override
    {
        // Assign each cell with a random state
        for (int x = 0; x < WIDTH; x++)
        {
            for (int y = 0; y < HEIGHT; y++)
            {
                nextCells.at(x).at(y) = (int)rand() % 2 == 0;
            }
        }
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        Clear(olc::BLACK);

        std::array<std::array<bool, HEIGHT>, WIDTH> currentCells = nextCells;

        // Draw the cells
        for (int x = 0; x < WIDTH; x++)
        {
            for (int y = 0; y < HEIGHT; y++)
            {
                if (currentCells.at(x).at(y))
                    FillRect(x, y, CELL_SIZE, CELL_SIZE, olc::WHITE);
            }
        }

        // Calculate nextCells
        for (int x = 0; x < WIDTH; x++)
        {
            for (int y = 0; y < HEIGHT; y++)
            {
                // Get neigboring coordinates
                // '% WIDTH' ensures leftCoord is always between 0 and WIDTH-1
                int leftCoord = (x - 1 + WIDTH) % WIDTH;
                int rightCoord = (x + 1) % WIDTH;
                int upCoord = (y - 1 + HEIGHT) % HEIGHT;
                int downCoord = (y + 1) % HEIGHT;

                // Count number of living neighbours
                int numNeighbours = 0;
                if (currentCells.at(leftCoord).at(upCoord))
                    numNeighbours++;
                if (currentCells.at(x).at(upCoord))
                    numNeighbours++;
                if (currentCells.at(rightCoord).at(upCoord))
                    numNeighbours++;
                if (currentCells.at(leftCoord).at(y))
                    numNeighbours++;
                if (currentCells.at(rightCoord).at(y))
                    numNeighbours++;
                if (currentCells.at(leftCoord).at(downCoord))
                    numNeighbours++;
                if (currentCells.at(x).at(downCoord))
                    numNeighbours++;
                if (currentCells.at(rightCoord).at(downCoord))
                    numNeighbours++;

                // Set cells based on Conway's Game of Life rules:
                if (currentCells.at(x).at(y) && (numNeighbours == 2 || numNeighbours == 3))
                    nextCells.at(x).at(y) = false;
                else if (!currentCells.at(x).at(y) && numNeighbours == 3)
                    nextCells.at(x).at(y) = true;
                else
                    currentCells.at(x).at(y) = false;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        return true;
    }
};

int main()
{
    Conway app;
    if (app.Construct(HEIGHT, WIDTH, PIXEL_SIZE, PIXEL_SIZE))
        app.Start();
    return 0;
}