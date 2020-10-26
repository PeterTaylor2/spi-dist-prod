/*
***************************************************************************
** Registers an error handler in order to catch C-runtime errors from
** the ISDA-CDS library.
**
** Provides function to throw any such errors, but this needs to be called
** explicitly in cases of failure.
***************************************************************************
*/

#include "isdaErrorHandler.hpp"

#include <sstream>
#include <stdexcept>
#include <isda/cerror.h>

static TBoolean ErrorHandlerFunction(
    char* msg, CErrorHandler* handler)
{
    handler->addMessage(msg);
    return FALSE;
}

CErrorHandler::CErrorHandler()
    :
    errors()
{
    JpmcdsErrMsgAddCallback(
        (JpmcdsErrCallBackFunc*)ErrorHandlerFunction,
        FALSE, this);

    JpmcdsErrMsgOn();
}

CErrorHandler::~CErrorHandler()
{
    // just in case the library is being called elsewhere we need to
    // remove the call back when this goes out of scope
    JpmcdsErrMsgAddCallback(NULL, FALSE, NULL);
}

void CErrorHandler::operator()(const char* function)
{
    std::ostringstream oss;
    for (size_t i = 0; i < errors.size(); ++i)
        oss << errors[i];
    if (function)
        oss << function << ": Failed.";

    throw std::runtime_error(oss.str());
}

void CErrorHandler::addMessage(const char* msg)
{
    errors.push_back(std::string(msg));
}


