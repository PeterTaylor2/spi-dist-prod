#include "ClassWrappers.hpp"
#include "ClassWrapper.hpp"

#include "cpplib_cashflow_classes.hpp"
#include <rates/CashFlowList.hpp>

static spi::ObjectConstSP WrapCashFlowList(
    const boost::shared_ptr<const void>& pv)
{
    return cpplib::CashFlowList::Wrap(
        boost::static_pointer_cast<const CCashFlowList>(pv));
}

void RegisterClassWrappers() 
{
    RegisterClassWrapper("CashFlowList", WrapCashFlowList);
}

