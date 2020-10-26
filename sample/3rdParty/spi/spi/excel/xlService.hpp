/*

Copyright (C) 2012-2019 Sartorial Programming Ltd.

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/
#ifndef SPI_XL_SERVICE_HPP
#define SPI_XL_SERVICE_HPP

/*
***************************************************************************
** xlService.hpp
**
** Defines the Excel service class. This is linked to the Service class
** for the underlying C++ outer library.
***************************************************************************
*/

#include "DeclSpec.h"
#include <spi/Service.hpp>
#include <spi/spdoc_configTypes.hpp>
#include <spi_util/ClockUtil.hpp>
#include <vector>
#include <string>

typedef struct xloper XLOPER;

#include "xlValue.hpp"

SPI_BEGIN_NAMESPACE

SPI_XL_IMPORT
std::string GetXLLName();

SPI_XL_IMPORT
void StartupError(const std::string& xllName, const char* errmsg);

struct ExcelTimings
{
    ExcelTimings();

    int numCalls;
    int numFailures;
    double totalTime;
};

class SPI_XL_IMPORT ExcelService
{
public:
    enum CompatibilityMode
    {
        SPI_COMPATIBILITY,
        ALIB_COMPATIBILITY
    };

    ExcelService(const ServiceSP& service, const std::string& xllName,
        CompatibilityMode = SPI_COMPATIBILITY);

    /**
     * Registers the standard functions that are provided for all libraries.
     */
    void RegisterStandardFunctions(const std::string& xllName);

    /**
     * Register a function with Excel and keep track of its name so that
     * we can unregister the function later.
     *
     * @param pxDll
     *    XLOPER containing some sort of handle to the XLL.
     * @param cFuncName
     *    The name of the c-function that Excel will call when the user
     *    requests xlFuncName.
     * @param xlFuncName
     *    The name of the function that will appear in Excel.
     * @param xlCatName
     *    The Excel category name where we will register all the functions.
     * @param args
     *    Vector of argument names.
     * @param funcHelp
     *    Function help for the function as a whole.
     * @param argsHelp
     *    Short help for each parameter.
     */
    void RegisterFunction(
        const std::string&              xllName,
        const std::string&              cFuncName,
        const std::string&              xlFuncName,
        const std::string&              xlCatName,
        const std::vector<std::string>& args,
        const std::string&              funcHelp,
        const std::vector<std::string>& argsHelp,
        bool hiddenFromWizard = false,
        bool volatileInExcel = false);

    /**
     * Goes through the list of registered functions and unregisters
     * them all.
     */
    void UnregisterAllFunctions();

    /**
     * Make an XLOPER error object. If we have error popups, this will also
     * show up in a pop-up window.
     */
    XLOPER* ErrorHandler(const char* err);

    /**
     * These methods need the service. You should create functions
     * that can call these methods using get_service() to get the
     * service object.
     */
    XLOPER* StartLogging(XLOPER* filename, XLOPER* options);
    XLOPER* StopLogging(void);
    XLOPER* IsLogging(void);
    XLOPER* SetErrorPopups(XLOPER* errorPopups);
    XLOPER* HelpFunc(XLOPER* name);
    XLOPER* HelpEnum(XLOPER* name);
    XLOPER* StartTiming();
    XLOPER* StopTiming();
    XLOPER* ClearTimings();
    XLOPER* GetTimings();

    XLOPER* ObjectToString(XLOPER* handle, XLOPER* format, XLOPER* options, XLOPER* hMetaData);
    XLOPER* ObjectFromString(XLOPER* baseName, XLOPER* strings);

    XLOPER* ObjectToFile(XLOPER* handle, XLOPER* fileName, XLOPER* format,
        XLOPER* options, XLOPER* hMetaData);
    XLOPER* ObjectFromFile(XLOPER* baseName, XLOPER* fileName);
    XLOPER* ObjectFromURL(XLOPER* baseName, XLOPER* url, XLOPER* timeout,
        XLOPER* names, 
        XLOPER* v1, XLOPER* v2, XLOPER* v3, XLOPER* v4, XLOPER* v5,
        XLOPER* v6, XLOPER* v7, XLOPER* v8, XLOPER* v9, XLOPER* v10,
        XLOPER* v11, XLOPER* v12, XLOPER* v13, XLOPER* v14, XLOPER* v15,
        XLOPER* v16, XLOPER* v17, XLOPER* v18, XLOPER* v19, XLOPER* v20);

    XLOPER* ObjectGet(XLOPER* handle, XLOPER* name);
    XLOPER* ObjectPut(XLOPER* baseName, XLOPER* handle, XLOPER* names,
        XLOPER* v1, XLOPER* v2, XLOPER* v3, XLOPER* v4, XLOPER* v5,
        XLOPER* v6, XLOPER* v7, XLOPER* v8, XLOPER* v9, XLOPER* v10,
        XLOPER* v11, XLOPER* v12, XLOPER* v13, XLOPER* v14, XLOPER* v15,
        XLOPER* v16, XLOPER* v17, XLOPER* v18, XLOPER* v19, XLOPER* v20,
        XLOPER* v21, XLOPER* v22, XLOPER* v23, XLOPER* v24, XLOPER* v25);
    XLOPER* ObjectToMap(XLOPER* baseName, XLOPER* handle);

    XLOPER* ObjectCount(XLOPER* className);
    XLOPER* ObjectFree(XLOPER* handle);
    XLOPER* ObjectFreeAll();
    XLOPER* ObjectList(XLOPER* prefix, XLOPER* className);
    XLOPER* ObjectClassName(XLOPER* handle);
    XLOPER* ObjectCoerce(XLOPER* baseName, XLOPER* className, XLOPER* value);

    XLOPER* UrlCacheSize();
    XLOPER* UrlCacheSave(XLOPER* filename);
    XLOPER* UrlCacheLoad(XLOPER* filename);
    XLOPER* UrlCacheInit();
    XLOPER* UrlCacheClear();

    FunctionCaller* getFunctionCaller(const char* name);
    InputContext* getInputContext();
    spdoc::ServiceConstSP getDoc();
    bool isLogging() const;
    bool isTiming() const;
    void addTiming(const std::string& name, bool failed, double time);
    bool mandatoryBaseName() const;
    void logMessage(const std::string& msg);
    const std::string& getNamespace() const;

private:
    ServiceSP m_service;
    std::vector<std::string> m_registeredFunctions;
    bool m_errorPopups;
    std::string m_dirname;
    spdoc::ServiceConstSP m_doc;
    InputContext* m_inputContext;
    CompatibilityMode m_compatibilityMode;
    bool m_timing;
    std::map<std::string, ExcelTimings> m_timings;
};

class SPI_XL_IMPORT ExcelTimer
{
public:
    ExcelTimer(ExcelService* svc, const char* name);
    ~ExcelTimer();

    void SetNotCalled();
    void SetFailure();
    const char* Name();

private:
    ExcelService* m_svc;
    const char* m_name;
    bool m_failed;
    bool m_notCalled;
    spi_util::Clock m_clock;
};

SPI_END_NAMESPACE

#endif /* SPI_XL_SERVICE_HPP */
