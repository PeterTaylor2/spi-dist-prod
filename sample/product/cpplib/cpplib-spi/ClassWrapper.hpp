#ifndef CPPLIB_SPI_CLASS_WRAPPER_HPP
#define CPPLIB_SPI_CLASS_WRAPPER_HPP

#include <spi/Object.hpp>
#include <spi/Map.hpp>
#include <boost/shared_ptr.hpp>

typedef spi::ObjectConstSP (ClassWrapper)(const boost::shared_ptr<const void>&);

void RegisterClassWrapper(const char*, ClassWrapper*);
ClassWrapper* GetClassWrapper(const char*);

#endif

