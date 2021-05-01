

//----------------------------------------------------------------
// ClientSprite.h
//----------------------------------------------------------------

#ifndef __GAME_CLIENT_SPRITE_H__
#define __GAME_CLIENT_SPRITE_H__

#include "ClientEntity.h"

class qwtaClientSprite : public sdClientScriptEntity {
public:

	CLASS_PROTOTYPE( qwtaClientSprite );

	qwtaClientSprite ( void );
	qwtaClientSprite ( const idMaterial* material );
	~qwtaClientSprite ( void );

	bool				Play( int startTime, bool loop = false, const idVec3& origin = vec3_origin );
	virtual void			Think( void );
	//virtual void			CleanUp( void );
	void						Dispose( void );

	void					SetGravity( const idVec3& gravity );
protected:
	void					Init( const idMaterial* material );
	void					FreeModelDef( void );

	renderEntity_t			renderEntity;
	qhandle_t				renderEntityHandle;
	int						startTime;
};

typedef rvClientEntityPtr<qwtaClientSprite>	qwtaClientSpritePtr;

#endif // __GAME_CLIENT_SPRITE_H__
