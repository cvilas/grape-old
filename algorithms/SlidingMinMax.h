//==============================================================================
// Copyright (c) Vilas Chitrakaran 2016. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//    * Neither the name of the copyright holder nor the names of its
//      contributors may be used to endorse or promote products derived from
//      this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR ITS CONTRIBUTORS
// BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
// THE POSSIBILITY OF SUCH DAMAGE.
//==============================================================================

#pragma once

#include <vector>
#include <deque>

namespace grape
{

/**
 * Fast sliding window minimum and maximum
 * https://people.cs.uct.ac.za/~ksmith/articles/sliding_window_minimum.html
 *
 * Usage:
 * - call reset to set window size
 * - push data into computeMin() and/or computeMax() following by step()
 */
class SlidingMinMax
{
public:

    SlidingMinMax() : _index(0), _windowSize(1) {}

    ~SlidingMinMax() {}

    inline void reset(int64_t windowSize);

    void step() { ++_index; }

    inline double computeMin(double d);

    inline double computeMax(double d);

private:
    int64_t _index;
    int64_t _windowSize;
    std::deque< std::pair<double, int> > _minWindow;
    std::deque< std::pair<double, int> > _maxWindow;
};

//---------------------------------------------------------------------------------------------------------------------
void SlidingMinMax::reset(int64_t windowSize)
//---------------------------------------------------------------------------------------------------------------------
{
    _minWindow.clear();
    _maxWindow.clear();
    _index = 0;
    _windowSize = windowSize;
}

//---------------------------------------------------------------------------------------------------------------------
double SlidingMinMax::computeMin(double d)
//---------------------------------------------------------------------------------------------------------------------
{
    while (!_minWindow.empty() && _minWindow.back().first >= d)
    {
        _minWindow.pop_back();
    }
    _minWindow.push_back(std::make_pair(d, _index));

    while(_minWindow.front().second <= _index - _windowSize)
    {
        _minWindow.pop_front();
    }

   return _minWindow.front().first;
}

//---------------------------------------------------------------------------------------------------------------------
double SlidingMinMax::computeMax(double d)
//---------------------------------------------------------------------------------------------------------------------
{
    while (!_maxWindow.empty() && _maxWindow.back().first <= d)
    {
        _maxWindow.pop_back();
    }
    _maxWindow.push_back(std::make_pair(d, _index));

    while(_maxWindow.front().second <= _index - _windowSize)
    {
        _maxWindow.pop_front();
    }

    return _maxWindow.front().first;
}


} // grape
