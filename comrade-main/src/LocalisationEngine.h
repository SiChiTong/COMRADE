#ifndef LOCALISATION_ENGINE_H
#define LOCALISATION_ENGINE_H

#include <utility>
#include <vector>
#include "EvidenceGrid.hh"

namespace Comrade {
	namespace Osiris {

class LocalisationEngine
{
protected:	Comrade::Corelib::EvidenceGrid<int>* vote_grid;

public:		LocalisationEngine();

		void assign_vote_grid(Comrade::Corelib::EvidenceGrid<int>* vgrid);
			
		std::pair<int,int> localise
		(Comrade::Corelib::EvidenceGrid<char>* local_grid,
		 Comrade::Corelib::EvidenceGrid<char>* global_grid,int layer);
	 
		void build_vote_grid(Comrade::Corelib::EvidenceGrid<char>* local_grid,
				     Comrade::Corelib::EvidenceGrid<char>* global_grid,
				     int layer);

		std::pair<int,int> filter_results();

		~LocalisationEngine();
};

}
}

#endif
