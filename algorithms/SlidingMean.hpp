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
SlidingMean<scalar, nR, nC>::SlidingMean()
//---------------------------------------------------------------------------------------------------------------------
    : _windowSize(1)
{}

//---------------------------------------------------------------------------------------------------------------------
template<typename scalar, int nR, int nC>
SlidingMean<scalar, nR, nC>::~SlidingMean()
//---------------------------------------------------------------------------------------------------------------------
{}

//---------------------------------------------------------------------------------------------------------------------
template<typename scalar, int nR, int nC>
void SlidingMean<scalar, nR, nC>::reset(unsigned long long int windowSize)
//---------------------------------------------------------------------------------------------------------------------
{
    _windowSize = windowSize;
    _window.clear();
    _mean.setZero();
    _scaledVariance.setZero();
}

//---------------------------------------------------------------------------------------------------------------------
template<typename scalar, int nR, int nC>
void SlidingMean<scalar, nR, nC>::addData(const Eigen::Array<scalar, nR, nC>& d)
//---------------------------------------------------------------------------------------------------------------------
{
    unsigned long long int sz = _window.size();
    _window.push_back(d);
    if(sz < _windowSize)
    {
        // See Knuth TAOCP vol 2, 3rd edition, page 232
        // See 'online algorithm' in https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance

        // m(k) = m(k-1) + { x(k) - m(k-1) } / k
        const Eigen::Array<scalar, nR, nC> delta = d - _mean;
        const Eigen::Array<scalar, nR, nC> newMean = _mean + delta/((double)sz+1);

        // s(k) = s(k-1) + { x(k) - m(k-1) } * { x(k) - m(k) }
        _scaledVariance += delta * (d - newMean);

        _mean = newMean;
    }
    else
    {
        // See http://jonisalonen.com/2014/efficient-and-accurate-rolling-standard-deviation/

        const Eigen::Array<scalar, nR, nC> x0 = _window.front();
        const Eigen::Array<scalar, nR, nC> delta = d - x0;

        // m(k) = m(k-1) + { x(k) - x(0) } / k
        const Eigen::Array<scalar, nR, nC> newMean = _mean + delta/((double)sz);

        // s(k) = s(k-1) + { x(k) - x(0) } * { x(k) - m(k) + x(0) - m(k-1) }
        _scaledVariance += delta * (d - newMean + x0 - _mean);

        _mean = newMean;

        _window.pop_front();
    }
}

} // grape
