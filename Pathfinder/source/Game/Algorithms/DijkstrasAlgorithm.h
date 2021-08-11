#pragma once

#include "IAlgorithm.h"

class DijkstrasAlgorithm : public IAlgorithm
{
public:
	DijkstrasAlgorithm();

	virtual void Execute() override;
};