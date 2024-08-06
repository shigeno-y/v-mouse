// SPDX-License-Identifier: Apache-2.0

#include "temporary_directory.hpp"

std::ofstream usdRemoteViewer::messaging::tempfile::logfile{};

usdRemoteViewer::messaging::tempfile::TemporaryDirectory::TemporaryDirectory(
    const std::string& prefix,
    const std::string& suffix,
    const std::filesystem::path& dir,
    const bool delete_at_eol)
{
    std::string tmpname_buf(L_tmpnam * 2, '\0');
    {
        std::tmpnam(tmpname_buf.data());
    }
    std::filesystem::path tempBase = std::filesystem::temp_directory_path();
    std::filesystem::path basename{ tmpname_buf.c_str() };
    if (!dir.empty() && std::filesystem::is_directory(dir))
    {
        tempBase = dir;
    }

    this->tempdir_       = tempBase / (prefix + basename.filename().generic_string() + suffix);
    this->delete_at_eol_ = std::filesystem::create_directories(this->tempdir_) && delete_at_eol;
    std::filesystem::permissions(this->tempdir_, std::filesystem::perms::owner_all);
}

usdRemoteViewer::messaging::tempfile::TemporaryDirectory::~TemporaryDirectory()
{
    if (this->delete_at_eol_)
    {
        std::filesystem::remove_all(this->tempdir_);
    }
}

std::filesystem::path
usdRemoteViewer::messaging::tempfile::TemporaryDirectory::name() const
{
    return this->tempdir_;
}

std::filesystem::path
usdRemoteViewer::messaging::tempfile::TemporaryDirectory::tmpfile()
{
    std::string tmpname_buf(L_tmpnam * 2, '\0');
    {
        std::tmpnam(tmpname_buf.data());
    }
    std::filesystem::path basename{ tmpname_buf.c_str() };
    {
        std::ostringstream oss;
        oss << std::setw(3) << std::setfill('0') << std::to_string(this->counter_);
        basename.replace_filename(oss.str() + "_" + basename.filename().generic_string());
    }
    return std::filesystem::path{ this->tempdir_ / basename.filename() };
}
