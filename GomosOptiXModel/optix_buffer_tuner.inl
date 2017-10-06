template<typename T> struct OptiXBufferTuner
{
    inline static RTbuffer create_buffer(RTcontext native_optix_context, OptiXBufferKind buffer_kind)
    {
        RTbuffer rv;
        LOG_OPTIX_ERROR(native_optix_context,
            rtBufferCreate(native_optix_context, static_cast<unsigned int>(buffer_kind), &rv));
        LOG_OPTIX_ERROR(native_optix_context, rtBufferSetElementSize(rv, sizeof(T)));
        LOG_OPTIX_ERROR(native_optix_context, rtBufferSetFormat(rv, RT_FORMAT_USER));

        return rv;
    }
};

template<> struct OptiXBufferTuner<float>
{
    inline static RTbuffer create_buffer(RTcontext native_optix_context, OptiXBufferKind buffer_kind)
    {
        RTbuffer rv;
        LOG_OPTIX_ERROR(native_optix_context,
            rtBufferCreate(native_optix_context, static_cast<unsigned int>(buffer_kind), &rv));
        LOG_OPTIX_ERROR(native_optix_context, rtBufferSetFormat(rv, RT_FORMAT_FLOAT));

        return rv;
    }
};

template<> struct OptiXBufferTuner<float2>
{
    inline static RTbuffer create_buffer(RTcontext native_optix_context, OptiXBufferKind buffer_kind)
    {
        RTbuffer rv;
        LOG_OPTIX_ERROR(native_optix_context,
            rtBufferCreate(native_optix_context, static_cast<unsigned int>(buffer_kind), &rv));
        LOG_OPTIX_ERROR(native_optix_context, rtBufferSetFormat(rv, RT_FORMAT_FLOAT2));

        return rv;
    }
};

template<> struct OptiXBufferTuner<float3>
{
    inline static RTbuffer create_buffer(RTcontext native_optix_context, OptiXBufferKind buffer_kind)
    {
        RTbuffer rv;
        LOG_OPTIX_ERROR(native_optix_context,
            rtBufferCreate(native_optix_context, static_cast<unsigned int>(buffer_kind), &rv));
        LOG_OPTIX_ERROR(native_optix_context, rtBufferSetFormat(rv, RT_FORMAT_FLOAT3));

        return rv;
    }
};

template<> struct OptiXBufferTuner<float4>
{
    inline static RTbuffer create_buffer(RTcontext native_optix_context, OptiXBufferKind buffer_kind)
    {
        RTbuffer rv;
        LOG_OPTIX_ERROR(native_optix_context,
            rtBufferCreate(native_optix_context, static_cast<unsigned int>(buffer_kind), &rv));
        LOG_OPTIX_ERROR(native_optix_context, rtBufferSetFormat(rv, RT_FORMAT_FLOAT4));

        return rv;
    }
};



template<> struct OptiXBufferTuner<int>
{
    inline static RTbuffer create_buffer(RTcontext native_optix_context, OptiXBufferKind buffer_kind)
    {
        RTbuffer rv;
        LOG_OPTIX_ERROR(native_optix_context,
            rtBufferCreate(native_optix_context, static_cast<unsigned int>(buffer_kind), &rv));
        LOG_OPTIX_ERROR(native_optix_context, rtBufferSetFormat(rv, RT_FORMAT_INT));

        return rv;
    }
};

template<> struct OptiXBufferTuner<int2>
{
    inline static RTbuffer create_buffer(RTcontext native_optix_context, OptiXBufferKind buffer_kind)
    {
        RTbuffer rv;
        LOG_OPTIX_ERROR(native_optix_context,
            rtBufferCreate(native_optix_context, static_cast<unsigned int>(buffer_kind), &rv));
        LOG_OPTIX_ERROR(native_optix_context, rtBufferSetFormat(rv, RT_FORMAT_INT2));

        return rv;
    }
};

template<> struct OptiXBufferTuner<int3>
{
    inline static RTbuffer create_buffer(RTcontext native_optix_context, OptiXBufferKind buffer_kind)
    {
        RTbuffer rv;
        LOG_OPTIX_ERROR(native_optix_context,
            rtBufferCreate(native_optix_context, static_cast<unsigned int>(buffer_kind), &rv));
        LOG_OPTIX_ERROR(native_optix_context, rtBufferSetFormat(rv, RT_FORMAT_INT3));

        return rv;
    }
};

template<> struct OptiXBufferTuner<int4>
{
    inline static RTbuffer create_buffer(RTcontext native_optix_context, OptiXBufferKind buffer_kind)
    {
        RTbuffer rv;
        LOG_OPTIX_ERROR(native_optix_context,
            rtBufferCreate(native_optix_context, static_cast<unsigned int>(buffer_kind), &rv));
        LOG_OPTIX_ERROR(native_optix_context, rtBufferSetFormat(rv, RT_FORMAT_INT4));

        return rv;
    }
};



template<> struct OptiXBufferTuner<unsigned int>
{
    inline static RTbuffer create_buffer(RTcontext native_optix_context, OptiXBufferKind buffer_kind)
    {
        RTbuffer rv;
        LOG_OPTIX_ERROR(native_optix_context,
            rtBufferCreate(native_optix_context, static_cast<unsigned int>(buffer_kind), &rv));
        LOG_OPTIX_ERROR(native_optix_context, rtBufferSetFormat(rv, RT_FORMAT_UNSIGNED_INT));

        return rv;
    }
};

template<> struct OptiXBufferTuner<uint2>
{
    inline static RTbuffer create_buffer(RTcontext native_optix_context, OptiXBufferKind buffer_kind)
    {
        RTbuffer rv;
        LOG_OPTIX_ERROR(native_optix_context,
            rtBufferCreate(native_optix_context, static_cast<unsigned int>(buffer_kind), &rv));
        LOG_OPTIX_ERROR(native_optix_context, rtBufferSetFormat(rv, RT_FORMAT_UNSIGNED_INT2));

        return rv;
    }
};

template<> struct OptiXBufferTuner<uint3>
{
    inline static RTbuffer create_buffer(RTcontext native_optix_context, OptiXBufferKind buffer_kind)
    {
        RTbuffer rv;
        LOG_OPTIX_ERROR(native_optix_context,
            rtBufferCreate(native_optix_context, static_cast<unsigned int>(buffer_kind), &rv));
        LOG_OPTIX_ERROR(native_optix_context, rtBufferSetFormat(rv, RT_FORMAT_UNSIGNED_INT3));

        return rv;
    }
};

template<> struct OptiXBufferTuner<uint4>
{
    inline static RTbuffer create_buffer(RTcontext native_optix_context, OptiXBufferKind buffer_kind)
    {
        RTbuffer rv;
        LOG_OPTIX_ERROR(native_optix_context,
            rtBufferCreate(native_optix_context, static_cast<unsigned int>(buffer_kind), &rv));
        LOG_OPTIX_ERROR(native_optix_context, rtBufferSetFormat(rv, RT_FORMAT_UNSIGNED_INT4));

        return rv;
    }
};



template<> struct OptiXBufferTuner<short>
{
    inline static RTbuffer create_buffer(RTcontext native_optix_context, OptiXBufferKind buffer_kind)
    {
        RTbuffer rv;
        LOG_OPTIX_ERROR(native_optix_context,
            rtBufferCreate(native_optix_context, static_cast<unsigned int>(buffer_kind), &rv));
        LOG_OPTIX_ERROR(native_optix_context, rtBufferSetFormat(rv, RT_FORMAT_SHORT));

        return rv;
    }
};

template<> struct OptiXBufferTuner<short2>
{
    inline static RTbuffer create_buffer(RTcontext native_optix_context, OptiXBufferKind buffer_kind)
    {
        RTbuffer rv;
        LOG_OPTIX_ERROR(native_optix_context,
            rtBufferCreate(native_optix_context, static_cast<unsigned int>(buffer_kind), &rv));
        LOG_OPTIX_ERROR(native_optix_context, rtBufferSetFormat(rv, RT_FORMAT_SHORT2));

        return rv;
    }
};

template<> struct OptiXBufferTuner<short3>
{
    inline static RTbuffer create_buffer(RTcontext native_optix_context, OptiXBufferKind buffer_kind)
    {
        RTbuffer rv;
        LOG_OPTIX_ERROR(native_optix_context,
            rtBufferCreate(native_optix_context, static_cast<unsigned int>(buffer_kind), &rv));
        LOG_OPTIX_ERROR(native_optix_context, rtBufferSetFormat(rv, RT_FORMAT_SHORT3));

        return rv;
    }
};

template<> struct OptiXBufferTuner<short4>
{
    inline static RTbuffer create_buffer(RTcontext native_optix_context, OptiXBufferKind buffer_kind)
    {
        RTbuffer rv;
        LOG_OPTIX_ERROR(native_optix_context,
            rtBufferCreate(native_optix_context, static_cast<unsigned int>(buffer_kind), &rv));
        LOG_OPTIX_ERROR(native_optix_context, rtBufferSetFormat(rv, RT_FORMAT_SHORT4));

        return rv;
    }
};



template<> struct OptiXBufferTuner<unsigned short>
{
    inline static RTbuffer create_buffer(RTcontext native_optix_context, OptiXBufferKind buffer_kind)
    {
        RTbuffer rv;
        LOG_OPTIX_ERROR(native_optix_context,
            rtBufferCreate(native_optix_context, static_cast<unsigned int>(buffer_kind), &rv));
        LOG_OPTIX_ERROR(native_optix_context, rtBufferSetFormat(rv, RT_FORMAT_UNSIGNED_SHORT));

        return rv;
    }
};

template<> struct OptiXBufferTuner<ushort2>
{
    inline static RTbuffer create_buffer(RTcontext native_optix_context, OptiXBufferKind buffer_kind)
    {
        RTbuffer rv;
        LOG_OPTIX_ERROR(native_optix_context,
            rtBufferCreate(native_optix_context, static_cast<unsigned int>(buffer_kind), &rv));
        LOG_OPTIX_ERROR(native_optix_context, rtBufferSetFormat(rv, RT_FORMAT_UNSIGNED_SHORT2));

        return rv;
    }
};

template<> struct OptiXBufferTuner<ushort3>
{
    inline static RTbuffer create_buffer(RTcontext native_optix_context, OptiXBufferKind buffer_kind)
    {
        RTbuffer rv;
        LOG_OPTIX_ERROR(native_optix_context,
            rtBufferCreate(native_optix_context, static_cast<unsigned int>(buffer_kind), &rv));
        LOG_OPTIX_ERROR(native_optix_context, rtBufferSetFormat(rv, RT_FORMAT_UNSIGNED_SHORT3));

        return rv;
    }
};

template<> struct OptiXBufferTuner<ushort4>
{
    inline static RTbuffer create_buffer(RTcontext native_optix_context, OptiXBufferKind buffer_kind)
    {
        RTbuffer rv;
        LOG_OPTIX_ERROR(native_optix_context,
            rtBufferCreate(native_optix_context, static_cast<unsigned int>(buffer_kind), &rv));
        LOG_OPTIX_ERROR(native_optix_context, rtBufferSetFormat(rv, RT_FORMAT_UNSIGNED_SHORT4));

        return rv;
    }
};



template<> struct OptiXBufferTuner<char>
{
    inline static RTbuffer create_buffer(RTcontext native_optix_context, OptiXBufferKind buffer_kind)
    {
        RTbuffer rv;
        LOG_OPTIX_ERROR(native_optix_context,
            rtBufferCreate(native_optix_context, static_cast<unsigned int>(buffer_kind), &rv));
        LOG_OPTIX_ERROR(native_optix_context, rtBufferSetFormat(rv, RT_FORMAT_BYTE));

        return rv;
    }
};

template<> struct OptiXBufferTuner<char2>
{
    inline static RTbuffer create_buffer(RTcontext native_optix_context, OptiXBufferKind buffer_kind)
    {
        RTbuffer rv;
        LOG_OPTIX_ERROR(native_optix_context,
            rtBufferCreate(native_optix_context, static_cast<unsigned int>(buffer_kind), &rv));
        LOG_OPTIX_ERROR(native_optix_context, rtBufferSetFormat(rv, RT_FORMAT_BYTE2));

        return rv;
    }
};

template<> struct OptiXBufferTuner<char3>
{
    inline static RTbuffer create_buffer(RTcontext native_optix_context, OptiXBufferKind buffer_kind)
    {
        RTbuffer rv;
        LOG_OPTIX_ERROR(native_optix_context,
            rtBufferCreate(native_optix_context, static_cast<unsigned int>(buffer_kind), &rv));
        LOG_OPTIX_ERROR(native_optix_context, rtBufferSetFormat(rv, RT_FORMAT_BYTE3));

        return rv;
    }
};

template<> struct OptiXBufferTuner<char4>
{
    inline static RTbuffer create_buffer(RTcontext native_optix_context, OptiXBufferKind buffer_kind)
    {
        RTbuffer rv;
        LOG_OPTIX_ERROR(native_optix_context,
            rtBufferCreate(native_optix_context, static_cast<unsigned int>(buffer_kind), &rv));
        LOG_OPTIX_ERROR(native_optix_context, rtBufferSetFormat(rv, RT_FORMAT_BYTE4));

        return rv;
    }
};



template<> struct OptiXBufferTuner<unsigned char>
{
    inline static RTbuffer create_buffer(RTcontext native_optix_context, OptiXBufferKind buffer_kind)
    {
        RTbuffer rv;
        LOG_OPTIX_ERROR(native_optix_context,
            rtBufferCreate(native_optix_context, static_cast<unsigned int>(buffer_kind), &rv));
        LOG_OPTIX_ERROR(native_optix_context, rtBufferSetFormat(rv, RT_FORMAT_UNSIGNED_BYTE));

        return rv;
    }
};

template<> struct OptiXBufferTuner<uchar2>
{
    inline static RTbuffer create_buffer(RTcontext native_optix_context, OptiXBufferKind buffer_kind)
    {
        RTbuffer rv;
        LOG_OPTIX_ERROR(native_optix_context,
            rtBufferCreate(native_optix_context, static_cast<unsigned int>(buffer_kind), &rv));
        LOG_OPTIX_ERROR(native_optix_context, rtBufferSetFormat(rv, RT_FORMAT_UNSIGNED_BYTE2));

        return rv;
    }
};

template<> struct OptiXBufferTuner<uchar3>
{
    inline static RTbuffer create_buffer(RTcontext native_optix_context, OptiXBufferKind buffer_kind)
    {
        RTbuffer rv;
        LOG_OPTIX_ERROR(native_optix_context,
            rtBufferCreate(native_optix_context, static_cast<unsigned int>(buffer_kind), &rv));
        LOG_OPTIX_ERROR(native_optix_context, rtBufferSetFormat(rv, RT_FORMAT_UNSIGNED_BYTE3));

        return rv;
    }
};

template<> struct OptiXBufferTuner<uchar4>
{
    inline static RTbuffer create_buffer(RTcontext native_optix_context, OptiXBufferKind buffer_kind)
    {
        RTbuffer rv;
        LOG_OPTIX_ERROR(native_optix_context,
            rtBufferCreate(native_optix_context, static_cast<unsigned int>(buffer_kind), &rv));
        LOG_OPTIX_ERROR(native_optix_context, rtBufferSetFormat(rv, RT_FORMAT_UNSIGNED_BYTE4));

        return rv;
    }
};



template<> struct OptiXBufferTuner<bufferId>
{
    inline static RTbuffer create_buffer(RTcontext native_optix_context, OptiXBufferKind buffer_kind)
    {
        RTbuffer rv;
        LOG_OPTIX_ERROR(native_optix_context,
            rtBufferCreate(native_optix_context, static_cast<unsigned int>(buffer_kind), &rv));
        LOG_OPTIX_ERROR(native_optix_context, rtBufferSetFormat(rv, RT_FORMAT_BUFFER_ID));

        return rv;
    }
};