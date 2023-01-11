#include <iostream>
#include "geesespotter.h"
#include "geesespotter_lib.h"

char *createBoard(std::size_t xdim, std::size_t ydim);
void cleanBoard(char *board);
void printBoard(char *board, std::size_t xdim, std::size_t ydim);
void computeNeighbors(char *board, std::size_t xdim, std::size_t ydim);
void hideBoard(char *board, std::size_t xdim, std::size_t ydim);
int reveal(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc);
int mark(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc);
bool isGameWon(char *board, std::size_t xdim, std::size_t ydim);

char *createBoard(std::size_t xdim, std::size_t ydim)
{
    // create a new xdim x ydim dimension board by allocating spaces in memory
    char *board = new char[xdim * ydim];
    return board;
}

void cleanBoard(char *board)
{
    // deallocate the board by deleting its address in memory
    delete[] board;
}

void printBoard(char *board, std::size_t xdim, std::size_t ydim)
{
    char marked = 0;
    char hidden = 0;
    for (int i = 0; i < ydim; i++)
    {
        for (int j = 0; j < xdim; j++)
        {
            // Use & to check if the digit 0x10 has been marked 1&1=1
            if ((board[i * xdim + j] & markedBit()) == markedBit())
            {
                std::cout << 'M';
            }
            // Use & to check if the digit is still hidden
            else if ((board[i * xdim + j] & hiddenBit()) == hiddenBit())
            {
                std::cout << '*';
            }
            // Use valueMask function to print out the number of goose
            else
            {
                std::cout << (board[i * xdim + j] & valueMask());
            }
        }
        std::cout << std::endl;
    }
}

void computeNeighbors(char *board, std::size_t xdim, std::size_t ydim)
{
    for (int i = 0; i < ydim; i++)
    {
        for (int j = 0; j < xdim; j++)
        {
            if ((board[i * xdim + j] != 9))
                for (int insideRow = i - 1; insideRow <= i + 1; insideRow++)
                {
                    for (int insideCol = j - 1; insideCol <= j + 1; insideCol++)
                    {
                        if ((insideRow >= 0) && (insideCol >= 0) && (insideRow < ydim) && (insideCol < xdim) && (board[insideRow * xdim + insideCol] == 9))
                            board[i * xdim + j]++;
                    }
                }
        }
    }
}

void hideBoard(char *board, std::size_t xdim, std::size_t ydim)
{
    for (int i = 0; i < xdim * ydim; i++)
    {
        board[i] += hiddenBit();
    }
}

int reveal(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc)
{
    // 1.判断这个格子是什么状态，如果是M return1。如果是revealed return 2。如果是hidden先翻开再判定
    // 2.翻开格子后有几种可能：1，如果是鹅，return 9。如果是0，翻开周围的9个格子。如果不是0也不是9，正常显示数字

    // If the block is marked, return 1
    if ((board[yloc * xdim + xloc] & markedBit()) == markedBit())
        return 1;
    // If the block is revealed, return 2
    else if (!(board[yloc * xdim + xloc] & hiddenBit()))
        return 2;
    // check if the block is hidden

    // if the block is goose
    if ((board[yloc * xdim + xloc] & valueMask()) == 9)
    {
        board[yloc * xdim + xloc] -= hiddenBit();
        return 9;
    }
    // if reveal the block and it's 0
    else if ((board[yloc * xdim + xloc] & valueMask()) == 0)
    {
        for (int i = (int)yloc - 1; i <= (int)yloc + 1 && i < (int)ydim; i++)
        {
            for (int j = (int)xloc - 1; j <= (int)xloc + 1 && j < (int)xdim; j++)
            {
                //check翻开的不动，其他全开
                if ((i >= 0) && (j >= 0) && 
                    (i < ydim) && (j < xdim) && 
                    (board[i * xdim + j] & hiddenBit()) && 
                    (!((board[i * xdim + j] & markedBit()) ) )
                   )
                    board[i * xdim + j] -= hiddenBit();
            }
        }
        return 0;
    }
    // not 9, not 0, other values
    else
    {
        board[yloc * xdim + xloc] &= (~hiddenBit());
        return 0;
    }
}
int mark(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc)
{
    //如果是已经reveal就return2
    //如果是hidden的就变成M
    //如果是marked的就变成hidden

    if ((board[yloc * xdim + xloc] & hiddenBit()) != hiddenBit())
        return 2;
    else
    {
        board[yloc * xdim + xloc] ^= markedBit();
        return 0;
    }
}
bool isGameWon(char *board, std::size_t xdim, std::size_t ydim)
{
    //把所有不是鹅的格子都翻开就赢了
    for (int row = 0; row < ydim; row++)
    {
        for (int col = 0; col < xdim; col++)
        {
            if (((board[row * xdim + col] & hiddenBit()) == hiddenBit()) && ((board[row * xdim + col] & valueMask()) != 9))
            {
                return false;
            }
        }
    }
    return true;
}
