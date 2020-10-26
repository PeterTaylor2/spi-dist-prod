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
#ifndef SPI_REPLAY_HPP
#define SPI_REPLAY_HPP

/*
***************************************************************************
* Replay.hpp
***************************************************************************
* Functions and classes for reading a replay log and playing it back.
***************************************************************************
*/

#include "RefCounter.hpp"

#include <string>
#include <vector>

SPI_BEGIN_NAMESPACE

SPI_DECLARE_RC_CLASS(Service);
SPI_DECLARE_RC_CLASS(Map);
SPI_DECLARE_RC_CLASS(ReplayLog);
SPI_DECLARE_RC_CLASS(ReplayAction);
SPI_DECLARE_RC_CLASS(ObjectRefCache);
SPI_DECLARE_RC_CLASS(ReplayFunctionAction);
SPI_DECLARE_RC_CLASS(ReplayObjectAction);

/**
 * Main function which runs the replay.
 *
 * You must start any services necessary for reading the log file before
 * starting the replay.
 */
SPI_IMPORT
void mainReplay(int argc, char* argv[]);

class ReplayCodeGenerator : public RefCounter
{
public:
    virtual ~ReplayCodeGenerator() {}
    virtual void GenerateFunction(const ReplayFunctionAction* action) = 0;
    virtual void GenerateObject(const ReplayObjectAction* action) = 0;
};

/**
 * The base class for replay actions.
 *
 * After we read the replay log we convert it to an ordered list of actions.
 * This can be done without reference to the underlying service. However the
 * service is needed to execute each action.
 */
class SPI_IMPORT ReplayAction : public RefCounter
{
public:
    virtual void execute(
        const ServiceSP& svc,
        const ObjectRefCacheSP& cache) = 0;

    virtual void generateCode(ReplayCodeGenerator* generator) const = 0;
    virtual std::string description() const = 0;
};

class SPI_IMPORT ReplayFunctionAction : public ReplayAction
{
public:
    ReplayFunctionAction(const MapConstSP& inputs, const MapConstSP& output);
    void execute(
        const ServiceSP& svc,
        const ObjectRefCacheSP& cache);
    void generateCode(ReplayCodeGenerator* generator) const;
    std::string description() const;

    const MapConstSP& Inputs() const;
    const MapConstSP& Output() const;
private:
    MapConstSP m_inputs;
    MapConstSP m_output;
};

class SPI_IMPORT ReplayObjectAction : public ReplayAction
{
public:
    ReplayObjectAction(const MapConstSP& inputs);
    void execute(
        const ServiceSP& svc,
        const ObjectRefCacheSP& cache);
    void generateCode(ReplayCodeGenerator* generator) const;
    std::string description() const;

    const MapConstSP& Inputs() const;
private:
    MapConstSP  m_inputs;
};

/**
 * Defines a replay log - a log of all calls made in a session.
 */
class SPI_IMPORT ReplayLog : public RefCounter
{
public:
    ReplayLog(const std::vector<MapConstSP>& items);
    ~ReplayLog();

    /** Executes the replay log. This will essentially repeat all the functions
        that were logged in the ReplayLog.

        If you provide the logfilename, then logging will be started in
        which case the output in the logfilename should match the output
        in the original file if you want to use this for regression testing.

        @param svc
            The service object which knows how to convert maps to objects.
        @param logfilename
            The name of a logfile - if this is not empty then a log will be
            written to this file. This should be the match the original file.
    */
    void execute(const ServiceSP& svc, const std::string& logfilename, bool verbose = false);

    void generateCode(ReplayCodeGenerator* generator);

    /** reads the replay log from a filename */
    static ReplayLogSP Read(const std::string& infilename);

    /** reads the replay log from an input stream and name */
    static ReplayLogSP Read(const char* streamName, std::istream& istr);

    /** returns all the maps in the replay log */
    const std::vector<MapConstSP>& items() const;

    /** returns all the actions in the replay log */
    const std::vector<ReplayActionSP>& actions();

private:
    ReplayLog();

    void findActions();

    std::vector<MapConstSP>     m_items;
    std::vector<ReplayActionSP> m_actions;

    // prevent copying */
    ReplayLog(const ReplayLog&);
    ReplayLog& operator=(const ReplayLog&);
};

SPI_END_NAMESPACE

#endif
