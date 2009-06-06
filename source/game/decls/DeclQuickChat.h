// Copyright (C) 2007 Id Software, Inc.
//

#ifndef __DECLQUICKCHAT_H__
#define __DECLQUICKCHAT_H__

class sdDeclQuickChat : public idDecl {
public:
								sdDeclQuickChat( void );
	virtual						~sdDeclQuickChat( void );

	virtual const char*			DefaultDefinition( void ) const;
	virtual bool				Parse( const char *text, const int textLength );
	virtual void				FreeData( void );

	const idSoundShader*		GetAudio( void ) const { return audio; }
	const sdDeclLocStr*			GetText( void ) const { return text; }
	bool						IsTeam( void ) const { return team; }
	bool						IsFireTeam( void ) const { return fireteam; }
	const char*					GetCallback( void ) const { return callback; }
	int							GetType( void ) const { return type; }
	bool						Check( idEntity* main, idEntity* other ) const { return requirements.Check( main, other ); }
	bool						GetCVarAVD( void ) const { return cvarQuickchatFlags.cvarAVD; }
	bool						GetCVarAVDEnabled( void ) const { return cvarQuickchatFlags.cvarAVDEnabled; }

protected:
	const sdDeclLocStr*			text;
	const idSoundShader*		audio;
	bool						team;
	bool						fireteam;
	idStr						callback;
	int							type;
	sdRequirementContainer		requirements;

	typedef struct cvarQuickchatFlags_s {
		bool					cvarAVD					: 1; // Must be enabled for cvarAVDEnabled to take effect.
		bool					cvarAVDEnabled			: 1; // Disable or enable quickchat option based upon Advanced Vehicle Drop cvar setting.
	} cvarQuickchatFlags_t;

	cvarQuickchatFlags_t		cvarQuickchatFlags;
};

#endif // __DECLQUICKCHAT_H__

