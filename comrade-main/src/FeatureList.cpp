#include "FeatureList.h"

Comrade::Osiris::FeatureList::FeatureList()
{}

void Comrade::Osiris::FeatureList::add_feature(Comrade::Osiris::Feature* feature)
{
	feature_list.push_back(feature);
}
			
bool Comrade::Osiris::FeatureList::is_complete()
{
	for (itor=feature_list.begin(); itor!=feature_list.end(); ++itor)
	{
		if ((*itor)->is_present()==false)
		{
			return false;
		}
	}
	
	return true;
}

bool Comrade::Osiris::FeatureList::status_of_feature(int fno)
{
    if (feature_list[fno]->is_present()==true)
    {
        return true;
    }
    
    else
    {
        return false;
    }
}

Comrade::Osiris::FeatureList::~FeatureList()
{}


