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


#include "Plot.h"

#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QDebug>

#include <inttypes.h>
#include <limits>

namespace grape
{


//---------------------------------------------------------------------------------------------------------------------
Plot::Plot(int numTraces, QWidget* pParent)
//---------------------------------------------------------------------------------------------------------------------
    : QtCharts::QChartView(pParent),
      m_data(numTraces),
      m_numVisibleSamples(10),
      m_xTickWidth(std::numeric_limits<float>::max()),
      m_yTickWidth(std::numeric_limits<float>::max()),
      m_minY(std::numeric_limits<float>::max()),
      m_maxY(-std::numeric_limits<float>::max()),
      m_minX(0),
      m_maxX(0),
      m_autoYRange(false)
{
    QtCharts::QChart* pChart = chart();
    QtCharts::QValueAxis* pAxisX = new QtCharts::QValueAxis;
    QtCharts::QValueAxis* pAxisY = new QtCharts::QValueAxis;
    for(int i = 0; i < numTraces; ++i)
    {
        QtCharts::QLineSeries* pSeries = new QtCharts::QLineSeries;
        pSeries->setUseOpenGL(true);
        pChart->addSeries(pSeries);
        pChart->setAxisX(pAxisX, pSeries);
        pChart->setAxisY(pAxisY, pSeries);
        m_data[i].reserve(m_numVisibleSamples);
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool Plot::addData(float timestamp, const Eigen::ArrayXd& data)
//---------------------------------------------------------------------------------------------------------------------
{
    int nTraces = getNumTraces();
    if( data.size() != nTraces )
    {
        return false;
    }

    // append data to show
    QList<QtCharts::QAbstractSeries*> pSeriesList = chart()->series();
    for(int i = 0; i < nTraces; ++i )
    {
        const float& value = static_cast<float>(data[i]);
        m_data[i].append(QPointF(timestamp, value));
        static_cast<QtCharts::QLineSeries*>(pSeriesList[i])->replace(m_data[i]);
    }

    // clear data we aren't showing anymore
    for(int i = 0; i < nTraces; ++i )
    {
        std::size_t sz = m_data[i].size();
        if( sz > m_numVisibleSamples )
        {
            const auto& it = m_data[i].begin();
            m_data[i].erase(it, it+(sz-m_numVisibleSamples));
        }
    }

    m_minX = m_data[0][0].x();
    m_maxX = timestamp;

    // set grid lines etc
    decorateXAxis();
    if(m_autoYRange)
    {
        m_maxY = m_slidingMinMax.computeMax(data.maxCoeff());
        m_minY = m_slidingMinMax.computeMin(data.minCoeff());
        m_slidingMinMax.step();
        decorateYAxis();
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
void Plot::clear()
//---------------------------------------------------------------------------------------------------------------------
{
    for(auto& it : m_data )
    {
        it.clear();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void Plot::setNumVisibleSamples(std::size_t samples)
//---------------------------------------------------------------------------------------------------------------------
{
    m_numVisibleSamples = samples;
    m_slidingMinMax.reset(m_numVisibleSamples);
}

//---------------------------------------------------------------------------------------------------------------------
void Plot::setTitle(const QString& title)
//---------------------------------------------------------------------------------------------------------------------
{
    chart()->setTitle(title);
}

//---------------------------------------------------------------------------------------------------------------------
void Plot::setXLabel(const QString& str)
//---------------------------------------------------------------------------------------------------------------------
{
    static_cast<QtCharts::QValueAxis*>(chart()->axisX())->setTitleText(str);
}

//---------------------------------------------------------------------------------------------------------------------
void Plot::setYLabel(const QString& str)
//---------------------------------------------------------------------------------------------------------------------
{
    static_cast<QtCharts::QValueAxis*>(chart()->axisY())->setTitleText(str);
}

//---------------------------------------------------------------------------------------------------------------------
void Plot::setLegend(const QStringList& strList)
//---------------------------------------------------------------------------------------------------------------------
{
    QtCharts::QChart* pChart = chart();
    QList<QtCharts::QAbstractSeries*> pSeriesList = pChart->series();

    int nLegends = std::min(getNumTraces(), strList.size());
    for(int i = 0; i < nLegends; ++i)
    {
        pSeriesList[i]->setName(strList[i]);
    }

    pChart->legend()->setVisible(true);
    pChart->legend()->setAlignment(Qt::AlignBottom);
}

//---------------------------------------------------------------------------------------------------------------------
void Plot::setYRange(float minY, float maxY)
//---------------------------------------------------------------------------------------------------------------------
{
    m_autoYRange = false;
    m_minY = minY;
    m_maxY = maxY;
    decorateYAxis();
}

//---------------------------------------------------------------------------------------------------------------------
void Plot::setYRangeAuto()
//---------------------------------------------------------------------------------------------------------------------
{
    m_autoYRange = true;
}

//---------------------------------------------------------------------------------------------------------------------
void Plot::setXTickWidth(float w)
//---------------------------------------------------------------------------------------------------------------------
{
    m_xTickWidth = w;
}

//---------------------------------------------------------------------------------------------------------------------
void Plot::setYTickWidth(float w)
//---------------------------------------------------------------------------------------------------------------------
{
    if( fabs(m_yTickWidth - w) > 1e-6 )
    {
        m_yTickWidth = w;
        decorateYAxis();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void Plot::decorateYAxis()
//---------------------------------------------------------------------------------------------------------------------
{
    m_maxY = ceil(m_maxY/m_yTickWidth)*m_yTickWidth;
    m_minY = floor(m_minY/m_yTickWidth)*m_yTickWidth;
    QtCharts::QValueAxis* axisY = static_cast<QtCharts::QValueAxis*>(chart()->axisY());
    axisY->setRange(m_minY, m_maxY);
    axisY->setTickCount( static_cast<int>(1+(m_maxY - m_minY)/m_yTickWidth) );
}

//---------------------------------------------------------------------------------------------------------------------
void Plot::decorateXAxis()
//---------------------------------------------------------------------------------------------------------------------
{
    QtCharts::QValueAxis* axisX = static_cast<QtCharts::QValueAxis*>(chart()->axisX());
    axisX->setRange(m_minX, m_maxX);
    axisX->setTickCount( static_cast<int>(2+(m_maxX - m_minX)/m_xTickWidth) );
}

} // grape
