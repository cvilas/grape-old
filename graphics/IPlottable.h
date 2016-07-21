#ifndef IPLOTTABLE_H
#define IPLOTTABLE_H

#include "grapegraphics_common.h"

namespace grape
{

/// \class IPlottable
/// \ingroup core
/// \brief Interface definition for plottable objects
///
/// Generic interface to objects that generate data that can be plotted
/// on grape::Plot.
///
/// Essentially, I want to be able to do this to update the chart with data
/// streamed from an object obj
///
/// grape::Plot chart;
/// chart << obj;

class GRAPEGRAPHICS_DLL_API IPlottable
{
protected:
    IPlottable() {}

public:
    virtual ~IPlottable() {}

};

} // grape

#endif // IPLOTTABLE_H
