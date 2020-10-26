#ifndef CPPLIB_SPI_MAP_CONVERT_HPP
#define CPPLIB_SPI_MAP_CONVERT_HPP

#include <spi/Map.hpp>
#include <spi/MapObject.hpp>
#include <boost/shared_ptr.hpp>
#include <utils/Results.hpp>

spi::Value CValueToValue(const CValue& value);
spi::MapSP CMapToMap(const CMapConstSP& results);

#endif

