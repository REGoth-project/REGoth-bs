#include "$ENGINE$\GBufferOutput.bslinc"
#include "$ENGINE$\PerCameraData.bslinc"
#include "$ENGINE$\PerObjectData.bslinc"
#include "$ENGINE$\VertexInput.bslinc"

shader Surface
{
  mixin GBufferOutput;
  mixin PerCameraData;
  mixin PerObjectData;
  mixin VertexInput;

  code
  {
    [alias(gAlbedoTex)] SamplerState gAlbedoSamp;

    Texture2D gAlbedoTex = white;

    struct ZenVertexInput
    {
      #if POSITION_2D
        float2 position : POSITION;
      #else
        float3 position : POSITION;
      #endif

      float2 uv0 : TEXCOORD0;

      #if LIGHTING_DATA
        float3 normal : NORMAL; // Note: Half-precision could be used
        float4 tangent : TANGENT; // Note: Half-precision could be used
      #endif

      #if SKINNED
        uint4 blendIndices : BLENDINDICES;
        float4 blendWeights : BLENDWEIGHT;
      #endif

      #if MORPH
        float3 deltaPosition : POSITION1;
        float4 deltaNormal : NORMAL1;
      #endif

      float4 color : COLOR;

    };


    struct ZenVStoFS
    {
      // Position in clip space (multiplied by world-view-projection matrix)
      float4 position : SV_Position;

      // Texture coordinates
      float2 uv0 : TEXCOORD0;

      // Position in world space (multiplied by the world matrix)
      float3 worldPosition : TEXCOORD1;

      // Normal vector in world space (multiplied by the world matrix)
      float3 worldNormal : TEXCOORD2;

      #if LIGHTING_DATA
        float3 tangentToWorldZ : NORMAL; // Note: Half-precision could be used
        float4 tangentToWorldX : TANGENT; // Note: Half-precision could be used
      #endif

      #if CLIP_POS
        float4 clipPos : TEXCOORD2;
      #endif

      // Per-vertex color
      float4 color : COLOR;
    };

    VertexInput toVertexInput(ZenVertexInput input)
    {
      VertexInput bsfInput;
      bsfInput.position = input.position;
      bsfInput.uv0 = input.uv0;

      #if LIGHTING_DATA
      bsfInput.normal = input.normal;
      bsfInput.tangent = input.tangent;
      #endif

      #if SKINNED
      bsfInput.blendIndices = input.blendIndices;
      bsfInput.blendWeights = input.blendWeights;
      #endif

      #if MORPH
      bsfInput.deltaPosition = input.deltaPosition;
      bsfInput.deltaNormal = input.deltaNormal;
      #endif

      return bsfInput;
    }

    ZenVStoFS vsmain(ZenVertexInput input)
    {
      ZenVStoFS output;


      VertexInput bsfInput = toVertexInput(input);
      VertexIntermediate intermediate = getVertexIntermediate(bsfInput);

      float4 worldPosition = getVertexWorldPosition(bsfInput, intermediate);

      output.uv0           = input.uv0;
      output.worldPosition = worldPosition.xyz;
      output.position      = mul(gMatViewProj, worldPosition);
      output.worldNormal   = mul((float3x3)gMatWorld, input.normal);
      output.color         = float4(input.color.rgb, 1.0);

      #if LIGHTING_DATA
        output.tangentToWorldZ = intermediate.worldNormal;
        output.tangentToWorldX = intermediate.worldTangent;
      #endif

      #if CLIP_POS
        output.clipPos = result.position;
      #endif

      return output;
    }

    void fsmain(in ZenVStoFS input,
                out float3 OutSceneColor : SV_Target0,
                out float4 OutGBufferA : SV_Target1,
                out float4 OutGBufferB : SV_Target2,
                out float2 OutGBufferC : SV_Target3,
                out float OutGBufferD : SV_Target4)
    {
      float2 uv = input.uv0;

      SurfaceData surfaceData;
      surfaceData.albedo          = gAlbedoTex.Sample(gAlbedoSamp, uv);
      surfaceData.worldNormal.xyz = input.worldNormal;
      surfaceData.roughness       = 1.0f;
      surfaceData.metalness       = 0.0f;
      surfaceData.mask            = gLayer;

      clip(surfaceData.albedo.a - 0.5f);

      encodeGBuffer(surfaceData, OutGBufferA, OutGBufferB, OutGBufferC, OutGBufferD);

      OutSceneColor = surfaceData.albedo.rgb * input.color.rgb * gColor.rgb;
    }
  };
};

subshader DeferredDirectLighting
{
  // An example shader that implements the basic Lambert BRDF
  mixin StandardBRDF
  {
    code
    {
      float3 evaluateStandardBRDF(float3 V,
                                  float3 L,
                                  float specLobeEnergy,
                                  SurfaceData surfaceData)
      {
        return surfaceData.albedo.rgb / 3.14f;
      }
    };
  };
};
