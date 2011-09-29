#include "LocalisationEngine.h"

using namespace Comrade::Corelib;

Comrade::Osiris::LocalisationEngine::LocalisationEngine()
{
	vote_grid=new EvidenceGrid<int>(1,100,100);
	// Import the dimensions of the global evidence grid here...
}

void Comrade::Osiris::LocalisationEngine::
assign_vote_grid(EvidenceGrid<int>*  vgrid)
{
	vote_grid=vgrid;
}

std::pair<int,int> Comrade::Osiris::LocalisationEngine::localise
(EvidenceGrid<char>* local_grid,
EvidenceGrid<char>* global_grid,int layer)
{
	build_vote_grid(local_grid,global_grid,0);
	return filter_results();
}

void Comrade::Osiris::LocalisationEngine::
build_vote_grid(EvidenceGrid<char>* local_grid,
				EvidenceGrid<char>* global_grid,int layer)
{
	// Outer loop on global grid goes here...

	for (int oyy=0; oyy<=global_grid->ysz-1-local_grid->ysz; ++oyy)
	{
		for (int oxx=0; oxx<=global_grid->xsz-1-local_grid->xsz; ++oxx)
		{
			for (int yy=0; yy<=local_grid->ysz-1; ++yy)
			{
				for (int xx=0; xx<=local_grid->xsz-1; ++xx)
				{
					bool ans=(global_grid->grid[layer][oyy+yy][oxx+xx]>10)&&
							 (local_grid->grid[layer][yy][xx]>10);

					if (ans==true)
					{
						vote_grid->grid[layer][oyy+yy][oxx+xx]++;
					}
				}
			}
		}
	}
}

std::pair<int,int> Comrade::Osiris::LocalisationEngine::filter_results()
{
	std::pair<int,int> result;
	result.first=0;
	result.second=0;
	int max=vote_grid->grid[0][0][0];
	
	for (int yy=0; yy<=vote_grid->ysz-1; ++yy)
	{
		for (int xx=0; xx<=vote_grid->xsz-1; ++xx)
		{
			if (vote_grid->grid[0][yy][xx]>max)
			{
				max=vote_grid->grid[0][yy][xx];
				result.first=xx;
				result.second=yy;
			}
		}
	}

	return result;
}

Comrade::Osiris::LocalisationEngine::~LocalisationEngine()
{}
