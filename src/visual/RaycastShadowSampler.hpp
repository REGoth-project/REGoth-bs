#pragma once

#include <BsPrerequisites.h>

#include "ShadowSampler.hpp"
#include <RTTI/RTTIUtil.hpp>

#include <Physics/BsPhysicsCommon.h>

namespace REGoth
{
  struct RaycastShadowSample : ShadowSample
  {
    bs::PhysicsQueryHit hit;
  };

  /**
   * Shadow sampler that, starting from the query scene object's position, performs a down-pointing
   * raycast against a given geometry and uses vertex color data of the geometry to derive shadow
   * information.
   *
   * TODO: Add to the description how this is the method used in the original engine.
   */
  class RaycastShadowSampler : public ShadowSampler
  {
  public:
    /**
     * Construct a new instance.
     *
     * @param  mesh     Mesh containing vertex color information.
     * @param  collider Mesh collider against which the raycast queries are performed. It must've
     *                  been created with a physics mesh that was directly derived from #mesh.
     */
    RaycastShadowSampler(const bs::HMesh& mesh, const bs::HMeshCollider& collider);

    /**
     * Obtain shadow information for the given query object by performing a down-pointing raycast
     * into the associated geometry.
     *
     * @param  querySO  The query object. Must have an associated visual representation
     *                  (i. e. renderable component).
     * @param  sample   Reference to which the query result is written.
     *                  Only filled with valid data if the return value is true.
     *
     * @return True if the given object has a renderable component and sampling was successful, false
     *         otherwise.
     */
    bool sampleFor(bs::HSceneObject querySO, ShadowSample& sample) const override;

    bool sampleFor(bs::HSceneObject querySO, RaycastShadowSample& sample) const;

  private:
    struct Face
    {
      bs::UINT32 vertexIdx1;
      bs::UINT32 vertexIdx2;
      bs::UINT32 vertexIdx3;
    };

    using FaceAccessorType = std::function<Face(const bs::MeshData&, bs::UINT32)>;

    using VertexColorUnpackerType = std::function<bs::Color(bs::UINT32)>;

    /**
     * Checks if the the given mesh and collider are valid and if the mesh has vertex colors.
     */
    static void doSanityChecks(const bs::HMesh& mesh, const bs::HMeshCollider& collider);

    /**
     * Utility returning a function that can unpack raw vertex color data being in the format of the
     * vertex colors in the given bs::MeshData.
     */
    static VertexColorUnpackerType getVertexColorUnpackFunction(const bs::MeshData& meshData);

    /**
     * Utility returning a function that can access faces in a bs::MeshData object having the same
     * index type as the given one.
     */
    static FaceAccessorType getFaceAccessor(const bs::MeshData& meshData);

    /**
     * Extracts the brightness per vertex from the given meshData as average over the three vertex
     * color channels.
     */
    void extractBrightnessPerVertex(bs::MeshData& meshData);

    /**
     * Construct a (downward pointing) sample ray which is used for shadow sampling.
     * The method assumes that the query object has a CRenderable component attached and returns
     * false if any errors are encountered.
     */
    bool getSampleRay(bs::HSceneObject querySO, bs::Ray& ray) const;

    bs::HMesh mMesh;
    bs::HMeshCollider mCollider;
    bs::Vector<float> mBrightnessPerVertex;
    FaceAccessorType mFaceAccessor;

  public:
    REGOTH_DECLARE_RTTI(RaycastShadowSampler)

    static bs::SPtr<RaycastShadowSampler> createEmpty();

  private:
    RaycastShadowSampler() = default;  // For RTTI
  };
}  // namespace REGoth