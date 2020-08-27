
template<typename T> struct RaBufferTuner
{
    inline static RTbuffer create_buffer(RaBuffer<T> const& optix_buffer_wrapper, RaBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, RT_FORMAT_USER));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetElementSize(rv, sizeof(T)));

        return rv;
    }
};

template<> struct RaBufferTuner<float>
{
    inline static RTbuffer create_buffer(RaBuffer<float> const& optix_buffer_wrapper, RaBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, ra::util::RaOptiXFormatTraits<float>::optix_format));

        return rv;
    }
};

template<> struct RaBufferTuner<float2>
{
    inline static RTbuffer create_buffer(RaBuffer<float2> const& optix_buffer_wrapper, RaBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, ra::util::RaOptiXFormatTraits<float2>::optix_format));

        return rv;
    }
};

template<> struct RaBufferTuner<float3>
{
    inline static RTbuffer create_buffer(RaBuffer<float3> const& optix_buffer_wrapper, RaBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, ra::util::RaOptiXFormatTraits<float3>::optix_format));

        return rv;
    }
};

template<> struct RaBufferTuner<float4>
{
    inline static RTbuffer create_buffer(RaBuffer<float4> const& optix_buffer_wrapper, RaBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, ra::util::RaOptiXFormatTraits<float4>::optix_format));

        return rv;
    }
};



template<> struct RaBufferTuner<int>
{
    inline static RTbuffer create_buffer(RaBuffer<int> const& optix_buffer_wrapper, RaBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, ra::util::RaOptiXFormatTraits<int>::optix_format));

        return rv;
    }
};

template<> struct RaBufferTuner<int2>
{
    inline static RTbuffer create_buffer(RaBuffer<int2> const& optix_buffer_wrapper, RaBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, ra::util::RaOptiXFormatTraits<int2>::optix_format));

        return rv;
    }
};

template<> struct RaBufferTuner<int3>
{
    inline static RTbuffer create_buffer(RaBuffer<int3> const& optix_buffer_wrapper, RaBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, ra::util::RaOptiXFormatTraits<int3>::optix_format));

        return rv;
    }
};

template<> struct RaBufferTuner<int4>
{
    inline static RTbuffer create_buffer(RaBuffer<int4> const& optix_buffer_wrapper, RaBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, ra::util::RaOptiXFormatTraits<int4>::optix_format));

        return rv;
    }
};



template<> struct RaBufferTuner<unsigned int>
{
    inline static RTbuffer create_buffer(RaBuffer<unsigned int> const& optix_buffer_wrapper, RaBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, ra::util::RaOptiXFormatTraits<unsigned int>::optix_format));

        return rv;
    }
};

template<> struct RaBufferTuner<uint2>
{
    inline static RTbuffer create_buffer(RaBuffer<uint2> const& optix_buffer_wrapper, RaBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, ra::util::RaOptiXFormatTraits<uint2>::optix_format));

        return rv;
    }
};

template<> struct RaBufferTuner<uint3>
{
    inline static RTbuffer create_buffer(RaBuffer<uint3> const& optix_buffer_wrapper, RaBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, ra::util::RaOptiXFormatTraits<uint3>::optix_format));

        return rv;
    }
};

template<> struct RaBufferTuner<uint4>
{
    inline static RTbuffer create_buffer(RaBuffer<uint4> const& optix_buffer_wrapper, RaBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, ra::util::RaOptiXFormatTraits<uint4>::optix_format));

        return rv;
    }
};



template<> struct RaBufferTuner<short>
{
    inline static RTbuffer create_buffer(RaBuffer<short> const& optix_buffer_wrapper, RaBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, ra::util::RaOptiXFormatTraits<short>::optix_format));

        return rv;
    }
};

template<> struct RaBufferTuner<short2>
{
    inline static RTbuffer create_buffer(RaBuffer<short2> const& optix_buffer_wrapper, RaBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, ra::util::RaOptiXFormatTraits<short2>::optix_format));

        return rv;
    }
};

template<> struct RaBufferTuner<short3>
{
    inline static RTbuffer create_buffer(RaBuffer<short3> const& optix_buffer_wrapper, RaBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, ra::util::RaOptiXFormatTraits<short3>::optix_format));

        return rv;
    }
};

template<> struct RaBufferTuner<short4>
{
    inline static RTbuffer create_buffer(RaBuffer<short4> const& optix_buffer_wrapper, RaBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, ra::util::RaOptiXFormatTraits<short4>::optix_format));

        return rv;
    }
};



template<> struct RaBufferTuner<unsigned short>
{
    inline static RTbuffer create_buffer(RaBuffer<unsigned short> const& optix_buffer_wrapper, RaBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, ra::util::RaOptiXFormatTraits<unsigned short>::optix_format));

        return rv;
    }
};

template<> struct RaBufferTuner<ushort2>
{
    inline static RTbuffer create_buffer(RaBuffer<ushort2> const& optix_buffer_wrapper, RaBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, ra::util::RaOptiXFormatTraits<ushort2>::optix_format));

        return rv;
    }
};

template<> struct RaBufferTuner<ushort3>
{
    inline static RTbuffer create_buffer(RaBuffer<ushort3> const& optix_buffer_wrapper, RaBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, ra::util::RaOptiXFormatTraits<ushort3>::optix_format));

        return rv;
    }
};

template<> struct RaBufferTuner<ushort4>
{
    inline static RTbuffer create_buffer(RaBuffer<ushort4> const& optix_buffer_wrapper, RaBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, ra::util::RaOptiXFormatTraits<ushort4>::optix_format));

        return rv;
    }
};



template<> struct RaBufferTuner<char>
{
    inline static RTbuffer create_buffer(RaBuffer<char> const& optix_buffer_wrapper, RaBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, ra::util::RaOptiXFormatTraits<char>::optix_format));

        return rv;
    }
};

template<> struct RaBufferTuner<char2>
{
    inline static RTbuffer create_buffer(RaBuffer<char2> const& optix_buffer_wrapper, RaBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, ra::util::RaOptiXFormatTraits<char2>::optix_format));

        return rv;
    }
};

template<> struct RaBufferTuner<char3>
{
    inline static RTbuffer create_buffer(RaBuffer<char3> const& optix_buffer_wrapper, RaBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, ra::util::RaOptiXFormatTraits<char3>::optix_format));

        return rv;
    }
};

template<> struct RaBufferTuner<char4>
{
    inline static RTbuffer create_buffer(RaBuffer<char4> const& optix_buffer_wrapper, RaBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, ra::util::RaOptiXFormatTraits<char4>::optix_format));

        return rv;
    }
};



template<> struct RaBufferTuner<unsigned char>
{
    inline static RTbuffer create_buffer(RaBuffer<unsigned char> const& optix_buffer_wrapper, RaBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, ra::util::RaOptiXFormatTraits<unsigned char>::optix_format));

        return rv;
    }
};

template<> struct RaBufferTuner<uchar2>
{
    inline static RTbuffer create_buffer(RaBuffer<uchar2> const& optix_buffer_wrapper, RaBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, ra::util::RaOptiXFormatTraits<uchar2>::optix_format));

        return rv;
    }
};

template<> struct RaBufferTuner<uchar3>
{
    inline static RTbuffer create_buffer(RaBuffer<uchar3> const& optix_buffer_wrapper, RaBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, ra::util::RaOptiXFormatTraits<uchar3>::optix_format));

        return rv;
    }
};

template<> struct RaBufferTuner<uchar4>
{
    inline static RTbuffer create_buffer(RaBuffer<uchar4> const& optix_buffer_wrapper, RaBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, ra::util::RaOptiXFormatTraits<uchar4>::optix_format));

        return rv;
    }
};



template<> struct RaBufferTuner<RaBufferId>
{
    inline static RTbuffer create_buffer(RaBuffer<RaBufferId> const& optix_buffer_wrapper, RaBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, RT_FORMAT_BUFFER_ID));

        return rv;
    }
};