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
#include "algorithms/SlidingMinMax.h"
#include <QtCharts/QChartView>
#include <Eigen/Core>

namespace grape
{

class GRAPEGRAPHICS_DLL_API Plot : public QtCharts::QChartView
{
    Q_OBJECT
public:
    explicit Plot(int numTraces, QWidget* pParent = nullptr);

    virtual ~Plot() = default;

    void setNumVisibleSamples(std::size_t samples);

    int getNumTraces() const { return m_data.size(); }

    void clear();

    bool addData(float timestamp, const Eigen::ArrayXd& data);

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
    QVector< QVector<QPointF> > m_data;

    SlidingMinMax   m_slidingMinMax;
    std::size_t     m_numVisibleSamples;
    float           m_xTickWidth;
    float           m_yTickWidth;
    float           m_minY;
    float           m_maxY;
    float           m_minX;
    float           m_maxX;
    float           m_autoYRange;
};

} // grape
