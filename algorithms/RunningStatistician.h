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
 * The template parameter is Eigen::Matrix type
 *
 * See Knuth TAOCP vol 2, 3rd edition, page 232
 * See 'online algorithm' in https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance
 */
template<typename scalar, int nRows, int nColumns>
class RunningStatistician
{
public:
    RunningStatistician();

    ~RunningStatistician() {}

    inline void clear();

    inline void addData(const Eigen::Array<scalar, nRows, nColumns>& data, double timestamp);

    unsigned long long int numData() const { return m_numData; }

    inline double secondsSinceLastData(double tstamp) const;

    inline double secondsSinceFirstData(double tstamp) const;

    inline double spanSeconds() const;

    const Eigen::Array<scalar, nRows, nColumns>& getMean() const { return m_mean; }

    Eigen::Array<scalar, nRows, nColumns> getVariance() const { return m_scaledVariance / ((double)m_numData - 1); }

private:
    unsigned long long int m_numData;
    Eigen::Array<scalar, nRows, nColumns> m_mean;
    Eigen::Array<scalar, nRows, nColumns> m_scaledVariance;
    double m_firstTimestamp;
    double m_lastTimestamp;
};

} // grape

#include "RunningStatistician.hpp"
