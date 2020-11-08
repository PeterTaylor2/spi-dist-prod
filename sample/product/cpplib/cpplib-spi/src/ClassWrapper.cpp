#include "ClassWrapper.hpp"
#include <utils/RuntimeError.hpp>

static std::map<std::string, ClassWrapper*> ClassWrappers;

void RegisterClassWrapper(const char* classname, ClassWrapper* wrapper)
{
    ClassWrappers[classname] = wrapper;
}

ClassWrapper* GetClassWrapper(const char* classname)
{
    std::map<std::string, ClassWrapper*>::const_iterator iter =
        ClassWrappers.find(classname);

    if (iter == ClassWrappers.end())
        THROW_RUNTIME_ERROR("No wrapping defined for '" << classname << "'");

    return iter->second;
}

