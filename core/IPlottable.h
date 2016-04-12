#ifndef IPLOTTABLE_H
#define IPLOTTABLE_H

#include "grapecore_common.h"

namespace Grape
{
/// \class IPlottable
/// \ingroup core
/// \brief Interface definition for plottable objects
///
/// Generic interface to objects that generate data that can be plotted
/// on GrapeChart.
///
/// Essentially, I want to be able to do this to update the chart with data
/// streamed from an object obj
///
/// GrapeChart chart;
/// chart << obj;

class GRAPECORE_DLL_API IPlottable
{
protected:
    IPlottable() {}

public:
    virtual ~IPlottable() {}

};

} // Grape

#endif // IPLOTTABLE_H
