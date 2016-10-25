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
RunningStatistician<scalar, nR, nC>::RunningStatistician()
//---------------------------------------------------------------------------------------------------------------------
    : m_firstTimestamp(0),
      m_lastTimestamp(0)
{
    clear();
}

//---------------------------------------------------------------------------------------------------------------------
template<typename scalar, int nR, int nC>
void RunningStatistician<scalar, nR, nC>::clear()
//---------------------------------------------------------------------------------------------------------------------
{
    m_numData = 0;
    m_mean.setZero();
    m_scaledVariance.setZero();
}

//---------------------------------------------------------------------------------------------------------------------
template<typename scalar, int nR, int nC>
double RunningStatistician<scalar, nR, nC>::secondsSinceLastData(double tstamp) const
//---------------------------------------------------------------------------------------------------------------------
{
    if(m_numData == 0)
    {
        return 0;
    }
    else
    {
        return tstamp - m_lastTimestamp;
    }
}

//---------------------------------------------------------------------------------------------------------------------
template<typename scalar, int nR, int nC>
double RunningStatistician<scalar, nR, nC>::secondsSinceFirstData(double tstamp) const
//---------------------------------------------------------------------------------------------------------------------
{
    if(m_numData == 0)
    {
        return 0;
    }
    else
    {
        return tstamp - m_firstTimestamp;
    }
}

//---------------------------------------------------------------------------------------------------------------------
template<typename scalar, int nR, int nC>
double RunningStatistician<scalar, nR, nC>::spanSeconds() const
//---------------------------------------------------------------------------------------------------------------------
{
    if(m_numData == 0)
    {
        return 0;
    }
    return m_lastTimestamp - m_firstTimestamp;
}

//---------------------------------------------------------------------------------------------------------------------
template<typename scalar, int nR, int nC>
void RunningStatistician<scalar, nR, nC>::addData(const Eigen::Array<scalar, nR, nC>& data,
                                                                           double tstamp)
//---------------------------------------------------------------------------------------------------------------------
{
    if( m_numData == 0 )
    {
        m_firstTimestamp = tstamp;
    }

    m_numData++;
    m_lastTimestamp = tstamp;

    // m(k) = m(k-1) + { x(k) - m(k-1) } / k
    Eigen::Array<scalar, nR, nC> newMean = m_mean + (data - m_mean) / ((double)m_numData);

    // s(k) = s(k-1) + { x(k) - m(k-1) } * { x(k) - m(k) }
    Eigen::Array<scalar, nR, nC> scaledVar = m_scaledVariance + (data - m_mean) * (data - newMean);

    m_mean = newMean;
    m_scaledVariance = scaledVar;
}

} // grape
