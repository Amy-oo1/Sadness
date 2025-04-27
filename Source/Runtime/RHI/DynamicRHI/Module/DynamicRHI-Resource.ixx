export module DynamicRHI :Resource;

import Platform;
import Utility;
import Container;

import RHI;


//I will Set Public to op
export template<typename Derived, typename APITag = g_CurrentAPI>
class RHIResourceBase :public MoveAbleOnly {
protected:
	RHIResourceBase(void) = default;


public:
	virtual ~RHIResourceBase(void) = default;
	
	using APITypeTrait = typename RHITypeTraits<APITag>;

	//Empty TODO
};



export template<typename APITag = g_CurrentAPI>
class RHITextureResource final: public RHIResourceBase<RHITextureResource<APITag>, APITag> {
public:
	using APITypeTrait = typename RHITypeTraits<APITag>;

	~RHITextureResource(void) = default;

	APITypeTrait::TextureResourcePtr m_Native { nullptr };

	RHITextureFlags Flags { RHITextureFlags::UNKNOWN };
	RHIPixelFormat Format { RHIPixelFormat::UNKNOWN };
	Uint32 MipCount { 1 };
	Uint32 SamplerCount { 1 };
	Optional<RHIClearValue>  ClearValue { NullOpt };
	RHIExtent3D Size { 0,0,0 };

};

export using RHITexture = RHITextureResource<g_CurrentAPI>;
