/**
 * @file  process.hpp
 * @brief Cross-platform child process with bidirectional pipe I/O
 *
 * Thin platform abstraction: POSIX (fork/exec/pipe/select) on Linux/macOS,
 * Win32 (CreateProcess/CreatePipe/OVERLAPPED) on Windows.
 */

#ifndef _PROCESS_HPP_
#define _PROCESS_HPP_

#include <string>

#ifdef _WIN32
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#else
#  include <unistd.h>
#endif

/**
 * @brief Manages a child process and its stdin/stdout pipes
 */
class Process
{
public:
    Process();
    ~Process();

    /**
     * @brief Spawn a child process
     * @param[in] executable Full path (or name on PATH) to the executable
     * @return true if the process started successfully
     */
    bool start(std::string const& executable);

    /**
     * @brief Write a line to the child's stdin (appends '\n')
     * @param[in] line Text to send
     * @return true on success
     */
    bool writeLine(std::string const& line);

    /**
     * @brief Read one line from the child's stdout
     * @param[out] out  Received line (without trailing '\n')
     * @param[in]  timeoutMs Maximum wait in milliseconds (default 5 s)
     * @return true if a line was read before the timeout
     */
    bool readLine(std::string& out, int timeoutMs = 5000);

    /**
     * @brief Terminate the child process
     */
    void terminate();

    /**
     * @brief Returns true if the child process is currently running
     */
    bool isRunning() const;

private:
#ifdef _WIN32
    HANDLE hProcess;
    HANDLE hStdinWrite;
    HANDLE hStdoutRead;
    HANDLE hStdoutReadOverlapped; /* for OVERLAPPED read */
#else
    pid_t  pid;
    int    stdinWrite;
    int    stdoutRead;
#endif

    /* Accumulate partial lines between readLine() calls */
    std::string readBuf;

    /* Non-copyable */
    Process(Process const&)            = delete;
    Process& operator=(Process const&) = delete;
};

#endif /* _PROCESS_HPP_ */
