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
#ifndef SPI_SERVICE_HPP
#define SPI_SERVICE_HPP

#include "Object.hpp"

#include <fstream>
#include <vector>
#include <list>
#include <set>

SPI_BEGIN_NAMESPACE

SPI_DECLARE_RC_CLASS(CommonRuntime);
SPI_DECLARE_RC_CLASS(Service);
SPI_DECLARE_RC_CLASS(Function);
SPI_DECLARE_RC_CLASS(ObjectWriter);
SPI_DECLARE_RC_CLASS(ObjectTextStreamer);
SPI_DECLARE_RC_CLASS(ObjectRefCache);

class Value;
class EnumInfo;
struct FunctionCaller;
struct ServiceFunctionCaller;
class InputContext;
class ValueToObject;

// increments the log level and returns the previous log level
SPI_IMPORT
int IncrementLogLevel();

SPI_IMPORT
void DecrementLogLevel();

class SPI_IMPORT CommonRuntime : public RefCounter
{
public:
    friend class Service;

    ~CommonRuntime();

    // logging methods
    bool is_logging() const;
    void start_logging(const char* filename, const char* options);
    void stop_logging();
    void log_inputs(const FunctionConstSP& func);
    void log_output(const Value& output);
    void log_outputs(const MapConstSP& outputs);
    void log_error(const std::exception& e);
    void log_message(const std::string& msg);
    void set_last_error(const std::string& error);
    const std::string& get_last_error() const;
    const Service* get_service(const std::string& name) const;

    static CommonRuntimeSP Make();

    bool use_read_cache;
    ObjectConstSP object_from_read_cache(
        const std::string& filename,
        double timestamp);

    void object_to_read_cache(
        const ObjectConstSP& obj,
        const std::string& filename,
        double timestamp);

    void clear_read_cache();

private:
    CommonRuntime();

    void add_service(Service* svc);
    void remove_service(Service* svc);
    void add_client(const char* client);
    bool has_client(const char* client) const;

    ObjectType* get_object_type(const std::string& className) const;

    std::ofstream         m_logStream;
    ObjectTextStreamerSP  m_logger;
    std::list<bool*>      m_isLoggingLocations;
    std::list<Service*>   m_allServices;
    std::string           m_lastError;
    std::set<std::string> m_clients;

    mutable ObjectType::Registry m_typeRegistry;

    typedef std::pair<ObjectConstSP, double> ReadCacheEntry;
    typedef std::map <std::string, ReadCacheEntry> ReadCache;
    ReadCache m_readCache;

    // not implemented - prevents compiler construction
    CommonRuntime(const CommonRuntime&);
    CommonRuntime& operator=(const CommonRuntime&);
};

class SPI_IMPORT Service : public RefCounter
{
public:
    friend class CommonRuntime;

    // constructs stand-alone service (or service to be used as base for
    // other services)
    static ServiceSP Make(
        const char* name,
        const char* ns,
        bool* isLogging);
    // constructs a service using an existing service as a base
    // the namespace must be the same for all services linked in this manner
    static ServiceSP Make(
        const char* name,
        const ServiceSP& svc,
        bool* isLogging);
    // returns a singleton service which has no object types itself, but which
    // will have access to all the other object types
    static ServiceSP CommonService();

    // shut down operations applied to a service
    void start_up();
    void shut_down();
    bool is_shut_down() const;

    // calls shut_down if it has not already been called
    ~Service();

    // object registry functions
    void add_object_type(ObjectType* type);
    ObjectType* get_object_type(const char* className) const;
    ObjectType* get_object_type(const std::string& className) const;
    ObjectConstSP object_from_map(
        IObjectMap* m,
        const ObjectRefCacheSP& cache,
        const MapConstSP& metaData = MapConstSP()) const;
    ObjectConstSP object_from_stream(std::istream& istr,
        const std::string& streamName,
        bool allowBinary,
        const MapConstSP& metaData = MapConstSP()) const;
    // convenience method that call the fundamental object_from_stream method
    ObjectConstSP object_from_string(const std::string& objectString) const;
    ObjectConstSP object_from_file(const std::string& filename) const;

    ObjectConstSP object_coerce(const std::string& className,
        const Value& value,
        const InputContext* context=0);

    // function registry functions
    void add_function_caller(FunctionCaller* caller);
    FunctionCaller* get_function_caller(const char* funcName) const;
    std::vector<std::string> get_function_names() const;

    // enum registry functions
    void add_enum_info(EnumInfo* info);
    EnumInfo* get_enum_info(const std::string& enumName) const;
    std::vector<std::string> get_enums() const;

    // logging methods
    bool is_logging() const;
    //void increment_log_level();
    //void decrement_log_level();
    void start_logging(const char* filename, const char* options);
    void stop_logging();
    void log_inputs(const FunctionConstSP& func);
    void log_output(const Value& output);
    void log_outputs(const MapConstSP& outputs);
    void log_error(const std::exception& e);
    void log_message(const std::string& msg);
    void set_last_error(const std::string& error);
    const std::string& get_last_error() const;

    // functions for managing the cache of previously read objects
    void implement_read_cache(bool useReadCache);
    void clear_read_cache();

    // define client types, e.g. EXCEL, PYTHON, DOTNET etc
    void add_client(const char* client);
    bool has_client(const char* client) const;

    // this method is a bit of a cop-out from general principles
    CommonRuntimeSP common_runtime() const;

    // information methods
    const std::string& get_name() const;
    const std::string& get_namespace() const;
    bool is_common_service() const;

private:
    Service(const char* name, const char* ns, bool* isLogging);
    Service(const char* name, const ServiceSP& svc, bool* isLogging);
    Service(bool* isLogging);

    bool                   m_commonService;
    const std::string      m_name;
    const std::string      m_namespace;
    ObjectType::Registry m_typeRegistry;
    std::map<std::string, FunctionCaller*> m_functions;
    std::map<std::string, EnumInfo*> m_enums;
    int                    m_connections;

    // the idea of the next two pointers is that we want the test for logging
    // to be as fast as possible - hence there is a static bool inside
    // each DLL which indicates whether logging is on or not and a static
    // int which shows the log level (to prevent nested function calls
    // from being logged) - so this means we don't have to de-reference a
    // shared pointer (i.e the Service object) to determine whether logging
    // is on
    //
    // however when the logging speed is not so important (when for
    // example we are using replay), then we have these pointers
    // (which are initialised when the Service is constructed) available
    // so that we can set the relevant bool value when the logging status
    // changes, and maintain the log level so that we don't log nested
    // function calls
    bool*                  m_isLogging;
    //int*                   m_logLevel;

    CommonRuntimeSP        m_commonRuntime;
    void set_logging(bool logging);


    // not implemented - prevents compiler construction
    Service();
    Service(const Service&);
    Service& operator=(const Service&);

    // initialisation - called by all constructors
    void Init(bool* isLogging);
};

SPI_END_NAMESPACE

#endif
