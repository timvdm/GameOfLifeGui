#ifndef INC_GAMEOFLIFE_H
#define INC_GAMEOFLIFE_H

#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <map>

#include <iostream>

class GameOfLife
{
  public:
    typedef std::vector<std::vector<bool> > Board;

    GameOfLife(int rows, int cols)
    {
      m_board.resize(rows + 2, std::vector<bool>(cols + 2));
    }

    GameOfLife(const Board &board)
    {
      fromBoard(board);
    }

    GameOfLife(const std::string &board)
    {
      fromString(board);
    }

    std::size_t rows() const
    {
      return m_board.size() - 2;
    }

    std::size_t cols() const
    {
      return m_board.front().size() - 2;
    }

    bool operator()(int row, int col) const
    {
      return m_board[row + 1][col + 1];
    }

    std::vector<bool>::reference operator()(int row, int col)
    {
      return m_board[row + 1][col + 1];
    }

    void fromBoard(const Board &board)
    {
      if (board.size() < 1)
        throw std::runtime_error("Invalid board dimensions, board must be at least 1x1 cells.");
      for (std::size_t i = 1; i < board.size(); ++i)
        if (board[i].size() != board.front().size())
          throw std::runtime_error("Not all rows have the same number of columns.");

      // add extra cells around board to make neighbor accesses easier
      m_board.resize(board.size() + 2);
      // initialize top & bottom rows
      m_board.front().resize(board[0].size() + 2, false);
      m_board.back().resize(board[0].size() + 2, false);
      // copy board
      for (std::size_t i = 0; i < board.size(); ++i) {
        m_board[i + 1].resize(board[0].size() + 2, false);
        std::copy(board[i].begin(), board[i].end(), m_board[i + 1].begin() + 1);
      }
    }

    void fromString(const std::string &board)
    {
      Board brd;
      std::stringstream ss(board);
      std::string line;
      while (std::getline(ss, line)) {
        brd.resize(brd.size() + 1);
        for (std::size_t i = 0; i < line.size(); ++i) 
          if (line[i] == '.')
            brd.back().push_back(false);
          else if (line[i] == 'X')
            brd.back().push_back(true);
          else
            throw std::runtime_error("Invalid character in board, boards contain only '.' and 'X'.");
      }
      fromBoard(brd);
    }

    void next()
    {
      // look for changes that need to be made
      std::map<std::pair<int, int>, bool> changes;
      for (std::size_t i = 0; i < rows(); ++i)
        for (std::size_t j = 0; j < cols(); ++j) {
          if (this->operator()(i, j)) {
            // the cell is alive
            switch (countNeighbors(i, j)) {
              case 0:
              case 1:
                // cell dies from under-population
                changes[std::make_pair(i, j)] = false;
                break;
              case 2:
              case 3:
                // cell lives on
                break;
              default:
                // cell dies from over-population
                changes[std::make_pair(i, j)] = false;
                break;
            }
          } else {
            // the cell is dead
            if (countNeighbors(i, j) == 3)
              changes[std::make_pair(i, j)] = true;
          }
        }

      // apply changes
      std::map<std::pair<int, int>, bool>::const_iterator change;
      for (change = changes.begin(); change != changes.end(); ++change)
        this->operator()(change->first.first, change->first.second) = change->second;
    }

  private:
    int countNeighbors(int row, int col)
    {
      ++row;
      ++col;
      int count = 0;
      if (m_board[row - 1][col - 1])
        ++count;
      if (m_board[row - 1][col])
        ++count;
      if (m_board[row - 1][col + 1])
        ++count;
      if (m_board[row][col - 1])
        ++count;
      if (m_board[row][col + 1])
        ++count;
      if (m_board[row + 1][col - 1])
        ++count;
      if (m_board[row + 1][col])
        ++count;
      if (m_board[row + 1][col + 1])
        ++count;
      return count;
    }

    Board m_board;
};


inline std::ostream& operator<<(std::ostream &os, const GameOfLife &game)
{
  for (std::size_t i = 0; i < game.rows(); ++i) {
    for (std::size_t j = 0; j < game.cols(); ++j)
      os << (game(i, j) ? "X" : ".");
    if (i + 1 < game.rows())
      os << std::endl;
  }
  return os;
}


#endif
