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
#ifndef SPI_FILE_UTIL_HPP
#define SPI_FILE_UTIL_HPP

/*
***************************************************************************
* FileUtil.hpp
***************************************************************************
* Some file manipulation functions - starting off with file names and
* directory names.
***************************************************************************
*/

#include "DeclSpec.h"
#include "Namespace.hpp"

#include <string>
#include <vector>
#include <set>
#include <deque>

SPI_UTIL_NAMESPACE

namespace path
{
    std::string getcwd();

    SPI_UTIL_IMPORT
    std::string dirname(const std::string& path);

    SPI_UTIL_IMPORT
    std::string basename(const std::string& path);
    
    SPI_UTIL_IMPORT
    std::string join(const char* path, ...);

    SPI_UTIL_IMPORT
    std::string posix(const std::string& path);
    std::string normpath(const std::string& path);

    /**
     * Returns the absolute path of a file. Essentially this gives the
     * same answer as joining the current working directory with the path
     * and using normpath on the result.
     */
    SPI_UTIL_IMPORT
    std::string abspath(const std::string& path);

    /**
     * Assuming we are currently in directory dn, this function returns
     * the relative path to get to the path described by path.
     */
    std::string relativePath(const std::string& path, const std::string& dn);

    /**
     * Returns true if the path represents a directory
     */
    SPI_UTIL_IMPORT
    bool isdir(const std::string& path);

    /**
     * Returns true if the path represents a file
     */
    SPI_UTIL_IMPORT
    bool isfile(const std::string& path);
} // end of namespace path

/**
 * Returns the last update time for a file.
 *
 * The integer component matches the date serial number used within DateUtil.hpp
 *
 * Returns 0 if the file does not exist.
 */
SPI_UTIL_IMPORT
double FileLastUpdateTime(const std::string& fn);

/**
 * Directory contains the contents of a directory.
 * It will exclude . and .. and separate entries into files and directories.
 */
struct SPI_UTIL_IMPORT Directory
{
    std::string dn;
    std::set<std::string> fns;
    std::set<std::string> dns;

    Directory();
    Directory(const std::string& dn);
};

/**
 * DirectoryWalker is to be used as follows:
 *
 * std::string dn; // initialise to the directory of interest
 * DirectoryWalker walker(dn);
 * Directory* d = walker.Next();
 * while (d)
 * {
 *     // process directory using d->dirName, d->fileNames, d->dirNames
 *     // dirNames is a set and you can remove directories from it in
 *     // order to restrict the further directories from the search
 *
 *     d = walker.Next();
 * }
 *
 * alternatively:
 *
 * for (Directory* d = walker.Next(); d; d = walker.Next())
 * {
 *    // process directory d as above
 * }
 *
 */
class DirectoryWalker
{
public:
    // initialises the directory walker
    DirectoryWalker(
        const std::string& dirName,
        const std::set<std::string>& excludeDirNames=std::set<std::string>(),
        bool depthFirst=false);

    /** Get details about the next directory. Returns NULL if there are no
        more directories.

        You can remove items from dirNames and as a result the directory
        walker will not go into those directories.
    */
    Directory* Next();

private:

    // this is the directory returned by Next()
    Directory m_current;

    std::string             m_dirName;
    std::set<std::string>   m_excludeDirNames;
    std::deque<std::string> m_unvisited;
    bool m_depthFirst;
    bool m_started;
};

SPI_UTIL_END_NAMESPACE

#endif
