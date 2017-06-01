#include "Graphics/Material.h"
#include "Graphics/GLSLProgram.h"

#include <assert.h>

namespace Enjon {

	//------------------------------------------------------------------------
	Material::Material()
		: mShader(nullptr)
	{
	}

	//------------------------------------------------------------------------
	Material::~Material()
	{
	} 
	
	//------------------------------------------------------------------------
	
	void Material::SetTexture(const TextureSlotType& type, const AssetHandle<Texture>& textureHandle)
	{
		assert((u32)type < (u32)TextureSlotType::Count);
		mTextureHandles[(u32)type] = textureHandle;
	}

	AssetHandle<Texture> Material::GetTexture(const TextureSlotType& type) const
	{ 
		assert((u32)type < (u32)TextureSlotType::Count);
		return mTextureHandles[(u32)type];
	}

	//------------------------------------------------------------------------
	void Material::SetColor(TextureSlotType type, const ColorRGBA16& color)
	{
		assert((u32)type < (u32)TextureSlotType::Count);
		mColors[(u32)type] = color;
	}

	//------------------------------------------------------------------------
	ColorRGBA16 Material::GetColor(TextureSlotType type) const
	{
		assert((u32)type < (u32)TextureSlotType::Count);
		return mColors[(u32)type];
	}

	//------------------------------------------------------------------------

	//GLSLProgram* Material::GetShader() 
	//{
	//	return mShader;
	//}

	//------------------------------------------------------------------------

	//void Material::SetShader(GLSLProgram* shader)
	//{
	//	mShader = shader;

	//}

	//========================================================================

	void Material::SetShader( const Enjon::Shader* shader )
	{
		mMaterialShader = shader;
	}

	//========================================================================

	const Enjon::Shader* Material::GetShader( ) const
	{
		return mMaterialShader;
	}

	//========================================================================
			
	void Material::AddUniform( ShaderUniform* uniform )
	{
		auto query = mUniforms.find( uniform->GetName( ) );
		if ( query == mUniforms.end( ) )
		{
			mUniforms[ uniform->GetName( ) ] = uniform;
		}
	} 

	//========================================================================

	void Material::SetUniforms( )
	{
		// Make sure that material shader is valid
		assert( mMaterialShader != nullptr );

		// Iterate through uniforms and set with shader
		for ( auto& u : mUniforms )
		{
			switch ( u.second->GetType() )
			{
				case UniformType::TextureSampler:
				{
					UniformTexture* texUni = u.second->Cast< UniformTexture >( );
					const_cast< Enjon::Shader* >( mMaterialShader )->BindTexture( texUni->GetName( ), texUni->GetTexture( ).Get( )->GetTextureId(), texUni->GetLocation( ) );
				} break;

				default:
				{

				} break;
			}
		}
	}

}