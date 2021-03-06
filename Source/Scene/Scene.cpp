// @file Scene.cpp
// Copyright 2016-2018 John Jackson. All Rights Reserved.

#include "Scene/Scene.h"
#include "SubsystemCatalog.h"
#include "Entity/EntityManager.h"
#include "Asset/SceneAssetLoader.h"
#include "Serialize/EntityArchiver.h"
#include "Engine.h"

namespace Enjon
{ 
	//====================================================================

	Result Scene::SerializeData( ByteBuffer* archiver ) const
	{
		// For serializaing the data, need to write all of the entities in the scene at the moment
		// NOTE(John): This will fail with the current multiple-world setup, since all the entities will be saved into this scene, regardless
		// of the world they belong to. Need to take that into account for this, and the scene will need to know which world context it belongs to
		EntityManager* em = EngineSubsystem( EntityManager );
		if ( em )
		{ 
			// Get all root level entities in default world
			Vector<EntityHandle> rootEntities = em->GetRootLevelEntities( );

			// Write out count of vector
			archiver->Write<u32>( rootEntities.size( ) );

			// Serialize all root level entities into archive
			for ( auto& e : rootEntities )
			{
				EntityArchiver::Serialize( e, archiver );
			}
		}

		return Result::SUCCESS;
	}

	//====================================================================
	
	Result Scene::DeserializeData( ByteBuffer* archiver )
	{
		EntityManager* em = EngineSubsystem( EntityManager );
		if ( em )
		{
			// Read size from buffer
			u32 rootSize = archiver->Read< u32 >( );

			// Deserilaize all root level entities from buffer
			for ( u32 i = 0; i < rootSize; ++i )
			{
				// Deserialize all entities in buffer
				EntityArchiver::Deserialize( archiver ); 
			}
		}

		return Result::SUCCESS; 
	}

	//====================================================================
}
