// Copyright (C) 2007 Id Software, Inc.
//

#ifndef __DECLDEPLOYABLEOBJECT_H__
#define __DECLDEPLOYABLEOBJECT_H__

#include "../../framework/declManager.h"
#include "decls/DeclRank.h"

class sdDeclStringMap;

class sdDeclDeployableObject : public idDecl {
public:
									sdDeclDeployableObject( void );
	virtual							~sdDeclDeployableObject( void );

	virtual const char*				DefaultDefinition( void ) const;
	virtual bool					Parse( const char *text, const int textLength );
	virtual void					FreeData( void );

	const char*						GetTitle( void ) const { return title; }
	const char*						GetGuiTitle( void ) const { return guiTitle; }
	const idDeclEntityDef*			GetEntityDef( void ) const { return entity; }
	const idDeclEntityDef*			GetCarrierDef( void ) const { return carrier; }
	float							GetWaitTime( void ) const { return waitTime; }
	const sdDeclStringMap*			GetPlacementInfo( void ) const { return placementInfo; }
	qhandle_t						GetDeploymentMask( void ) const { return mask; }
	float							GetObjectSize( void ) const { return objectSize; }
	float							GetCreditRequired( void ) const { return g_useBaseETQWVehicleCredits.GetBool() ? creditRequired : qwtaCreditRequired; }
	int								GetRankLevelRequired( void ) const { return rankRequired != NULL ? rankRequired->GetLevel() : -1; }
	int								GetForceEscalationRequired( void ) const { return forceEscalationRequired; }
	int								GetLogisticsPointsRequired( void ) const { return logisticsPointsRequired; }
	int								GetAVDBit( void ) const { return avdbit; }
	int								GetTerrainDisallowed( void ) const { return terrainDisallowed; }
	static void						CacheFromDict( const idDict& dict );
	bool							AllowRotation( void ) const { return allowRotation; }

private:
	float							waitTime;
	float							distanceLimit;
	idStr							title;
	idStr							guiTitle;
	const idDeclEntityDef*			entity;
	const idDeclEntityDef*			carrier;
	const sdDeclStringMap*			placementInfo;
	qhandle_t						mask;
	float							objectSize;
	bool							allowRotation;
	float							creditRequired;
	float							qwtaCreditRequired;
	const sdDeclRank*				rankRequired;
	int								forceEscalationRequired;
	int								logisticsPointsRequired;
	int								avdbit;
	int								terrainDisallowed;
};

#endif // __DECLDEPLOYABLEOBJECT_H__
