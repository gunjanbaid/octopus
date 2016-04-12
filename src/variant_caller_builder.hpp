//
//  variant_caller_builder.hpp
//  Octopus
//
//  Created by Daniel Cooke on 11/01/2016.
//  Copyright © 2016 Oxford University. All rights reserved.
//

#ifndef variant_caller_builder_hpp
#define variant_caller_builder_hpp

#include <unordered_map>
#include <string>
#include <memory>
#include <functional>

#include <boost/optional.hpp>

#include "common.hpp"
#include "variant_caller.hpp"
#include "read_pipe.hpp"
#include "candidate_generator_builder.hpp"
#include "haplotype_prior_model_factory.hpp"

#include "pedigree.hpp"

namespace Octopus {
    
    class VariantCallerBuilder
    {
    public:
        VariantCallerBuilder()  = delete;
        
        explicit VariantCallerBuilder(const ReferenceGenome& reference,
                                      ReadPipe& read_pipe,
                                      const CandidateGeneratorBuilder& candidate_generator_builder);
        
        ~VariantCallerBuilder() = default;
        
        VariantCallerBuilder(const VariantCallerBuilder&);
        VariantCallerBuilder& operator=(const VariantCallerBuilder&);
        VariantCallerBuilder(VariantCallerBuilder&&);
        VariantCallerBuilder& operator=(VariantCallerBuilder&&);
        
        // common
        VariantCallerBuilder& set_reference(const ReferenceGenome& reference) noexcept;
        VariantCallerBuilder& set_read_pipe(ReadPipe& read_pipe) noexcept;
        VariantCallerBuilder& set_candidate_generator_builder(const CandidateGeneratorBuilder& candidate_generator_builder) noexcept;        
        VariantCallerBuilder& set_ploidy(unsigned ploidy) noexcept;
        VariantCallerBuilder& set_model(std::string model);
        VariantCallerBuilder& set_refcall_type(VariantCaller::RefCallType refcall_type) noexcept;
        VariantCallerBuilder& set_sites_only() noexcept;
        VariantCallerBuilder& set_min_variant_posterior(double min_posterior) noexcept;
        VariantCallerBuilder& set_min_refcall_posterior(double min_posterior) noexcept;
        VariantCallerBuilder& set_max_haplotypes(unsigned max_haplotypes) noexcept;
        
        // cancer
        VariantCallerBuilder& set_normal_sample(SampleIdType normal_sample);
        VariantCallerBuilder& set_somatic_mutation_rate(double somatic_mutation_rate);
        VariantCallerBuilder& set_min_somatic_posterior(double min_posterior) noexcept;
        VariantCallerBuilder& set_somatic_only_calls() noexcept;
        VariantCallerBuilder& set_somatic_and_variant_calls() noexcept;
        VariantCallerBuilder& set_somatic_and_variant_and_refcalls_calls() noexcept;
        
        // trio
        
        VariantCallerBuilder& set_maternal_sample(SampleIdType mother);
        VariantCallerBuilder& set_paternal_sample(SampleIdType father);
        
        // pedigree
        
        VariantCallerBuilder& set_pedigree(Pedigree pedigree);
        
        // build
        
        std::unique_ptr<VariantCaller> build() const;
        
    private:
        struct Parameters
        {
            Parameters()  = delete;
            explicit Parameters(const ReferenceGenome& reference,
                                ReadPipe& read_pipe,
                                const CandidateGeneratorBuilder& candidate_generator_builder);
            ~Parameters() = default;
            
            Parameters(const Parameters&)            = default;
            Parameters& operator=(const Parameters&) = default;
            Parameters(Parameters&&)                 = default;
            Parameters& operator=(Parameters&&)      = default;
            
            // common
            std::reference_wrapper<const ReferenceGenome> reference;
            std::reference_wrapper<ReadPipe> read_pipe;
            
            HaplotypePriorModelFactory haplotype_prior_model_factory;
            
            unsigned ploidy;
            std::string model;
            std::reference_wrapper<const CandidateGeneratorBuilder> candidate_generator_builder;
            VariantCaller::RefCallType refcall_type = VariantCaller::RefCallType::None;
            bool call_sites_only = false;
            double min_variant_posterior;
            double min_refcall_posterior;
            unsigned max_haplotypes;
            
            // cancer
            
            boost::optional<SampleIdType> normal_sample;
            double somatic_mutation_rate;
            double min_somatic_posterior;
            bool call_somatics_only;
            
            // trio
            
            boost::optional<SampleIdType> maternal_sample, paternal_sample;
            
            // pedigree
            
            boost::optional<Pedigree> pedigree;
        };
        
        using ModelFactoryMap = std::unordered_map<std::string, std::function<std::unique_ptr<VariantCaller>()>>;
        
        Parameters parameters_;
        ModelFactoryMap factory_;
        
        ModelFactoryMap generate_factory() const;
    };
} // namespace Octopus

#endif /* variant_caller_builder_hpp */
