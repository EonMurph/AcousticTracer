#include "../src/at_bvh.h"
#include "../src/at_internal.h"
#include "../src/at_trigroup.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main()
{
    const char *filepath = "../assets/glb/Sponza.glb";

    AT_Model *model = NULL;
    if (AT_model_create(&model, filepath) != AT_OK) {
        perror("Failed to create model");
        return 1;
    }

    AT_Source sources[1];
    sources[0] = (AT_Source){
        .position = {0},
        .direction = {{1, 1, 1}},
        .intensity = 0,
    };

    AT_SceneConfig conf = {
        .environment = model,
        .material = AT_MATERIAL_PLASTIC,
        .num_sources = 1,
        .sources = sources,
    };

    AT_Scene *scene = NULL;
    if (AT_scene_create(&scene, &conf) != AT_OK) {
        perror("Failed to create scene");
        AT_model_destroy(model);
        return 1;
    }

    AT_Settings settings = {
        .fps = 120,
        .voxel_size = 0.1f,
    };

    AT_Simulation *sim = NULL;
    AT_Result res = AT_simulation_create(&sim, scene, &settings);
    if (res != AT_OK) {
        perror("Failed to create simulation");
        printf("%d", res);
        AT_scene_destroy(scene);
        AT_model_destroy(model);
        return 1;
    }

    struct timespec start_time, end_time;
    for (int bvh = 0; bvh < 2; bvh++) {
        for (int num_rays = 10; num_rays < 100001; num_rays *= 10) {
            settings.num_rays = num_rays;
            sim->num_rays = settings.num_rays;
            sim->rays = (AT_Ray *)calloc(settings.num_rays * scene->num_sources, sizeof(AT_Ray));
            if (!sim->rays) {
                free(sim);
                return AT_ERR_ALLOC_ERROR;
            }

            clock_gettime(CLOCK_MONOTONIC, &start_time);
            if (AT_simulation_run(sim, bvh) != AT_OK) {
                perror("Failed to run simulation");
                AT_simulation_destroy(sim);
                AT_scene_destroy(scene);
                AT_model_destroy(model);
                return 1;
            }
            clock_gettime(CLOCK_MONOTONIC, &end_time);
            double elapsed_ms =
                (end_time.tv_sec - start_time.tv_sec) * 1000.0 +
                (end_time.tv_nsec - start_time.tv_nsec) / 1e6;
            printf("%f\n", elapsed_ms);

            free(sim->rays);
        }
        printf("\n");
    }
}
