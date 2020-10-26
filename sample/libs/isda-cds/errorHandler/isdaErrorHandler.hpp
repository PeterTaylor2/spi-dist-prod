#ifndef ISDA_ERROR_HANDLER_HPP
#define ISDA_ERROR_HANDLER_HPP

/*
***************************************************************************
** Registers an error handler in order to catch C-runtime errors from
** the ISDA-CDS library.
**
** Provides function to throw any such errors, but this needs to be called
** explicitly in cases of failure.
***************************************************************************
*/

#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>

/**
 * Defines the CErrorHandler.
 *
 * In c++ code about to call isda C-functions you do the following:
 *
 * CErrorHandler errorHandler;
 *
 * if (JpmcdsFunc(...) != SUCCESS)
 *     errorHandler(__FUNCTION__);
 */

class CErrorHandler
{
public:
    CErrorHandler();
    ~CErrorHandler();
    void operator()(const char* function=0);
    void addMessage(const char* msg);

private:
    std::vector<std::string> errors;
};

#define THROW_RUNTIME_ERROR(msg) { \
std::ostringstream oss_;\
oss_ << __FUNCTION__ << ": " << msg;\
throw std::runtime_error(oss_.str());\
}

#undef PRE_CONDITION
#define PRE_CONDITION(cond) do { if(!(cond)) {\
THROW_RUNTIME_ERROR("Required pre-condition (" << #cond << ") fails"); \
}} while (0)

#undef POST_CONDITION
#define POST_CONDITION(cond) do { if(!(cond)) {\
THROW_RUNTIME_ERROR("Required post-condition (" << #cond << ") fails"); \
}} while (0)

#define NOT_IMPLEMENTED THROW_RUNTIME_ERROR("Not implemented")

#endif /* ISDA_ERROR_HANDLER_HPP */

