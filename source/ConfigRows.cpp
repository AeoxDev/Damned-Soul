#include "ConfigRows.h"

void SetupRows(ConfigRows& rows)
{
	//Create ConfigRow:s and add to ConfigRows
	rows.rows[0] = ConfigRow{ "ResolutionX", 1600 };//Name, default value
	rows.rows[1] = ConfigRow{ "ResolutionY", 900 };
}
