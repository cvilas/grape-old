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

#include "grapegraphics_common.h"
#include "algorithms/SlidingMax.h"
#include "algorithms/SlidingMin.h"
#include <QtCharts/QChartView>
#include <array>

namespace grape
{

template<int numTraces>
class GRAPEGRAPHICS_DLL_API Plot : public QtCharts::QChartView
{
public:
    explicit Plot(QWidget* pParent = nullptr);

    ~Plot() {}

    void setNumVisibleSamples(std::size_t samples);

    void clear();

    bool addData(float timestamp, const std::array<double, numTraces>& data);

    void setTitle(const QString& title);

    void setXLabel(const QString& str);

    void setYLabel(const QString& str);

    void setLegend(const QStringList& strList);

    void setYRange(float minY, float maxY);

    void setYRangeAuto();

    void setXTickWidth(float w);

    void setYTickWidth(float w);

private:
    void decorateXAxis();
    void decorateYAxis();

private:
    std::array< QVector<QPointF>, numTraces > m_data;

    SlidingMin  m_slidingMin;
    SlidingMax  m_slidingMax;
    std::size_t m_numVisibleSamples;
    float       m_xTickWidth;
    float       m_yTickWidth;
    float       m_minY;
    float       m_maxY;
    float       m_minX;
    float       m_maxX;
    float       m_autoYRange;
};

} // grape

#include "Plot.hpp"
