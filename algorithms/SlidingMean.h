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

#include <Eigen/Core>

#include <deque>
#include <inttypes.h>

namespace grape
{

/**
 * Compute statistics over a sliding window
 */
template<typename scalar, int nRows, int nColumns>
class SlidingMean
{
public:
    SlidingMean();

    ~SlidingMean();

    void reset(unsigned long long int windowSize);

    void addData(const Eigen::Array<scalar, nRows, nColumns>& data);

    unsigned long long int numData() const { return _window.size(); }

    const Eigen::Array<scalar, nRows, nColumns>& mean() const { return _mean; }

    Eigen::Array<scalar, nRows, nColumns> variance() const { return _scaledVariance/((double)numData() - 1); }

private:
    unsigned long long int                  _windowSize;
    Eigen::Array<scalar, nRows, nColumns>   _mean;
    Eigen::Array<scalar, nRows, nColumns>   _scaledVariance;
    using WindowList = std::deque<Eigen::Array<scalar, nRows, nColumns>>;
    WindowList                              _window;
};

} // grape

#include "SlidingMean.hpp"
