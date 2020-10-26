#ifndef _SPDOC_SPDOC_SERVICE_HPP_
#define _SPDOC_SPDOC_SERVICE_HPP_

/**
****************************************************************************
* Header file: spdoc_spdoc_service.hpp
****************************************************************************
*/

#include <spi/spi.hpp>
#include "spdoc_spdoc_decl_spec.h"
#include "spdoc_namespace.hpp"

SPDOC_BEGIN_NAMESPACE

SPI_IMPORT
spi::ServiceSP spdoc_start_service();

SPI_IMPORT
void spdoc_stop_service();

SPI_IMPORT
spi::ServiceSP spdoc_exported_service();

SPI_IMPORT
spi::Date spdoc_get_time_out();

SPDOC_END_NAMESPACE

#endif /* _SPDOC_SPDOC_SERVICE_HPP_*/

