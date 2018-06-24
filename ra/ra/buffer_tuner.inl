
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
            rtBufferSetFormat(rv, RT_FORMAT_FLOAT));

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
            rtBufferSetFormat(rv, RT_FORMAT_FLOAT2));

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
            rtBufferSetFormat(rv, RT_FORMAT_FLOAT3));

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
            rtBufferSetFormat(rv, RT_FORMAT_FLOAT4));

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
            rtBufferSetFormat(rv, RT_FORMAT_INT));

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
            rtBufferSetFormat(rv, RT_FORMAT_INT2));

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
            rtBufferSetFormat(rv, RT_FORMAT_INT3));

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
            rtBufferSetFormat(rv, RT_FORMAT_INT4));

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
            rtBufferSetFormat(rv, RT_FORMAT_UNSIGNED_INT));

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
            rtBufferSetFormat(rv, RT_FORMAT_UNSIGNED_INT2));

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
            rtBufferSetFormat(rv, RT_FORMAT_UNSIGNED_INT3));

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
            rtBufferSetFormat(rv, RT_FORMAT_UNSIGNED_INT4));

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
            rtBufferSetFormat(rv, RT_FORMAT_SHORT));

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
            rtBufferSetFormat(rv, RT_FORMAT_SHORT2));

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
            rtBufferSetFormat(rv, RT_FORMAT_SHORT3));

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
            rtBufferSetFormat(rv, RT_FORMAT_SHORT4));

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
            rtBufferSetFormat(rv, RT_FORMAT_UNSIGNED_SHORT));

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
            rtBufferSetFormat(rv, RT_FORMAT_UNSIGNED_SHORT2));

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
            rtBufferSetFormat(rv, RT_FORMAT_UNSIGNED_SHORT3));

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
            rtBufferSetFormat(rv, RT_FORMAT_UNSIGNED_SHORT4));

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
            rtBufferSetFormat(rv, RT_FORMAT_BYTE));

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
            rtBufferSetFormat(rv, RT_FORMAT_BYTE2));

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
            rtBufferSetFormat(rv, RT_FORMAT_BYTE3));

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
            rtBufferSetFormat(rv, RT_FORMAT_BYTE4));

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
            rtBufferSetFormat(rv, RT_FORMAT_UNSIGNED_BYTE));

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
            rtBufferSetFormat(rv, RT_FORMAT_UNSIGNED_BYTE2));

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
            rtBufferSetFormat(rv, RT_FORMAT_UNSIGNED_BYTE3));

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
            rtBufferSetFormat(rv, RT_FORMAT_UNSIGNED_BYTE4));

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