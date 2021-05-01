
//----------------------------------------------------------------
// ClientSprite.cpp
//----------------------------------------------------------------

#include "../precompiled.h"
#pragma hdrstop

#if defined( _DEBUG ) && !defined( ID_REDIRECT_NEWDELETE )
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "ClientSprite.h"

#include "../../bse/BSEInterface.h"
#include "../../bse/BSE_Envelope.h"
#include "../../bse/BSE_SpawnDomains.h"
#include "../../bse/BSE_Particle.h"
#include "../../bse/BSE.h"

#include "../Player.h"

/*
===============================================================================

rvClientEffect

===============================================================================
*/
/*
const idEventDef EV_SetEffectEndOrigin( "setEffectEndOrigin", '\0', DOC_TEXT( "Sets the end point of the effect, for use with beam effects." ), 1, NULL, "v", "position", "Position to set as the end point, in world space." );
const idEventDef EV_SetEffectLooping( "setEffectLooping", '\0', DOC_TEXT( "Sets or clears the looping flag for the effect." ), 1, NULL, "b", "value", "Value to set the looping flag to." );
const idEventDef EV_UseRenderBounds( "useRenderBounds", '\0', DOC_TEXT( "Sets or clears the useRenderBounds flag for the effect." ), 1, NULL, "b", "value", "Value to set the useRenderBounds flag to." );
const idEventDef EV_EndEffect( "endEffect", '\0', DOC_TEXT( "Stops the effect, and optionally removes any existing particles." ), 1, NULL, "b", "killParticles", "Whether to remove existing particles or not." );	// called endEffect, not stopEffect due to confict with idEntity
*/
CLASS_DECLARATION( sdClientScriptEntity, qwtaClientSprite )
/*	EVENT( EV_SetEffectEndOrigin,			rvClientEffect::Event_SetEffectEndOrigin )	
	EVENT( EV_SetEffectLooping,				rvClientEffect::Event_SetEffectLooping )
	EVENT( EV_UseRenderBounds,				rvClientEffect::Event_UseRenderBounds )
	EVENT( EV_EndEffect,					rvClientEffect::Event_EndEffect )*/
END_CLASS

/*
================
qwtaClientSprite::qwtaClientSprite
================
*/
qwtaClientSprite::qwtaClientSprite( void ) {
	gameLocal.Printf("qwtaClientSprite constructor called.\n");
	Init ( NULL );
}

qwtaClientSprite::qwtaClientSprite( const idMaterial* material ) {
	Init ( material );
}
void qwtaClientSprite::Init( const idMaterial* material ) {

	renderEntityHandle = -1;
	memset( &renderEntity, 0, sizeof( renderEntity ) );

	renderEntity.hModel = renderModelManager->FindModel( "_SPRITE" );
	renderEntity.customShader = material;
	startTime		= -1;

	//gameLocal.RegisterClientEntity ( this );
	//this->renderEntity.shaderParms
/*	memset( &renderEffect, 0, sizeof( renderEffect ) );
	
	effectIndex		= _effectIndex;
	effectDefHandle = -1;
	startTime		= -1;
	endOriginJoint	= INVALID_JOINT;
	viewSuppress    = true;

	monitorSpawnId  = -1;*/
}

/*
================
qwtaClientSprite::~qwtaClientSprite
================
*/
qwtaClientSprite::~qwtaClientSprite( void ) {
	CleanUp();

	//FreeModelDef();
	//gameLocal.UnregisterClientEntity( this );
}

void qwtaClientSprite::Think( void ) {

	if ( renderEntityHandle < 0 ) {
		if ( startTime >= 0 ) {
			UpdateBind( false );
			renderEntityHandle = gameRenderWorld->AddEntityDef( &renderEntity );
			if ( renderEntityHandle < 0 ) {
				Dispose();
			}
		}
		return;
	} else {
		gameRenderWorld->UpdateEntityDef( renderEntityHandle, &renderEntity );
	}

	// If we lost our effect def handle then just remove ourself
	if( renderEntityHandle < 0 ) {
		Dispose();
		return;
	}

	// Dont do anything else if its not a new client frame
	if( !gameLocal.isNewFrame ) {
		return;
	}

	if ( bindMaster.IsValid() && bindMaster->IsInterpolated() ) {
		UpdateBind( true );
	} else {
		UpdateBind( false );
	}

	// Update the actual render effect now
	/*if( gameRenderWorld->UpdateEntityDef( renderEntityHandle, &renderEntity ) ) {
		FreeModelDef();
		Dispose();
		return;
	}*/

	//UpdateSound();
}

/*void qwtaClientSprite::CleanUp( void ) {
	Unbind();

	RemoveClientEntities();


	FreeModelDef();

	sdClientScriptEntity::CleanUp();
}*/

bool qwtaClientSprite::Play( int _startTime, bool _loop, const idVec3& endOrigin ) {
	/*if ( effectIndex < 0 ) {
		return false;
	}*/

	// Initialize the render entity
	if ( bindMaster ) {
		renderEntity_t* renderEnt = bindMaster->GetRenderEntity ( );
		assert( renderEnt );
		
		// Copy suppress values from parent entity
/*		if ( viewSuppress ) {
			renderEffect.allowSurfaceInViewID = bindMaster->GetRenderEntity()->allowSurfaceInViewID;
			renderEffect.suppressSurfaceInViewID = bindMaster->GetRenderEntity()->suppressSurfaceInViewID;
		} else {*/
			//renderEffect.allowSurfaceInViewID = 0;
			//renderEffect.suppressSurfaceInViewID = 0;
		//}
		//renderEffect.weaponDepthHackInViewID = renderEnt->flags.weaponDepthHack;
  	}

	renderEntity.shaderParms[SHADERPARM_RED] = 1.0f;
	renderEntity.shaderParms[SHADERPARM_GREEN] = 1.0f;
	renderEntity.shaderParms[SHADERPARM_BLUE] = 1.0f;
	renderEntity.shaderParms[SHADERPARM_ALPHA] = 1.0f;
	renderEntity.shaderParms[SHADERPARM_BRIGHTNESS] = 1.0f; 
	renderEntity.shaderParms[SHADERPARM_TIMEOFFSET] = -MS2SEC( gameLocal.time );
	//renderEntity.hasEndOrigin = false;//( endOrigin != vec3_origin );
	//renderEntity.endOrigin	  = vec3_origin;//endOrigin;		
	//renderEntity.loop		  = _loop;

	assert( renderEntityHandle < 0 );

	startTime = _startTime;

	return true;	
}

void qwtaClientSprite::Dispose( void ) {
	CleanUp();
	PostEventMS( &EV_Remove, 0 );
}

void qwtaClientSprite::FreeModelDef( void ) {
	//gameEdit->DestroyRenderEntity( renderEntity );
	if ( renderEntityHandle != -1 && gameRenderWorld ) {
		gameRenderWorld->FreeEntityDef( renderEntityHandle );
		renderEntityHandle = -1;
	}
}