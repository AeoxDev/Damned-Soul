#pragma once

enum POI
{
	POI_INACTIVE,
	POI_ACTIVE,//Camera looks at average position of objects.
	POI_FORCE //Force camera to look at this object, disregard all other actives.
};

struct PointOfInterestComponent
{
	POI active = POI_ACTIVE;
};