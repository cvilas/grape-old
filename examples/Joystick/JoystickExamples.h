//==============================================================================
// File     :   JoystickExamples.h
// Brief    :   Example programs for Joystick classes
//==============================================================================

#ifndef JOYSTICK_EXAMPLES_H
#define JOYSTICK_EXAMPLES_H

bool SimpleJoystickExample();

// platform specific examples:
#ifdef _MSC_VER
bool Dx8JoystickManagerExample();
#endif

#endif // JOYSTICK_EXAMPLES_H
