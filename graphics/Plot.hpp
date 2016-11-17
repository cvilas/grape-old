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

#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

#include <inttypes.h>
#include <limits>

namespace grape
{

//---------------------------------------------------------------------------------------------------------------------
template<int numTraces>
Plot<numTraces>::Plot(QWidget* pParent)
//---------------------------------------------------------------------------------------------------------------------
    : QtCharts::QChartView(pParent),
      m_numVisibleSamples(10),
      m_xTickWidth(std::numeric_limits<float>::max()),
      m_yTickWidth(std::numeric_limits<float>::max()),
      m_minY(std::numeric_limits<float>::max()),
      m_maxY(std::numeric_limits<float>::lowest()),
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
template<int numTraces>
bool Plot<numTraces>::addData(float timestamp, const std::array<double, numTraces>& data)
//---------------------------------------------------------------------------------------------------------------------
{
    // append data to show
    QList<QtCharts::QAbstractSeries*> pSeriesList = chart()->series();
    float dataMin = std::numeric_limits<float>::max();
    float dataMax = std::numeric_limits<float>::min();
    for(int i = 0; i < numTraces; ++i )
    {
        const float& value = static_cast<float>(data[i]);
        m_data[i].append(QPointF(timestamp, value));

        // clear data we aren't showing anymore
        const std::size_t sz = m_data[i].size();
        if( sz > m_numVisibleSamples )
        {
            const auto& it = m_data[i].begin();
            m_data[i].erase(it, it+(sz-m_numVisibleSamples));
        }
        static_cast<QtCharts::QLineSeries*>(pSeriesList[i])->replace(m_data[i]);
        if( value < dataMin) dataMin = value;
        if( value > dataMax) dataMax = value;
    }

    m_minX = m_data[0][0].x();
    m_maxX = timestamp;

    // set grid lines etc
    decorateXAxis();
    if(m_autoYRange)
    {
        m_maxY = m_slidingMax.push(dataMax);
        m_minY = m_slidingMin.push(dataMin);
        decorateYAxis();
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
template<int numTraces>
void Plot<numTraces>::clear()
//---------------------------------------------------------------------------------------------------------------------
{
    for(auto& it : m_data )
    {
        it.clear();
    }
}

//---------------------------------------------------------------------------------------------------------------------
template<int numTraces>
void Plot<numTraces>::setNumVisibleSamples(std::size_t samples)
//---------------------------------------------------------------------------------------------------------------------
{
    m_numVisibleSamples = std::max(std::size_t(1), samples);
    m_slidingMin.reset(m_numVisibleSamples);
    m_slidingMax.reset(m_numVisibleSamples);
}

//---------------------------------------------------------------------------------------------------------------------
template<int numTraces>
void Plot<numTraces>::setTitle(const QString& title)
//---------------------------------------------------------------------------------------------------------------------
{
    chart()->setTitle(title);
}

//---------------------------------------------------------------------------------------------------------------------
template<int numTraces>
void Plot<numTraces>::setXLabel(const QString& str)
//---------------------------------------------------------------------------------------------------------------------
{
    static_cast<QtCharts::QValueAxis*>(chart()->axisX())->setTitleText(str);
}

//---------------------------------------------------------------------------------------------------------------------
template<int numTraces>
void Plot<numTraces>::setYLabel(const QString& str)
//---------------------------------------------------------------------------------------------------------------------
{
    static_cast<QtCharts::QValueAxis*>(chart()->axisY())->setTitleText(str);
}

//---------------------------------------------------------------------------------------------------------------------
template<int numTraces>
void Plot<numTraces>::setLegend(const QStringList& strList)
//---------------------------------------------------------------------------------------------------------------------
{
    QtCharts::QChart* pChart = chart();
    QList<QtCharts::QAbstractSeries*> pSeriesList = pChart->series();

    int nLegends = std::min(numTraces, strList.size());
    for(int i = 0; i < nLegends; ++i)
    {
        pSeriesList[i]->setName(strList[i]);
    }

    pChart->legend()->setVisible(true);
    pChart->legend()->setAlignment(Qt::AlignBottom);
}

//---------------------------------------------------------------------------------------------------------------------
template<int numTraces>
void Plot<numTraces>::setYRange(float minY, float maxY)
//---------------------------------------------------------------------------------------------------------------------
{
    m_autoYRange = false;
    m_minY = minY;
    m_maxY = maxY;
    decorateYAxis();
}

//---------------------------------------------------------------------------------------------------------------------
template<int numTraces>
void Plot<numTraces>::setYRangeAuto()
//---------------------------------------------------------------------------------------------------------------------
{
    m_autoYRange = true;
}

//---------------------------------------------------------------------------------------------------------------------
template<int numTraces>
void Plot<numTraces>::setXTickWidth(float w)
//---------------------------------------------------------------------------------------------------------------------
{
    m_xTickWidth = fabs(w);
}

//---------------------------------------------------------------------------------------------------------------------
template<int numTraces>
void Plot<numTraces>::setYTickWidth(float w)
//---------------------------------------------------------------------------------------------------------------------
{
    m_yTickWidth = fabs(w);
    decorateYAxis();
}

//---------------------------------------------------------------------------------------------------------------------
template<int numTraces>
void Plot<numTraces>::decorateYAxis()
//---------------------------------------------------------------------------------------------------------------------
{
    QtCharts::QValueAxis* axisY = static_cast<QtCharts::QValueAxis*>(chart()->axisY());
    if((m_yTickWidth > fabs(m_maxY)) && (m_yTickWidth > fabs(m_minY)))
    {
        axisY->setRange(m_minY, m_maxY);
        axisY->setTickCount( static_cast<int>(2) );
    }
    else
    {
        const float maxY = ceil(m_maxY/m_yTickWidth)*m_yTickWidth;
        const float minY = floor(m_minY/m_yTickWidth)*m_yTickWidth;
        axisY->setRange(minY, maxY);
        axisY->setTickCount( static_cast<int>(1+(maxY - minY)/m_yTickWidth) );
    }
}

//---------------------------------------------------------------------------------------------------------------------
template<int numTraces>
void Plot<numTraces>::decorateXAxis()
//---------------------------------------------------------------------------------------------------------------------
{
    QtCharts::QValueAxis* axisX = static_cast<QtCharts::QValueAxis*>(chart()->axisX());
    axisX->setRange(m_minX, m_maxX);
    axisX->setTickCount( static_cast<int>(2+(m_maxX - m_minX)/m_xTickWidth) );
}

} // grape
