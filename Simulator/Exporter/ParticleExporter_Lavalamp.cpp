#include "ParticleExporter_Lavalamp.h"
#include <Utilities/Logger.h>
#include <Utilities/FileSystem.h>
#include "SPlisHSPlasH/Simulation.h"

using namespace SPH;
using namespace Utilities;

ParticleExporter_Lavalamp::ParticleExporter_Lavalamp(SimulatorBase* base) :
	ExporterBase(base)
{

}

SPH::ParticleExporter_Lavalamp::~ParticleExporter_Lavalamp(void)
{
}

void SPH::ParticleExporter_Lavalamp::init(const std::string& outputPath)
{
	m_exportPath = FileSystem::normalizePath(outputPath + "/lamp");
}

void SPH::ParticleExporter_Lavalamp::step(const unsigned int frame)
{
	if (!m_active)
		return;

	Simulation* sim = Simulation::getCurrent();
	for (unsigned int i = 0; i < sim->numberOfFluidModels(); i++)
	{
		FluidModel* model = sim->getFluidModel(i);
		std::string fileName = "FluidMesh";
		if (!m_base->getValue<bool>(SimulatorBase::EXPORT_OBJECT_SPLITTING))
		{
			fileName = fileName + "_" + model->getId() + "_" + std::to_string(frame);
			std::string exportFileName = FileSystem::normalizePath(m_exportPath + "/" + fileName);
			createFluidMesh(exportFileName + ".obj", model);
		}
		else
		{
			// object splitting
			for (auto j = 0u; j < m_base->getLastObjectId(); j++)
			{
				std::string fileName2 = fileName + "_" + model->getId() + "_" + std::to_string(j) + "_" + std::to_string(frame);
				std::string exportFileName = FileSystem::normalizePath(m_exportPath + "/" + fileName2);
				createFluidMesh(exportFileName + ".obj", model, j);
				
			}
		}
	}
}

void SPH::ParticleExporter_Lavalamp::reset() 
{
}

void SPH::ParticleExporter_Lavalamp::setActive(const bool active)
{
	ExporterBase::setActive(active);
	if (m_active)
		FileSystem::makeDirs(m_exportPath);
}

void ParticleExporter_Lavalamp::createFluidMesh(const std::string& fileName, FluidModel* model, const unsigned int objId)
{
	Grid g = Grid(model, 1.0f);
	Simulation* sim = Simulation::getCurrent();
	

}




Grid::Grid(FluidModel* model, float cellSize) : model(model), cellSize(cellSize) {
	calcBounds();
	generateGridPoints();
}

void Grid::calcBounds()
{
	minBound = maxBound = model->getPosition(0);

	const unsigned int numParticles = model->numActiveParticles();
	for (unsigned int i = 1; i < numParticles; i++)
	{
		const Vector3r& position = model->getPosition(i);
		for (int j = 0; j < 3; j++) {
			if (position[j] < minBound[j]) minBound[j] = position[j];
			if (position[j] > maxBound[j]) maxBound[j] = position[j];
		}

	}
	/*
	Vector3r buffer = Vector3r(cellSize, cellSize, cellSize);
	minBound -= buffer;
	maxBound += buffer;
	*/

}

void Grid::generateGridPoints()
{
	gridPoints.clear();
	for (float z = minBound.z(); z <= maxBound.z(); z += cellSize) {
		for (float y = minBound.y(); y <= maxBound.y(); y += cellSize) {
			for (float x = minBound.x(); x <= maxBound.x(); x += cellSize) {
				gridPoints.push_back(Vector3r(x, y, z));
			}
		}
	}
}


