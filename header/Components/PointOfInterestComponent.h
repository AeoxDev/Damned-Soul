#pragma once

enum POI
{
	POI_INACTIVE,
	POI_ACTIVE,//Camera looks at average position of objects.

	POI_ACTIVE_FOR_X_TIME,//Becomes inactive after x seconds
	POI_INACTIVE_FOR_X_TIME,//Becomes m_mode after x seconds

	POI_FORCE //Force camera to look at this object, disregard all other actives.
};

struct PointOfInterestComponent
{
	POI m_mode = POI_ACTIVE;
	float m_time = 0.0f;//Time in seconds
	float m_weight = 1.0f;//Higher weight moves the camera point closer.
	float m_height = .0f;//Only in forced m_mode
	float m_rotationY = 0.0f;//Only in forced m_mode
};

