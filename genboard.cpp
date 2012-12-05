#include <sstream>
#include <fstream>
#include <cstdlib>
#include <iostream>

int main(int argc, char **argv)
{
  if (argc < 4) {
    std::cerr << "Usage: " << argv[0] << " <filename> <rows> <columns>" << std::endl;
    return 1;
  }

  int rows, columns;
  std::stringstream ss1(argv[2]);
  ss1 >> rows;
  std::stringstream ss2(argv[3]);
  ss2 >> columns;

  std::ofstream ofs(argv[1]);
  if (!ofs) {
    std::cerr << "Could not open " << argv[1] << " for writing." << std::endl;
    return 1;
  }


  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      bool val = !(rand() % 5);
      if (val)
        ofs << "X";
      else
        ofs << ".";
    }

    if (i + 1 < rows)
      ofs << std::endl;
  }
}
