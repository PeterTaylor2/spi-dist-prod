#include "MapConvert.hpp"
#include "ClassWrapper.hpp"

spi::Value CValueToValue(const CValue& value)
{
    switch(value.GetType())
    {
    case CValue::Double:
        return spi::Value(value.GetDouble());
    case CValue::Integer:
        return spi::Value(value.GetInt());
    case CValue::Date:
        return spi::Value(spi::Date((int)value.GetDate()));
    case CValue::String:
        return spi::Value(value.GetString());
    case CValue::Bool:
        return spi::Value(value.GetBool());
    case CValue::Map:
        return spi::Value(spi::MapConstSP(CMapToMap(value.GetMap())));
    case CValue::Array:
    {
        CArrayConstSP tmp = value.GetArray();
        size_t size = tmp->Size();
        std::vector<spi::Value> values;
        values.reserve(size);
        for (size_t i = 0; i < size; ++i)
            values.push_back(CValueToValue(tmp->Item(i)));
        return spi::Value(values);
    }
    case CValue::Null:
        return spi::Value();
    case CValue::Pointer:
    {
        CPointerConstSP tmp = value.GetPointer();
        if (!tmp)
            return spi::Value(spi::ObjectConstSP());
        ClassWrapper* wrapper = GetClassWrapper(tmp->TypeName());
        return spi::Value(wrapper(tmp->SP()));
    }
    default:
        throw spi::RuntimeError("Unknown value type '%d'",
            (int)value.GetType());
    }
}

spi::MapSP CMapToMap(const CMapConstSP& results)
{
    const std::vector<std::string>& names = results->Names();
    spi::MapSP out(new spi::Map(""));
    for (size_t i = 0; i < names.size(); ++i)
    {
        const std::string& name = names[i];
        const spi::Value& value = CValueToValue(results->Item(name));

        if (!value.isUndefined())
            out->SetValue(name, value);
    }
    return out;
}

