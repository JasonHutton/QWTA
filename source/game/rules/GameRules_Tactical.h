
#ifndef __GAME_RULES_GAMERULES_TACTICAL_H__
#define	__GAME_RULES_GAMERULES_TACTICAL_H__

#include "GameRules.h"

class sdGameRulesTacticalNetworkState : public sdGameRulesNetworkState {
public:
								sdGameRulesTacticalNetworkState( void );

	void						MakeDefault( void );

	void						Write( idFile* file ) const;
	void						Read( idFile* file );

	sdTeamInfo*					winningTeam;
};

class sdGameRulesTactical : public sdGameRules {
public:
	CLASS_PROTOTYPE( sdGameRulesTactical );

	enum gameProgression_t {
		GP_FIRST_MATCH,
		GP_RETURN_MATCH,
	};

							sdGameRulesTactical( void );
	virtual					~sdGameRulesTactical( void );

	virtual void			Clear( void );
	virtual void			EndGame( void );

	virtual sdTeamInfo*		GetWinningTeam( void ) const { return winningTeam; }
	virtual void			SetWinner( sdTeamInfo* team );
	
	virtual void			ArgCompletion_StartGame( const idCmdArgs& args, argCompletionCallback_t callback );

	virtual void						ApplyNetworkState( const sdEntityStateNetworkData& newState );
	virtual void						ReadNetworkState( const sdEntityStateNetworkData& baseState, sdEntityStateNetworkData& newState, const idBitMsg& msg ) const;
	virtual void						WriteNetworkState( const sdEntityStateNetworkData& baseState, sdEntityStateNetworkData& newState, idBitMsg& msg ) const;
	virtual bool						CheckNetworkStateChanges( const sdEntityStateNetworkData& baseState ) const;
	virtual sdEntityStateNetworkData*	CreateNetworkStructure( void ) const;

	virtual bool						ChangeMap( const char* mapName );
	virtual userMapChangeResult_e		OnUserStartMap( const char* text, idStr& reason, idStr& mapName );

	virtual const sdDeclLocStr*			GetTypeText( void ) const;

	virtual int							GetGameTime( void ) const;

	virtual void						UpdateClientFromServerInfo( const idDict& serverInfo, bool allowMedia );

	virtual int							GetServerBrowserScore( const sdNetSession& session ) const;

	virtual const char*		GetDemoNameInfo( void ) { return "tactical"; }

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

private:
	sdTeamInfo*				winningTeam;
};

#endif // __GAME_RULES_GAMERULES_TACTICAL_H__