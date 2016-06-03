//
//  vcf_writer.cpp
//  Octopus
//
//  Created by Daniel Cooke on 29/07/2015.
//  Copyright (c) 2015 Oxford University. All rights reserved.
//

#include "vcf_writer.hpp"

#include <stdexcept>
#include <utility>

#include "vcf_header.hpp"
#include "vcf_record.hpp"

#include <iostream> // TEST

VcfWriter::VcfWriter(Path file_path)
:
file_path_ {std::move(file_path)},
writer_ {std::make_unique<HtslibBcfFacade>(file_path_, "w")}
{}

VcfWriter::VcfWriter(Path file_path, const VcfHeader& header)
:
VcfWriter {std::move(file_path)}
{
    this->write(std::move(header));
}

VcfWriter::VcfWriter(VcfWriter&& other)
{
    std::lock_guard<std::mutex> lock {other.mutex_};
    file_path_         = std::move(other.file_path_);
    is_header_written_ = other.is_header_written_;
    writer_            = std::move(other.writer_);
}

void swap(VcfWriter& lhs, VcfWriter& rhs) noexcept
{
    using std::swap;
    if (&lhs == &rhs) return;
    std::lock(lhs.mutex_, rhs.mutex_);
    std::lock_guard<std::mutex> lock_lhs {lhs.mutex_, std::adopt_lock}, lock_rhs {rhs.mutex_, std::adopt_lock};
    swap(lhs.file_path_, rhs.file_path_);
    swap(lhs.is_header_written_, rhs.is_header_written_);
    swap(lhs.writer_, rhs.writer_);
}

bool VcfWriter::is_open() const noexcept
{
    std::lock_guard<std::mutex> lock {mutex_};
    return writer_ != nullptr;
}

void VcfWriter::open(Path file_path) noexcept
{
    std::lock_guard<std::mutex> lock {mutex_};
    try {
        file_path_         = std::move(file_path);
        writer_            = std::make_unique<HtslibBcfFacade>(file_path_, "w");
        is_header_written_ = writer_->is_header_written();
    } catch (...) {
        this->close();
    }
}

void VcfWriter::close() noexcept
{
    std::lock_guard<std::mutex> lock {mutex_};
    writer_.reset(nullptr);
}

bool VcfWriter::is_header_written() const noexcept
{
    std::lock_guard<std::mutex> lock {mutex_};
    return is_header_written_;
}

const VcfWriter::Path& VcfWriter::path() const noexcept
{
    std::lock_guard<std::mutex> lock {mutex_};
    return file_path_;
}

void VcfWriter::write(const VcfHeader& header)
{
    std::lock_guard<std::mutex> lock {mutex_};
    writer_->write(header);
    is_header_written_ = true;
}

void VcfWriter::write(const VcfRecord& record)
{
    std::lock_guard<std::mutex> lock {mutex_};
    if (is_header_written_) {
        writer_->write(record);
    } else {
        throw std::runtime_error {"VcfWriter::write: cannot write record as header has not been written"};
    }
}

// non member methods

VcfWriter& operator<<(VcfWriter& dst, const VcfHeader& header)
{
    dst.write(header);
    return dst;
}

VcfWriter& operator<<(VcfWriter& dst, const VcfRecord& record)
{
    dst.write(record);
    return dst;
}

bool operator==(const VcfWriter& lhs, const VcfWriter& rhs)
{
    return lhs.path() == rhs.path();
}
