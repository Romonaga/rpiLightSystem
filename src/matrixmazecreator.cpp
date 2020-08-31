#include "matrixmazecreator.h"



MatrixMazeCreator::MatrixMazeCreator(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{


}

void MatrixMazeCreator::genMaze(uint32_t row, uint32_t col)
{
    std::vector<uint32_t> directions = {N, E, S, W};
    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());
    std::shuffle(directions.begin(), directions.end(), generator);

    for (uint32_t dir = 0; dir < directions.size(); dir++)
    {

        if (N == directions[dir])
        {
          if ((row > 2) && (Wall == _surface[row-2][col]))
          {
            for (uint32_t counter = 0; counter < 3; counter++)
            {
              _surface[row - 2 + counter][col] = Passage;
            }

            genMaze(row - 2, col);
          }
        }
        else if (S == directions[dir])
        {
          if (((row + 2) < (_surface.size() - 1)) && (Wall == _surface[row + 2][col]))
          {
            for (uint32_t counter = 0; counter < 3; counter++)
            {
              _surface[row + counter][col] = Passage;
            }

            genMaze(row + 2, col);
          }
        }
        else if (W == directions[dir])
        {
          if ((col > 2) && (Wall == _surface[row][col - 2]))
          {
            for (uint32_t counter = 0; counter < 3; counter++)
            {
              _surface[row][col - 2 + counter] = Passage;
            }

            genMaze(row, col - 2);
          }
        }
        else if (E == directions[dir])
        {
          if (((col+2) < (_surface[row].size() - 1)) && (Wall == _surface[row][col + 2]))
          {
            for (uint32_t counter = 0; counter < 3; counter++)
            {
              _surface[row][col + counter] = Passage;
            }

            genMaze(row, col + 2);
          }
        }

    }


}

void MatrixMazeCreator::startShow()
{

    _surface.resize(_ledWrapper->getRows());
    for (uint32_t row = 0; row < _ledWrapper->getRows(); row++)
    {
      _surface[row].resize(_ledWrapper->getColumns());
      for (uint32_t col = 0; col < _ledWrapper->getColumns(); col++)
      {
        _surface[row][col] = Wall;

      }
    }

    _surface[0][1] = Passage;
    _surface[_ledWrapper->getRows()][_ledWrapper->getColumns()] = Passage;

    genMaze(genRand(1,_ledWrapper->getRows()) /2u*2u+1u, genRand(1, _ledWrapper->getColumns() - 2)  /2u*2u+1u);

    for(uint32_t row = 0; row < _surface.size(); row++)
    {
         for(uint32_t col = 0; col < _surface[row].size(); col++)
         {
             if(_surface[row][col] == Wall)
                 _ledWrapper->setPixelColor(row, col, 0);
             else
                _ledWrapper->setPixelColor(row, col, _color1);

         }
    }

    _ledWrapper->show();


}
