
#include <mapinfo/mapinfo.include>

mapInfoDef "valley" {
	_default_mapinfo( "valley" )

	data {
		"mapLocation"			"maps/valley/location"
		"script_entrypoint"		"Valley_MapScript"
		"mapBriefing"			"maps/valley/briefing"
		"campaignDescription"	"maps/valley/campaign"
		"numObjectives"			"4"
		"mtr_serverShot"		"levelshots/valley"
		"mtr_backdrop"			"levelshots/campaigns/northamerica"
		"mapPosition"			"311 140"
		"snd_music"				"sounds/music/load1"
		"strogg_endgame_pause"	"6.0"
		"gdf_endgame_pause"		"5.0"
	}
}

// =================================
// TEST MAPS
// =================================
/*
mapMetaData maps/valley_bot {
	"mapinfo"	"valley"
	"dz_deployInfo"	"valley"
	"massive_zone_name"		"ghost town"	
}
*/