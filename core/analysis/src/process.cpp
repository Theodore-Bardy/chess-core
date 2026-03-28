/**
 * @file  process.cpp
 * @brief Cross-platform child process implementation
 *
 * POSIX path: fork + exec, two anonymous pipes (stdin, stdout).
 *             readLine() uses select() for non-blocking wait with timeout.
 *
 * Windows path: CreatePipe + CreateProcess with STARTUPINFOA.
 *               readLine() uses PeekNamedPipe + polling loop for timeout.
 */

#include "process.hpp"

#include <cstring>
#include <stdexcept>

#ifdef _WIN32

/* ================================================================== */
/* Windows implementation                                               */
/* ================================================================== */

Process::Process()
    : hProcess(INVALID_HANDLE_VALUE)
    , hStdinWrite(INVALID_HANDLE_VALUE)
    , hStdoutRead(INVALID_HANDLE_VALUE)
    , hStdoutReadOverlapped(INVALID_HANDLE_VALUE)
{}

Process::~Process()
{
    terminate();
}

bool Process::start(std::string const& executable)
{
    SECURITY_ATTRIBUTES sa;
    sa.nLength              = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle       = TRUE;
    sa.lpSecurityDescriptor = nullptr;

    HANDLE hStdinRead  = INVALID_HANDLE_VALUE;
    HANDLE hStdoutWrite = INVALID_HANDLE_VALUE;

    if (!CreatePipe(&hStdinRead, &hStdinWrite, &sa, 0)) return false;
    if (!CreatePipe(&hStdoutRead, &hStdoutWrite, &sa, 0))
    {
        CloseHandle(hStdinRead);
        CloseHandle(hStdinWrite);
        return false;
    }

    /* Child inherits the read end of stdin and the write end of stdout.
     * The parent's handles must NOT be inherited. */
    SetHandleInformation(hStdinWrite, HANDLE_FLAG_INHERIT, 0);
    SetHandleInformation(hStdoutRead, HANDLE_FLAG_INHERIT, 0);

    STARTUPINFOA si;
    ZeroMemory(&si, sizeof(si));
    si.cb          = sizeof(si);
    si.hStdInput   = hStdinRead;
    si.hStdOutput  = hStdoutWrite;
    si.hStdError   = hStdoutWrite;
    si.dwFlags    |= STARTF_USESTDHANDLES;

    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));

    std::string cmd = executable; /* CreateProcess may modify the buffer */
    BOOL ok = CreateProcessA(
        nullptr, &cmd[0], nullptr, nullptr,
        TRUE,                /* inherit handles */
        CREATE_NO_WINDOW,
        nullptr, nullptr,
        &si, &pi);

    /* Close child-side handles in the parent — we only keep parent-side */
    CloseHandle(hStdinRead);
    CloseHandle(hStdoutWrite);

    if (!ok)
    {
        CloseHandle(hStdinWrite);
        CloseHandle(hStdoutRead);
        hStdinWrite = INVALID_HANDLE_VALUE;
        hStdoutRead = INVALID_HANDLE_VALUE;
        return false;
    }

    hProcess = pi.hProcess;
    CloseHandle(pi.hThread);
    return true;
}

bool Process::writeLine(std::string const& line)
{
    if (hStdinWrite == INVALID_HANDLE_VALUE) return false;
    std::string msg = line + '\n';
    DWORD written = 0;
    return WriteFile(hStdinWrite, msg.c_str(),
                     static_cast<DWORD>(msg.size()), &written, nullptr) != 0;
}

bool Process::readLine(std::string& out, int timeoutMs)
{
    /* Check if we already have a complete line in the buffer */
    auto newline = readBuf.find('\n');
    if (newline != std::string::npos)
    {
        out = readBuf.substr(0, newline);
        readBuf.erase(0, newline + 1);
        return true;
    }

    if (hStdoutRead == INVALID_HANDLE_VALUE) return false;

    DWORD deadline = GetTickCount() + static_cast<DWORD>(timeoutMs);
    char  buf[256];

    while (GetTickCount() < deadline)
    {
        DWORD available = 0;
        if (!PeekNamedPipe(hStdoutRead, nullptr, 0, nullptr, &available, nullptr))
            return false;

        if (available == 0)
        {
            Sleep(5);
            continue;
        }

        DWORD toRead = min(available, static_cast<DWORD>(sizeof(buf)));
        DWORD read   = 0;
        if (!ReadFile(hStdoutRead, buf, toRead, &read, nullptr) || read == 0)
            return false;

        readBuf.append(buf, read);
        newline = readBuf.find('\n');
        if (newline != std::string::npos)
        {
            out = readBuf.substr(0, newline);
            if (!out.empty() && out.back() == '\r') out.pop_back();
            readBuf.erase(0, newline + 1);
            return true;
        }
    }
    return false; /* timeout */
}

void Process::terminate()
{
    if (hProcess != INVALID_HANDLE_VALUE)
    {
        TerminateProcess(hProcess, 0);
        CloseHandle(hProcess);
        hProcess = INVALID_HANDLE_VALUE;
    }
    if (hStdinWrite  != INVALID_HANDLE_VALUE) { CloseHandle(hStdinWrite);  hStdinWrite  = INVALID_HANDLE_VALUE; }
    if (hStdoutRead  != INVALID_HANDLE_VALUE) { CloseHandle(hStdoutRead);  hStdoutRead  = INVALID_HANDLE_VALUE; }
}

bool Process::isRunning() const
{
    if (hProcess == INVALID_HANDLE_VALUE) return false;
    DWORD exitCode = 0;
    return GetExitCodeProcess(hProcess, &exitCode) && exitCode == STILL_ACTIVE;
}

#else /* POSIX */

/* ================================================================== */
/* POSIX implementation                                                 */
/* ================================================================== */

#include <cerrno>
#include <csignal>
#include <cstdio>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/wait.h>

Process::Process()
    : pid(-1)
    , stdinWrite(-1)
    , stdoutRead(-1)
{}

Process::~Process()
{
    terminate();
}

bool Process::start(std::string const& executable)
{
    int stdinPipe[2];  /* [0]=read, [1]=write */
    int stdoutPipe[2];

    if (pipe(stdinPipe)  < 0) return false;
    if (pipe(stdoutPipe) < 0)
    {
        close(stdinPipe[0]); close(stdinPipe[1]);
        return false;
    }

    pid = fork();
    if (pid < 0)
    {
        close(stdinPipe[0]);  close(stdinPipe[1]);
        close(stdoutPipe[0]); close(stdoutPipe[1]);
        return false;
    }

    if (pid == 0)
    {
        /* Child process */
        dup2(stdinPipe[0],  STDIN_FILENO);
        dup2(stdoutPipe[1], STDOUT_FILENO);
        dup2(stdoutPipe[1], STDERR_FILENO);

        close(stdinPipe[0]);  close(stdinPipe[1]);
        close(stdoutPipe[0]); close(stdoutPipe[1]);

        execlp(executable.c_str(), executable.c_str(), nullptr);
        _exit(1); /* exec failed */
    }

    /* Parent process — close child-side pipe ends */
    close(stdinPipe[0]);
    close(stdoutPipe[1]);

    stdinWrite = stdinPipe[1];
    stdoutRead = stdoutPipe[0];
    return true;
}

bool Process::writeLine(std::string const& line)
{
    if (stdinWrite < 0) return false;
    std::string msg = line + '\n';
    ssize_t written = write(stdinWrite, msg.c_str(), msg.size());
    return written == static_cast<ssize_t>(msg.size());
}

bool Process::readLine(std::string& out, int timeoutMs)
{
    /* Return a buffered complete line immediately if available */
    auto newline = readBuf.find('\n');
    if (newline != std::string::npos)
    {
        out = readBuf.substr(0, newline);
        readBuf.erase(0, newline + 1);
        return true;
    }

    if (stdoutRead < 0) return false;

    struct timeval tv;
    tv.tv_sec  = timeoutMs / 1000;
    tv.tv_usec = (timeoutMs % 1000) * 1000;

    char buf[256];

    while (true)
    {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(stdoutRead, &fds);

        int ret = select(stdoutRead + 1, &fds, nullptr, nullptr, &tv);
        if (ret <= 0) return false; /* timeout or error */

        ssize_t n = read(stdoutRead, buf, sizeof(buf));
        if (n <= 0) return false;

        readBuf.append(buf, static_cast<size_t>(n));

        newline = readBuf.find('\n');
        if (newline != std::string::npos)
        {
            out = readBuf.substr(0, newline);
            readBuf.erase(0, newline + 1);
            return true;
        }
    }
}

void Process::terminate()
{
    if (stdinWrite  >= 0) { close(stdinWrite);  stdinWrite  = -1; }
    if (stdoutRead  >= 0) { close(stdoutRead);  stdoutRead  = -1; }
    if (pid > 0)
    {
        kill(pid, SIGTERM);
        int status = 0;
        waitpid(pid, &status, 0);
        pid = -1;
    }
}

bool Process::isRunning() const
{
    if (pid <= 0) return false;
    return waitpid(pid, nullptr, WNOHANG) == 0;
}

#endif /* _WIN32 / POSIX */
