struct ProjectileComponent
{
	int type = 0;
	EntityID parentID;

	ProjectileComponent(int type, EntityID parent) : type(type), parentID(parent) {}
};