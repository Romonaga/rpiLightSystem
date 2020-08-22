#include "showbouncingballs.h"
#include <QDebug>

ShowBouncingBalls::ShowBouncingBalls(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{
   popSize_ = _ledWrapper->getColumns() * _ledWrapper->getRows();

   // Allocate memory
   children_ = new citizen[popSize_];
   parents_ = new citizen[popSize_];
   srand(time(NULL));
}

ShowBouncingBalls::~ShowBouncingBalls()
{
    delete [] children_;
    delete [] parents_;
}


  /// fitness here is how "similar" the color is to the target
int ShowBouncingBalls::calcFitness(const int value, const int target)
{
    // Count the number of differing bits
    int diffBits = 0;
    for (unsigned int diff = value ^ target; diff; diff &= diff - 1)
    {
      ++diffBits;
    }
    return diffBits;
}

void ShowBouncingBalls::mate()
{
    // Adjust these for fun and profit
    const float eliteRate = 0.30f;
    const float mutationRate = 0.20f;

    const int numElite = popSize_ * eliteRate;
    for (int i = 0; i < numElite; ++i) {
      children_[i] = parents_[i];
    }

    for (int i = numElite; i < popSize_; ++i) {
      //select the parents randomly
      const float sexuallyActive = 1.0 - eliteRate;
      const int p1 = rand() % (int)(popSize_ * sexuallyActive);
      const int p2 = rand() % (int)(popSize_ * sexuallyActive);
      const unsigned matingMask = (~0u) << (rand() % bitsPerPixel);

      // Make a baby
      unsigned baby = (parents_[p1].dna & matingMask)
        | (parents_[p2].dna & ~matingMask);
      children_[i].dna = baby;

      // Mutate randomly based on mutation rate
      if ((rand() / (float)RAND_MAX) < mutationRate) {
        mutate(children_[i]);
      }
    }
  }

  /// parents make children,
  /// children become parents,
  /// and they make children...
void ShowBouncingBalls::swap()
  {
    citizen* temp = parents_;
    parents_ = children_;
    children_ = temp;
  }

  void ShowBouncingBalls::mutate(citizen& c)
  {
    // Flip a random bit
    c.dna ^= 1 << (rand() % bitsPerPixel);
  }

  /// can adjust this threshold to make transition to new target seamless
  bool ShowBouncingBalls::is85PercentFit()
  {
    int numFit = 0;
    for (int i = 0; i < popSize_; ++i) {
      if (calcFitness(children_[i].dna, target_) < 1)
      {
        ++numFit;
      }
    }
    return ((numFit / (float)popSize_) > 0.85f);
  }


void ShowBouncingBalls::startShow()
{
    target_ = rand() & 0xFFFFFF;

    // Create the first generation of random children_
    for (int i = 0; i < popSize_; ++i)
    {
      children_[i].dna = rand() & 0xFFFFFF;
    }

    while (_running && _endTime > time(nullptr))
    {
        swap();
        sort();
        mate();
        std::random_shuffle (children_, children_ + popSize_, rnd);

        // Draw citizens to canvas
        for(int i=0; i < popSize_; i++)
        {
            int c = children_[i].dna;
            int x = i % _ledWrapper->getColumns();
            int y = (int)(i / _ledWrapper->getColumns());
            _ledWrapper->setPixelColor(y, x, _ledWrapper->Red(c), _ledWrapper->Green(c), _ledWrapper->Blue(c));

        }
        _ledWrapper->show();

        // When we reach the 85% fitness threshold...
        if(is85PercentFit())
        {
            // ...set a new random target_
            target_ = rand() & 0xFFFFFF;

            // Randomly mutate everyone for sake of new colors
            for (int i = 0; i < popSize_; ++i)
            {
                mutate(children_[i]);
            }
        }

        _ledWrapper->waitMillSec(_wait);

    }
}
