#pragma once

struct ArchiveEvents
{
	struct Map
	{
		unsigned input, output;
	};
	Map *map;
	ArchiveEvents()
	{
		map = new Map;
	}
	~ArchiveEvents()
	{
		delete map;
	}

};

extern ArchiveEvents archiveEvents;