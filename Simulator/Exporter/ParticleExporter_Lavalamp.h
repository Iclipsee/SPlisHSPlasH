#ifndef __ParticleExporter_Lavalamp_h__
#define __ParticleExporter_Lavalamp_h__

#include "ExporterBase.h"

namespace SPH
{
	/** \brief Exporting Particles with a marching cubes algorithm for the OBJ format.
	*/

	class ParticleExporter_Lavalamp : public ExporterBase
	{
	protected:
		std::string m_exportPath;
		std::string m_particleFile;

		void createFluidMesh(const std::string& fileName, FluidModel* model, const unsigned int objId = 0xffffffff);

	public:
		ParticleExporter_Lavalamp(SimulatorBase* base);
		ParticleExporter_Lavalamp(const ParticleExporter_Lavalamp&) = delete;
		ParticleExporter_Lavalamp& operator=(const ParticleExporter_Lavalamp&) = delete;
		virtual ~ParticleExporter_Lavalamp(void);

		virtual void init(const std::string& outputPath);
		virtual void step(const unsigned int frame);
		virtual void reset();
		virtual void setActive(const bool active);
	};

	class Grid
	{
	public:
		FluidModel* model;
		Vector3r minBound, maxBound;
		float cellSize;
		std::vector<Vector3r> gridPoints;

		Grid(FluidModel* model, float cellSize);
	private:
		void calcBounds();
		void generateGridPoints();
		void calcPoint(int index);

	};

}

#endif