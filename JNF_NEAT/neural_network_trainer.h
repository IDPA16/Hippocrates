#pragma once
#include "neural_network.h"
#include "body.h"
#include "training_parameters.h"
#include "genome.h"
#include "organism.h"
#include "species.h"
#include "trained_neural_network.h"
#include <vector>
#include <list>

class NeuralNetworkTrainer {
	private:
		std::vector<Species> species;
		TrainingParameters parameters;
		size_t populationSize = 0U;
		std::vector<IBody *>& trainers;

	public:
		NeuralNetworkTrainer() = delete;
		NeuralNetworkTrainer(std::vector<IBody*>& population, const TrainingParameters& parameters);
		NeuralNetworkTrainer(std::vector<IBody*>& population, TrainingParameters&& parameters);
		NeuralNetworkTrainer(const NeuralNetworkTrainer& other) = default;

		~NeuralNetworkTrainer() = default;

		void TrainUntilFitnessEquals(int fitnessToReach);
		void TrainUntilGenerationEquals(unsigned int generationsToTrain);

		TrainedNeuralNetwork GetTrainedNeuralNetwork();

	private:
		void SetBodies(std::vector<IBody*>& bodies);
		
		void ResetPopulationToTeachableState();
		void Repopulate();
		void CategorizeOrganismsIntoSpecies(std::vector<Organism> && organisms);
		void LetGenerationLive();

		Species &SelectSpeciesToBreed();
		Organism& GetFittestOrganism();
};