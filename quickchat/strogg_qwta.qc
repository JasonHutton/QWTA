#include <quickchat/common.include>

// Vehicles /////////////////////////////////////////////////

quickChatDef strogg/quickchat/vehicles/vehicledrops/nv_icarus {
	_chat( "strogg/quickchat/vehicles/vehicledrops/nv_icarus", "RequestIcarus", "team", BOT_TYPE_NONE, "same_entity" )
	avdbit	128
}

quickChatDef strogg/quickchat/vehicles/vehicledrops/nv_hog {
	_chat( "strogg/quickchat/vehicles/vehicledrops/nv_hog", "RequestHog", "team", BOT_TYPE_NONE, "same_entity" )
	avdbit	256
}

quickChatDef strogg/quickchat/vehicles/vehicledrops/nv_desecrator {
	_chat( "strogg/quickchat/vehicles/vehicledrops/nv_desecrator", "RequestDesecrator", "team", BOT_TYPE_NONE, "same_entity" )
	avdbit	512
}

quickChatDef strogg/quickchat/vehicles/vehicledrops/nv_cyclops {
	_chat( "strogg/quickchat/vehicles/vehicledrops/nv_cyclops", "RequestCyclops", "team", BOT_TYPE_NONE, "same_entity" )
	avdbit	1024
}

quickChatDef strogg/quickchat/vehicles/vehicledrops/nv_tormentor {
	_chat( "strogg/quickchat/vehicles/vehicledrops/nv_tormentor", "RequestTormentor", "team", BOT_TYPE_NONE, "same_entity" )
	avdbit	2048
}