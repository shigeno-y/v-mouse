// SPDX-License-Identifier: Apache-2.0

#ifndef USDREMOTEVIEWER_TEMPORARY_DIRECTORY_HPP_INCLUDED
#define USDREMOTEVIEWER_TEMPORARY_DIRECTORY_HPP_INCLUDED

#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

namespace usdRemoteViewer::messaging::tempfile {
extern std::ofstream logfile;

class TemporaryDirectory
{
private:
    bool delete_at_eol_;
    std::filesystem::path tempdir_;
    std::uint8_t counter_{ 0 };

public:
    TemporaryDirectory(const std::string& prefix        = {},
                       const std::string& suffix        = {},
                       const std::filesystem::path& dir = {},
                       const bool delete_at_eol         = true);
    TemporaryDirectory(const TemporaryDirectory&) = default;
    TemporaryDirectory(TemporaryDirectory&&)      = default;
    ~TemporaryDirectory();

public:
    std::filesystem::path name() const;

    std::filesystem::path tmpfile();
};

} // namespace usdRemoteViewer::messaging::tempfile

#endif