
#include "../precompiled.h"
#pragma hdrstop

#if defined( _DEBUG ) && !defined( ID_REDIRECT_NEWDELETE )
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "GameRules_Tactical.h"
#include "../Player.h"
#include "../script/Script_Helper.h"
#include "../script/Script_ScriptObject.h"
#include "../guis/UserInterfaceLocal.h"
#include "../guis/UIList.h"
#include "../rules/VoteManager.h"
#include "../rules/AdminSystem.h"

sdGameRulesTacticalNetworkState::sdGameRulesTacticalNetworkState( void ) {
}

void sdGameRulesTacticalNetworkState::MakeDefault( void ) {
	sdGameRulesNetworkState::MakeDefault();

	winningTeam			= NULL;
	campaignWinningTeam = NULL;
}

void sdGameRulesTacticalNetworkState::Write( idFile* file ) const {
	sdGameRulesNetworkState::Write( file );

	file->WriteInt( winningTeam ? winningTeam->GetIndex() : -1 );
	file->WriteInt( campaignWinningTeam ? campaignWinningTeam->GetIndex() : -1 );	
}

void sdGameRulesTacticalNetworkState::Read( idFile* file ) {
	sdGameRulesNetworkState::Read( file );

	int winningTeamIndex;
	file->ReadInt( winningTeamIndex );
	winningTeam = winningTeamIndex == -1 ? NULL : &sdTeamManager::GetInstance().GetTeamByIndex( winningTeamIndex );

	int campaignWinningTeamIndex;
	file->ReadInt( campaignWinningTeamIndex );
	campaignWinningTeam = campaignWinningTeamIndex == -1 ? NULL : &sdTeamManager::GetInstance().GetTeamByIndex( campaignWinningTeamIndex );	
}

/*
===============================================================================

	sdGameRulesTactical

===============================================================================
*/

CLASS_DECLARATION( sdGameRules, sdGameRulesTactical )
END_CLASS

/*
================
sdGameRulesTactical::sdGameRulesTactical
================
*/
sdGameRulesTactical::sdGameRulesTactical( void ) {
	currentMapIndex		= 0;
	winningTeam			= NULL;
	campaignWinningTeam	= NULL;
	campaignDecl		= NULL;
}

/*
================
sdGameRulesTactical::~sdGameRulesTactical
================
*/
sdGameRulesTactical::~sdGameRulesTactical( void ) {
}

/*
================
sdGameRulesTactical::SetCampaign
================
*/
void sdGameRulesTactical::SetCampaign( const sdDeclCampaign* newCampaign ) {
	assert( newCampaign != NULL );

	currentMapIndex = 0;
	campaignWinningTeam = NULL;
	Reset();

	campaignDecl = newCampaign;

	if ( !gameLocal.isClient ) {
		sys->SetServerInfo( "si_campaign", campaignDecl->GetName() );
		sys->SetServerInfo( "si_campaignInfo", "" );
	}

	campaignMapData.SetNum( campaignDecl->GetNumMaps() );
	for ( int i = 0; i < campaignMapData.Num(); i++ ) {
		campaignMapData[ i ].winner		= NULL;
		campaignMapData[ i ].written	= false;

		int numTeams = sdTeamManager::GetInstance().GetNumTeams();
		campaignMapData[ i ].teamData.SetNum( numTeams );
		for ( int j = 0; j < numTeams; j++ ) {
			int numTypes = gameLocal.declProficiencyTypeType.Num();
			campaignMapData[ i ].teamData[ j ].xp.SetNum( numTypes );
			for ( int k = 0; k < numTypes; k++ ) {
				campaignMapData[ i ].teamData[ j ].xp[ k ] = 0.f;
			}
		}

		campaignMapData[ i ].mapInfo = NULL;

		idStr mapName = campaignDecl->GetMap( i );
		sdGameRules_SingleMapHelper::SanitizeMapName( mapName, false );
		const idDict* mapMetaData = gameLocal.mapMetaDataList->FindMetaData( mapName, &gameLocal.defaultMetaData );
		campaignMapData[ i ].metaDataName = mapName;
		campaignMapData[ i ].mapInfo = gameLocal.declMapInfoType[ mapMetaData->GetString( "mapinfo" ) ];
	}

	if( gameLocal.GetLocalPlayer() != NULL ) {
		if( sdUserInterfaceScope* scope = gameLocal.globalProperties.GetSubScope( "campaignInfo" ) ) {
			if( sdProperties::sdProperty* property = scope->GetProperty( "statsReady", sdProperties::PT_FLOAT ) ) {
				*property->value.floatValue = 0.0f;
			}
		} else {
			gameLocal.Warning( "sdGameRulesCampaign::SetCampaign: Couldn't find global 'campaignInfo' scope in guiGlobals." );
		}
	}

	if ( gameLocal.isServer ) {
		SendCampaignInfo( sdReliableMessageClientInfoAll() );
	}
}

/*
================
sdGameRulesTactical::SendCampaignInfo
================
*/
void sdGameRulesTactical::SendCampaignInfo( const sdReliableMessageClientInfoBase& target ) {
	sdReliableServerMessage msg( GAME_RELIABLE_SMESSAGE_RULES_DATA );
	msg.WriteLong( EVENT_SETCAMPAIGN );
	msg.WriteLong( campaignDecl->Index() );
	msg.Send( target );
}

/*
================
sdGameRulesTactical::ReadCampaignInfo
================
*/
void sdGameRulesTactical::ReadCampaignInfo( const idBitMsg& msg ) {
	int campaignIndex = msg.ReadLong();

	const sdDeclCampaign* newCampaign = NULL;
	if ( campaignIndex != -1 ) {
		newCampaign = gameLocal.declCampaignType[ campaignIndex ];
	}

	SetCampaign( newCampaign );
}

/*
================
sdGameRulesTactical::GameState_Review
================
*/
void sdGameRulesTactical::GameState_Review( void ) {
	if ( nextState != GS_INACTIVE ) {
		return;
	}

	if ( si_gameReviewReadyWait.GetBool() ) {
		if ( ArePlayersReady() != RS_READY ) {
			return;
		}
	}

	NextStateDelayed( GS_NEXTMAP, MINS2MS( g_gameReviewPause.GetFloat() ) );
}

/*
================
sdGameRulesTactical::GameState_NextGame
================
*/
void sdGameRulesTactical::GameState_NextGame( void ) {
	if ( nextState != GS_INACTIVE ) {
		return;
	}

	NewState( GS_WARMUP );

	// put everyone back in from endgame spectate
	CheckRespawns( true );
}

/*
================
sdGameRulesTactical::GameState_Warmup
================
*/
void sdGameRulesTactical::GameState_Warmup( void ) {
	if ( !CanStartMatch() ) {
		needsRestart = true;
		return;
	}
	StartMatch();
}

/*
================
sdGameRulesTactical::GameState_Countdown
================
*/
void sdGameRulesTactical::GameState_Countdown( void ) {
}

/*
================
sdGameRulesTactical::GameState_GameOn
================
*/
void sdGameRulesTactical::GameState_GameOn( void ) {
	if ( gameLocal.GameState() == GAMESTATE_STARTUP || nextState != GS_INACTIVE ) {
		return;
	}

	int timeLimit = GetTimeLimit();
	if ( timeLimit != 0 ) {
		if ( gameLocal.time - matchStartedTime >= timeLimit ) {
			OnTimeLimitHit();
			EndGame();
		}
	}
}

/*
================
sdGameRulesTactical::StartMap
================
*/
void sdGameRulesTactical::StartMap( void ) {
	assert( campaignDecl != NULL );

	BackupPlayerTeams();

	idStr sessionCommand = va( "map %s", campaignDecl->GetMap( currentMapIndex ) );
	cmdSystem->PushFrameCommand( sessionCommand );

	NewState( GS_NEXTGAME );
}

/*
================
sdGameRulesTactical::GameState_NextMap
================
*/
void sdGameRulesTactical::GameState_NextMap( void ) {
	currentMapIndex++;

	winningTeam = NULL;

	if ( currentMapIndex >= campaignDecl->GetNumMaps() ) {
		if ( gameLocal.NextMap() ) {
			return;
		}
		SetCampaign( campaignDecl );
	} else {
		ClearChatData();
	}

	StartMap();
}

/*
================
sdGameRulesTactical::SetWinner
================
*/
void sdGameRulesTactical::SetWinner( sdTeamInfo* team ) {
	if ( !gameLocal.isClient && ( gameState == GS_GAMEREVIEW ) ) {
		gameLocal.Warning( "Attempted to set winning team after game has ended" );
		return;
	}

	if ( winningTeam == team ) {
		return;
	}

	winningTeam = team;
}

/*
================
sdGameRulesTactical::SendMapStats
================
*/
void sdGameRulesTactical::SendMapStats( int index, const sdReliableMessageClientInfoBase& target ) {
	mapData_t& mapData = campaignMapData[ index ];

	sdReliableServerMessage msg( GAME_RELIABLE_SMESSAGE_RULES_DATA );
	msg.WriteLong( EVENT_MAPSTATS );

	msg.WriteLong( index );

	sdTeamManager::GetInstance().WriteTeamToStream( mapData.winner, msg );

	for ( int i = 0; i < mapData.teamData.Num(); i++ ) {
		teamMapData_t& teamData = mapData.teamData[ i ];

		for ( int j = 0; j < teamData.xp.Num(); j++ ) {
			msg.WriteFloat( teamData.xp[ j ] );
		}
	}

	msg.Send( target );
}

/*
================
sdGameRulesTactical::ReadMapStats
================
*/
void sdGameRulesTactical::ReadMapStats( const idBitMsg& msg ) {
	int index = msg.ReadLong();

	if ( index >= campaignMapData.Num() ) {
		gameLocal.Warning( "sdGameRulesCampaign::ReadStats Out of Bounds Map Stats Received" );
		return;
	}

	mapData_t& mapData = campaignMapData[ index ];

	mapData.winner = sdTeamManager::GetInstance().ReadTeamFromStream( msg );
	mapData.written = true;

	for ( int i = 0; i < mapData.teamData.Num(); i++ ) {
		teamMapData_t& teamData = mapData.teamData[ i ];

		for ( int j = 0; j < teamData.xp.Num(); j++ ) {
			teamData.xp[ j ] = msg.ReadFloat();
		}
	}
	OnMapStatsReceived( index );
}

/*
================
sdGameRulesTactical::EndGame
================
*/
void sdGameRulesTactical::EndGame( void ) {
	if ( IsWarmup() ) {
		return;
	}

	NextStateDelayed( GS_GAMEREVIEW, 1000 );

	RecordWinningTeam( winningTeam, "maps", true );

	if ( !gameLocal.isClient ) {
		if ( winningTeam ) {
			winningTeam->AddWin();
		}

		sdTeamManagerLocal& manager = sdTeamManager::GetInstance();

		campaignWinningTeam = NULL;

		int bestWins = -1;
		for ( int i = 0; i < manager.GetNumTeams(); i++ ) {
			sdTeamInfo& testTeam = manager.GetTeamByIndex( i );

			if ( testTeam.GetNumWins() > bestWins ) {
				campaignWinningTeam = &testTeam;
				bestWins = testTeam.GetNumWins();
			} else if ( testTeam.GetNumWins() == bestWins ) {
				campaignWinningTeam = NULL;
			}
		}

		int index = winningTeam == NULL ? 0 : winningTeam->GetIndex() + 1;
		idStr currentText = si_campaignInfo.GetString();
		idStr newText;
		if ( currentText.Length() == 0 ) {
			newText = va( "%d", index );
		} else {
			newText = va( "%s %d", currentText.c_str(), index );
		}
		sys->SetServerInfo( "si_campaignInfo", newText );

		if ( currentMapIndex < campaignMapData.Num() ) {
			mapData_t& mapData = campaignMapData[ currentMapIndex ];

			mapData.winner	= winningTeam;
			mapData.written	= true;

			sdTeamManagerLocal& teamManager = sdTeamManager::GetInstance();
			for ( int i = 0; i < teamManager.GetNumTeams(); i++ ) {
				teamMapData_t& teamData = mapData.teamData[ i ];
				for ( int k = 0; k < MAX_CLIENTS; k++ ) {
					idPlayer* player = gameLocal.GetClient( k );
					if ( !player ) {
						continue;
					}

					sdTeamInfo* team = player->GetTeam();
					if ( !team || team->GetIndex() != i ) {
						continue;
					}

					for ( int j = 0; j < teamData.xp.Num(); j++ ) {
						teamData.xp[ j ] += player->GetProficiencyTable().GetPointsSinceBase( j );
					}
				}
			}

			SendMapStats( currentMapIndex, sdReliableMessageClientInfoAll() );
			if( gameLocal.DoClientSideStuff() ) {
				OnMapStatsReceived( currentMapIndex );
			}			
		}

		CallScriptEndGame();
	}
}

/*
================
sdGameRulesTactical::OnGameState_Review
================
*/
void sdGameRulesTactical::OnGameState_Review( void ) {
	if ( !gameLocal.isClient ) {
		nextState = GS_INACTIVE;	// used to abort a game. cancel out any upcoming state change
		ClearPlayerReadyFlags();
	}
}

/*
================
sdGameRulesTactical::OnGameState_Countdown
================
*/
void sdGameRulesTactical::OnGameState_Countdown( void ) {
}

/*
================
sdGameRulesTactical::OnGameState_GameOn
================
*/
void sdGameRulesTactical::OnGameState_GameOn( void ) {
	if ( !gameLocal.isClient ) {
		if ( needsRestart ) {
			needsRestart = false;
			gameLocal.LocalMapRestart();
		}

		matchStartedTime = gameLocal.time;
	}
}

/*
================
sdGameRulesTactical::OnGameState_NextMap
================
*/
void sdGameRulesTactical::OnGameState_NextMap( void ) {
	sdProficiencyManager::GetInstance().StoreBasePoints();
	ClearPlayerReadyFlags();
}

/*
================
sdGameRulesTactical::WriteInitialReliableMessages
================
*/
void sdGameRulesTactical::WriteInitialReliableMessages( const sdReliableMessageClientInfoBase& target ) {
	sdGameRules::WriteInitialReliableMessages( target );

	SendCampaignInfo( target );

	for ( int i = 0; i < campaignMapData.Num(); i++ ) {
		if ( !campaignMapData[ i ].written ) {
			continue;
		}

		SendMapStats( i, target );
	}
}

/*
================
sdGameRulesTactical::OnUserStartMap
================
*/
userMapChangeResult_e sdGameRulesTactical::OnUserStartMap( const char* text, idStr& reason, idStr& mapName ) {
	const metaDataContext_t* metaData = gameLocal.campaignMetaDataList->FindMetaDataContext( text );
	if ( metaData == NULL ) {
		reason = va( "Unknown Campaign '%s'", text );
		return UMCR_ERROR;
	}

	if ( !gameLocal.IsMetaDataValidForPlay( *metaData, false ) ) {
		reason = va( "Campaign '%s' not supported in this mode.", text );
		return UMCR_ERROR;
	}

	const sdDeclCampaign* campaign = gameLocal.declCampaignType[ text ];
	if ( campaign == NULL ) {
		if( !metaData->addon ) {
			reason = va( "Unknown Campaign '%s'", text );
			return UMCR_ERROR;
		}
		
		if( fileSystem->IsAddonPackReferenced( metaData->pak ) ) {
			reason = va( "Unknown Campaign '%s'", text );
			return UMCR_ERROR;
		}
		fileSystem->ReferenceAddonPack( metaData->pak );
		
		idCmdArgs args;
		args.AppendArg( "spawnServer" );
		args.AppendArg( text );
		cmdSystem->SetupReloadEngine( args );
		return UMCR_STOP;
	}

	if ( campaign->GetNumMaps() == 0 ) {
		reason = va( "Campaign '%s' Contains No Maps", text );
		return UMCR_ERROR;
	}

	SetCampaign( campaign );
	mapName = campaign->GetMap( 0 );
	sdGameRules_SingleMapHelper::SanitizeMapName( mapName, true );

	return UMCR_CONTINUE;
}

/*
================
sdGameRulesTactical::ApplyNetworkState
================
*/
void sdGameRulesTactical::ApplyNetworkState( const sdEntityStateNetworkData& newState ) {
	sdGameRules::ApplyNetworkState( newState );

	NET_GET_NEW( sdGameRulesTacticalNetworkState );

	SetWinner( newData.winningTeam );
	SetCampaignWinner( newData.campaignWinningTeam );
}

/*
================
sdGameRulesTactical::ReadNetworkState
================
*/
void sdGameRulesTactical::ReadNetworkState( const sdEntityStateNetworkData& baseState, sdEntityStateNetworkData& newState, const idBitMsg& msg ) const {
	sdGameRules::ReadNetworkState( baseState, newState, msg );

	NET_GET_STATES( sdGameRulesTacticalNetworkState );

	sdTeamManagerLocal& teamManager = sdTeamManager::GetInstance();

	newData.winningTeam			= teamManager.ReadTeamFromStream( baseData.winningTeam, msg );
	newData.campaignWinningTeam	= teamManager.ReadTeamFromStream( baseData.campaignWinningTeam, msg );
}

/*
================
sdGameRulesTactical::WriteNetworkState
================
*/
void sdGameRulesTactical::WriteNetworkState( const sdEntityStateNetworkData& baseState, sdEntityStateNetworkData& newState, idBitMsg& msg ) const {
	sdGameRules::WriteNetworkState( baseState, newState, msg );

	NET_GET_STATES( sdGameRulesTacticalNetworkState );

	newData.winningTeam			= GetWinningTeam();
	newData.campaignWinningTeam	= GetCampaignWinner();

	sdTeamManagerLocal& teamManager = sdTeamManager::GetInstance();

	teamManager.WriteTeamToStream( baseData.winningTeam, newData.winningTeam, msg );
	teamManager.WriteTeamToStream( baseData.campaignWinningTeam, newData.campaignWinningTeam, msg );
}

/*
================
sdGameRulesTactical::CheckNetworkStateChanges
================
*/
bool sdGameRulesTactical::CheckNetworkStateChanges( const sdEntityStateNetworkData& baseState ) const {
	if ( sdGameRules::CheckNetworkStateChanges( baseState ) ) {
		return true;
	}

	NET_GET_BASE( sdGameRulesTacticalNetworkState );
	
	NET_CHECK_FIELD( winningTeam, GetWinningTeam() );
	NET_CHECK_FIELD( campaignWinningTeam, GetCampaignWinner() );

	return false;
}

/*
================
sdGameRulesTactical::CreateNetworkStructure
================
*/
sdEntityStateNetworkData* sdGameRulesTactical::CreateNetworkStructure( void ) const {
	return new sdGameRulesTacticalNetworkState();
}

/*
============
sdGameRulesTactical::ParseNetworkMessage
============
*/
bool sdGameRulesTactical::ParseNetworkMessage( int msgType, const idBitMsg& msg ) {
	switch ( msgType ) {
		case EVENT_SETCAMPAIGN: {
			ReadCampaignInfo( msg );
			return true;
		}
		case EVENT_MAPSTATS: {
			ReadMapStats( msg );
			return true;
		}
	}

	return sdGameRules::ParseNetworkMessage( msgType, msg );
}

/*
================
sdGameRulesTactical::GameTime
================
*/
int sdGameRulesTactical::GetGameTime( void ) const {
	int ms;

	if ( gameState == GS_WARMUP ) {
		ms = 0;
	} else if ( gameState == GS_COUNTDOWN ) {
		ms = nextStateSwitch - gameLocal.time;
	} else {
		ms = GetTimeLimit() - ( gameLocal.time - matchStartedTime );
		if ( ms < 0 ) {
			ms = 0;
		}
	}

	return ms;
}

/*
================
sdGameRulesTactical::GetTypeText
================
*/
const sdDeclLocStr*	sdGameRulesTactical::GetTypeText( void ) const {
	return declHolder.declLocStrType[ "game/gametype/tactical" ];
}


/*
================
sdGameRulesTactical::InitVotes
================
*/
void sdGameRulesTactical::InitVotes( void ) {
	sdGameRules::InitVotes();

	callVotes.Alloc() = new sdCallVoteCampaignReset();
}

/*
================
sdGameRulesTactical::ArgCompletion_StartGame
================
*/
void sdGameRulesTactical::ArgCompletion_StartGame( const idCmdArgs& args, argCompletionCallback_t callback ) {
	if( gameLocal.campaignMetaDataList == NULL ) {
		return;
	}

	const char* cmd = args.Argv( 1 );
	int len = idStr::Length( cmd );

	int num = gameLocal.campaignMetaDataList->GetNumMetaData();
	for ( int i = 0; i < num; i++ ) {
		const metaDataContext_t& metaData = gameLocal.campaignMetaDataList->GetMetaDataContext( i );
		if ( !gameLocal.IsMetaDataValidForPlay( metaData, false ) ) {
			continue;
		}
		const idDict& meta = *metaData.meta;
		
		const char* metaName = meta.GetString( "metadata_name" );
		if ( idStr::Icmpn( metaName, cmd, len ) ) {
			continue;
		}

		callback( va( "%s %s", args.Argv( 0 ), metaName ) );
	}	
}

/*
============
sdGameRulesTactical::GetCurrentMapData
============
*/
const sdGameRules::mapData_t* sdGameRulesTactical::GetCurrentMapData() const {
	for( int i = 0; i < campaignMapData.Num() - 1; i++ ) {
		if( !campaignMapData[ i + 1 ].written ) {
			return &campaignMapData[ i ];
		}
	}
	if( campaignMapData.Empty() ) {
		return NULL;
	}
	return &campaignMapData[ campaignMapData.Num() - 1 ];
}


/*
============
sdGameRulesTactical::OnMapStatsReceived
============
*/
void sdGameRulesTactical::OnMapStatsReceived( int index ) {
	if( index < 0 || index >= campaignMapData.Num() || campaignMapData[ index ].written == false ) {
		if( sdUserInterfaceScope* scope = gameLocal.globalProperties.GetSubScope( "campaignInfo" ) ) {
			if( sdProperties::sdProperty* property = scope->GetProperty( "statsReady", sdProperties::PT_FLOAT ) ) {
				*property->value.floatValue = 0.0f;
			}
		} else {
			gameLocal.Warning( "sdGameRulesCampaign::OnMapStatsReceived: Couldn't find global 'mapInfo' scope in guiGlobals." );
		}
		return;
	}

	if( sdUserInterfaceScope* scope = gameLocal.globalProperties.GetSubScope( "campaignInfo" ) ) {
		if( sdProperties::sdProperty* property = scope->GetProperty( "statsReady", sdProperties::PT_FLOAT ) ) {
			// force a signal
			*property->value.floatValue = 0.0f;
			*property->value.floatValue = 1.0f;
		}
	} else {
		gameLocal.Warning( "sdGameRulesCampaign::OnMapStatsReceived: Couldn't find global 'mapInfo' scope in guiGlobals." );
	}
}

/*
============
sdGameRulesTactical::UpdateClientFromServerInfo
============
*/
void sdGameRulesTactical::UpdateClientFromServerInfo( const idDict& serverInfo, bool allowMedia ) {
	sdGameRules::UpdateClientFromServerInfo( serverInfo, allowMedia );

	idStr mapName = serverInfo.GetString( "si_map" );
	if( mapName.IsEmpty() ) {
		return;
	}
	mapName.StripFileExtension();

	idStr defaultPosition = va( "%i %i", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 );

	using namespace sdProperties;

	// update campaign status
	if ( sdUserInterfaceScope* scope = gameLocal.globalProperties.GetSubScope( "campaignInfo" ) ) {

		sdHashMapGeneric< idStr, int > teamWins;	// number of wins per team
		teamWins.SetGranularity( sdTeamManager::GetInstance().GetNumTeams() );

		for( int i = 0; i < sdTeamManager::GetInstance().GetNumTeams(); i++ ) {
			teamWins[ sdTeamManager::GetInstance().GetTeamByIndex( i ).GetLookupName() ] = 0;
		}

		const idDict* metaData = gameLocal.mapMetaDataList->FindMetaData( mapName, &gameLocal.defaultMetaData );		

		const char* campaignName = serverInfo.GetString( "si_campaign" );
		const idDict* campaignMetaData = gameLocal.campaignMetaDataList->FindMetaData( campaignName, &gameLocal.defaultMetaData );

		idList< const sdDeclMapInfo* > cachedMapInfo;
		idList< const char* > cachedMapMetaData;

		if( allowMedia ) {
			const sdDeclCampaign* campaign = gameLocal.declCampaignType.LocalFind( campaignName );			

			// setup the backdrop
			if ( sdProperty* property = scope->GetProperty( "backdrop", PT_STRING ) ) {
				const sdDeclMapInfo* mapInfo = gameLocal.declMapInfoType.LocalFind( metaData->GetString( "mapinfo", "_default" ) );
				*property->value.stringValue = campaign->GetBackdrop()->GetName();
			}

			cachedMapInfo.SetNum( campaign->GetNumMaps() );
			cachedMapMetaData.SetNum( campaign->GetNumMaps() );
			for( int i = 0; i < campaign->GetNumMaps(); i++ ) {
				const char* name = campaign->GetMap( i );
				const idDict* metaData = gameLocal.mapMetaDataList->FindMetaData( name, &gameLocal.defaultMetaData );
				cachedMapInfo[ i ] = gameLocal.declMapInfoType.LocalFind( metaData->GetString( "mapinfo", "_default" ) );
				cachedMapMetaData[ i ] = name;

			}
		} else {
			cachedMapInfo.SetNum( campaignMapData.Num() );
			cachedMapMetaData.SetNum( campaignMapData.Num() );
			for( int i = 0; i < campaignMapData.Num(); i++ ) {
				cachedMapInfo[ i ] = campaignMapData[ i ].mapInfo;
				cachedMapMetaData[ i ] = campaignMapData[ i ].metaDataName.c_str();
			}
		}

		// setup the name
		if ( sdProperty* property = scope->GetProperty( "name", PT_WSTRING ) ) {
			*property->value.wstringValue = va( L"%hs", campaignMetaData->GetString( "pretty_name" ) );
		}

		idStr campaignState = serverInfo.GetString( "si_campaignInfo" );
		idLexer src( campaignState.c_str(), campaignState.Length(), "UpdateCampaignStats", LEXFL_NOERRORS );
		idToken token;

		bool firstUnplayed = true;
		bool showCurrentOnly = campaignMetaData->GetBool( "show_current_map_only", "0" );

		if ( sdProperty* property = scope->GetProperty( "numMaps", PT_FLOAT ) ) {
			*property->value.floatValue = showCurrentOnly ? Min( cachedMapInfo.Num(), 1 ) : cachedMapInfo.Num();
		}
		
		int mapIndex = 1;
		int numRead = 1;

		bool readAny = false;
		// setup the map state (who won, current, or not played)
		for( int i = 0; i < cachedMapMetaData.Num(); i++ ) {						
			idStr status = "unplayed";

			bool current = false;
			if( src.ReadToken( &token ) ) {
				if( token.GetIntValue() > 0 ) {
					const sdTeamInfo& teamInfo = sdTeamManager::GetInstance().GetTeamByIndex( token.GetIntValue() - 1 );
					status = teamInfo.GetLookupName();
					teamWins[ status ]++;
				} else {
					status = "tied";
				}

				readAny = true;
				numRead++;
			} else {
				if( firstUnplayed ) {
					status = "current";
					firstUnplayed = false;
					current = true;
				} else {
					status = "unplayed";
				}
			}
			
			if( showCurrentOnly && !current ) {
				continue;
			}

			const sdDeclMapInfo* info = NULL;
			if( allowMedia ) {
				info = cachedMapInfo[ i ];
			}
			const idDict* metaData = gameLocal.mapMetaDataList->FindMetaData( cachedMapMetaData[ i ], &gameLocal.defaultMetaData );
			SetupLoadScreenUI( *scope, status.c_str(), current, mapIndex, *metaData, info );
			mapIndex++;
		}

		if( readAny ) {
			numRead--;
		}
		// setup the status
		// jrad - NB: this is only useful during endgame once the campaign's new state has been received
		if ( sdProperty* property = scope->GetProperty( "currentMap", PT_FLOAT ) ) {
			*property->value.floatValue = numRead;
		}

		sdHashMapGeneric< idStr, int >::Iterator iter = teamWins.Begin();
		while( iter != teamWins.End() ) {
			if ( sdProperty* property = scope->GetProperty( va( "%sWins", iter->first.c_str() ), PT_FLOAT ) ) {
				*property->value.floatValue = iter->second;
			}
			++iter;
		}
		
		idWStr value;
		if( teamWins.Num() == 2 ) {
			idWStrList args( 2 );
			args.Fill( 2, L"0" );

			args[ 0 ] = va( L"%d", teamWins[ "strogg" ] );
			args[ 1 ] = va( L"%d", teamWins[ "gdf" ] );
			value = common->LocalizeText( "guis/mainmenu/campaignwins", args );
		}

		// setup the status
		if ( sdProperty* property = scope->GetProperty( "ruleStatus", PT_WSTRING ) ) {
			*property->value.wstringValue = value;
		}
	}
}

/*
============
sdGameRulesTactical::Clear
============
*/
void sdGameRulesTactical::Clear( void ) {
	sdGameRules::Clear();
	winningTeam = NULL;
}

/*
============
sdGameRules::GetBrowserStatusString
============
*/
void sdGameRulesTactical::GetBrowserStatusString( idWStr& str, const sdNetSession& netSession ) const {
	str.Clear();

	const idDict& serverInfo = netSession.GetServerInfo();
	const char* campaignName = serverInfo.GetString( "si_campaign" );
	if( campaignName[ 0 ] != '\0' ) {
		const sdDeclCampaign* campaign = gameLocal.declCampaignType.LocalFind( campaignName, false );		

		if( campaign != NULL ) {			
			const char* campaignInfo = serverInfo.GetString( "si_campaignInfo" );
			int num = 1;
			int i = 0;
			while( campaignInfo[ i ] != '\0' ) {
				if( idStr::CharIsNumeric( campaignInfo[ i ] ) ) {
					num++;
				}
				i++;
			}

			// jrad - if we're reviewing,
			// we've received the win info, but haven't moved on to the next map yet
			if( netSession.GetGameState() & PGS_REVIEWING ) {
				num--;
			}

			idWStrList args( 3 );
			args.SetNum( 3 );
			
			sdGameRules::GetBrowserStatusString( args[ 0 ], netSession );
			args[ 1 ] = va( L"%i", num );
			args[ 2 ] = va( L"%i", campaign->GetNumMaps() );
			str = common->LocalizeText( "guis/mainmenu/campaign_status", args );
		}
	}
}

/*
============
sdGameRulesTactical::GetServerBrowserScore
============
*/
int sdGameRulesTactical::GetServerBrowserScore( const sdNetSession& session ) const {
	int score = 0;

	const char* statusString = session.GetServerInfo().GetString( "si_campaignInfo" );
	int numMapsPlayed = idStr::Length( statusString );
	if ( numMapsPlayed == 0 ) {
		score += sdHotServerList::BROWSER_GOOD_BONUS;
	} else if ( numMapsPlayed == 1 ) {
		score += sdHotServerList::BROWSER_OK_BONUS;
	}

	score += sdGameRules::GetServerBrowserScore( session );
	return score;
}

