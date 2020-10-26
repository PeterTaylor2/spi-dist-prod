#ifndef _SPDOC_SPDOC_DECL_SPEC_H_
#define _SPDOC_SPDOC_DECL_SPEC_H_


/*
****************************************************************************
** spdoc_spdoc_decl_spec.h
**
** Defines declaration specifications. Windows specific.
**
** SPI_IMPORT is defined for Microsoft compilations.
** If SPI_EXPORT is defined then SPI_IMPORT is defined as dllexport.
** Otherwise SPI_IMPORT is defined as dllimport
****************************************************************************
*/

#ifdef SPI_STATIC
#define SPI_IMPORT
#else

#ifdef _MSC_VER

#ifdef SPI_EXPORT
#define SPI_IMPORT __declspec(dllexport)
#else
#define SPI_IMPORT __declspec(dllimport)
#endif

#else

#define SPI_IMPORT

#endif

#endif

#endif /* _SPDOC_SPDOC_DECL_SPEC_H_ */

