export module Render :StaticSampler;

import RHI;
import DynamicRHI;

namespace RenderSystem {

	export RHISamplerDesc g_VolumeWrapDesc;
	export RHISamplerDesc g_PointClampDesc;
	export RHISamplerDesc g_PointBorderDesc;
	export RHISamplerDesc g_LinearWrapDesc;
	export RHISamplerDesc g_LinearClampDesc;
	export RHISamplerDesc g_LinearBorderDesc;
	export RHISamplerDesc g_AnisoWrapDesc;
	export RHISamplerDesc g_ShadowDesc;

	export void CreateStaticSamplers(void) {

		RHISamplerDescBuilder Builder {};

		g_LinearWrapDesc = Builder
			.Reset()
			.SetFilter(RHIFilter::Trilinear)
			.SetAddressUVW(RHIAddressMode::Wrap)
			.Build();

		g_AnisoWrapDesc = Builder
			.Reset()
			.SetFilter(RHIFilter::Anisotropic)
			.SetMaxAnisotropy(4)
			.SetAddressUVW(RHIAddressMode::Wrap)
			.Build();

		g_ShadowDesc = Builder
			.Reset()
			.SetFilter(RHIFilter::ComparsionPoint)
			.SetCompareFunc(RHISamplerCompareFunc::GREATER_EQUAL)
			.SetAddressUVW(RHIAddressMode::Clamp)
			.Build();

		g_LinearClampDesc = Builder
			.Reset()
			.SetFilter(RHIFilter::Trilinear)
			.SetAddressUVW(RHIAddressMode::Clamp)
			.Build();

		g_VolumeWrapDesc = Builder
			.Reset()
			.SetFilter(RHIFilter::Point)
			.SetAddressUVW(RHIAddressMode::Wrap)
			.Build();

		g_PointClampDesc = Builder
			.Reset()
			.SetFilter(RHIFilter::Point)
			.SetAddressUVW(RHIAddressMode::Clamp)
			.Build();

		g_LinearBorderDesc = Builder
			.Reset()
			.SetFilter(RHIFilter::Trilinear)
			.SetAddressUVW(RHIAddressMode::Border)
			.SetBorderColor(0.f)
			.Build();

		g_PointBorderDesc = Builder
			.Reset()
			.SetFilter(RHIFilter::Point)
			.SetAddressUVW(RHIAddressMode::Border)
			.SetBorderColor(0.f)
			.Build();
	}

}