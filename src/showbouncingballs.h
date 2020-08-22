// Copyright (C) 2020 Robert Winslow <robertwinslow@comcast.net>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation version 2.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//


#ifndef ShowBouncingBalls_H
#define ShowBouncingBalls_H

#include "ilightshow.h"
#include <algorithm>







class ShowBouncingBalls : public ILightShow
{
    Q_OBJECT
public:
    ShowBouncingBalls(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);
    ~ShowBouncingBalls();

private:
    struct citizen
    {
        citizen() { }

        citizen(int chrom)
          : dna(chrom) {
        }

        int dna;
      };

    class comparer
    {
    public:
      comparer(int t)
        : target_(t) { }

      inline bool operator() (const citizen& c1, const citizen& c2) {
        return (calcFitness(c1.dna, target_) < calcFitness(c2.dna, target_));
      }

    private:
      const int target_;
    };


    // ILightShow interface
public:
    void startShow();

private:
    static int rnd (int i) { return rand() % i; }
    static int R(const int cit) { return at(cit, 16); }
    static int G(const int cit) { return at(cit, 8); }
    static int B(const int cit) { return at(cit, 0); }
    static int at(const int v, const  int offset) { return (v >> offset) & 0xFF; }
    static int calcFitness(const int value, const int target);
    void sort() {std::sort(parents_, parents_ + popSize_, comparer(target_)); }
    bool is85PercentFit();
    void mutate(citizen& c);
    void swap();

    void mate();



    static const int bitsPerPixel = 24;
    int popSize_;

    int target_;
    citizen* children_;
    citizen* parents_;



};

#endif // ShowBouncingBalls_H
