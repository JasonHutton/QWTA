// Copyright (C) 2007 Id Software, Inc.
//


#include "precompiled.h"
#pragma hdrstop

#include "DeclDeployableObject.h"
#include "GameDeclIdentifiers.h"
#include "../Game_local.h"

#include "../../framework/DeclParseHelper.h"

/*
===============================================================================

	sdDeclDeployableObject

===============================================================================
*/

/*
================
sdDeclDeployableObject::sdDeclDeployableObject
================
*/
sdDeclDeployableObject::sdDeclDeployableObject( void ) {
	FreeData();
}

/*
================
sdDeclDeployableObject::~sdDeclDeployableObject
================
*/
sdDeclDeployableObject::~sdDeclDeployableObject( void ) {
}

/*
================
sdDeclDeployableObject::DefaultDefinition
================
*/
const char* sdDeclDeployableObject::DefaultDefinition( void ) const {
	return 
		"{\n"							\
		"}\n";
}

/*
================
sdDeclDeployableObject::Parse
================
*/
bool sdDeclDeployableObject::Parse( const char *text, const int textLength ) {
	idToken token;
	idParser src;

	src.SetFlags( DECL_LEXER_FLAGS );
	//src.LoadMemory( text, textLength, GetFileName(), GetLineNum() );
	sdDeclParseHelper declHelper( this, text, textLength, src );

	src.SkipUntilString( "{", &token );
	src.UnreadToken( token );

	idDict temp;
	if ( !temp.Parse( src ) ) {
		gameLocal.Error( "sdDeclDeployableObject::Parse Error Parsing Deployable Decl Info" );
		return false;
	}

	game->CacheDictionaryMedia( temp );

	waitTime		= temp.GetFloat( "wait_time", "10" );

	creditRequired	= temp.GetFloat( "credit_required", "0.1" );

	if ( !temp.GetFloat( "qwta_credit_required", "0.1", qwtaCreditRequired ) ) {
		qwtaCreditRequired = creditRequired;
	}


	idStr		rankRequiredString;
	if ( temp.GetString( "rank_required", "", rankRequiredString ) ) {
		rankRequired	= static_cast<const sdDeclRank*>( declManager->FindType( declManager->GetDeclTypeHandle( declRankIdentifier ), rankRequiredString, false ) );
		if ( rankRequired == NULL ) {
			gameLocal.Warning( "sdDeclDeployableObject::Parse: Unknown rank def '%s' for deployable", rankRequiredString.c_str() );
		}
	}
	forceEscalationRequired = temp.GetInt( "force_escalation_required" );
	logisticsPointsRequired = temp.GetInt( "logistics_points_required" );
	avdbit	= temp.GetInt( "avdbit", "-1" );
	terrainDisallowed = temp.GetInt( "terrain_disallowed", "0" );

	const char* entityType;

	entityType	= temp.GetString( "def_deployable" );
	entity		= gameLocal.declEntityDefType[ entityType ];

	if ( entity == NULL && GetState() != DS_DEFAULTED ) {
		src.Error( "sdDeclDeployableObject::Parse Unknown entity def '%s' for deployable", entityType );
		return false;
	}

	objectSize = 0.f;

	const char* placementInfoText = temp.GetString( "str_placement" );
	placementInfo = gameLocal.declStringMapType[ placementInfoText ];
	if ( GetState() != DS_DEFAULTED ) {
		if ( placementInfo ) {
			float modelScaleFactor = temp.GetFloat( "mdl_scale_factor", "1" );

			renderEntity_t renderEnt;
			gameEdit->ParseSpawnArgsToRenderEntity( placementInfo->GetDict(), renderEnt );
			idBounds objBounds = renderEnt.hModel->Bounds();
			objBounds.GetMins()[ 2 ] = 0.f;
			objBounds.GetMaxs()[ 2 ] = 0.f;
			objectSize = objBounds.GetRadius() * modelScaleFactor;
		} else {
			objectSize = 128.f;
		}
	}

	if ( GetState() != DS_DEFAULTED ) {
		mask		= gameLocal.GetDeploymentMask( temp.GetString( "mask" ) );
	}

	title		= temp.GetString( "name" );
	guiTitle	= temp.GetString( "title" );

	carrier = gameLocal.declEntityDefType[ temp.GetString( "def_deployable_carrier" ) ];
	if ( !carrier && GetState() != DS_DEFAULTED ) {
		src.Error( "sdDeclDeployableObject::Parse No Carrier Supplied" );
		return false;
	}

	allowRotation = temp.GetBool( "allow_rotation", "1" );

	return true;
}

/*
================
sdDeclDeployableObject::FreeData
================
*/
void sdDeclDeployableObject::FreeData( void ) {
	waitTime			= 10;
	entity				= NULL;
	placementInfo		= NULL;
	carrier				= NULL;
	mask				= -1;
	objectSize			= 0.f;
	allowRotation		= true;
	creditRequired		= 0.1f;
	qwtaCreditRequired	= creditRequired;
	rankRequired		= NULL;
	forceEscalationRequired = 0;
	logisticsPointsRequired = 0;
	avdbit				= -1;
	terrainDisallowed	= 0;
}


/*
================
sdDeclDeployableObject::CacheFromDict
================
*/
void sdDeclDeployableObject::CacheFromDict( const idDict& dict ) {
	const idKeyValue* kv = NULL;

	while( kv = dict.MatchPrefix( "do", kv ) ) {
		if ( kv->GetValue().Length() ) {
			gameLocal.declDeployableObjectType[ kv->GetValue() ];
		}
	}
}
