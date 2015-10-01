//
//  variant_assembler.cpp
//  Octopus
//
//  Created by Daniel Cooke on 24/02/2015.
//  Copyright (c) 2015 Oxford University. All rights reserved.
//

#include "variant_assembler.hpp"

#include "aligned_read.hpp"
#include "genomic_region.hpp"
#include "variant.hpp"

VariantAssembler::VariantAssembler(unsigned k)
:
the_assembler_ {k}
{}

void VariantAssembler::add_read(const AlignedRead& a_read)
{
    the_assembler_.add_sequence(a_read.get_sequence(), get_begin(a_read), Colour::Read);
}

void VariantAssembler::add_reference_sequence(const GenomicRegion& the_region,
                                              const std::string& the_sequence)
{
    the_assembler_.add_sequence(the_sequence, the_region.get_begin(), Colour::Reference);
}

std::vector<Variant> VariantAssembler::get_variants(const GenomicRegion& a_region)
{
    std::vector<Variant> result {};
    return result;
}

void VariantAssembler::clear() noexcept
{
    the_assembler_.clear();
}