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
 * Fast sliding window minimum
 * https://people.cs.uct.ac.za/~ksmith/articles/sliding_window_minimum.html
 *
 * Usage:
 * - call reset to set window size
 * - push() data to compute min() value within window
 */
class SlidingMin
{
public:

    SlidingMin() : _index(0), _windowSize(1) {}

    ~SlidingMin() {}

    inline void reset(int64_t windowSize);

    inline double push(double d);

private:
    int64_t _index;
    int64_t _windowSize;
    std::deque< std::pair<double, int> > _window;
};

//---------------------------------------------------------------------------------------------------------------------
void SlidingMin::reset(int64_t windowSize)
//---------------------------------------------------------------------------------------------------------------------
{
    _window.clear();
    _index = 0;
    _windowSize = windowSize;
}

//---------------------------------------------------------------------------------------------------------------------
double SlidingMin::push(double d)
//---------------------------------------------------------------------------------------------------------------------
{
    while (!_window.empty() && _window.back().first >= d)
    {
        _window.pop_back();
    }
    _window.push_back(std::make_pair(d, _index));

    while(_window.front().second <= _index - _windowSize)
    {
        _window.pop_front();
    }
    ++_index;
    return _window.front().first;
}

} // grape
