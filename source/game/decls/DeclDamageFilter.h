// Copyright (C) 2007 Id Software, Inc.
//

#ifndef __DECLDAMAGEFILTER_H__
#define __DECLDAMAGEFILTER_H__

class sdDeclTargetInfo;

enum damageFilterMode_t {
	DFM_NORMAL,
	DFM_PERCENT,
};

typedef struct damageFilter_s {
	const sdDeclTargetInfo*		target;
	float						damage;
	float						baseETQW12damage;
	damageFilterMode_t			mode;
	damageFilterMode_t			baseETQW12mode;
	bool						noScale;
} damageFilter_t;

class sdDeclDamageFilter : public idDecl {
public:
								sdDeclDamageFilter( void );
	virtual						~sdDeclDamageFilter( void );

	virtual const char*			DefaultDefinition( void ) const;
	virtual bool				Parse( const char *text, const int textLength );
	virtual void				FreeData( void );

	bool						ParseFilter( damageFilter_t& filter, idParser& src );

	int							GetNumFilters( void ) const { return filters.Num(); }
	const damageFilter_t&		GetFilter( int index ) const { return filters[ index ]; }

protected:
	idList< damageFilter_t >	filters;
};

#endif // __DECLDAMAGEFILTER_H__
