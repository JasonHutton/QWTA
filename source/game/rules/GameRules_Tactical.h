
#ifndef __GAME_RULES_GAMERULES_TACTICAL_H__
#define	__GAME_RULES_GAMERULES_TACTICAL_H__

class sdDeclTactical;

#include "GameRules.h"

class sdGameRulesTacticalNetworkState : public sdGameRulesNetworkState {
public:
								sdGameRulesTacticalNetworkState( void );

	void						MakeDefault( void );

	void						Write( idFile* file ) const;
	void						Read( idFile* file );

	sdTeamInfo*					winningTeam;
	sdTeamInfo*					campaignWinningTeam;
};

class sdGameRulesTactical : public sdGameRules {
public:
	CLASS_PROTOTYPE( sdGameRulesTactical );

	enum networkEvent_t {
		EVENT_MAPSTATS = sdGameRules::MAX_NET_EVENTS,
		EVENT_SETTACTICAL,
		MAX_NET_EVENTS,
	};

							sdGameRulesTactical( void );
	virtual					~sdGameRulesTactical( void );

	virtual void			Clear( void );
	virtual void			EndGame( void );
	virtual void			SetWinner( sdTeamInfo* team );
	
	void					SetCampaignWinner( sdTeamInfo* team ) { campaignWinningTeam = team; }

	virtual void			GetBrowserStatusString( idWStr& str, const sdNetSession& netSession ) const;

	virtual int				GetGameTime( void ) const;

	virtual sdTeamInfo*		GetWinningTeam( void ) const { return winningTeam; }
	sdTeamInfo*				GetCampaignWinner( void ) const { return campaignWinningTeam; }

	virtual void			WriteInitialReliableMessages( const sdReliableMessageClientInfoBase& target );

	virtual void			ArgCompletion_StartGame( const idCmdArgs& args, argCompletionCallback_t callback );

	virtual void						ApplyNetworkState( const sdEntityStateNetworkData& newState );
	virtual void						ReadNetworkState( const sdEntityStateNetworkData& baseState, sdEntityStateNetworkData& newState, const idBitMsg& msg ) const;
	virtual void						WriteNetworkState( const sdEntityStateNetworkData& baseState, sdEntityStateNetworkData& newState, idBitMsg& msg ) const;
	virtual bool						CheckNetworkStateChanges( const sdEntityStateNetworkData& baseState ) const;
	virtual sdEntityStateNetworkData*	CreateNetworkStructure( void ) const;

	virtual bool			ParseNetworkMessage( int msgType, const idBitMsg& msg );

	virtual userMapChangeResult_e	OnUserStartMap( const char* text, idStr& reason, idStr& mapName );

	virtual int					GetNumMaps( void ) const		{ return campaignMapData.Num(); }
	virtual const mapData_t*	GetMapData( int index ) const	{ return &campaignMapData[ index ]; }
	virtual const mapData_t*	GetCurrentMapData() const;

	virtual teamMapData_t*	GetTeamData( int team, int data ) { return NULL; }

	virtual void			InitVotes( void );
	virtual const sdDeclLocStr*	GetTypeText( void ) const;

	void					SendCampaignInfo( const sdReliableMessageClientInfoBase& target );
	virtual void			ReadCampaignInfo( const idBitMsg& msg );

	void					SetTactical( const sdDeclTactical* newTactical );
	const sdDeclTactical*	GetTactical( void ) const { return tacticalDecl; }
	void					StartMap( void );

	void					OnMapStatsReceived( int index );

	virtual int				GetServerBrowserScore( const sdNetSession& session ) const;

	virtual void			UpdateClientFromServerInfo( const idDict& serverInfo, bool allowMedia );

	virtual const char*		GetDemoNameInfo( void ) { return "tactical"; }

protected:
	void					SendMapStats( int index, const sdReliableMessageClientInfoBase& target );
	void					ReadMapStats( const idBitMsg& msg );
	bool					SetTactical( const char* text, idStr& mapName );

protected:
	virtual void			GameState_Review( void );
	virtual void			GameState_NextGame( void );
	virtual void			GameState_Warmup( void );	
	virtual void			GameState_Countdown( void );
	virtual void			GameState_GameOn( void );
	virtual void			GameState_NextMap( void );

	virtual void			OnGameState_Review( void );
	virtual void			OnGameState_Countdown( void );
	virtual void			OnGameState_GameOn( void );
	virtual void			OnGameState_NextMap( void );

protected:
	int						currentSubMapIndex;
	const sdDeclTactical*	tacticalDecl;
	sdTeamInfo*				winningTeam;
	sdTeamInfo*				campaignWinningTeam;

	idList< mapData_t >		campaignMapData;
};

#endif // __GAME_RULES_GAMERULES_TACTICAL_H__
