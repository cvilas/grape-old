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

namespace grape
{

//---------------------------------------------------------------------------------------------------------------------
template<typename scalar, int nR, int nC>
RollingMean<scalar, nR, nC>::RollingMean()
//---------------------------------------------------------------------------------------------------------------------
{
    reset();
}

//---------------------------------------------------------------------------------------------------------------------
template<typename scalar, int nR, int nC>
void RollingMean<scalar, nR, nC>::reset()
//---------------------------------------------------------------------------------------------------------------------
{
    _numData = 0;
    _mean.setZero();
    _scaledVariance.setZero();
}

//---------------------------------------------------------------------------------------------------------------------
template<typename scalar, int nR, int nC>
void RollingMean<scalar, nR, nC>::addData(const Eigen::Array<scalar, nR, nC>& data)
//---------------------------------------------------------------------------------------------------------------------
{
    _numData++;

    // m(k) = m(k-1) + { x(k) - m(k-1) } / k
    const Eigen::Array<scalar, nR, nC> delta = data - _mean;
    const Eigen::Array<scalar, nR, nC> newMean = _mean + delta/ ((double)_numData);

    // s(k) = s(k-1) + { x(k) - m(k-1) } * { x(k) - m(k) }
    Eigen::Array<scalar, nR, nC> scaledVar = _scaledVariance + delta * (data - newMean);

    _mean = newMean;
    _scaledVariance = scaledVar;
}

} // grape
