// Copyright (c) 2016 Daniel Cooke
// Use of this source code is governed by the MIT license that can be found in the LICENSE file.

#ifndef __Octopus__downloader__
#define __Octopus__downloader__

#include <vector>
#include <functional>
#include <memory>

#include "variant_generator.hpp"
#include <core/types/variant.hpp>

namespace octopus {

class GenomicRegion;
class ReferenceGenome;
class AlignedRead;

namespace coretools {

class Downloader : public VariantGenerator
{
public:
    Downloader() = delete;
    
    Downloader(const ReferenceGenome& reference, Variant::RegionType::Size max_variant_size = 100);
    
    Downloader(const Downloader&)            = default;
    Downloader& operator=(const Downloader&) = default;
    Downloader(Downloader&&)                 = default;
    Downloader& operator=(Downloader&&)      = default;
    
    ~Downloader() override = default;
    
private:
    std::unique_ptr<VariantGenerator> do_clone() const override;
    
    std::vector<Variant> do_generate_variants(const GenomicRegion& region) override;
    
    std::string name() const override;
    
    std::reference_wrapper<const ReferenceGenome> reference_;
    Variant::RegionType::Size max_variant_size_;
};

} // namespace coretools
} // namespace octopus

#endif /* defined(__Octopus__downloader__) */
