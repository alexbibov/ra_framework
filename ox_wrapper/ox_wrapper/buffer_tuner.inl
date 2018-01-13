
template<typename T> struct OxBufferTuner
{
    inline static RTbuffer create_buffer(OxBuffer<T> const& optix_buffer_wrapper, OxBufferKind buffer_kind)
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

template<> struct OxBufferTuner<float>
{
    inline static RTbuffer create_buffer(OxBuffer<float> const& optix_buffer_wrapper, OxBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(), 
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(), 
            rtBufferSetFormat(rv, RT_FORMAT_FLOAT));

        return rv;
    }
};

template<> struct OxBufferTuner<float2>
{
    inline static RTbuffer create_buffer(OxBuffer<float2> const& optix_buffer_wrapper, OxBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, RT_FORMAT_FLOAT2));

        return rv;
    }
};

template<> struct OxBufferTuner<float3>
{
    inline static RTbuffer create_buffer(OxBuffer<float3> const& optix_buffer_wrapper, OxBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, RT_FORMAT_FLOAT3));

        return rv;
    }
};

template<> struct OxBufferTuner<float4>
{
    inline static RTbuffer create_buffer(OxBuffer<float4> const& optix_buffer_wrapper, OxBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, RT_FORMAT_FLOAT4));

        return rv;
    }
};



template<> struct OxBufferTuner<int>
{
    inline static RTbuffer create_buffer(OxBuffer<int> const& optix_buffer_wrapper, OxBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, RT_FORMAT_INT));

        return rv;
    }
};

template<> struct OxBufferTuner<int2>
{
    inline static RTbuffer create_buffer(OxBuffer<int2> const& optix_buffer_wrapper, OxBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, RT_FORMAT_INT2));

        return rv;
    }
};

template<> struct OxBufferTuner<int3>
{
    inline static RTbuffer create_buffer(OxBuffer<int3> const& optix_buffer_wrapper, OxBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, RT_FORMAT_INT3));

        return rv;
    }
};

template<> struct OxBufferTuner<int4>
{
    inline static RTbuffer create_buffer(OxBuffer<int4> const& optix_buffer_wrapper, OxBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, RT_FORMAT_INT4));

        return rv;
    }
};



template<> struct OxBufferTuner<unsigned int>
{
    inline static RTbuffer create_buffer(OxBuffer<unsigned int> const& optix_buffer_wrapper, OxBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, RT_FORMAT_UNSIGNED_INT));

        return rv;
    }
};

template<> struct OxBufferTuner<uint2>
{
    inline static RTbuffer create_buffer(OxBuffer<uint2> const& optix_buffer_wrapper, OxBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, RT_FORMAT_UNSIGNED_INT2));

        return rv;
    }
};

template<> struct OxBufferTuner<uint3>
{
    inline static RTbuffer create_buffer(OxBuffer<uint3> const& optix_buffer_wrapper, OxBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, RT_FORMAT_UNSIGNED_INT3));

        return rv;
    }
};

template<> struct OxBufferTuner<uint4>
{
    inline static RTbuffer create_buffer(OxBuffer<uint4> const& optix_buffer_wrapper, OxBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, RT_FORMAT_UNSIGNED_INT4));

        return rv;
    }
};



template<> struct OxBufferTuner<short>
{
    inline static RTbuffer create_buffer(OxBuffer<short> const& optix_buffer_wrapper, OxBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(), 
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(), 
            rtBufferSetFormat(rv, RT_FORMAT_SHORT));

        return rv;
    }
};

template<> struct OxBufferTuner<short2>
{
    inline static RTbuffer create_buffer(OxBuffer<short2> const& optix_buffer_wrapper, OxBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, RT_FORMAT_SHORT2));

        return rv;
    }
};

template<> struct OxBufferTuner<short3>
{
    inline static RTbuffer create_buffer(OxBuffer<short3> const& optix_buffer_wrapper, OxBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, RT_FORMAT_SHORT3));

        return rv;
    }
};

template<> struct OxBufferTuner<short4>
{
    inline static RTbuffer create_buffer(OxBuffer<short4> const& optix_buffer_wrapper, OxBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, RT_FORMAT_SHORT4));

        return rv;
    }
};



template<> struct OxBufferTuner<unsigned short>
{
    inline static RTbuffer create_buffer(OxBuffer<unsigned short> const& optix_buffer_wrapper, OxBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, RT_FORMAT_UNSIGNED_SHORT));

        return rv;
    }
};

template<> struct OxBufferTuner<ushort2>
{
    inline static RTbuffer create_buffer(OxBuffer<ushort2> const& optix_buffer_wrapper, OxBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, RT_FORMAT_UNSIGNED_SHORT2));

        return rv;
    }
};

template<> struct OxBufferTuner<ushort3>
{
    inline static RTbuffer create_buffer(OxBuffer<ushort3> const& optix_buffer_wrapper, OxBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, RT_FORMAT_UNSIGNED_SHORT3));

        return rv;
    }
};

template<> struct OxBufferTuner<ushort4>
{
    inline static RTbuffer create_buffer(OxBuffer<ushort4> const& optix_buffer_wrapper, OxBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, RT_FORMAT_UNSIGNED_SHORT4));

        return rv;
    }
};



template<> struct OxBufferTuner<char>
{
    inline static RTbuffer create_buffer(OxBuffer<char> const& optix_buffer_wrapper, OxBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, RT_FORMAT_BYTE));

        return rv;
    }
};

template<> struct OxBufferTuner<char2>
{
    inline static RTbuffer create_buffer(OxBuffer<char2> const& optix_buffer_wrapper, OxBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, RT_FORMAT_BYTE2));

        return rv;
    }
};

template<> struct OxBufferTuner<char3>
{
    inline static RTbuffer create_buffer(OxBuffer<char3> const& optix_buffer_wrapper, OxBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, RT_FORMAT_BYTE3));

        return rv;
    }
};

template<> struct OxBufferTuner<char4>
{
    inline static RTbuffer create_buffer(OxBuffer<char4> const& optix_buffer_wrapper, OxBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, RT_FORMAT_BYTE4));

        return rv;
    }
};



template<> struct OxBufferTuner<unsigned char>
{
    inline static RTbuffer create_buffer(OxBuffer<unsigned char> const& optix_buffer_wrapper, OxBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, RT_FORMAT_UNSIGNED_BYTE));

        return rv;
    }
};

template<> struct OxBufferTuner<uchar2>
{
    inline static RTbuffer create_buffer(OxBuffer<uchar2> const& optix_buffer_wrapper, OxBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, RT_FORMAT_UNSIGNED_BYTE2));

        return rv;
    }
};

template<> struct OxBufferTuner<uchar3>
{
    inline static RTbuffer create_buffer(OxBuffer<uchar3> const& optix_buffer_wrapper, OxBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, RT_FORMAT_UNSIGNED_BYTE3));

        return rv;
    }
};

template<> struct OxBufferTuner<uchar4>
{
    inline static RTbuffer create_buffer(OxBuffer<uchar4> const& optix_buffer_wrapper, OxBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, RT_FORMAT_UNSIGNED_BYTE4));

        return rv;
    }
};



template<> struct OxBufferTuner<OxBufferId>
{
    inline static RTbuffer create_buffer(OxBuffer<OxBufferId> const& optix_buffer_wrapper, OxBufferKind buffer_kind)
    {
        RTbuffer rv;
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferCreate(optix_buffer_wrapper.nativeOptiXContextHandle(), static_cast<unsigned int>(buffer_kind), &rv));
        THROW_OPTIX_ERROR(optix_buffer_wrapper.nativeOptiXContextHandle(),
            rtBufferSetFormat(rv, RT_FORMAT_BUFFER_ID));

        return rv;
    }
};