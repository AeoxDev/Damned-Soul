#pragma once

enum POI
{
	POI_INACTIVE,
	POI_ACTIVE,//Camera looks at average position of objects.

	POI_ACTIVE_FOR_X_TIME,//Becomes inactive after x seconds
	POI_INACTIVE_FOR_X_TIME,//Becomes mode after x seconds

	POI_FORCE, //Force camera to look at this object, disregard all other actives.
	POI_MOUSE //Special case for mouse cursor
};

struct PointOfInterestComponent
{
	POI mode = POI_ACTIVE;
	float time = 0.0f;//Time in seconds
	float weight = 1.0f;//Higher weight moves the camera point closer.
	float height = .0f;//Only in forced mode
	float rotationY = 0.0f;//Only in forced mode
	float rotationAccel = 1.0f;
	float rotationRadius = 1.0f;
};

