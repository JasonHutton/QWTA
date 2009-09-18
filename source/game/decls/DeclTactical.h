// Copyright (C) 2007 Id Software, Inc.
//

#ifndef __DECLTACTICAL_H__
#define __DECLTACTICAL_H__

class sdDeclTactical : public idDecl {
public:
							sdDeclTactical( void );
	virtual					~sdDeclTactical( void );

	virtual const char*		DefaultDefinition( void ) const;
	virtual bool			Parse( const char *text, const int textLength );
	virtual void			FreeData( void );

	int						GetNumMaps( void ) const { return maps.Num(); }
	const char*				GetMap( int index ) const { return maps[ index ]; }
	const idMaterial*		GetBackdrop( void ) const;

private:
	idStrList				maps;
	idStr					backdrop;
};

#endif // __DECLTACTICAL_H__
