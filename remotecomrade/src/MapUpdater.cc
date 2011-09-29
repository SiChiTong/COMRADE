#include "MapUpdater.hh"
#include "GridDrawingArea.hh"
#include "CIRC.hh"
#include "EvidenceGrid.hh"
#include "GLDrawingArea.hh"
#include "CompassDA.hh"
#include <sigc++/sigc++.h>

using namespace Comrade::Corelib;

MapUpdater::MapUpdater()
{
	evidence_grid=new EvidenceGrid<unsigned char>(1,600,800);
	evidence_grid->fill(0);
	sectordraw.on_pixel_receive.connect(sigc::mem_fun(evidence_grid,&EvidenceGrid<unsigned char>::update_vfh));
	//Comrade::GUI::GridDrawingArea::get_singleton().set_evidence_grid(evidence_grid);
	GL3DScene::get_singleton().set_evidence_grid(evidence_grid);
}

MapUpdater::~MapUpdater()
{
	delete evidence_grid;
}

void MapUpdater::updater(map_update& m)
{
	//std::cout<<"\nreceived map update";
	evidence_grid->update_from_sonar(m,sectordraw);
	//GridDrawingArea::get_singleton().update();
	GL3DScene::get_singleton().update(m.x, m.y, m.orientation);
	compassview::get_singleton().update(m.orientation);
}
