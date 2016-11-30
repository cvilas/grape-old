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

namespace grape
{

/**
 * Compute statistics online as new data is added.
 *
 * See Knuth TAOCP vol 2, 3rd edition, page 232
 * See 'online algorithm' in https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance
 */
template<typename scalar, int nRows, int nColumns>
class RollingMean
{
public:
    RollingMean();

    ~RollingMean() {}

    inline void reset();

    inline void addData(const Eigen::Array<scalar, nRows, nColumns>& data);

    long long int numData() const { return _numData; }

    const Eigen::Array<scalar, nRows, nColumns>& mean() const { return _mean; }

    Eigen::Array<scalar, nRows, nColumns> variance() const { return _scaledVariance / ((double)_numData - 1); }

private:
    long long int                           _numData;
    Eigen::Array<scalar, nRows, nColumns>   _mean;
    Eigen::Array<scalar, nRows, nColumns>   _scaledVariance;
};

} // grape

#include "RollingMean.hpp"
