/*
	This file is part of solidity.

	solidity is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	solidity is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with solidity.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <test/yulPhaser/TestHelpers.h>

#include <tools/yulPhaser/Mutations.h>

#include <tools/yulPhaser/SimulationRNG.h>

#include <boost/test/unit_test.hpp>

#include <algorithm>
#include <vector>

using namespace std;

namespace solidity::phaser::test
{

BOOST_AUTO_TEST_SUITE(Phaser)
BOOST_AUTO_TEST_SUITE(MutationsTest)
BOOST_AUTO_TEST_SUITE(GeneRandomisationTest)

BOOST_AUTO_TEST_CASE(geneRandomisation_should_iterate_over_genes_and_replace_them_with_random_ones_with_given_probability)
{
	Chromosome chromosome("fcCUnDvejs");
	function<Mutation> mutation01 = geneRandomisation(0.1);
	function<Mutation> mutation05 = geneRandomisation(0.5);
	function<Mutation> mutation10 = geneRandomisation(1.0);

	SimulationRNG::reset(1);
	BOOST_TEST(countDifferences(mutation01(chromosome), chromosome), 2);
	BOOST_TEST(countDifferences(mutation05(chromosome), chromosome), 5);
	BOOST_TEST(countDifferences(mutation10(chromosome), chromosome), 7);
	SimulationRNG::reset(2);
	BOOST_TEST(countDifferences(mutation01(chromosome), chromosome), 1);
	BOOST_TEST(countDifferences(mutation05(chromosome), chromosome), 3);
	BOOST_TEST(countDifferences(mutation10(chromosome), chromosome), 9);
}

BOOST_AUTO_TEST_CASE(geneRandomisation_should_return_identical_chromosome_if_probability_is_zero)
{
	Chromosome chromosome("fcCUnDvejsrmV");
	function<Mutation> mutation = geneRandomisation(0.0);

	BOOST_TEST(mutation(chromosome) == chromosome);
}

BOOST_AUTO_TEST_CASE(geneDeletion_should_iterate_over_genes_and_delete_them_with_given_probability)
{
	Chromosome chromosome("fcCUnDvejs");
	function<Mutation> mutation01 = geneDeletion(0.1);
	function<Mutation> mutation05 = geneDeletion(0.5);

	SimulationRNG::reset(1);
	//                                                               fcCUnDvejs
	BOOST_TEST(mutation01(chromosome) == Chromosome(stripWhitespace("fcCU Dvejs")));
	BOOST_TEST(mutation05(chromosome) == Chromosome(stripWhitespace("     D ejs")));
	SimulationRNG::reset(2);
	BOOST_TEST(mutation01(chromosome) == Chromosome(stripWhitespace("fcUnDvejs")));
	BOOST_TEST(mutation05(chromosome) == Chromosome(stripWhitespace("  Un    s")));
}

BOOST_AUTO_TEST_CASE(geneDeletion_should_return_identical_chromosome_if_probability_is_zero)
{
	Chromosome chromosome("fcCUnDvejsrmV");
	function<Mutation> mutation = geneDeletion(0.0);

	BOOST_TEST(mutation(chromosome) == chromosome);
}

BOOST_AUTO_TEST_CASE(geneDeletion_should_delete_all_genes_if_probability_is_one)
{
	Chromosome chromosome("fcCUnDvejsrmV");
	function<Mutation> mutation = geneDeletion(1.0);

	BOOST_TEST(mutation(chromosome) == Chromosome(""));
}

BOOST_AUTO_TEST_CASE(geneAddition_should_iterate_over_gene_positions_and_insert_new_genes_with_given_probability)
{
	Chromosome chromosome("fcCUnDvejs");
	function<Mutation> mutation01 = geneAddition(0.1);
	function<Mutation> mutation05 = geneAddition(0.5);

	SimulationRNG::reset(1);
	//                                                                 f  c  C  U  n  D  v  e  j  s
	BOOST_TEST(mutation01(chromosome) == Chromosome(stripWhitespace("  f  c  C  UC n  D  v  e  jx s")));  //  20% more
	BOOST_TEST(mutation05(chromosome) == Chromosome(stripWhitespace("j f  cu C  U  ne D  v  eI j  sf"))); //  50% more
	SimulationRNG::reset(2);
	BOOST_TEST(mutation01(chromosome) == Chromosome(stripWhitespace("  f  cu C  U  n  D  v  e  j  s")));  //  10% more
	BOOST_TEST(mutation05(chromosome) == Chromosome(stripWhitespace("L f  ce Cv U  n  D  v  e  jO s")));  //  40% more
}

BOOST_AUTO_TEST_CASE(geneAddition_should_be_able_to_insert_before_first_position)
{
	SimulationRNG::reset(7);
	Chromosome chromosome("fcCUnDvejs");
	function<Mutation> mutation = geneAddition(0.1);

	Chromosome mutatedChromosome = mutation(chromosome);
	BOOST_TEST(mutatedChromosome.length() > chromosome.length());

	vector<string> suffix(
		mutatedChromosome.optimisationSteps().end() - chromosome.length(),
		mutatedChromosome.optimisationSteps().end()
	);
	BOOST_TEST(suffix == chromosome.optimisationSteps());
}

BOOST_AUTO_TEST_CASE(geneAddition_should_be_able_to_insert_after_last_position)
{
	SimulationRNG::reset(81);
	Chromosome chromosome("fcCUnDvejs");
	function<Mutation> mutation = geneAddition(0.1);

	Chromosome mutatedChromosome = mutation(chromosome);
	BOOST_TEST(mutatedChromosome.length() > chromosome.length());

	vector<string> prefix(
		mutatedChromosome.optimisationSteps().begin(),
		mutatedChromosome.optimisationSteps().begin() + chromosome.length()
	);
	BOOST_TEST(prefix == chromosome.optimisationSteps());
}

BOOST_AUTO_TEST_CASE(geneAddition_should_return_identical_chromosome_if_probability_is_zero)
{
	Chromosome chromosome("fcCUnDvejsrmV");
	function<Mutation> mutation = geneAddition(0.0);

	BOOST_TEST(mutation(chromosome) == chromosome);
}

BOOST_AUTO_TEST_CASE(geneAddition_should_insert_genes_at_all_positions_if_probability_is_one)
{
	Chromosome chromosome("fcCUnDvejsrmV");
	function<Mutation> mutation = geneAddition(1.0);

	Chromosome mutatedChromosome = mutation(chromosome);
	BOOST_TEST(mutatedChromosome.length() == chromosome.length() * 2 + 1);

	vector<string> originalGenes;
	for (size_t i = 0; i < mutatedChromosome.length() - 1; ++i)
		if (i % 2 == 1)
			originalGenes.push_back(mutatedChromosome.optimisationSteps()[i]);

	BOOST_TEST(Chromosome(originalGenes) == chromosome);
}

BOOST_AUTO_TEST_CASE(alternativeMutations_should_choose_between_mutations_with_given_probability)
{
	SimulationRNG::reset(1);
	Chromosome chromosome("a");
	function<Mutation> mutation = alternativeMutations(
		0.8,
		wholeChromosomeReplacement(Chromosome("c")),
		wholeChromosomeReplacement(Chromosome("f"))
	);

	size_t cCount = 0;
	size_t fCount = 0;
	for (size_t i = 0; i < 10; ++i)
	{
		Chromosome mutatedChromosome = mutation(chromosome);
		cCount += static_cast<int>(mutatedChromosome == Chromosome("c"));
		fCount += static_cast<int>(mutatedChromosome == Chromosome("f"));
	}

	// This particular seed results in 7 "c"s out of 10 which looks plausible given the 80% chance.
	BOOST_TEST(cCount == 7);
	BOOST_TEST(fCount == 3);
}

BOOST_AUTO_TEST_CASE(alternativeMutations_should_always_choose_first_mutation_if_probability_is_one)
{
	Chromosome chromosome("a");
	function<Mutation> mutation = alternativeMutations(
		1.0,
		wholeChromosomeReplacement(Chromosome("c")),
		wholeChromosomeReplacement(Chromosome("f"))
	);

	for (size_t i = 0; i < 10; ++i)
		BOOST_TEST(mutation(chromosome) == Chromosome("c"));
}

BOOST_AUTO_TEST_CASE(alternativeMutations_should_always_choose_second_mutation_if_probability_is_zero)
{
	Chromosome chromosome("a");
	function<Mutation> mutation = alternativeMutations(
		0.0,
		wholeChromosomeReplacement(Chromosome("c")),
		wholeChromosomeReplacement(Chromosome("f"))
	);

	for (size_t i = 0; i < 10; ++i)
		BOOST_TEST(mutation(chromosome) == Chromosome("f"));
}

BOOST_AUTO_TEST_CASE(randomPointCrossover_should_swap_chromosome_parts_at_random_point)
{
	function<Crossover> crossover = randomPointCrossover();

	SimulationRNG::reset(1);
	Chromosome result1 = crossover(Chromosome("aaaaaaaaaa"), Chromosome("cccccc"));
	BOOST_TEST(result1 == Chromosome("aaaccc"));

	SimulationRNG::reset(1);
	Chromosome result2 = crossover(Chromosome("cccccc"), Chromosome("aaaaaaaaaa"));
	BOOST_TEST(result2 == Chromosome("cccaaaaaaa"));
}

BOOST_AUTO_TEST_CASE(randomPointCrossover_should_only_consider_points_available_on_both_chromosomes)
{
	SimulationRNG::reset(1);
	function<Crossover> crossover = randomPointCrossover();

	for (size_t i = 0; i < 30; ++i)
	{
		Chromosome result1 = crossover(Chromosome("aaa"), Chromosome("TTTTTTTTTTTTTTTTTTTT"));
		Chromosome result2 = crossover(Chromosome("TTTTTTTTTTTTTTTTTTTT"), Chromosome("aaa"));
		BOOST_TEST((
			result1 == Chromosome("TTTTTTTTTTTTTTTTTTTT") ||
			result1 == Chromosome("aTTTTTTTTTTTTTTTTTTT") ||
			result1 == Chromosome("aaTTTTTTTTTTTTTTTTTT") ||
			result1 == Chromosome("aaaTTTTTTTTTTTTTTTTT")
		));
		BOOST_TEST((
			result2 == Chromosome("aaa") ||
			result2 == Chromosome("Taa") ||
			result2 == Chromosome("TTa") ||
			result2 == Chromosome("TTT")
		));
	}
}

BOOST_AUTO_TEST_CASE(randomPointCrossover_should_never_split_at_position_zero_if_chromosomes_are_splittable)
{
	SimulationRNG::reset(1);
	function<Crossover> crossover = randomPointCrossover();

	for (size_t i = 0; i < 30; ++i)
	{
		Chromosome result1 = crossover(Chromosome("aa"), Chromosome("TTTTTTTTTTTTTTTTTTTT"));
		Chromosome result2 = crossover(Chromosome("TTTTTTTTTTTTTTTTTTTT"), Chromosome("aa"));
		BOOST_TEST(result1 != Chromosome("TTTTTTTTTTTTTTTTTTTT"));
		BOOST_TEST(result2 != Chromosome("aa"));
	}
}

BOOST_AUTO_TEST_CASE(randomPointCrossover_should_never_split_at_position_zero_if_chromosomes_are_not_empty)
{
	SimulationRNG::reset(1);
	function<Crossover> crossover = randomPointCrossover();

	for (size_t i = 0; i < 30; ++i)
	{
		Chromosome result1 = crossover(Chromosome("a"), Chromosome("T"));
		Chromosome result2 = crossover(Chromosome("T"), Chromosome("a"));
		BOOST_TEST(result1 == Chromosome("a"));
		BOOST_TEST(result2 == Chromosome("T"));
	}
}

BOOST_AUTO_TEST_CASE(randomPointCrossover_should_work_even_if_one_chromosome_is_unsplittable)
{
	function<Crossover> crossover = randomPointCrossover();

	SimulationRNG::reset(1);
	BOOST_CHECK(crossover(Chromosome("ff"), Chromosome("a")) == Chromosome("f"));
	BOOST_CHECK(crossover(Chromosome("a"), Chromosome("ff")) == Chromosome("af"));
}

BOOST_AUTO_TEST_CASE(randomPointCrossover_should_split_at_position_zero_only_if_at_least_one_chromosome_is_empty)
{
	Chromosome empty("");
	Chromosome unsplittable("a");
	Chromosome splittable("aaaa");
	function<Crossover> crossover = randomPointCrossover();

	SimulationRNG::reset(1);
	BOOST_CHECK(crossover(empty, empty) == empty);
	BOOST_CHECK(crossover(unsplittable, empty) == empty);
	BOOST_CHECK(crossover(empty, unsplittable) == unsplittable);
	BOOST_CHECK(crossover(splittable, empty) == empty);
	BOOST_CHECK(crossover(empty, splittable) == splittable);
}

BOOST_AUTO_TEST_CASE(fixedPointCrossover_should_swap_chromosome_parts_at_given_point)
{
	Chromosome result1 = fixedPointCrossover(0.8)(Chromosome("aaaaaaaaaa"), Chromosome("cccccccccc"));
	Chromosome result2 = fixedPointCrossover(0.8)(Chromosome("cccccccccc"), Chromosome("aaaaaaaaaa"));
	BOOST_TEST(result1 == Chromosome("aaaaaaaacc"));
	BOOST_TEST(result2 == Chromosome("ccccccccaa"));
}

BOOST_AUTO_TEST_CASE(fixedPointCrossover_should_determine_crossover_point_based_on_length_of_shorter_chromosome)
{
	Chromosome result1 = fixedPointCrossover(0.4)(Chromosome("aaaaa"), Chromosome("cccccccccc"));
	Chromosome result2 = fixedPointCrossover(0.4)(Chromosome("cccccccccc"), Chromosome("aaaaa"));
	BOOST_TEST(result1 == Chromosome("aacccccccc"));
	BOOST_TEST(result2 == Chromosome("ccaaa"));
}

BOOST_AUTO_TEST_CASE(fixedPointCrossover_should_round_split_point)
{
	Chromosome result1 = fixedPointCrossover(0.49)(Chromosome("aaaaa"), Chromosome("ccccc"));
	Chromosome result2 = fixedPointCrossover(0.49)(Chromosome("ccccc"), Chromosome("aaaaa"));
	BOOST_TEST(result1 == Chromosome("aaccc"));
	BOOST_TEST(result2 == Chromosome("ccaaa"));

	Chromosome result3 = fixedPointCrossover(0.50)(Chromosome("aaaaa"), Chromosome("ccccc"));
	Chromosome result4 = fixedPointCrossover(0.50)(Chromosome("ccccc"), Chromosome("aaaaa"));
	BOOST_TEST(result3 == Chromosome("aaacc"));
	BOOST_TEST(result4 == Chromosome("cccaa"));

	Chromosome result5 = fixedPointCrossover(0.51)(Chromosome("aaaaa"), Chromosome("ccccc"));
	Chromosome result6 = fixedPointCrossover(0.51)(Chromosome("ccccc"), Chromosome("aaaaa"));
	BOOST_TEST(result5 == Chromosome("aaacc"));
	BOOST_TEST(result6 == Chromosome("cccaa"));
}

BOOST_AUTO_TEST_CASE(fixedPointCrossover_should_split_at_position_zero_if_explicitly_requested)
{
	Chromosome result1 = fixedPointCrossover(0.0)(Chromosome("aaaaa"), Chromosome("cccccccccc"));
	Chromosome result2 = fixedPointCrossover(0.0)(Chromosome("cccccccccc"), Chromosome("aaaaa"));
	BOOST_TEST(result1 == Chromosome("cccccccccc"));
	BOOST_TEST(result2 == Chromosome("aaaaa"));
}

BOOST_AUTO_TEST_CASE(fixedPointCrossover_should_split_at_end_of_shorter_chromosome_if_crossover_point_is_after_last_position)
{
	Chromosome result1 = fixedPointCrossover(1.0)(Chromosome("aaaaa"), Chromosome("cccccccccc"));
	Chromosome result2 = fixedPointCrossover(1.0)(Chromosome("cccccccccc"), Chromosome("aaaaa"));
	BOOST_TEST(result1 == Chromosome("aaaaaccccc"));
	BOOST_TEST(result2 == Chromosome("ccccc"));
}

BOOST_AUTO_TEST_CASE(fixedPointCrossover_should_select_correct_split_point_for_unsplittable_chromosomes)
{
	function<Crossover> crossover00 = fixedPointCrossover(0.0);
	BOOST_CHECK(crossover00(Chromosome("fff"), Chromosome("a")) == Chromosome("a"));
	BOOST_CHECK(crossover00(Chromosome("a"), Chromosome("fff")) == Chromosome("fff"));

	BOOST_CHECK(crossover00(Chromosome("f"), Chromosome("a")) == Chromosome("a"));

	function<Crossover> crossover10 = fixedPointCrossover(1.0);
	BOOST_CHECK(crossover10(Chromosome("fff"), Chromosome("a")) == Chromosome("f"));
	BOOST_CHECK(crossover10(Chromosome("a"), Chromosome("fff")) == Chromosome("aff"));

	BOOST_CHECK(crossover10(Chromosome("f"), Chromosome("a")) == Chromosome("f"));
}

BOOST_AUTO_TEST_CASE(fixedPointCrossover_should_always_use_position_zero_as_split_point_when_chromosome_empty)
{
	Chromosome empty("");
	Chromosome unsplittable("f");
	Chromosome splittable("aaaa");

	function<Crossover> crossover00 = fixedPointCrossover(0.0);
	BOOST_CHECK(crossover00(empty, empty) == empty);
	BOOST_CHECK(crossover00(unsplittable, empty) == empty);
	BOOST_CHECK(crossover00(empty, unsplittable) == unsplittable);
	BOOST_CHECK(crossover00(splittable, empty) == empty);
	BOOST_CHECK(crossover00(empty, splittable) == splittable);

	function<Crossover> crossover10 = fixedPointCrossover(1.0);
	BOOST_CHECK(crossover10(empty, empty) == empty);
	BOOST_CHECK(crossover10(unsplittable, empty) == empty);
	BOOST_CHECK(crossover10(empty, unsplittable) == unsplittable);
	BOOST_CHECK(crossover10(splittable, empty) == empty);
	BOOST_CHECK(crossover10(empty, splittable) == splittable);
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()

}
