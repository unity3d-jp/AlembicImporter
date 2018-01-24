#pragma once

#include <cstdint>

struct  aiConfig;
struct  aiCameraData;
struct  aiXFormData;
struct  aiMeshSummary;
struct  aiMeshSampleSummary;
struct  aiPolyMeshData;
struct  aiSubmeshSummary;
struct  aiSubmeshData;
struct  aiFacesets;

class   aiContext;
class   aiObject;
#ifdef abciImpl
    class aiSchemaBase;
    class aiSampleBase;
    class aiXFormSample;    // : aiSampleBase
    class aiCameraSample;   // : aiSampleBase
    class aiPolyMeshSample; // : aiSampleBase
    class aiPointsSample;   // : aiSampleBase
    class aiCurvesSample;   // : aiSampleBase
    class aiSubDSample;     // : aiSampleBase
#else
    // force make castable
    using aiSchemaBase     = void;
    using aiSampleBase     = void;
    using aiXFormSample    = void;
    using aiCameraSample   = void;
    using aiPolyMeshSample = void;
    using aiPointsSample   = void;
    using aiCurvesSample   = void;
    using aiSubDSample     = void;
#endif

class   aiXForm;    // : aiSchemaBase
class   aiCamera;   // : aiSchemaBase
class   aiPolyMesh; // : aiSchemaBase
class   aiPoints;   // : aiSchemaBase
class   aiCurves;   // : aiSchemaBase
class   aiSubD;     // : aiSchemaBase
class   aiProperty;

struct  aiPolyMeshData;
struct  aiPointsData;
struct  aiCurvesSampleData;
struct  aiSubDSampleData;

enum class aiNormalsMode
{
    ReadFromFile,
    ComputeIfMissing,
    AlwaysCompute,
    Ignore
};

enum class aiTangentsMode
{
    None,
    Smooth,
};

enum class aiTimeSamplingType
{
    Uniform,
    Cyclic,
    Acyclic,
};

enum class aiPropertyType
{
    Unknown,

    // scalar types
    Bool,
    Int,
    UInt,
    Float,
    Float2,
    Float3,
    Float4,
    Float4x4,

    // array types
    BoolArray,
    IntArray,
    UIntArray,
    FloatArray,
    Float2Array,
    Float3Array,
    Float4Array,
    Float4x4Array,

    ScalarTypeBegin  = Bool,
    ScalarTypeEnd    = Float4x4,

    ArrayTypeBegin   = BoolArray,
    ArrayTypeEnd     = Float4x4Array,
};

struct aiConfig
{
    bool swap_handedness = true;
    bool swap_face_winding = false;
    aiNormalsMode normals_mode = aiNormalsMode::ComputeIfMissing;
    aiTangentsMode tangents_mode = aiTangentsMode::None;
    bool cache_tangents_splits = true;
    float aspect_ratio = -1.0f;
    bool force_update = false;
    bool cache_samples = false;
    bool share_vertices = true;
    bool treat_vertex_extra_data_as_static = false;
    bool interpolate_samples = true;
    bool turn_quad_edges = false;
    float vertex_motion_scale = 1.0f;
    bool use_32bit_index_buffer = false;
};

struct aiTimeSamplingData
{
    aiTimeSamplingType type = aiTimeSamplingType::Uniform;
    float start_time = 0.0f;
    float end_time = 0.0f;
    float interval = 1.0f / 30.0f;  // relevant only if type is Uniform or Cyclic
    int numTimes = 0;               // relevant only if type is Acyclic
    double *times = nullptr;        // relevant only if type is Acyclic
};

struct aiXFormData
{
    abcV3 translation = { 0.0f, 0.0f, 0.0f };
    abcV4 rotation = { 0.0f, 0.0f, 0.0f, 1.0f }; // quaternion
    abcV3 scale = { 1.0f, 1.0f, 1.0f };
    bool inherits = false;
};

struct aiCameraData
{
    float near_clipping_plane = 0.3f;
    float far_clipping_plane = 1000.0f;
    float field_of_view = 60.0f;      // in degree. vertical one
    float aspect_ratio = 16.0f / 9.0f;

    float focus_distance = 5.0f;     // in cm
    float focal_length = 0.0f;       // in mm
    float aperture = 2.4f;          // in cm. vertical one
};

struct aiMeshSummary
{
    int32_t topology_variance = 0;
    int32_t peak_vertex_count = 0;
    int32_t peak_index_count = 0;
    int32_t peak_triangulated_index_count = 0;
    int32_t peak_submesh_count = 0;
};

struct aiMeshSampleSummary
{
    int32_t splitCount = 0;
    bool hasNormals = false;
    bool hasUVs = false;
    bool hasTangents = false;
    bool hasVelocities = false;
};

struct aiPolyMeshData
{
    abcV3 *points = nullptr;
    abcV3 *velocities = nullptr;
    abcV2 *interpolated_velocities_xy = nullptr;
    abcV2 *interpolated_velocities_z = nullptr;
    abcV3 *normals = nullptr;
    abcV2 *uvs = nullptr;
    abcV4 *tangents = nullptr;

    int *indices = nullptr;
    int *normal_indices = nullptr;
    int *uv_indices = nullptr;
    int *faces = nullptr;

    int position_count = 0;
    int normal_count = 0;
    int uv_count = 0;

    int index_count = 0;
    int normal_index_count = 0;
    int uv_index_count = 0;
    int face_count = 0;

    int triangulated_index_count = 0;

    abcV3 center = { 0.0f, 0.0f, 0.0f };
    abcV3 size = { 0.0f, 0.0f, 0.0f };
};

struct aiSubmeshSummary
{
    int32_t index = 0;
    int32_t split_index = 0;
    int32_t split_submesh_index = 0;
    int32_t faceset_index = -1;
    int32_t triangle_count = 0;
};

struct aiSubmeshData
{
    int32_t *indices = nullptr;

};

struct aiPointsSummary
{
    bool has_velocity = false;
    bool positionIs_constant = false;
    bool id_is_constant = false;
    int32_t peak_count = 0;
    uint64_t min_id = 0;
    uint64_t max_id = 0;
    abcV3 bounds_center = { 0.0f, 0.0f, 0.0f };
    abcV3 bounds_extents = { 0.0f, 0.0f, 0.0f };
};

struct aiPointsData
{
    abcV3       *points = nullptr;
    abcV3       *velocities = nullptr;
    uint64_t    *ids = nullptr;
    int32_t     count = 0;

    abcV3       center = { 0.0f, 0.0f, 0.0f };
    abcV3       size = { 0.0f, 0.0f, 0.0f };
};

struct aiPropertyData
{
    void *data = nullptr;
    int size = 0;
    aiPropertyType type = aiPropertyType::Unknown;

    aiPropertyData() {}
    aiPropertyData(void *d, int s, aiPropertyType t) : data(d), size(s), type(t) {}
};


using aiNodeEnumerator = void (abciSTDCall*)(aiObject *node, void *user_data);
using aiConfigCallback =  void (abciSTDCall*)(void *csObj, aiConfig *config);
using aiSampleCallback = void (abciSTDCall*)(void *csObj, aiSampleBase *sample, bool topology_changed);

abciAPI abcSampleSelector aiTimeToSampleSelector(float time);
abciAPI abcSampleSelector aiIndexToSampleSelector(int64_t index);
abciAPI void            aiCleanup();
abciAPI void            aiClearContextsWithPath(const char *path);
abciAPI aiContext*      aiCreateContext(int uid);
abciAPI void            aiDestroyContext(aiContext* ctx);

abciAPI bool            aiLoad(aiContext* ctx, const char *path);
abciAPI void            aiSetConfig(aiContext* ctx, const aiConfig* conf);
abciAPI float           aiGetStartTime(aiContext* ctx);
abciAPI float           aiGetEndTime(aiContext* ctx);
abciAPI int             aiGetFrameCount(aiContext* ctx);
abciAPI aiObject*       aiGetTopObject(aiContext* ctx);
abciAPI void            aiDestroyObject(aiContext* ctx, aiObject* obj);
abciAPI int             aiGetNumTimeSamplings(aiContext* ctx);
abciAPI void            aiGetTimeSampling(aiContext* ctx, int i, aiTimeSamplingData *dst);

abciAPI void            aiUpdateSamples(aiContext* ctx, float time);

abciAPI void            aiEnumerateChild(aiObject *obj, aiNodeEnumerator e, void *user_data);
abciAPI const char*     aiGetNameS(aiObject* obj);
abciAPI int             aiGetNumChildren(aiObject* obj);
abciAPI aiObject*       aiGetChild(aiObject* obj, int i);

abciAPI void            aiSchemaSetSampleCallback(aiSchemaBase* schema, aiSampleCallback cb, void* arg);
abciAPI void            aiSchemaSetConfigCallback(aiSchemaBase* schema, aiConfigCallback cb, void* arg);
abciAPI aiSampleBase*   aiSchemaUpdateSample(aiSchemaBase* schema, const abcSampleSelector *ss);
abciAPI aiSampleBase*   aiSchemaGetSample(aiSchemaBase* schema, const abcSampleSelector *ss);
abciAPI int             aiSchemaGetNumSamples(aiSchemaBase* schema);

abciAPI aiXForm*        aiGetXForm(aiObject* obj);
abciAPI void            aiXFormGetData(aiXFormSample* sample, aiXFormData *dst);

abciAPI aiPolyMesh*     aiGetPolyMesh(aiObject* obj);
abciAPI void            aiPolyMeshGetSummary(aiPolyMesh* schema, aiMeshSummary* summary);
abciAPI void            aiPolyMeshGetSampleSummary(aiPolyMeshSample* sample, aiMeshSampleSummary* summary, bool force_refresh=false);
// return pointers to actual data. no conversions (swap handedness / faces) are applied.
abciAPI void            aiPolyMeshGetDataPointer(aiPolyMeshSample* sample, aiPolyMeshData* data);
// all these below aiPolyMesh* are mesh splitting functions
abciAPI int             aiPolyMeshGetVertexBufferLength(aiPolyMeshSample* sample, int split_index);
abciAPI void            aiPolyMeshFillVertexBuffer(aiPolyMeshSample* sample, int split_index, aiPolyMeshData* data);
abciAPI int             aiPolyMeshPrepareSubmeshes(aiPolyMeshSample* sample);
abciAPI int             aiPolyMeshGetSplitSubmeshCount(aiPolyMeshSample* sample, int split_index);
abciAPI bool            aiPolyMeshGetNextSubmesh(aiPolyMeshSample* sample, aiSubmeshSummary* summary);
abciAPI void            aiPolyMeshFillSubmeshIndices(aiPolyMeshSample* sample, const aiSubmeshSummary* summary, aiSubmeshData* data);

abciAPI aiCamera*       aiGetCamera(aiObject* obj);
abciAPI void            aiCameraGetData(aiCameraSample* sample, aiCameraData *dst);

abciAPI aiPoints*       aiGetPoints(aiObject* obj);
abciAPI void            aiPointsGetSummary(aiPoints *schema, aiPointsSummary *summary);
abciAPI void            aiPointsSetSort(aiPoints* schema, bool v);
abciAPI void            aiPointsSetSortBasePosition(aiPoints* schema, abcV3 v);
abciAPI void            aiPointsGetDataPointer(aiPointsSample* sample, aiPointsData *dst);
abciAPI void            aiPointsCopyData(aiPointsSample* sample, aiPointsData *dst);

abciAPI int             aiSchemaGetNumProperties(aiSchemaBase* schema);
abciAPI aiProperty*     aiSchemaGetPropertyByIndex(aiSchemaBase* schema, int i);
abciAPI aiProperty*     aiSchemaGetPropertyByName(aiSchemaBase* schema, const char *name);
abciAPI const char*     aiPropertyGetNameS(aiProperty* prop);
abciAPI aiPropertyType  aiPropertyGetType(aiProperty* prop);
abciAPI void            aiPropertyCopyData(aiProperty* prop, const abcSampleSelector *ss, aiPropertyData *data);
