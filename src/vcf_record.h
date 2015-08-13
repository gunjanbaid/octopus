//
//  vcf_record.h
//  Octopus
//
//  Created by Daniel Cooke on 28/07/2015.
//  Copyright (c) 2015 Oxford University. All rights reserved.
//

#ifndef __Octopus__vcf_record__
#define __Octopus__vcf_record__

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <ostream>
#include <utility> // std::forward

class VcfRecord
{
public:
    class Builder;
    
    using SizeType     = std::uint_fast32_t;
    using SequenceType = std::string;
    using QualityType  = std::uint_fast8_t;
    using SampleIdType = std::string;
    using KeyType      = std::string;
    
    VcfRecord()  = default;
    
    // constructor without genotype fields
    template <typename StringType1_, typename StringType2_, typename SequenceType1_, typename SequenceType2_,
              typename Filters_, typename Info_>
    VcfRecord(StringType1_&& chrom, SizeType pos, StringType2_&& id, SequenceType1_&& ref, SequenceType2_&& alt,
              QualityType qual, Filters_&& filters, Info_&& info);
    
    // constructor with genotype fields
    template <typename StringType1_, typename StringType2_, typename SequenceType1_, typename SequenceType2_,
    typename Filters_, typename Info_, typename Format_, typename Genotypes_, typename Samples_>
    VcfRecord(StringType1_&& chrom, SizeType pos, StringType2_&& id, SequenceType1_&& ref, SequenceType2_&& alt,
              QualityType qual, Filters_&& filters, Info_&& info, Format_&& format, Genotypes_&& genotypes, Samples_&& samples);
    
    ~VcfRecord() = default;
    
    VcfRecord(const VcfRecord&)            = default;
    VcfRecord& operator=(const VcfRecord&) = default;
    VcfRecord(VcfRecord&&)                 = default;
    VcfRecord& operator=(VcfRecord&&)      = default;
    
    const std::string& get_chromosome_name() const noexcept;
    SizeType get_position() const noexcept;
    const std::string& get_id() const noexcept;
    const SequenceType& get_ref_allele() const noexcept;
    unsigned num_alt_alleles() const noexcept;
    const std::vector<SequenceType>& get_alt_alleles() const noexcept;
    QualityType get_quality() const noexcept;
    bool has_filter(const KeyType& filter) const noexcept;
    const std::vector<KeyType> get_filters() const noexcept;
    bool has_info(const KeyType& key) const noexcept;
    std::vector<KeyType> get_info_keys() const;
    const std::vector<std::string>& get_info_value(const KeyType& key) const;
    
    // sample related functions
    bool has_format(const KeyType& key) const noexcept;
    bool has_sample_data() const noexcept;
    unsigned num_samples() const noexcept;
    
    // genotype related functions
    bool has_genotype_data() const noexcept;
    unsigned sample_ploidy() const noexcept;
    bool is_sample_phased(const SampleIdType& sample) const;
    bool is_homozygous(const SampleIdType& sample) const;
    bool is_heterozygous(const SampleIdType& sample) const;
    bool is_homozygous_ref(const SampleIdType& sample) const;
    bool is_homozygous_non_ref(const SampleIdType& sample) const;
    bool has_ref_allele(const SampleIdType& sample) const;
    bool has_alt_allele(const SampleIdType& sample) const;
    
    unsigned format_cardinality(const KeyType& key) const noexcept;
    
    const std::vector<KeyType>& get_format() const noexcept;
    const std::vector<std::string>& get_sample_value(const SampleIdType& sample, const KeyType& key) const;
    
    friend std::ostream& operator<<(std::ostream& os, const VcfRecord& record);
    
private:
    using Genotype = std::pair<std::vector<SequenceType>, bool>;
    
    // mandatory fields
    std::string chromosome_;
    SizeType position_;
    std::string id_;
    SequenceType ref_allele_;
    std::vector<SequenceType> alt_alleles_;
    QualityType quality_;
    std::vector<KeyType> filters_;
    std::unordered_map<KeyType, std::vector<std::string>> info_;
    
    // optional fields
    std::vector<KeyType> format_;
    std::unordered_map<SampleIdType, Genotype> genotypes_;
    std::unordered_map<SampleIdType, std::unordered_map<KeyType, std::vector<std::string>>> samples_;
    
    std::string get_allele_number(const SequenceType& allele) const;
    
    void print_info(std::ostream& os) const;
    void print_genotype_allele_numbers(std::ostream& os, const SampleIdType& sample) const;
    void print_other_sample_data(std::ostream& os, const SampleIdType& sample) const;
    void print_sample_data(std::ostream& os) const;
};

template <typename StringType1_, typename StringType2_, typename SequenceType1_, typename SequenceType2_,
          typename Filters_, typename Info_>
VcfRecord::VcfRecord(StringType1_&& chrom, SizeType pos, StringType2_&& id, SequenceType1_&& ref,
                     SequenceType2_&& alt, QualityType qual, Filters_&& filters, Info_&& info)
:
chromosome_ {std::forward<StringType1_>(chrom)},
position_ {pos},
id_ {std::forward<StringType2_>(id)},
ref_allele_ {std::forward<SequenceType1_>(ref)},
alt_alleles_ {std::forward<SequenceType2_>(alt)},
quality_ {qual},
filters_ {std::forward<Filters_>(filters)},
info_ {std::forward<Info_>(info)},
format_ {},
genotypes_ {},
samples_ {}
{}

template <typename StringType1_, typename StringType2_, typename SequenceType1_, typename SequenceType2_,
          typename Filters_, typename Info_, typename Format_, typename Genotypes_, typename Samples_>
VcfRecord::VcfRecord(StringType1_&& chrom, SizeType pos, StringType2_&& id, SequenceType1_&& ref, SequenceType2_&& alt,
                     QualityType qual, Filters_&& filters, Info_&& info, Format_&& format, Genotypes_&& genotypes,
                     Samples_&& samples)
:
chromosome_ {std::forward<StringType1_>(chrom)},
position_ {pos},
id_ {std::forward<StringType2_>(id)},
ref_allele_ {std::forward<SequenceType1_>(ref)},
alt_alleles_ {std::forward<SequenceType2_>(alt)},
quality_ {qual},
filters_ {std::forward<Filters_>(filters)},
info_ {std::forward<Info_>(info)},
format_ {std::forward<Format_>(format)},
genotypes_ {std::forward<Genotypes_>(genotypes)},
samples_ {std::forward<Samples_>(samples)}
{}

// non-member functions

VcfRecord::SequenceType get_ancestral_allele(const VcfRecord& record);
std::vector<unsigned> get_allele_count(const VcfRecord& record);
std::vector<double> get_allele_frequency(const VcfRecord& record);
bool is_dbsnp_member(const VcfRecord& record) noexcept;
bool is_hapmap2_member(const VcfRecord& record) noexcept;
bool is_hapmap3_member(const VcfRecord& record) noexcept;
bool is_1000g_member(const VcfRecord& record) noexcept;
bool is_somatic(const VcfRecord& record) noexcept;
bool is_validated(const VcfRecord& record) noexcept;

std::ostream& operator<<(std::ostream& os, const VcfRecord& record);

class VcfRecord::Builder
{
public:
    using SizeType     = VcfRecord::SizeType;
    using SequenceType = VcfRecord::SequenceType;
    using QualityType  = VcfRecord::QualityType;
    using SampleIdType = VcfRecord::SampleIdType;
    using KeyType      = VcfRecord::KeyType;
    
    Builder() = default;
    
    Builder& set_chromosome(const std::string& chromosome);
    Builder& set_position(SizeType position);
    Builder& set_id(const std::string& id);
    
private:
    std::string chromosome_ = ".";
    SizeType position_ = 0;
    std::string id_ = ".";
    SequenceType ref_allele_ = ".";
    std::vector<SequenceType> alt_alleles_;
    QualityType quality_ = 0;
    std::vector<KeyType> filters_;
    std::unordered_map<KeyType, std::vector<std::string>> info_;
    
    // optional fields
    std::vector<KeyType> format_;
    std::unordered_map<SampleIdType, Genotype> genotypes_;
    std::unordered_map<SampleIdType, std::unordered_map<KeyType, std::vector<std::string>>> samples_;
};

#endif /* defined(__Octopus__vcf_record__) */
