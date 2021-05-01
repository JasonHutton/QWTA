
#ifndef __MODEL_LOCAL_H__
#define __MODEL_LOCAL_H__

/*
===============================================================================

	Static model

===============================================================================
*/

#include "Model.h"
#include "RenderWorld.h"

class idRenderModelStatic : public idRenderModel {
public:
	// the inherited public interface
								idRenderModelStatic();
	virtual						~idRenderModelStatic();

	virtual void				InitFromFile( const char *fileName );
	virtual void				PartialInitFromFile( const char *fileName );
	virtual void				PurgeModel();
	virtual void				Reset() {};
	virtual void				LoadModel();
	virtual bool				IsLoaded() const;
	virtual void				SetReferencedOutsideLevelLoad( bool referenced );
	virtual bool				IsReferencedOutsideLevelLoad() const;
	virtual void				SetLevelLoadReferenced( bool referenced );
	virtual bool				IsLevelLoadReferenced() const;
	virtual void				TouchData();
	virtual void				InitEmpty( const char *name );
	virtual void				AddSurface( modelSurface_t surface );
	virtual void				FinishSurfaces( bool duplicateGeometry = true, bool cleanupGeometry = true, bool concatDupeGeom = false );
	virtual bool				Validate( void ) const;
	virtual void				FreeVertexCache();
	virtual void				DirtyVertexAmbientCache();
	virtual const char *		Name() const;
	virtual void				Print() const;
	virtual void				List( bool csv ) const;
	virtual void				TexUsage() const;

	virtual void				Media( idFile *reportfile, sdHashMapGeneric< const idImage*, imageuseinfo > &texref ) const;
	virtual int					Memory() const;
	virtual unsigned			Timestamp() const;
	virtual int					NumSurfaces() const;
	virtual int					NumBaseSurfaces() const;
	virtual const modelSurface_t *Surface( int surfaceNum ) const;
	virtual srfTriangles_t *	AllocSurfaceTriangles( int numVerts, int numIndexes ) const;
	virtual void				FreeSurfaceTriangles( srfTriangles_t *tris ) const;
	virtual bool				IsStaticWorldModel() const;
	virtual dynamicModel_t		IsDynamicModel() const;
	virtual bool				IsDefaultModel() const;
	virtual bool				IsReloadable() const;
	virtual void				InstantiateDynamicModel( class idRenderEntityLocal *def, const struct viewDef_s *view, int lod );
	virtual void				UpdateDeferredSurface( class idRenderEntityLocal *def, modelSurface_t *surf ) {}
	virtual int					NumJoints( void ) const;
	virtual const idMD5Joint *	GetJoints( void ) const;
	virtual jointHandle_t		GetJointHandle( const char *name ) const;
	virtual const char *		GetJointName( jointHandle_t handle ) const;
	virtual const idJointQuat *	GetDefaultPose( void ) const;
	virtual int					NearestJoint( int surfaceNum, int a, int b, int c ) const;
	virtual int					NumGUISurfaces( void ) const;
	virtual const guiSurface_t*	GetGUISurface( int guiSurfaceNum ) const;
	virtual idBounds			Bounds( const struct renderEntity_t *ent ) const;
	virtual void				ReadFromDemoFile( class idDemoFile *f );
	virtual void				WriteToDemoFile( class idDemoFile *f );
	virtual float				DepthHack() const;

	virtual void				SetBounds( idBounds const &bb );

	virtual void				PurgePartialLoadableImages( void );
	virtual void				LoadPartialLoadableImages( bool blocking = false );
	virtual bool				IsFinishedPartialLoading( void ) const;

	virtual int					GetCurrentLod( void ) const;
	virtual void				SetCurrentLod( const int lod );
	virtual bool				UpdateLod( const struct renderEntity_t *ent, const struct viewDef_s *view, idRenderModel *cachedModel, int& newLod ) const;

	virtual bool				NeedsReinstantiating( class idRenderEntityLocal *def, const struct viewDef_s *view, int lod ) const;

	virtual int					FindSurfaceId( const char *surfaceName );

	virtual idList<int>*		GetFixedAreas( void );

	virtual void				SetFixedAreas( idList<int> const &a );
};

/*
===============================================================================

	MD5 animated model

===============================================================================
*/

class idMD5Mesh;

class idRenderModelMD5 : public idRenderModelStatic {
public:
									idRenderModelMD5();
	virtual							~idRenderModelMD5();

	virtual void					InitFromFile( const char *fileName );
	virtual dynamicModel_t			IsDynamicModel() const;
	virtual idBounds				Bounds( const struct renderEntity_t *ent ) const;
	virtual void					Print() const;
	virtual void					FreeVertexCache();
	virtual void					DirtyVertexAmbientCache();
	virtual void					List( bool csv ) const;
	virtual void					Media( idFile *reportfile, sdHashMapGeneric< const idImage*, imageuseinfo > &texref ) const;
	virtual void					TouchData();
	virtual void					PurgeModel();
	virtual void					LoadModel();
	virtual int						Memory() const;
	virtual void					InstantiateDynamicModel( idRenderEntityLocal *def, const struct viewDef_s *view, int lod );
	virtual void					UpdateDeferredSurface( class idRenderEntityLocal *def, modelSurface_t *surf );

	virtual int						NumJoints( void ) const;
	virtual const idMD5Joint *		GetJoints( void ) const;
	virtual jointHandle_t			GetJointHandle( const char *name ) const;
	virtual const char *			GetJointName( jointHandle_t handle ) const;
	virtual const idJointQuat *		GetDefaultPose( void ) const;
	virtual int						NearestJoint( int surfaceNum, int a, int b, int c ) const;
	virtual bool					UpdateLod( const struct renderEntity_t *ent, const struct viewDef_s *view, idRenderModel *cachedModel, int& newLod ) const;
	virtual int						NumMeshes( const int lod = 0 ) const;
	virtual idMD5Mesh*				Mesh( int i, const int lod = 0 );
	virtual const idMD5Mesh*		Mesh( int i, const int lod = 0 ) const;
	virtual int						NumLods() const;
	virtual bool					NeedsReinstantiating( class idRenderEntityLocal *def, const struct viewDef_s *view, int lod ) const;

	virtual int						FindSurfaceId( const char *surfaceName );

	virtual idBounds				CalcMeshBounds( int meshIndex, const idJointMat *joints, const idVec3 &offset, const idMat3 &axis, bool useDefaultAnim );

	virtual bool					Validate( void ) const;

	virtual void					PurgePartialLoadableImages( void );
	virtual void					LoadPartialLoadableImages( bool blocking = false );
	virtual bool					IsFinishedPartialLoading( void ) const;
};

#endif /* !__MODEL_LOCAL_H__ */
