/*
    This file generates generic bridge handlers that are called from various scripting languages.

    To port interactivity with this set of bridge handlers, you will need to
        implement proxy classes in the targeted language.
*/

#ifndef INTERFACES_PROXY_HPP_INCLUDED
#define INTERFACES_PROXY_HPP_INCLUDED
#define SWIG

#ifdef SWIG
#include "Interfaces.hpp"

/*
    \brief: DigitalInput constructor proxy bridge handler

    Executes the physical call to native constructor and returns a pointer
        to the DigitalInput
*/
GHI::Interfaces::DigitalInput *GADGETEERING_PROXY_DIGITAL_INPUT_NEW(int pin, bool initialState)
{
    return new GHI::Interfaces::DigitalInput((GHI::CPUPin)pin, initialState);
}

/*
    \brief: DigitalInput destructor proxy bridge handler

    Executes the physical call to native destructor of a referenced DigitalInput
*/
void GADGETEERING_PROXY_DIGITAL_INPUT_DEL(GHI::Interfaces::DigitalInput *input)
{
    delete input;
    return;
}

/*
    \brief: DigitalInput setResistorMode proxy bridge handler

    Executes the physical call to native method setResistorMode of a
        referenced DigitalInput
*/
void GADGETEERING_PROXY_DIGITAL_INPUT_SET_RESISTOR_MODE(GHI::Interfaces::DigitalInput *input, int mode)
{
    input->setResistorMode((GHI::ResistorMode)mode);
    return;
}

/*
    \brief: DigitalInput read proxy bridge handler

    Executes the physical call to native method read and returns
        the value.
*/
double GADGETEERING_PROXY_DIGITAL_INPUT_READ(GHI::Interfaces::DigitalInput *input)
{
    return input->read();
}

/*
    \brief: DigitalInput constructor proxy bridge handler

    Executes the physical call to native constructor and returns a pointer
        to the DigitalOutput
*/
GHI::Interfaces::DigitalOutput *GADGETEERING_PROXY_DIGITAL_OUTPUT_NEW(int pin, bool initialState)
{
    return new GHI::Interfaces::DigitalInput((GHI::CPUPin)pin, initialState);
}

/*
    \brief: DigitalOutput destructor proxy bridge handler

    Executes the physical call to native destructor of a referenced DigitalOutput
*/
void GADGETEERING_PROXY_DIGITAL_OUTPUT_DEL(GHI::Interfaces::DigitalOutput *output)
{
    delete output;
    return;
}

/*
    \brief: DigitalOutput write proxy bridge handler

    Executes the physical call to native method write
*/
void GADGETEERING_PROXY_DIGITAL_OUTPUT_WRITE(GHI::Interfaces::DigitalOutput *output, bool state)
{
    output->write(state);
    return;
}

/*
    \brief: DigitalIO constructor proxy bridge handler

    Executes the physical call to native constructor and returns a pointer
        to the DigitalIO
*/
GHI::Interfaces::DigitalIO *GADGETEERING_PROXY_DIGITAL_IO_NEW(int pin, bool initialState)
{
    return new GHI::Interfaces::DigitalIO((GHI::CPUPin)pin, initialState);
}

/*
    \brief: DigitalIO destructor proxy bridge handler

    Executes the physical call to native destructor of a referenced DigitalIO
*/
void GADGETEERING_PROXY_DIGITAL_IO_DEL(GHI::Interfaces::DigitalIO *input)
{
    delete input;
    return;
}

/*
    \brief: DigitalIO setResistorMode proxy bridge handler

    Executes the physical call to native method setResistorMode of a
        referenced DigitalIO
*/
void GADGETEERING_PROXY_DIGITAL_IO_SET_RESISTOR_MODE(GHI::Interfaces::DigitalIO *input, int mode)
{
    input->setResistorMode((GHI::ResistorMode)mode);
    return;
}

/*
    \brief: DigitalIO read proxy bridge handler

    Executes the physical call to native method read and returns
        the value.
*/
double GADGETEERING_PROXY_DIGITAL_IO_READ(GHI::Interfaces::DigitalIO *input)
{
    return input->read();
}

/*
    \brief: DigitalIO write proxy bridge handler

    Executes the physical call to native method write
*/
void GADGETEERING_PROXY_DIGITAL_IO_WRITE(GHI::Interfaces::DigitalOutput *output, bool state)
{
    output->write(state);
    return;
}

/*
    \brief: DigitalIO constructor proxy bridge handler

    Executes the physical call to native constructor and returns a pointer
        to the DigitalIO
*/
GHI::Interfaces::DigitalIO *GADGETEERING_PROXY_DIGITAL_IO_NEW(int pin, bool initialState)
{
    return new GHI::Interfaces::DigitalIO((GHI::CPUPin)pin, initialState);
}

/*
    \brief: DigitalIO destructor proxy bridge handler

    Executes the physical call to native destructor of a referenced DigitalIO
*/
void GADGETEERING_PROXY_DIGITAL_IO_DEL(GHI::Interfaces::DigitalIO *input)
{
    delete input;
    return;
}

/*
    \brief: DigitalIO setResistorMode proxy bridge handler

    Executes the physical call to native method setResistorMode of a
        referenced DigitalIO
*/
void GADGETEERING_PROXY_DIGITAL_IO_SET_RESISTOR_MODE(GHI::Interfaces::DigitalIO *input, int mode)
{
    input->setResistorMode((GHI::ResistorMode)mode);
    return;
}

/*
    \brief: DigitalIO setResistorMode proxy bridge handler

    Executes the physical call to native method setIOState of a
        referenced DigitalIO
*/
void GADGETEERING_PROXY_DIGITAL_IO_SET_IO_MODE(GHI::Interfaces::DigitalIO *input, int mode)
{
    input->setIOState((GHI::IOStates)mode);
    return;
}

/*
    \brief: DigitalIO read proxy bridge handler

    Executes the physical call to native method read and returns
        the value.
*/
double GADGETEERING_PROXY_DIGITAL_IO_READ(GHI::Interfaces::DigitalIO *input)
{
    return input->read();
}

/*
    \brief: AnalogInput constructor proxy bridge handler

    Executes the physical call to native constructor and returns a pointer
        to the AnalogInput
*/
GHI::Interfaces::AnalogInput *GADGETEERING_PROXY_ANALOG_INPUT_NEW(int pin, bool initialState)
{
    return new GHI::Interfaces::AnalogInput((GHI::CPUPin)pin, initialState);
}

/*
    \brief: AnalogInput destructor proxy bridge handler

    Executes the physical call to native destructor of a referenced AnalogInput
*/
void GADGETEERING_PROXY_ANALOG_INPUT_DEL(GHI::Interfaces::AnalogInput *input)
{
    delete input;
    return;
}

/*
    \brief: AnalogInput read proxy bridge handler

    Executes the physical call to native method read and returns
        the value.
*/
double GADGETEERING_PROXY_ANALOG_INPUT_READ(GHI::Interfaces::AnalogInput *input)
{
    return input->read();
}

/*
    \brief: AnalogInput constructor proxy bridge handler

    Executes the physical call to native constructor and returns a pointer
        to the AnalogOutput
*/
GHI::Interfaces::AnalogOutput *GADGETEERING_PROXY_ANALOG_OUTPUT_NEW(int pin, bool initialState)
{
    return new GHI::Interfaces::AnalogInput((GHI::CPUPin)pin, initialState);
}

/*
    \brief: AnalogOutput destructor proxy bridge handler

    Executes the physical call to native destructor of a referenced AnalogOutput
*/
void GADGETEERING_PROXY_ANALOG_OUTPUT_DEL(GHI::Interfaces::AnalogOutput *output)
{
    delete output;
    return;
}

/*
    \brief: AnalogOutput write proxy bridge handler

    Executes the physical call to native method write
*/
void GADGETEERING_PROXY_ANALOG_OUTPUT_WRITE(GHI::Interfaces::AnalogOutput *output, bool state)
{
    output->write(state);
    return;
}

/*
    \brief: PWMInput constructor proxy bridge handler

    Executes the physical call to native constructor and returns a pointer
        to the PWMOutput
*/
GHI::Interfaces::PWMOutput *GADGETEERING_PROXY_PWM_OUTPUT_NEW(int pin, bool initialState)
{
    return new GHI::Interfaces::PWMInput((GHI::CPUPin)pin, initialState);
}

/*
    \brief: PWMOutput destructor proxy bridge handler

    Executes the physical call to native destructor of a referenced PWMOutput
*/
void GADGETEERING_PROXY_PWM_OUTPUT_DEL(GHI::Interfaces::PWMOutput *output)
{
    delete output;
    return;
}

/*
    \brief: PWMOutput set proxy bridge handler

    Executes the physical call to native method set
*/
void GADGETEERING_PROXY_PWM_OUTPUT_SET(GHI::Interfaces::PWMOutput *output, double frequency, double dutyCycle)
{
    output->set(frequency, dutyCycle);
    return;
}

/*
    \brief: PWMOutput set proxy bridge handler

    Executes the physical call to native method set
*/
void GADGETEERING_PROXY_PWM_OUTPUT_SET_FREQUENCY(GHI::Interfaces::PWMOutput *output, double frequency)
{
    output->setFrequency(frequency);
    return;
}

/*
    \brief: PWMOutput set proxy bridge handler

    Executes the physical call to native method set
*/
void GADGETEERING_PROXY_PWM_OUTPUT_SET_DUTY_CYCLE(GHI::Interfaces::PWMOutput *output, double dutyCycle)
{
    output->setDutyCycle(dutyCycle);
    return;
}
#endif

#endif // INTERFACES_PROXY_HPP_INCLUDED
