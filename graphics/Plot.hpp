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
      _numVisibleSamples(10),
      _xTickWidth(std::numeric_limits<float>::max()),
      _yTickWidth(std::numeric_limits<float>::max()),
      _minY(std::numeric_limits<float>::max()),
      _maxY(std::numeric_limits<float>::lowest()),
      _minX(0),
      _maxX(0),
      _autoYRange(false)
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
        _data[i].reserve(_numVisibleSamples);
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
        _data[i].append(QPointF(timestamp, value));

        // clear data we aren't showing anymore
        const std::size_t sz = _data[i].size();
        if( sz > _numVisibleSamples )
        {
            const auto& it = _data[i].begin();
            _data[i].erase(it, it+(sz-_numVisibleSamples));
        }
        static_cast<QtCharts::QLineSeries*>(pSeriesList[i])->replace(_data[i]);
        if( value < dataMin) dataMin = value;
        if( value > dataMax) dataMax = value;
    }

    _minX = _data[0][0].x();
    _maxX = timestamp;

    // set grid lines etc
    decorateXAxis();
    if(_autoYRange)
    {
        _maxY = _slidingMax.push(dataMax);
        _minY = _slidingMin.push(dataMin);
        decorateYAxis();
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
template<int numTraces>
void Plot<numTraces>::clear()
//---------------------------------------------------------------------------------------------------------------------
{
    for(auto& it : _data )
    {
        it.clear();
    }
}

//---------------------------------------------------------------------------------------------------------------------
template<int numTraces>
void Plot<numTraces>::setNumVisibleSamples(std::size_t samples)
//---------------------------------------------------------------------------------------------------------------------
{
    _numVisibleSamples = std::max(std::size_t(1), samples);
    _slidingMin.reset(_numVisibleSamples);
    _slidingMax.reset(_numVisibleSamples);
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
    _autoYRange = false;
    _minY = minY;
    _maxY = maxY;
    decorateYAxis();
}

//---------------------------------------------------------------------------------------------------------------------
template<int numTraces>
void Plot<numTraces>::setYRangeAuto()
//---------------------------------------------------------------------------------------------------------------------
{
    _autoYRange = true;
}

//---------------------------------------------------------------------------------------------------------------------
template<int numTraces>
void Plot<numTraces>::setXTickWidth(float w)
//---------------------------------------------------------------------------------------------------------------------
{
    _xTickWidth = fabs(w);
}

//---------------------------------------------------------------------------------------------------------------------
template<int numTraces>
void Plot<numTraces>::setYTickWidth(float w)
//---------------------------------------------------------------------------------------------------------------------
{
    _yTickWidth = fabs(w);
    decorateYAxis();
}

//---------------------------------------------------------------------------------------------------------------------
template<int numTraces>
void Plot<numTraces>::decorateYAxis()
//---------------------------------------------------------------------------------------------------------------------
{
    QtCharts::QValueAxis* axisY = static_cast<QtCharts::QValueAxis*>(chart()->axisY());
    if((_yTickWidth > fabs(_maxY)) && (_yTickWidth > fabs(_minY)))
    {
        axisY->setRange(_minY, _maxY);
        axisY->setTickCount( static_cast<int>(2) );
    }
    else
    {
        const float maxY = ceil(_maxY/_yTickWidth)*_yTickWidth;
        const float minY = floor(_minY/_yTickWidth)*_yTickWidth;
        axisY->setRange(minY, maxY);
        axisY->setTickCount( static_cast<int>(1+(maxY - minY)/_yTickWidth) );
    }
}

//---------------------------------------------------------------------------------------------------------------------
template<int numTraces>
void Plot<numTraces>::decorateXAxis()
//---------------------------------------------------------------------------------------------------------------------
{
    QtCharts::QValueAxis* axisX = static_cast<QtCharts::QValueAxis*>(chart()->axisX());
    axisX->setRange(_minX, _maxX);
    axisX->setTickCount( static_cast<int>(2+(_maxX - _minX)/_xTickWidth) );
}

} // grape
