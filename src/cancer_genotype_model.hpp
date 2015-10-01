//
//  cancer_genotype_model.hpp
//  Octopus
//
//  Created by Daniel Cooke on 26/08/2015.
//  Copyright (c) 2015 Oxford University. All rights reserved.
//

#ifndef __Octopus__cancer_genotype_model__
#define __Octopus__cancer_genotype_model__

#include <string>
#include <vector>
#include <unordered_map>

#include "genotype_model.hpp"
#include "mappable_map.hpp"
#include "cancer_genotype.h"

class AlignedRead;
class Haplotype;

namespace Octopus
{
    namespace GenotypeModel
    {
    class Cancer
    {
    public:
        using GenotypeProbabilities          = std::unordered_map<CancerGenotype<Haplotype>, double>;
        using GenotypeWeightResponsibilities = std::unordered_map<SampleIdType, std::vector<std::array<double, 3>>>;
        using SampleGenotypeWeightsPriors    = std::array<double, 3>;
        using SampleGenotypeWeights          = std::array<double, 3>;
        using GenotypeWeightsPriors          = std::unordered_map<SampleIdType, SampleGenotypeWeightsPriors>;
        using GenotypeWeights                = std::unordered_map<SampleIdType, SampleGenotypeWeights>;
        
        struct Latents
        {
            GenotypeProbabilities genotype_posteriors;
            GenotypeWeights genotype_weights;
        };
        
        Cancer(SampleIdType normal_sample, unsigned max_em_iterations = 100, double em_epsilon = 0.001);
        
        Latents evaluate(const std::vector<Haplotype>& haplotypes, const ReadMap& reads);
        
    private:
        unsigned max_em_iterations_;
        double em_epsilon_;
        
        SampleIdType normal_sample_;
    };
    
    } // namespace GenotypeModel
} // namespace Octopus

#endif /* defined(__Octopus__cancer_genotype_model__) */
