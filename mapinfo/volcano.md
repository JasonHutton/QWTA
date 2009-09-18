
#include <mapinfo/mapinfo.include>

mapInfoDef "volcano" {
	_default_mapinfo( "volcano" )

	data {
		"mapLocation"			"maps/volcano/location"
		"script_entrypoint"		"Volcano_MapScript"
		"mapBriefing"			"maps/volcano/briefing"
		"campaignDescription"	"maps/volcano/campaign"
		"numObjectives"			"3"
		"mtr_serverShot"		"levelshots/volcano"
		"mtr_backdrop"			"levelshots/campaigns/pacific"
		"mapPosition"			"542 410"
		"snd_music"				"sounds/music/load3"
		"strogg_endgame_pause"	"10.0"
		"gdf_endgame_pause"		"5.0"
	}
}

