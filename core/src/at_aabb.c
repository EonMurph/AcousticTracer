#include "../src/at_internal.h"
#include "acoustic/at.h"
#include "acoustic/at_model.h"
#include "at_utils.h"
#include <float.h>

void AT_model_to_AABB(AT_AABB *out_aabb, const AT_Model *model)
{
    AT_Vec3 min_vec = AT_vec3(FLT_MAX, FLT_MAX, FLT_MAX);
    AT_Vec3 max_vec = AT_vec3(FLT_MIN, FLT_MIN, FLT_MIN);
    for (unsigned long i = 0; i < model->vertex_count; i++) {
        AT_Vec3 vec = model->vertices[i];
        min_vec.x = AT_min(min_vec.x, vec.x);
        min_vec.y = AT_min(min_vec.y, vec.y);
        min_vec.z = AT_min(min_vec.z, vec.z);
        max_vec.x = AT_max(max_vec.x, vec.x);
        max_vec.y = AT_max(max_vec.y, vec.y);
        max_vec.z = AT_max(max_vec.z, vec.z);
    }

    out_aabb->min = min_vec;
    out_aabb->max = max_vec;
}
