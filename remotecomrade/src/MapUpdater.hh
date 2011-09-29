#ifndef Map_Updater
#define Map_Updater

#include "SectorDraw.hh"
#include "CIRC.hh"
#include "EvidenceGrid.hh"

class MapUpdater
{
	public:
		MapUpdater();
		~MapUpdater();
		void updater(Comrade::Corelib::map_update& m);
	private:
		Comrade::Corelib::EvidenceGrid<unsigned char> *evidence_grid;
		Comrade::Corelib::SectorDraw sectordraw;
		
};

#endif
